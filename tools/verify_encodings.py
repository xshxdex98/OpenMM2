# -*- coding: utf-8 -*-
"""Decide, per instruction form, whether MASM can be trusted to reproduce retail's exact bytes.

WHY THIS EXISTS. ExportAsm.java currently emits the whole binary as `db` blobs - 99.76% of
game.asm is raw bytes, with only address operands symbolised. That is safe but unreadable, and it
is the single biggest thing standing between this repo and being reviewable by anyone else.

The obvious fix - emit `push ebp` instead of `db 055h` - is not obviously safe, and the reason is
x86 redundant encodings. `fadd st, st` is DC C0 in the retail binary, but MASM assembles that same
mnemonic to D8 C0. `add eax, 0` has a short form and a long form. `test edx, eax` and
`test eax, edx` differ only in the direction bit and disassemble to different operand orders. Any
of those silently changes an instruction length or content, which moves every address after it,
which breaks -FIXED -BASE:0x400000 and every layout anchor the build checks.

So the rule is: a mnemonic may be emitted ONLY if MASM assembles it back to the identical bytes.
This tool establishes that set empirically rather than by reasoning about the encoding tables.
Run it before changing the exporter, and after any toolchain upgrade - a new ml.exe is free to
pick different encodings, and this is what would notice.

Each candidate is assembled in its own 16-byte ALIGN slot. The longest x86-32 instruction is 15
bytes, so one wrongly-sized encoding cannot desync the candidates that follow it - without that, a
single early mismatch would report every later instruction as broken too.

Usage:
    py tools/verify_encodings.py                 # read the instruction stream out of game.asm
    py tools/verify_encodings.py --json out.json # also write the verdict per encoding
"""
import collections
import glob
import json
import os
import re
import struct
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
ASM = os.path.join(os.environ.get("MM2_ASM_DIR", r"C:\mm2ghidra\out"), "game.asm")

# Relative branches encode a displacement to a target that does not exist in a standalone test, so
# they cannot be round-tripped this way. They are also the one class the exporter already gets
# right, as `db <opcode>` followed by `dd <symbol> - ($ + 4)`.
BRANCH = re.compile(r"^(j[a-z]{1,3}|call|loop[a-z]*|xbegin)$")
HEXB = re.compile(r"0([0-9A-Fa-f]{2})h")


def instruction_stream(path):
    """Recover instruction boundaries from the exported assembly.

    The exporter writes one `db` line per instruction, with any symbolised operand on a following
    `dd` line, so the file itself carries the boundaries Ghidra established. Re-deriving them by
    linear disassembly would desync on the data-in-code this binary is full of.
    """
    pure = collections.Counter()
    symbolic = 0
    in_code = False
    cur, cur_sym = bytearray(), False

    def flush():
        nonlocal cur, cur_sym, symbolic
        if cur:
            if cur_sym:
                symbolic += 1
            else:
                pure[bytes(cur)] += 1
        cur, cur_sym = bytearray(), False

    with open(path, encoding="latin-1") as f:
        for ln in f:
            s = ln.strip()
            if s == ".CODE":
                in_code = True
                continue
            if s.startswith(".DATA") or s == "END":
                in_code = False
            if not in_code or not s or s.startswith(";"):
                continue
            if (s.startswith("PUBLIC") or " PROC" in s or " ENDP" in s or s.endswith(":")
                    or s.startswith("ALIGN") or s.startswith("INCLUDE")):
                flush()
                continue
            if s.startswith("db "):
                body = s[3:]
                hexes = HEXB.findall(body)
                if hexes and not re.search(r"[A-Za-z_?@$][\w?@$]{2,}",
                                          re.sub(r"0[0-9A-Fa-f]{2}h", "", body)):
                    flush()
                    cur.extend(int(h, 16) for h in hexes)
                else:
                    cur_sym = True
                continue
            if s.startswith("dd ") or s.startswith("dw "):
                cur_sym = True
                continue
            flush()
    flush()
    return pure, symbolic


