# -*- coding: utf-8 -*-
"""Compare the built .rdata and .data against retail, byte for byte.

tools/verify_text.py proves the code is faithful. Nothing proved the same for DATA, and that is
the larger silent hazard: ~800 KB of it, carrying every vtable, every float constant, every string
and every global's initial value. A mis-emitted data block produces no error at any stage - it
assembles, links, and the game reads a wrong constant or dispatches through a wrong vtable slot.

The comparison is harder than for .text because the data does not sit at the same address:

  - the linker builds its own import table into the FRONT of .rdata, pushing our emitted content
    down by 0x310 on top of the section's own 0x1000 move
  - .data moves wholesale

So the content has to be ALIGNED before it can be compared. The shift is found by searching for
retail's data in the built image rather than assuming it, which also means the number is measured
rather than trusted.

Then, as in verify_text, differences fall into expected classes:

  - a symbolised pointer, which correctly points at where the target lives in OUR image
  - the region the linker's import table occupies, which is not ours at all
  - BSS, which the linker may lay out differently

Anything else is a real defect.
"""
import json
import os
import struct
import sys
from collections import Counter

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)

sys.path.insert(0, HERE)
from pe import IMAGE_BASE  # noqa: E402

RETAIL = os.environ.get("MM2_EXE", r"C:\Users\xshxd\OneDrive\Desktop\PC Games\MM2\midtown2.exe")
BUILT = os.path.join(ROOT, "build", "OpenMM2.exe")


def sections(path):
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
        out[name] = dict(va=va, vsz=vsz, rp=rp, rsz=rsz,
                         bytes=raw[rp:rp + rsz])
    return raw, out


def find_alignment(retail_bytes, built_bytes):
    """How far into the built section our emitted copy of retail's data begins.

    Measured, not assumed: take a distinctive run from the middle of retail's data and find it in
    the built section. The middle avoids the import table at the front and any BSS tail.
    """
    # THE ANCHOR MUST BE POINTER-FREE, OR IT STOPS WORKING AS PORTING SCALES.
    #
    # A probe is matched verbatim, so any relocated pointer inside it breaks the match. .rdata is
    # full of pointers and every ported function relocates more of them, so fixed probe positions
    # survive early on and then quietly stop: at 342 functions .rdata had grown from retail's
    # 73,728 bytes to 109,056 and not one of the eight fixed probes still matched, which reads as
    # "could not locate retail's content" - indistinguishable from real corruption.
    #
    # Skipping any window containing a dword that looks like an image address makes the anchor
    # immune to that, because the only bytes it relies on are ones relocation cannot touch.
    votes = Counter()
    n = len(retail_bytes)

    def has_pointer(buf):
        for off in range(0, len(buf) - 4, 4):
            v = struct.unpack_from("<I", buf, off)[0]
            if IMAGE_BASE <= v <= IMAGE_BASE + 0x400000:
                return True
        return False

    # Many more positions than before, because a pointer-free run of useful length is not evenly
    # distributed - .rdata's vtables and string tables cluster.
    for step in range(1, 64):
        probe_at = int(n * step / 64.0)

        for size in (512, 256, 128, 64):
            probe = retail_bytes[probe_at:probe_at + size]

            if len(probe) < size:
                continue
            if probe.count(probe[:1]) == len(probe):
                continue  # a constant run matches anywhere; useless as an anchor
            if has_pointer(probe):
                continue  # relocation would rewrite it and the match would fail

            at = built_bytes.find(probe)
            if at >= 0 and built_bytes.find(probe, at + 1) < 0:
                votes[at - probe_at] += 1
                break

    if not votes:
        return None

    shift, agree = votes.most_common(1)[0]
    if agree < 2:
        return None  # one lone match is not evidence

    return shift


