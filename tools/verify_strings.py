# -*- coding: utf-8 -*-
"""String data in .rdata/.data must survive the export unchanged.

This gate exists because of a specific, very expensive failure. The exporter decides a dword is a
pointer partly by matching its value against a known symbol address. In .text that rule is strong.
In .data it is not, because .data is full of strings, and a four-byte run of ASCII is numerically
indistinguishable from an address: a real pointer here has high byte 0x00 and second byte in
0x40..0x6B, which is exactly the printable range.

"tga\0" is 0x00616774, and this binary has a data symbol at precisely 0x00616774. The extension
string inside "texture\0tga\0" was therefore relocated by +0x8000 and became "t\xe7a".
gfxLoadTexImage went looking for files that cannot exist, the loading screen failed to load, and
the game reported "not properly installed - insert the original CD" and exited.

Every existing verifier passed, and that is the point of this one: a corrupted string is
byte-for-byte identical to what a correctly relocated pointer looks like, so a checker that reasons
about deltas cannot tell them apart. This one does not reason about deltas. It finds printable
NUL-terminated runs in the retail image and demands they be present, unchanged, in ours.
"""
import json
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
sys.path.insert(0, HERE)

from pe import IMAGE_BASE
from verify_data import sections

RETAIL = os.environ.get(
    "MM2_RETAIL", "C:/Users/xshxd/OneDrive/Desktop/PC Games/MM2/midtown2.exe")
BUILT = os.path.join(ROOT, "build", "OpenMM2.exe")

MIN_LEN = 3          # "tga" is three characters, and it is the string this gate exists for
MIN_LETTERS = 2      # see below


def is_text(b):
    """Text, as opposed to a pointer whose bytes happen to be printable.

    Length alone cannot separate the two. At four characters this check missed "tga" - the exact
    string it was written to catch. At three it flagged sixty pointers like b',w\', which is
    0x005C772C rendered as ASCII and was relocated perfectly correctly.

    Letters are what separate them. A pointer into this image has its second byte in 0x40..0x6B and
    its others arbitrary, so one letter is common and two is not; real text almost always has
    several. This keeps "tga" and drops every one of those sixty.
    """
    return sum(1 for c in b if (65 <= c <= 90) or (97 <= c <= 122)) >= MIN_LETTERS


def runs(data):
    """Printable NUL-terminated strings, as (offset, bytes)."""
    out = []
    start = None
    for i, b in enumerate(data):
        if 0x20 <= b < 0x7F:
            if start is None:
                start = i
        else:
            if start is not None and b == 0 and i - start >= MIN_LEN:
                text = data[start:i]
                if is_text(text):
                    out.append((start, text))
            start = None
    return out


# Objects the exporter is told are arrays of pointers, not text.
#
# ExportAsm.java carries the same list, deliberately: the exporter has to decide and this has to
# catch it deciding wrongly. They must agree on what is not text, or a correct relocation gets
# reported as damage - which is exactly what happened here. lpOutputString's entries are three
# printable bytes with two letters, so every text heuristic calls them a string, and relocating
# them correctly turns "T=j" into "T=k".
POINTER_ARRAYS = ("lpOutputString",)

_known_ranges = None


def in_known_pointer_array(addr):
    global _known_ranges
    if _known_ranges is None:
        _known_ranges = []
        try:
            with open(os.path.join(ROOT, "data", "symbols.json"), encoding="utf-8") as f:
                syms = sorted((s["rva"], s["mangled"]) for s in json.load(f) if not s["code"])
            for i, (rva, name) in enumerate(syms):
                if name in POINTER_ARRAYS:
                    end = syms[i + 1][0] if i + 1 < len(syms) else rva + 0x40
                    _known_ranges.append((rva, end))
        except Exception:
            _known_ranges = []
    return any(lo <= addr < hi for lo, hi in _known_ranges)


