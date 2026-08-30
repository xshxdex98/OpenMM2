# -*- coding: utf-8 -*-
"""An absolute address in an INSTRUCTION OPERAND that should have moved and did not.

Seventh gate. verify_relocated.py asks this question of dwords stored in .rdata/.data; this asks
it of the addresses baked into .text, which is where most of them live and where nothing was
checking properly.

WHY THE OLD SCAN WAS NOT ENOUGH. tools/unrelocated.py slides a 4-byte window over .text and counts
dwords whose value falls in .rdata/.data and whose built bytes are unchanged. It is wrong in both
directions at once:

  1. It bounds the target range with .data's RAW size. That stops at 0x5DF000 and excludes ALL of
     BSS - 0xD6FB8 of it, 88% of .data's address range, and exactly where the game's globals live.
     `lpOutputString` pointed into that region. Opening the range to the full vsz takes its "89"
     to 6218.
  2. 6218 is not 6218 bugs. A sliding window does not know where an instruction starts, so it
     reads the `E8` of a `call rel32` as the top byte of an address and reports `call 0x411670` as
     a stale pointer to 0x005D30E8. Every one of the seven most promising hits found that way was
     this artefact and nothing else.

So this tool disassembles instead of sliding.

WHAT MAKES IT DECIDABLE. capstone reports, per instruction, the byte offset and width of the
displacement and immediate fields. A 4-byte displacement or immediate is the ONLY place an
absolute address can be encoded in 32-bit x86. Reading exactly those fields and nothing else
removes the opcode-swallowing false positives completely - there is no judgement call left. A
field is reported when:

  1. it is a 4-byte disp or imm at the offset capstone gives for it;
  2. its value lands inside retail's .rdata or .data, using the FULL virtual size so BSS counts;
  3. the same four bytes in our build are byte-identical to retail, i.e. the exporter did not
     rewrite them.

Branch immediates are excluded: call/jmp/jcc encode a rel32, not an address, and their targets are
.text anyway.

Only functions with a known size in data/symbols.json are disassembled, so the decoder always
starts on a real instruction boundary. A linear sweep over raw .text desynchronises on the first
jump table and manufactures nonsense.

WHAT A FINDING MEANS. .rdata moves +0x9310 and .data +0xF000 in our image. An operand left at its
retail value therefore reads or writes memory that far low - and because .data's displacement is a
function of where .text ends, the address it lands on CHANGES BY 0x1000 whenever .text crosses a
page boundary. The defect is constant; the damage it does is not. That is the section-layout
blocker. See docs/layout_hunt.md.
"""
import json
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
sys.path.insert(0, HERE)

from pe import IMAGE_BASE                         # noqa: E402
from verify_data import sections, find_alignment  # noqa: E402

RETAIL = os.environ.get(
    "MM2_RETAIL", os.environ.get(
        "MM2_EXE", "C:/Users/xshxd/OneDrive/Desktop/PC Games/MM2/midtown2.exe"))
BUILT = os.path.join(ROOT, "build", "OpenMM2.exe")

BRANCH = ("call", "jmp", "loop", "loope", "loopne")


def is_branch(mnemonic):
    return mnemonic in BRANCH or mnemonic.startswith("j")


def nearest(ordered, exact, addr):
    """Nearest preceding DATA symbol, for saying what an address actually names."""
    lo, hi, best = 0, len(ordered) - 1, None
    while lo <= hi:
        mid = (lo + hi) // 2
        if ordered[mid] <= addr:
            best = ordered[mid]
            lo = mid + 1
        else:
            hi = mid - 1
    if best is None:
        return ""
    off = addr - best
    return "%s+0x%X" % (exact[best], off) if off else exact[best]