# capstone prints operand spellings MASM rejects. None of these change which instruction is meant -
# they are spelling, and getting them right is what separates "MASM cannot parse this" from "MASM
# disagrees about the encoding". Those are very different findings and must not be conflated: the
# first is a gap in this converter, the second is a hard limit on what the exporter may emit.
STRING_OP = re.compile(r"^(?:(?:rep|repe|repne|repz|repnz)\s+)?(?:movs|stos|lods|scas|cmps)[bwd]$")
# The x87 arithmetic group, with or without the pop suffix. Deliberately not fld/fxch/fcom, which
# legitimately take a single st(n) operand in MASM and must not have `, st` appended.
X87_ARITH = re.compile(r"^f(?:add|sub|mul|div|subr|divr)p?$")
MNEMONIC_ALIAS = {"int3": "int 3", "pushal": "pushad", "popal": "popad"}


def to_masm(mnemonic, operands):
    def hexlit(m):
        v = m.group(1).upper()
        return ("0" + v if v[0].isalpha() else v) + "h"

    if mnemonic in MNEMONIC_ALIAS and not operands:
        return MNEMONIC_ALIAS[mnemonic]
    # `rep movsb byte ptr es:[edi], byte ptr [esi]` is just `rep movsb` to MASM, and a bare
    # `stosb byte ptr es:[edi], al` is just `stosb`; the operands are implicit in the opcode and
    # naming them is "too many operands to instruction".
    if STRING_OP.match(mnemonic):
        return mnemonic
    ops = re.sub(r"0x([0-9a-fA-F]+)", hexlit, operands)
    # capstone calls the 80-bit x87 memory operand `xword`; MASM only knows it as `tbyte`.
    ops = ops.replace("xword ptr", "tbyte ptr")
    # x87 arithmetic against a stack slot needs both operands spelled out in MASM, and the implied
    # destination depends on whether the instruction pops:
    #
    #   D8 CA  fmul  st(2)   ->  fmul st, st(2)      non-pop: destination is ST(0)
    #   DE C1  faddp st(1)   ->  faddp st(1), st     pop:     destination is ST(i)
    #
    # Getting this backwards still assembles - to the OTHER form, D8 CA becoming DC CA - so it
    # shows up as a silent re-encode rather than an error. That is exactly the failure this tool
    # exists to catch, and it caught it here.
    if X87_ARITH.match(mnemonic) and re.fullmatch(r"st\(\d\)", ops):
        ops = ops + ", st" if mnemonic.endswith("p") else "st, " + ops
    # MASM spells the implicit top-of-stack `st`, never `st(0)`.
    ops = re.sub(r"\bst\(0\)", "st", ops)
    return (mnemonic + " " + ops).strip() if ops else mnemonic


def find_ml():
    hits = sorted(glob.glob(r"C:\Program Files\Microsoft Visual Studio\*\*\VC\Tools\MSVC"
                            r"\*\bin\Hostx64\x86\ml.exe"))
    if not hits:
        sys.exit("could not find ml.exe")
    return hits[-1]


def object_text(path):
    """The .text bytes of a COFF object. MASM names the section .text$mn, not .text."""
    d = open(path, "rb").read()
    nsec = struct.unpack_from("<H", d, 2)[0]
    opt = struct.unpack_from("<H", d, 16)[0]
    for i in range(nsec):
        o = 20 + opt + i * 40
        if d[o:o + 8].rstrip(b"\x00").decode("latin-1").startswith(".text"):
            size, ptr = struct.unpack_from("<II", d, o + 16)
            return d[ptr:ptr + size]
    return b""


