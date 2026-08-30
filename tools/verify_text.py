# -*- coding: utf-8 -*-
"""Compare the built .text against the retail .text, byte for byte.

The build already checks the entry point and two anchor functions. That catches a wholesale shift,
but it says nothing about the other 1.7 MB: ExportAsm emits ~11,000 functions and ~73,000
relocations, and a single mis-encoded operand somewhere in the middle would sail past an anchor
check, assemble cleanly, link cleanly, and then misbehave at runtime with nothing pointing at the
cause.

So compare the whole section. Every byte of `.text` should be identical to the retail binary,
with exactly one class of exception: the regions where tools/asm.py replaced a ported function
with 0xCC padding. Those are known, their addresses and lengths come from data/symbols.json, and
they are the only differences permitted.

Anything else is a real defect in the pipeline. Reported here with its address, which is the
difference between "something is wrong" and "byte 0x4A17C3 in vehCarSim::Update is wrong".
"""
import json
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)

sys.path.insert(0, HERE)
from pe import IMAGE_BASE, load_sections  # noqa: E402

RETAIL = os.environ.get("MM2_EXE", r"C:\Users\xshxd\OneDrive\Desktop\PC Games\MM2\midtown2.exe")
BUILT = os.path.join(ROOT, "build", "OpenMM2.exe")
PORTED = os.path.join(ROOT, "data", "ported.json")
SYMBOLS = os.path.join(ROOT, "data", "symbols.json")

PAD = 0xCC


def text_of(path):
    data, secs = load_sections(path)

    with open(path, "rb") as f:
        raw = f.read()

    pe = struct.unpack_from("<I", raw, 0x3C)[0]
    n = struct.unpack_from("<H", raw, pe + 6)[0]
    tbl = pe + 24 + struct.unpack_from("<H", raw, pe + 20)[0]

    for i in range(n):
        b = tbl + i * 40
        name = raw[b:b + 8].rstrip(b"\0").decode()
        if name == ".text":
            vsz, va, rsz, rp = struct.unpack_from("<IIII", raw, b + 8)
            return raw[rp:rp + rsz], va, rsz

    sys.exit("no .text in %s" % path)