def main():
    try:
        import capstone
    except ImportError:
        print("  capstone not installed - cannot check instruction operands")
        return 0

    if not os.path.exists(BUILT):
        print("  no build to check")
        return 0
    if not os.path.exists(RETAIL):
        print("  retail binary not found: %s (set MM2_RETAIL)" % RETAIL)
        return 1

    _, retail = sections(RETAIL)
    _, built = sections(BUILT)

    for name in (".text", ".rdata", ".data"):
        if name not in retail or name not in built:
            print("  %s missing from one of the images" % name)
            return 1

    # Retail's own address range for each moved section, INCLUDING the uninitialised tail. Using
    # the raw size here is the defect this tool exists to correct.
    span = {}
    for name in (".rdata", ".data"):
        base = retail[name]["va"] + IMAGE_BASE
        span[name] = (base, base + retail[name]["vsz"])

    # Where retail's content actually lands in our image, measured rather than assumed.
    delta = {}
    for name in (".rdata", ".data"):
        at = find_alignment(retail[name]["bytes"], built[name]["bytes"])
        if at is None:
            print("  cannot locate our copy of %s - refusing to judge" % name)
            return 1
        delta[name] = (built[name]["va"] + IMAGE_BASE + at) - (retail[name]["va"] + IMAGE_BASE)

    print("  measured displacement   : %s"
          % ", ".join("%s %+#x" % (n, delta[n]) for n in sorted(delta)))

    def target_of(value):
        for n, (lo, hi) in span.items():
            if lo <= value < hi:
                return n
        return None

    rb = retail[".text"]["bytes"]
    bb = built[".text"]["bytes"]
    tva = retail[".text"]["va"] + IMAGE_BASE

    path = os.path.join(ROOT, "data", "symbols.json")
    if not os.path.exists(path):
        print("  data/symbols.json missing - nothing to bound the disassembly with")
        return 1
    with open(path, encoding="utf-8") as f:
        syms = json.load(f)

    funcs = sorted((s["rva"], s.get("size") or 0, s["mangled"])
                   for s in syms if s.get("code") and s.get("rva") and (s.get("size") or 0) > 0)

    exact = {}
    for s in syms:
        a = s.get("rva")
        if a and not s.get("code"):
            exact.setdefault(a, s["mangled"])
    ordered = sorted(exact)

    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True

    findings = []
    scanned = 0

    for rva, size, name in funcs:
        off = rva - tva
        if off < 0 or off + size > len(rb) or off + size > len(bb):
            continue
        scanned += 1
        for ins in md.disasm(rb[off:off + size], rva):
            io = ins.address - tva
            enc = ins.encoding
            fields = []
            if enc.disp_size == 4 and enc.disp_offset:
                fields.append(("disp", enc.disp_offset))
            if enc.imm_size == 4 and enc.imm_offset and not is_branch(ins.mnemonic):
                fields.append(("imm", enc.imm_offset))

            for kind, k in fields:
                if io + k + 4 > len(rb) or io + k + 4 > len(bb):
                    continue
                value = struct.unpack_from("<I", rb, io + k)[0]
                t = target_of(value)
                if t is None:
                    continue
                if struct.unpack_from("<I", bb, io + k)[0] != value:
                    continue  # relocated, which is the whole point
                findings.append((ins.address, name, "%s %s" % (ins.mnemonic, ins.op_str),
                                 kind, value, t))

    print("  functions disassembled  : %d" % scanned)
    print("  unrelocated operands    : %d" % len(findings))

    if not findings:
        print("  PASS: every absolute .rdata/.data operand in .text was relocated")
        return 0

    print()
    print("  UNRELOCATED OPERANDS: %d" % len(findings))
    print("  Each addresses memory %s low at runtime, and the address it actually lands on"
          % " / ".join("%#x" % delta[t] for t in sorted(delta)))
    print("  shifts by another 0x1000 every time .text crosses a page boundary.")
    print()

    for addr, fn, text, kind, value, t in findings[:40]:
        who = nearest(ordered, exact, value)
        print("    0x%08X  %s" % (addr, fn))
        print("        %s" % text)
        print("        %s operand holds 0x%08X (%s%s), and did not move"
              % (kind, value, t, ", " + who if who else ""))
        print("        should now hold 0x%08X (%s %+#x)" % (value + delta[t], t, delta[t]))
    if len(findings) > 40:
        print("    ... and %d more" % (len(findings) - 40))

    print()
    print("  FAIL: an instruction operand kept its retail address while its section moved")
    return 1


if __name__ == "__main__":
    sys.exit(main())