def main():
    try:
        import capstone
    except ImportError:
        sys.exit("needs capstone: py -m pip install capstone")

    if not os.path.exists(ASM):
        sys.exit("no game.asm at %s - run tools/ghidra/ExportAsm.java first" % ASM)

    pure, symbolic = instruction_stream(ASM)
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)

    cand, undecodable = [], 0
    for b, n in pure.items():
        ins = list(md.disasm(b, 0x400000))
        if len(ins) != 1 or ins[0].size != len(b):
            undecodable += n
            continue
        if BRANCH.match(ins[0].mnemonic):
            continue
        cand.append({"b": b, "mn": ins[0].mnemonic, "n": n,
                     "text": to_masm(ins[0].mnemonic, ins[0].op_str), "ok": True})

    out = os.path.join(ROOT, "build")
    os.makedirs(out, exist_ok=True)
    asm_path = os.path.join(out, "encodings.asm")
    obj_path = os.path.join(out, "encodings.obj")
    ml = find_ml()
    # Must match game.asm's own prologue. ASSUME FS:NOTHING in particular is load-bearing: without
    # it every `mov eax, dword ptr fs:[0]` is rejected with "use of register assumed to ERROR", and
    # 2405 perfectly good instructions get blamed on MASM rather than on this header.
    header = [".686P", ".XMM", ".MODEL FLAT", "ASSUME FS:NOTHING", "ASSUME GS:NOTHING",
              "OPTION CASEMAP:NONE", "OPTION NOSCOPED", ".CODE"]

    # Anything MASM refuses is re-emitted as db so the 16-byte slots stay 1:1 with the candidate
    # list, then the assembly is retried - one rejected line otherwise aborts the whole run and
    # tells us nothing about the remaining forms.
    for _ in range(12):
        src, line_of = list(header), {}
        for i, c in enumerate(cand):
            src.append("ALIGN 16")
            line_of[len(src) + 1] = i
            src.append("    " + (c["text"] if c["ok"]
                                 else "db " + ",".join("0%02Xh" % x for x in c["b"])))
        src += ["ALIGN 16", "END"]
        open(asm_path, "w").write("\n".join(src) + "\n")
        r = subprocess.run([ml, "-c", "-coff", "-Fo" + obj_path, asm_path],
                           cwd=out, capture_output=True, text=True)
        errs = re.findall(r"encodings\.asm\((\d+)\)\s*:\s*error", r.stdout + r.stderr)
        hit = [line_of[int(e)] for e in errs if int(e) in line_of]
        if not hit:
            break
        for i in hit:
            cand[i]["ok"] = False

    text = object_text(obj_path)
    if not text:
        sys.exit("ml.exe produced no .text")

    good = collections.Counter()
    reencoded = collections.Counter()
    unparsed = collections.Counter()
    examples, verdict = {}, {}
    for slot, c in enumerate(cand):
        got = text[slot * 16: slot * 16 + len(c["b"])]
        key = c["b"].hex()
        if not c["ok"]:
            unparsed[c["mn"]] += c["n"]
            verdict[key] = "db:syntax"
        elif got == c["b"]:
            good[c["mn"]] += c["n"]
            verdict[key] = "mnemonic"
        else:
            reencoded[c["mn"]] += c["n"]
            verdict[key] = "db:reencoded"
            examples.setdefault(c["mn"], (key, got.hex(), c["text"]))

    a = sum(good.values())
    b_ = sum(reencoded.values())
    c_ = sum(unparsed.values())
    total = a + b_ + c_
    print("instructions carrying a symbol   : %8d  (already emitted as db + dd <sym>)" % symbolic)
    print("instructions not decodable       : %8d  (data-in-code; must stay db)" % undecodable)
    print("testable instruction occurrences : %8d  in %d unique forms\n" % (total, len(cand)))
    print("  %-32s %8d  %6.2f%%" % ("emit as mnemonic (byte-identical)", a, 100.0 * a / total))
    print("  %-32s %8d  %6.2f%%" % ("must stay db (MASM re-encodes)", b_, 100.0 * b_ / total))
    print("  %-32s %8d  %6.2f%%" % ("must stay db (syntax unsupported)", c_, 100.0 * c_ / total))

    if reencoded:
        print("\nforms MASM re-encodes - these can never be emitted as mnemonics:")
        for k, v in reencoded.most_common(15):
            retail, masm, txt = examples[k]
            print("   %-9s %7d  retail=%-20s masm=%-20s  %s" % (k, v, retail, masm, txt[:40]))
    if unparsed:
        print("\nforms MASM would not parse (fixable in to_masm, then re-run):")
        for k, v in unparsed.most_common(15):
            print("   %-9s %7d" % (k, v))

    if "--json" in sys.argv:
        dest = sys.argv[sys.argv.index("--json") + 1]
        json.dump(verdict, open(dest, "w"))
        print("\nwrote per-encoding verdict for %d forms to %s" % (len(verdict), dest))


if __name__ == "__main__":
    main()
