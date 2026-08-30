# -*- coding: utf-8 -*-
"""A pointer that should have moved and did not is byte-identical to retail.

This is the sixth gate, and it exists for the one shape that beat every previous one twice.

`.rdata` and `.data` do not land at their retail addresses in our image, so every pointer INTO
them has to be rewritten. midtown2.exe has no relocation table, so the exporter decides what is a
pointer from Ghidra's analysis plus an exact match against a known symbol address - and where it
decides "not a pointer", the dword keeps its retail value. That miss produces NO byte difference
at all, which is why:

  - `verify_data.py` cannot see it. It asks "does this byte differ, and is the difference
    explainable". An unrelocated pointer does not differ.
  - `verify_text.py` cannot see it, for the same reason.
  - `verify_strings.py` cannot see it. It asks the opposite question - is retail's text still
    text - and an unrelocated pointer passes that trivially.
  - `verify_extents.py` cannot see it. It reads only retail, to decide what is safe to strip.

Two of the five defects that stood between "links" and "runs" were exactly this: 307 DirectInput
GUID pointers, and half of `vehLoadNameList`. Both were found by running the game.

WHAT MAKES IT DECIDABLE. On its own, "this dword looks like an address and did not move" is
useless: 349 dwords are in that state and most of them are four bytes of ASCII. "tga" is
0x00616774 and "cab" is 0x00626163, and both are numerically inside `.data`. Flagging those is how
a gate gets disabled.

What decides it is COMPANY, the same way verify_strings decides text. A pointer lives in a table,
and the exporter relocated its neighbours. So a dword is reported only when:

  1. it holds an address inside a section that moved, and it did not move;
  2. at least two OTHER entries at the same stride, within four steps, DID move by exactly the
     displacement that section takes; and
  3. those entries point at addresses NEAR this one - within 4 KB per step. A table of string
     pointers walks through a string blob in small increments.

Rule 3 is what separates a real finding from a string, and it is not optional. Without it this
reports "bug", "cab", "age", " %d" and "z%d" - the tails of the very strings a nearby pointer
table points AT, which is precisely why they sit next to relocated pointers. Their values are
hundreds of kilobytes away from what the table addresses; a real entry is a few bytes away.

Rule 2 takes two witnesses rather than one, so a lone accident cannot convict.

The gate is deliberately conservative. A table where nothing moved is invisible to it, and so is a
two-entry table with one entry wrong. It reports what is structurally certain, and it prints how
many candidates it declined to report so the number is never mistaken for coverage.

SECOND RULE, same question. A function-pointer table entry naming a function we have PORTED must
now point at the C++. tools/asm.py fills the original bytes with 0xCC, so an entry left at its
retail value dispatches into int 3. That is also byte-identical to retail, and it becomes possible
with every new port, so it is checked here too.
"""
import json
import os
import struct
import sys
from collections import Counter, defaultdict

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
sys.path.insert(0, HERE)

from pe import IMAGE_BASE                         # noqa: E402
from verify_data import sections, find_alignment  # noqa: E402

RETAIL = os.environ.get(
    "MM2_RETAIL", os.environ.get(
        "MM2_EXE", "C:/Users/xshxd/OneDrive/Desktop/PC Games/MM2/midtown2.exe"))
BUILT = os.path.join(ROOT, "build", "OpenMM2.exe")

SECTIONS = (".rdata", ".data")

# How far apart two entries of one table may point, per step between them. String tables step by
# the length of a string; 4 KB per step is generous for those and still three orders of magnitude
# tighter than the distance from a piece of ASCII to the table that quotes it.
NEAR_PER_STEP = 0x1000

# Strides a table of pointers is plausibly built on: bare arrays, and structs up to 64 bytes.
STRIDES = (4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 48, 64)

# Witnesses are looked for this many steps out in each direction.
REACH = 4

# Two witnesses. One relocated neighbour at some stride is an accident waiting to happen.
MIN_WITNESSES = 2


def load_symbols():
    """(address -> name, sorted address list). symbols.json's `rva` field holds a full VA."""
    path = os.path.join(ROOT, "data", "symbols.json")
    if not os.path.exists(path):
        return {}, []

    with open(path, encoding="utf-8") as f:
        syms = json.load(f)

    exact = {}
    for s in syms:
        a = s.get("rva")
        if a:
            exact.setdefault(a, s["mangled"])
    return exact, sorted(exact)