def in_pointer_table(data, off, va, lo, hi):
    """Is this dword part of an ascending run of data pointers?

    The exporter uses the same test, and it has to: a table of string pointers ascends in small
    steps because it points at consecutive strings, while four bytes of ASCII bear no such relation
    to the four after them. Without this the checker reports entries like b'Lx\' - which is
    0x005C784C, relocated perfectly correctly - as damaged text, purely because two of its three
    bytes are letters.

    This is not the checker trusting the exporter. It still verifies that content outside a pointer
    table is preserved byte for byte; it just declines to call a pointer array a string.
    """
    if off % 4:
        return False

    def dword(i):
        if i < 0 or i + 4 > len(data):
            return None
        return struct.unpack_from("<I", data, i)[0]

    def plausible(v):
        return v is not None and lo <= v < hi

    if not plausible(dword(off)):
        return False

    n = 1
    i = off
    while True:
        prev, cur = dword(i - 4), dword(i)
        if not plausible(prev) or cur <= prev or cur - prev > 256:
            break
        n += 1
        i -= 4

    i = off
    while True:
        cur, nxt = dword(i), dword(i + 4)
        if not plausible(nxt) or nxt <= cur or nxt - cur > 256:
            break
        n += 1
        i += 4

    return n >= 4


def in_string_table(data, off, length):
    """Does another string sit next to this one?

    The letter test alone still called real pointers text. b'Lh[' is 0x005B684C and b'adj' is
    0x006A6461; both have two letters, and both were relocated perfectly correctly. What separates
    them from b'tga' is company: a string lives in a table of strings, while a pointer lives among
    pointers. So require a neighbour - another printable, letter-bearing run either just before or
    just after, allowing for the NUL padding these tables are aligned with.

    Checking both directions matters because the first and last entries of a table only have a
    neighbour on one side.
    """
    def run_at(i, step):
        seen = 0
        while 0 <= i < len(data) and data[i] == 0 and seen < 12:
            i += step
            seen += 1
        chars = []
        while 0 <= i < len(data) and 0x20 <= data[i] < 0x7F and len(chars) < 24:
            chars.append(data[i])
            i += step
        return bytes(chars)

    after = run_at(off + length + 1, 1)
    if len(after) >= MIN_LEN and is_text(after):
        return True

    before = run_at(off - 1, -1)
    return len(before) >= MIN_LEN and is_text(before)


def main():
    if not os.path.exists(BUILT):
        print("no build to check")
        return 0

    _, retail = sections(RETAIL)
    _, built = sections(BUILT)

    global PTR_LO, PTR_HI
    PTR_LO = retail[".rdata"]["va"] + IMAGE_BASE
    PTR_HI = retail[".data"]["va"] + IMAGE_BASE + retail[".data"]["vsz"]

    total = damaged = 0
    bad = []

    for name in (".rdata", ".data"):
        if name not in retail or name not in built:
            continue

        rb = retail[name]["bytes"]
        bb = built[name]["bytes"]
        rva = retail[name]["va"] + IMAGE_BASE
        bva = built[name]["va"] + IMAGE_BASE

        # Our copy of a section does not necessarily start at offset 0 of ours: the linker puts its
        # own import table at the front of .rdata, pushing our content 784 bytes in. Comparing at
        # equal offsets without accounting for that reports every string as damaged, which is how
        # this check first "found" 285 corruptions that were only a misalignment.
        shift = bb.find(rb[:256])
        if shift < 0:
            print("  %s: cannot locate our copy of the section - skipped" % name)
            continue
        bva += shift

        for off, text in runs(rb):
            if shift + off + len(text) > len(bb):
                continue
            if in_pointer_table(rb, off, rva, PTR_LO, PTR_HI):
                continue
            if in_known_pointer_array(rva + off):
                continue
            if not in_string_table(rb, off, len(text)):
                continue
            total += 1
            if bb[shift + off:shift + off + len(text)] != text:
                damaged += 1
                if len(bad) < 12:
                    bad.append((name, rva + off, bva + off, text,
                                bb[shift + off:shift + off + len(text)]))

    print("  strings checked : %d" % total)

    if not damaged:
        print("  PASS: every string survived the export unchanged")
        return 0

    print("  DAMAGED         : %d" % damaged)
    for name, ra, ba, want, got in bad:
        print("    %s retail 0x%08X / built 0x%08X" % (name, ra, ba))
        print("      want %r" % (want,))
        print("      got  %r" % (got,))
    print("  FAIL: a string was rewritten as if it were a pointer")
    return 1


if __name__ == "__main__":
    sys.exit(main())