def main():
    if not os.path.exists(BUILT):
        sys.exit("no build at %s - run tools/build.py first" % BUILT)

    retail, rva_r, size_r = text_of(RETAIL)
    built, rva_b, size_b = text_of(BUILT)

    print(".text  retail va=0x%06X size=0x%X" % (rva_r, size_r))
    print("       built  va=0x%06X size=0x%X" % (rva_b, size_b))

    if rva_r != rva_b:
        sys.exit("FAIL: .text starts at a different address")

    # Regions we deliberately replaced with padding.
    with open(PORTED, encoding="utf-8") as f:
        ported = set(json.load(f).get("ported", []))

    with open(SYMBOLS, encoding="utf-8") as f:
        symbols = json.load(f)

    holes = []
    for s in symbols:
        if s["mangled"] in ported and s.get("code") and s.get("size"):
            start = s["rva"] - IMAGE_BASE - rva_r
            holes.append((start, start + s["size"], s["mangled"]))

    holes.sort()
    print("       %d ported functions expected to differ" % len(holes))

    def hole_at(off):
        for a, b, name in holes:
            if a <= off < b:
                return name
        return None

    # Section displacement, so a relocated address can be told apart from a corrupted one.
    #
    # This is the difference between "41,582 bytes are wrong" and "41,582 bytes are RIGHT". Every
    # `dd offset <symbol>` in .text resolves to wherever that symbol actually sits in OUR image,
    # and .rdata/.data are displaced (see docs/harness.md) - so those dwords legitimately differ
    # from retail by exactly the displacement of the section they point into. A byte that differs
    # for any other reason is a genuine defect.
    def section_map(path):
        with open(path, "rb") as f:
            raw = f.read()
        pe = struct.unpack_from("<I", raw, 0x3C)[0]
        n = struct.unpack_from("<H", raw, pe + 6)[0]
        tbl = pe + 24 + struct.unpack_from("<H", raw, pe + 20)[0]
        out = {}
        for i in range(n):
            b = tbl + i * 40
            name = raw[b:b + 8].rstrip(b"\0").decode()
            vsz, va, rsz, rp = struct.unpack_from("<IIII", raw, b + 8)
            out[name] = (va, max(vsz, rsz))
        return out

    rsecs = section_map(RETAIL)
    bsecs = section_map(BUILT)

    # Calibrate the displacement EMPIRICALLY, per section, instead of using the section start.
    #
    # .rdata's contents do not move by the same amount its header does: the linker places its own
    # import table at the front of the section, so our emitted data sits a further 0x310 in. The
    # section start moved 0x1000, the data inside it moved 0x1310, and assuming the former flags
    # thousands of perfectly correct references as corruption.
    #
    # So measure it: for every dword that points into a section, histogram (built - retail) and
    # take the mode. A real displacement is shared by thousands of references; a genuine defect is
    # a handful. The mode is the displacement, and anything not matching it is worth looking at.
    import collections

    votes = collections.defaultdict(collections.Counter)
    probe_limit = min(size_r, size_b)

    for base in range(0, probe_limit - 4, 1):
        r = struct.unpack_from("<I", retail, base)[0]
        if r < IMAGE_BASE or r > IMAGE_BASE + 0x400000:
            continue

        b = struct.unpack_from("<I", built, base)[0]
        rva = r - IMAGE_BASE

        for name, (start, size) in rsecs.items():
            if start <= rva < start + size:
                votes[name][b - r] += 1
                break

    deltas = {}
    for name, counter in votes.items():
        if counter:
            delta, count = counter.most_common(1)[0]
            deltas[name] = delta
            print("       %-7s displacement +0x%X  (%d references agree)" % (name, delta, count))

    measured_deltas = set(deltas.values())

    def delta_for(va):
        rva = va - IMAGE_BASE
        for name, (start, size) in rsecs.items():
            if start <= rva < start + size:
                return deltas.get(name)
        return None

    # Import address table bounds, for both images.
    #
    # A call into a DLL is `ff 15 <iat slot>` - it encodes the SLOT, not the target. The linker
    # builds its own import table at a different position inside .rdata, so those dwords do not
    # move by the section delta the way an ordinary data reference does; they land somewhere else
    # entirely. Without this they show up as 15,051 bytes of "corruption" that is nothing of the
    # kind.
    def iat_range(path):
        with open(path, "rb") as f:
            raw = f.read()
        pe = struct.unpack_from("<I", raw, 0x3C)[0]
        opt = pe + 24
        rva, size = struct.unpack_from("<II", raw, opt + 96 + 12 * 8)  # IAT directory
        if not rva:
            return None
        return (IMAGE_BASE + rva, IMAGE_BASE + rva + size)

    iat_r = iat_range(RETAIL)
    iat_b = iat_range(BUILT)

    def in_range(value, bounds):
        return bounds is not None and bounds[0] <= value < bounds[1]

    # Retail addresses of every ported function.
    #
    # A call to a function that has been reimplemented MUST now point somewhere else - at the C++,
    # which the linker placed past the end of the original .text. So its rel32 legitimately differs
    # from retail, and the difference is the whole point of the port. Without this rule every call
    # site of every ported function reads as corruption, and the count grows with each port.
    ported_at = set()
    for s in symbols:
        if s["mangled"] in ported and s.get("code"):
            ported_at.add(s["rva"])

    # VFTABLES HANDED OVER TO THE C++ SIDE.
    #
    # tools/asm_vtables.py moves a class's vftable when a constructor or destructor of that class
    # is ported, because MSVC emits its own ??_7C@@6B@ the moment it sees one. Every constructor
    # that is STILL assembly then stores a vptr that the linker resolves to the C++ table instead
    # of the .rdata one, so a dword inside that constructor legitimately changes.
    #
    # That is not the "relocated address" case below: the value does not move by a section delta,
    # it moves to an entirely different table. vehFeedback is the first class where it could show
    # up at all - mmCityInfo had its constructor ported too, so its vptr store went with it - and
    # it surfaced as three unexplained bytes at 0x004D56E1, inside ??0vehFeedback@@QAE@XZ.
    #
    # Derived the same way asm_vtables derives it, from ported.json rather than a second list, so
    # the two cannot disagree about which tables moved.
    handed_over = set()
    vt_path = os.path.join(ROOT, "data", "vtable_order.json")
    if os.path.exists(vt_path):
        with open(vt_path, encoding="utf-8") as f:
            for cls, entry in json.load(f).items():
                rva = entry.get("vftable_rva")
                if not rva:
                    continue
                if any(m.startswith(("??0" + cls + "@@", "??1" + cls + "@@")) for m in ported):
                    handed_over.add(rva)

    def is_handed_over_vftable(off):
        """Is the dword covering this byte the address of a vftable that moved to the C++ side?"""
        for base in range(max(0, off - 3), off + 1):
            if base + 4 > limit:
                continue
            if struct.unpack_from("<I", retail, base)[0] in handed_over:
                return True
        return False

    def is_call_to_ported(off):
        """Is this a relative call whose retail target was a function we have since ported?"""
        for base in range(max(0, off - 3), off + 1):
            if base + 4 > limit:
                continue
            rel = struct.unpack_from("<i", retail, base)[0]
            target = IMAGE_BASE + rva_r + base + 4 + rel
            if target in ported_at:
                return True
        return False

    def is_address_of_ported(off):
        """Is this the ADDRESS of a function we have ported, rather than a call to one?

        `push offset ??0Vector3@@QAE@XZ` takes a function's address instead of calling it, so the
        dword holds the retail entry point directly rather than a relative displacement. Porting
        Vector3 moved that function into the appended C++, so the value legitimately changes - for
        exactly the same reason a call to it does. Only the call form was recognised, so ptxGlass
        taking the constructor's address read as three unexplained bytes.
        """
        for base in range(max(0, off - 3), off + 1):
            if base + 4 > limit:
                continue
            if struct.unpack_from("<I", retail, base)[0] in ported_at:
                return True
        return False

    def is_relocation(off):
        """Is the dword covering this byte a correctly relocated address?"""
        for base in range(max(0, off - 3), off + 1):
            if base + 4 > limit:
                continue
            r = struct.unpack_from("<I", retail, base)[0]
            b = struct.unpack_from("<I", built, base)[0]

            if r < IMAGE_BASE or r > IMAGE_BASE + 0x400000:
                continue

            # An import slot in both images: correct, just relocated to our own IAT.
            if in_range(r, iat_r) and in_range(b, iat_b):
                return True

            d = delta_for(r)
            if d is not None and b == r + d:
                return True

            # A symbol can land in a DIFFERENT section than it occupied in retail, and then it
            # moves by that section's displacement rather than its original one. The CRT
            # initializer table does exactly this: `_initterm`'s bounds live in .data retail but
            # our link places them in .rdata, so both pointers shift by +0x1310 instead of +0x4000.
            # Both bounds move together, so the range _initterm walks is unchanged.
            #
            # Accepting any MEASURED displacement is still a strong test: each one is a mode shared
            # by thousands of references. A corrupted word would have to coincidentally equal one
            # of them.
            if (b - r) in measured_deltas:
                return True

        return False

    def is_retargeted_call(off):
        """A call/jmp whose rel32 now points at a reimplementation.

        Porting a function means every call site stops calling the original and starts calling
        ours - and ours lives past the end of the retail .text, so the encoded rel32 legitimately
        changes. This is not a difference to explain away: it is the porting actually taking
        effect, and if it were ABSENT the port would be a no-op.
        """
        for base in range(max(0, off - 3), off + 1):
            if base < 1 or base + 4 > limit:
                continue

            if built[base - 1] not in (0xE8, 0xE9):  # call rel32 / jmp rel32
                continue

            rel = struct.unpack_from("<i", built, base)[0]
            target = rva_r + base + 4 + rel  # RVA of the branch target

            # Past the retail .text is where the C++ was appended.
            if size_r <= target - rva_r < size_b:
                return True

        return False

    limit = min(size_r, size_b)
    diffs = 0
    in_hole = 0
    relocated = 0
    retargeted = 0
    reported = []
    i = 0

    # Byte offsets covered by a deliberate patch, from tools/patches.py.
    patched_offsets = set()
    try:
        from patches import PATCHES
        for p in PATCHES:
            base = p.address - IMAGE_BASE - rva_r
            for k in range(len(p.new)):
                patched_offsets.add(base + k)
    except Exception as e:
        print("warning: cannot read the patch list (%s) - patches will look unexpected" % e)

    def is_patched(off):
        return off in patched_offsets

    patched = 0

    while i < limit:
        if retail[i] == built[i]:
            i += 1
            continue

        # Walk the whole differing run at once - reporting per byte would drown the real signal.
        start = i
        while i < limit and retail[i] != built[i]:
            i += 1

        name = hole_at(start)
        run = i - start

        if name and all(b == PAD for b in built[start:i]):
            in_hole += run
            continue

        # A relocated address: correct for this image, merely different from retail.
        if all(is_relocation(off) for off in range(start, i)):
            relocated += run
            continue

        # A call redirected to a reimplementation: exactly what porting is supposed to do.
        if all(is_call_to_ported(off) for off in range(start, i)):
            retargeted += run
            continue

        if all(is_address_of_ported(off) for off in range(start, i)):
            retargeted += run
            continue

        # A vptr store in a constructor that is still assembly, now naming the C++ table.
        if all(is_handed_over_vftable(off) for off in range(start, i)):
            retargeted += run
            continue

        # A deliberate patch. These are the only bytes in the image that are *meant* to differ in
        # behaviour rather than merely in address, so they are declared in tools/patches.py and
        # matched here by address. Anything not on that list is still a failure - the point is that
        # an intentional change has to be written down somewhere to be accepted, not just look
        # plausible.
        if all(is_patched(off) for off in range(start, i)):
            patched += run
            continue

        diffs += run
        if len(reported) < 20:
            va = rva_r + start + IMAGE_BASE
            reported.append((va, run, name or "(not a ported function)"))

    print()
    print("bytes differing as deliberate patches : %d  (expected)" % patched)
    print("bytes differing inside ported regions : %d  (expected)" % in_hole)
    print("bytes differing as relocated addresses: %d  (expected - see below)" % relocated)
    print("bytes differing as calls into ported C++: %d  (expected)" % retargeted)
    print("bytes differing ANYWHERE ELSE         : %d" % diffs)

    if reported:
        print()
        print("unexpected differences:")
        for va, run, name in reported:
            print("   0x%08X  %5d bytes  %s" % (va, run, name))

    if size_b > size_r:
        extra = size_b - size_r
        tail = built[size_r:size_b]
        nonzero = sum(1 for b in tail if b not in (0, PAD))
        print()
        print("built .text is %d bytes longer (the reimplemented C++ lives here, %d non-fill)"
              % (extra, nonzero))

    print()
    if diffs:
        print("FAIL: %d bytes differ outside the ported regions" % diffs)
        return 1

    print("PASS: every byte of .text is either identical to retail, inside a ported")
    print("      function, or a correctly relocated address.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