def describe(addr, exact, ordered, secs, quote=True):
    """What lives at an address, said in terms a human can act on.

    `quote` is off for the address of the dword itself: reading ITS four bytes as text is what
    produces captions like 'xh[' - the very confusion this gate exists to resolve. Only the
    target is worth quoting.
    """
    name = exact.get(addr)

    if not name and ordered:
        lo, hi, best = 0, len(ordered) - 1, None
        while lo <= hi:
            mid = (lo + hi) // 2
            if ordered[mid] <= addr:
                best = ordered[mid]
                lo = mid + 1
            else:
                hi = mid - 1
        # A symbol a long way back is not a description of anything. verify_strings learned this
        # the hard way when nearest() answered "PLAYER + 219700" for a corrupted string, and the
        # implausible distance was the tell nobody read. 0x40 is one or two table entries.
        if best is not None and 0 < addr - best <= 0x40:
            name = "%s+0x%X" % (exact[best], addr - best)

    text = b""
    for s in secs.values():
        va = s["va"] + IMAGE_BASE
        if va <= addr < va + len(s["bytes"]):
            text = s["bytes"][addr - va:addr - va + 24].split(b"\0")[0]
            break

    bits = []
    if name:
        bits.append(name)
    # Only quote the target when it really reads as text - the same two-letter rule
    # verify_strings uses. Otherwise every pointer gets a nonsense caption like 'T=j'.
    if quote and len(text) >= 3 and all(0x20 <= c < 0x7F for c in text) \
            and sum(1 for c in text if (65 <= c <= 90) or (97 <= c <= 122)) >= 2:
        bits.append(repr(text.decode("latin1")))

    return "  (%s)" % ", ".join(bits) if bits else ""