def main():
    if not os.path.exists(BUILT):
        sys.exit("no build at %s - run tools/build.py first" % BUILT)

    _, rsec = sections(RETAIL)
    _, bsec = sections(BUILT)

    # Retail addresses of every ported function.
    #
    # .data holds function-pointer tables, and a pointer to a function we have reimplemented must
    # now point at the C++ instead - which the linker placed past the original .text, so it moves
    # by its own amount rather than by any section displacement. That is the port working, not
    # corruption, and the count grows with every function ported.
    ported_at = []
    ported_path = os.path.join(ROOT, "data", "ported.json")
    symbols_path = os.path.join(ROOT, "data", "symbols.json")

    if os.path.exists(ported_path) and os.path.exists(symbols_path):
        with open(ported_path, encoding="utf-8") as f:
            ported = set(json.load(f).get("ported", []))
        with open(symbols_path, encoding="utf-8") as f:
            for s in json.load(f):
                if s["mangled"] in ported and s.get("code"):
                    # The whole EXTENT, not just the entry. A pointer can land inside a function -
                    # a jump-table entry or an interior label - and when that function is ported
                    # the interior address moves with it. Matching only entry points left four
                    # pointers into Matrix34::Transform (entry 0x46EDA0, 256 bytes) reading as
                    # corruption.
                    ported_at.append((s["rva"], s["rva"] + (s.get("size") or 1)))

    # Section displacements, for deciding whether a differing dword is a correctly relocated
    # pointer. Same idea as verify_text: measure the mode rather than trusting a header.
    ok = True

    # Gather pointer displacements across BOTH sections FIRST, then classify.
    #
    # Done per-section with an occurrence threshold, a legitimate displacement can fall below it:
    # the +0x1310 .rdata shift appears 686 times among .rdata's own pointers but only 7 times among
    # .data's, so a threshold of 8 declared those 7 corrupt. A displacement is a property of the
    # whole image, not of the section doing the pointing.
    aligns = {}
    all_deltas = Counter()

    for name in (".rdata", ".data"):
        if name not in rsec or name not in bsec:
            continue

        rb, bb = rsec[name]["bytes"], bsec[name]["bytes"]
        shift = find_alignment(rb, bb)
        aligns[name] = shift

        if shift is None:
            continue

        limit = min(len(rb), len(bb) - shift)
        for base in range(0, limit - 4):
            r = struct.unpack_from("<I", rb, base)[0]
            if r < IMAGE_BASE or r > IMAGE_BASE + 0x400000:
                continue
            b = struct.unpack_from("<I", bb, base + shift)[0]

            # A relocated pointer still points INSIDE the image. Without this, scanning every
            # unaligned window manufactures deltas out of byte soup - +0x40000000 showed up 13
            # times and cleared the threshold, which would have let it excuse real corruption.
            if b < IMAGE_BASE or b > IMAGE_BASE + 0x400000:
                continue

            if b != r:
                all_deltas[b - r] += 1

    known = {d for d, c in all_deltas.items() if c >= 8}
    print("image-wide pointer displacements: %s"
          % ", ".join("+0x%X (%d)" % (d, all_deltas[d]) for d in sorted(known)))
    print()

    for name in (".rdata", ".data"):
        if name not in rsec or name not in bsec:
            print("%-7s missing from one of the images" % name)
            ok = False
            continue

        rb = rsec[name]["bytes"]
        bb = bsec[name]["bytes"]

        shift = aligns.get(name)

        print("%-7s retail %d bytes, built %d bytes" % (name, len(rb), len(bb)))

        if shift is None:
            print("        could not locate retail's content in the built section")
            ok = False
            continue

        print("        our copy begins %+d bytes in" % shift)

        limit = min(len(rb), len(bb) - shift)

        # The head of .rdata is the import directory and lookup tables. The linker builds its own,
        # so our emitted copy of retail's is not what ends up there - and should not be. Retail's
        # own symbols do not start until 0x308 into the section, which is exactly this region.
        skip_head = 0x310 if name == ".rdata" else 0

        differing = 0
        pointerish = 0
        runs = []

        i = skip_head

        while i < limit:
            if rb[i] == bb[i + shift]:
                i += 1
                continue

            start = i
            while i < limit and rb[i] != bb[i + shift]:
                i += 1

            # Is this run explained by a relocated pointer?
            explained = True
            for off in range(start, i):
                hit = False
                for base in range(max(0, off - 3), off + 1):
                    if base + 4 > limit:
                        continue
                    r = struct.unpack_from("<I", rb, base)[0]
                    b = struct.unpack_from("<I", bb, base + shift)[0]
                    if r >= IMAGE_BASE and (b - r) in known:
                        hit = True
                        break
                    if any(lo <= r < hi for lo, hi in ported_at):
                        hit = True  # points into a function that is now C++
                        break
                if not hit:
                    explained = False
                    break

            if explained:
                pointerish += i - start
            else:
                differing += i - start
                if len(runs) < 12:
                    runs.append((rsec[name]["va"] + start + IMAGE_BASE, i - start))

        print("        differing as relocated pointers : %d (expected)" % pointerish)
        print("        differing ANYWHERE ELSE         : %d" % differing)

        for va, run in runs:
            print("           0x%08X  %d bytes" % (va, run))

        if differing:
            ok = False

        print()

    if not ok:
        print("FAIL: data differs from retail in ways the pipeline cannot account for")
        return 1

    print("PASS: .rdata and .data match retail except for correctly relocated pointers")
    return 0


if __name__ == "__main__":
    sys.exit(main())
