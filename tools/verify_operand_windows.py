"""Is every relocated operand in .text a whole operand, moved by a whole section delta?

THE GAP THIS EXISTS TO CLOSE. verify_text.py accepts a differing dword whose delta equals a
measured section displacement, and that is the right test for "was this relocated correctly" - but
it is structurally blind to the inverse question. A FALSE relocation, one applied to four bytes that
were never an address, differs from retail by exactly the same delta as a true one. No test that
looks only at the value can tell them apart.

That blindness shipped a crash. ExportAsm.java's missed-operand sweep restricted itself to
"trailing" 4-byte windows, on the reasoning that x86 puts imm32 and disp32 last. It does - but a
window can END where an operand ends and still START inside the opcode. `mov eax, [esi + 6398h]` is
8B 86 98 63 00 00, so the window at offset 1 is 00639886h: one ModRM byte plus three bytes of
displacement. It lands in .data, so it was relocated, and relocating it moved a STRUCTURE OFFSET.
Every [reg + 6398h] in dgBangerActiveManager became [reg + 64D8h], 140h past the end of a 639Ch
object - the constructor wrote outside its own heap block, Attach() returned a float out of the
embedded actives array as a dgPhysEntity*, and the game died the first time the player hit a
trashcan. Seventeen instructions. Every gate passed.

THE TEST, and why it is this one. Disassemble RETAIL per sized code symbol and record where each
instruction actually encodes a 4-byte displacement or immediate. At each of those sites, compare the
retail dword against ours: if they differ, the difference must be a whole section delta. That is
what makes the two cases separable. A true relocation sits AT an operand and moves by a section
delta; the false one above sits at an offset two bytes earlier, so when read at the real operand
site it shows a delta of 140h - a number no section ever moved by.

Relative branches are excluded. A call or jmp encodes its target as a rel32, and retargeting one
into ported C++ changes it by an arbitrary amount by design; verify_text.py already accounts for
those.

Not wired into tools/build.py's gate list, because capstone is not a build dependency. Run it by
hand after any change to the exporter:

    py tools/verify_operand_windows.py
"""
import json
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
sys.path.insert(0, HERE)

from pe import IMAGE_BASE
from verify_text import text_of

RETAIL = os.environ.get("MM2_EXE", r"C:\Users\xshxd\OneDrive\Desktop\PC Games\MM2\midtown2.exe")
BUILT = os.path.join(ROOT, "build", "OpenMM2.exe")
PORTED = os.path.join(ROOT, "data", "ported.json")
SYMBOLS = os.path.join(ROOT, "data", "symbols.json")