def main():
    if not os.path.exists(BUILT):
        print("  no build to check")
        return 0
    if not os.path.exists(RETAIL):
        print("  retail binary not found: %s (set MM2_RETAIL)" % RETAIL)
        return 1

    _, retail = sections(RETAIL)
    _, built = sections(BUILT)

    for name in SECTIONS:
        if name not in retail or name not in built:
            print("  %s missing from one of the images" % name)
            return 1

    # Where each section's content lands in ours, measured rather than assumed - the linker puts
    # its own import table at the front of .rdata and pushes everything else down.
    shift = {}
    for name in SECTIONS:
        at = find_alignment(retail[name]["bytes"], built[name]["bytes"])
        if at is None:
            print("  cannot locate our copy of %s - refusing to judge" % name)
            return 1
        shift[name] = at

    # Retail's own address range for each section, INCLUDING the uninitialised tail: most globals
    # live in .data's BSS, and pointers to them are exactly what has to be rewritten.
    span = {n: (retail[n]["va"] + IMAGE_BASE,
                retail[n]["va"] + IMAGE_BASE + retail[n]["vsz"]) for n in SECTIONS}

    def target_of(value):
        for n, (lo, hi) in span.items():
            if lo <= value < hi:
                return n
        return None

    # Every dword that holds an address, as {address: (retail value, built value, target)}.
    cell = {}
    votes = defaultdict(Counter)

    for name in SECTIONS:
        rb, bb, sh = retail[name]["bytes"], built[name]["bytes"], shift[name]
        va = retail[name]["va"] + IMAGE_BASE
        limit = min(len(rb), len(bb) - sh)

        for off in range(0, limit - 4, 4):
            r = struct.unpack_from("<I", rb, off)[0]
            t = target_of(r)
            if t is None:
                continue
            b = struct.unpack_from("<I", bb, off + sh)[0]
            cell[va + off] = (r, b, t)
            if b != r:
                votes[t][b - r] += 1

    if not votes:
        print("  no relocated pointers found at all - the scan is broken, not the binary")
        return 1

    # The displacement a section takes, measured as the mode. A real displacement is shared by
    # hundreds of pointers, a defect by a handful. Same self-calibration as verify_text.
    delta = {}
    for t, counts in votes.items():
        d, n = counts.most_common(1)[0]
        if n < 16:
            print("  displacement for %s rests on only %d pointers - refusing to judge" % (t, n))
            return 1
        delta[t] = d

    print("  measured displacement   : %s"
          % ", ".join("%s %+#x (%d pointers agree)" % (t, delta[t], votes[t][delta[t]])
                      for t in sorted(delta)))

    moved = {}
    stale = []
    for addr, (r, b, t) in cell.items():
        if t not in delta:
            continue
        if b == r + delta[t]:
            moved[addr] = (r, t)
        elif b == r:
            stale.append(addr)

    # --- Rule 1: a stale entry among relocated company -------------------------------------------
    findings = []

    for addr in sorted(stale):
        r, _, t = cell[addr]
        best = None

        for s in STRIDES:
            witness = []
            for k in list(range(-REACH, 0)) + list(range(1, REACH + 1)):
                other = addr + k * s
                if other not in moved:
                    continue
                w, wt = moved[other]
                if wt != t:
                    continue
                if abs(w - r) > NEAR_PER_STEP * abs(k):
                    continue
                witness.append((other, w))

            if len(witness) >= MIN_WITNESSES and (best is None or len(witness) > len(best[1])):
                best = (s, witness)

        if best:
            findings.append((addr, r, t, best[0], best[1]))

    # Once one entry of a table is convicted, its immediate neighbours are convicted with it.
    #
    # A table can be broken in a run, and the entry in the middle of that run has no relocated
    # witness left within reach - the whole point being that its witnesses are the broken ones.
    # `lpOutputString` and the dword four bytes after it are exactly that: only the first has two
    # relocated neighbours close enough to convict, so reporting it alone would send someone to fix
    # half a fault. Half of `vehLoadNameList` was this shape.
    #
    # This does not loosen the rule, it only spreads a verdict already reached: the neighbour must
    # be within one table's reach of a CONFIRMED finding and point within 4 KB of what that finding
    # points at. A piece of ASCII near a broken table fails the second test by six orders of
    # magnitude, which is the same test that keeps "cab" and "bug" out of the report above.
    SPREAD = max(STRIDES) * REACH
    anchors = {a: (r, t) for a, r, t, _, _ in findings}
    for addr in sorted(stale):
        if addr in anchors:
            continue
        r, _, t = cell[addr]
        for anchor, (ar, at) in anchors.items():
            if at == t and abs(addr - anchor) <= SPREAD and abs(r - ar) <= NEAR_PER_STEP:
                findings.append((addr, r, t, 0, [(anchor, ar)]))
                break

    findings.sort()

    # --- Rule 2: a pointer at a function we have ported ------------------------------------------
    ported_stale = []
    ported_path = os.path.join(ROOT, "data", "ported.json")
    symbols_path = os.path.join(ROOT, "data", "symbols.json")

    if os.path.exists(ported_path) and os.path.exists(symbols_path):
        with open(ported_path, encoding="utf-8") as f:
            ported = set(json.load(f).get("ported", []))
        with open(symbols_path, encoding="utf-8") as f:
            entry = {s["rva"]: s["mangled"] for s in json.load(f)
                     if s.get("code") and s.get("rva") and s["mangled"] in ported}

        for name in SECTIONS:
            rb, bb, sh = retail[name]["bytes"], built[name]["bytes"], shift[name]
            va = retail[name]["va"] + IMAGE_BASE
            limit = min(len(rb), len(bb) - sh)
            for off in range(0, limit - 4, 4):
                r = struct.unpack_from("<I", rb, off)[0]
                if r in entry and struct.unpack_from("<I", bb, off + sh)[0] == r:
                    ported_stale.append((va + off, r, entry[r]))

    # --- Report ----------------------------------------------------------------------------------
    exact, ordered = load_symbols()

    print("  pointer-valued dwords   : %d relocated, %d unchanged" % (len(moved), len(stale)))
    print("  unchanged, and sitting in a table whose other entries moved: %d" % len(findings))
    print("  unchanged with no such evidence, so NOT judged here        : %d"
          % (len(stale) - len(findings)))

    if not findings and not ported_stale:
        print("  PASS: no pointer was left behind by a relocation its own table received")
        return 0

    if findings:
        print()
        print("  UNRELOCATED POINTERS: %d" % len(findings))
        print("  Each still holds its retail value while the table around it moved, so at runtime")
        print("  it addresses memory %s low."
              % " / ".join("%#x" % delta[t] for t in sorted(delta)))
        print()

        for n, (addr, r, t, stride, witness) in enumerate(findings):
            if n >= 20:
                print("    ... and %d more" % (len(findings) - 20))
                break
            print("    0x%08X%s" % (addr, describe(addr, exact, ordered, retail, quote=False)))
            print("        holds 0x%08X%s, which is in %s, and did not move"
                  % (r, describe(r, exact, ordered, retail), t))
            print("        should now hold 0x%08X (%s %+#x)" % (r + delta[t], t, delta[t]))
            if stride:
                print("        the table did move: stride %d, entries at %s"
                      % (stride, ", ".join("0x%08X" % w for w, _ in witness[:4])))
            else:
                print("        same table as 0x%08X above, and points %d bytes from it"
                      % (witness[0][0], abs(r - witness[0][1])))

    if ported_stale:
        print()
        print("  POINTERS AT PORTED FUNCTIONS: %d" % len(ported_stale))
        print("  tools/asm.py fills a ported function's original bytes with 0xCC, so dispatching")
        print("  through one of these executes int 3.")
        for addr, r, name in ported_stale[:20]:
            print("    0x%08X still points at 0x%08X, which is now 0xCC fill" % (addr, r))
            print("        %s" % name)

    print()
    print("  FAIL: a pointer kept its retail value while its section moved")
    return 1


if __name__ == "__main__":
    sys.exit(main())