def main():
    try:
        import capstone
    except ImportError:
        sys.exit("capstone is required: py -m pip install capstone")

    if not os.path.exists(BUILT):
        sys.exit("no build at %s - run tools/build.py first" % BUILT)

    retail, rva_r, size_r = text_of(RETAIL)
    built, rva_b, size_b = text_of(BUILT)

    if rva_r != rva_b:
        sys.exit("FAIL: .text starts at a different address")

    with open(SYMBOLS, encoding="utf-8") as f:
        symbols = json.load(f)
    with open(PORTED, encoding="utf-8") as f:
        ported = set(json.load(f).get("ported", []))

    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True

    # Retail addresses of ported functions, so a call retargeted into our C++ is not a finding.
    ported_at = set()
    holes = []
    for s in symbols:
        if s["mangled"] in ported and s.get("code"):
            ported_at.add(s["rva"] - IMAGE_BASE)
            if s.get("size"):
                start = s["rva"] - IMAGE_BASE - rva_r
                holes.append((start, start + s["size"]))
    holes.sort()

    def retail_import_range():
        """The span of retail's import address table, whose entries each move independently."""
        try:
            import pefile

            pe = pefile.PE(RETAIL)
            lo = hi = None

            for d in pe.DIRECTORY_ENTRY_IMPORT:
                for i in d.imports:
                    if i.address is None:
                        continue
                    lo = i.address if lo is None else min(lo, i.address)
                    hi = i.address if hi is None else max(hi, i.address)

            return (lo, hi + 4) if lo is not None else (0, 0)
        except Exception:
            return (0, 0)

    def patched_addresses():
        """Every byte tools/patches.py deliberately rewrites."""
        try:
            import patches

            out = set()

            for p in patches.PATCHES:
                for k in range(len(p.old)):
                    out.add(p.address + k)

            return out
        except Exception:
            return set()

    def in_hole(off):
        for a, b in holes:
            if a <= off < b:
                return True
        return False

    sites = []          # (offset into .text, "disp" | "imm")
    covered = 0

    for s in symbols:
        if not s.get("code") or not s.get("size"):
            continue
        start = s["rva"] - IMAGE_BASE - rva_r
        if start < 0 or start + s["size"] > len(retail):
            continue
        if s["mangled"] in ported:
            continue

        covered += s["size"]

        for insn in md.disasm(bytes(retail[start:start + s["size"]]), start):
            groups = insn.groups
            relative = (capstone.CS_GRP_JUMP in groups or capstone.CS_GRP_CALL in groups
                        or capstone.CS_GRP_BRANCH_RELATIVE in groups)

            enc = insn.encoding
            if enc.disp_size == 4:
                sites.append((insn.address + enc.disp_offset, "disp"))
            if enc.imm_size == 4 and not relative:
                sites.append((insn.address + enc.imm_offset, "imm"))

    # Count every observed difference and keep the ones common enough to be a section move.
    #
    # An earlier draft had a second loop above this one that computed the same thing and never
    # finished - it ended on a bare `deltas.setdefault`, which is an AttributeError on a set, so the
    # script had never run. Deleted rather than completed: the loop below already derives the
    # deltas by counting, which is the measurement this needs.
    seen = {}
    for off, _kind in sites:
        if off + 4 > min(len(retail), len(built)) or in_hole(off):
            continue
        r = struct.unpack_from("<I", retail, off)[0]
        b = struct.unpack_from("<I", built, off)[0]
        if r != b:
            seen[b - r] = seen.get(b - r, 0) + 1

    # A section delta shows up thousands of times; a corrupted operand a handful.
    section_deltas = {d for d, n in seen.items() if n >= 50}

    print("sized code symbols decoded : %d bytes (%.2f%% of .text)"
          % (covered, 100.0 * covered / len(retail)))
    print("4-byte operand sites       : %d" % len(sites))
    print("ported functions skipped   : %d" % len(holes))
    print("section deltas observed    : %s"
          % ", ".join("+0x%X (%d)" % (d, seen[d]) for d in sorted(section_deltas)))

    # TWO KINDS OF DIFFERENCE ARE EXPECTED AND MUST BE EXCLUDED, or this reports 711 findings and
    # none of them are real - which is exactly what it did on its first run.
    #
    # IMPORTS. The linker builds its own import table rather than reusing the one emitted as data,
    # so every IAT slot lands somewhere new and each moves by its OWN delta, not a section's. A
    # reference to one is correctly retargeted; it just does not look like a section move.
    #
    # DELIBERATE PATCHES. tools/patches.py rewrites immediates on purpose - the frontend resolution
    # patch turns `push 480` into `push 1440` - and a changed immediate is precisely what this
    # script hunts for. Reading the patch list keeps the two from being confused, and means a NEW
    # patch never has to be remembered here.
    import_lo, import_hi = retail_import_range()
    patched = patched_addresses()

    ported_starts = set()

    try:
        with open(os.path.join(ROOT, "data", "ported.json"), encoding="utf-8") as f:
            done = set(json.load(f)["ported"])

        for sym in symbols:
            if sym["mangled"] in done:
                ported_starts.add(sym["rva"])
    except Exception:
        pass

    findings = []
    for off, kind in sites:
        if off + 4 > min(len(retail), len(built)) or in_hole(off):
            continue
        r = struct.unpack_from("<I", retail, off)[0]
        b = struct.unpack_from("<I", built, off)[0]
        if r == b or (b - r) in section_deltas:
            continue
        if import_lo <= r < import_hi:
            continue
        if (rva_r + off + IMAGE_BASE) in patched:
            continue

        # A REFERENCE TO A PORTED FUNCTION. Its code now lives past retail's .text, so the operand
        # holding its address is retargeted to somewhere that is not a section move away - which is
        # correct, and is the whole point of porting it.
        #
        # This also absorbs identical COMDAT folding, which is worth naming because it looks alarming
        # the first time: Vector2::Vector2 and Vector3::Vector3 are both `mov eax, ecx / ret`, so the
        # linker merges them and two different retail addresses end up pointing at one built address.
        # Behaviourally identical - but it does mean function-pointer identity is not preserved, and
        # if that ever matters the fix is /OPT:NOICF in tools/link.py.
        if r in ported_starts:
            continue
        findings.append((rva_r + off + IMAGE_BASE, kind, r, b, b - r))

    if findings:
        print()
        print("FAIL: %d operand(s) changed by something other than a section delta" % len(findings))
        print("      A relocation applied to bytes that were not an operand looks exactly like this.")
        for va, kind, r, b, d in findings[:40]:
            print("  0x%08X  %-4s retail 0x%08X -> 0x%08X  (delta 0x%X)" % (va, kind, r, b, d & 0xFFFFFFFF))
        if len(findings) > 40:
            print("  ... and %d more" % (len(findings) - 40))
        return 1

    print()
    print("PASS: every changed operand moved by a whole section delta")
    return 0


if __name__ == "__main__":
    sys.exit(main())
