# -*- coding: utf-8 -*-
"""Recover the class hierarchy from vftable contents.

midtown2.exe was built without RTTI (`/GR-`), so there are no type descriptors to read and the
linker map says nothing about inheritance. The vftables do.

A derived class's vftable begins with its base's layout: every slot the derived class did not
override still holds the BASE's function pointer. And the map tells us which class owns every
address in the binary. So reading a vftable and looking up the owner of each slot yields the set
of classes that contributed to it - which is exactly the ancestry.

Slot 0 is the most telling: the class owning the earliest non-overridden slot is the deepest base.
"""
import json
import os
import struct
import sys
from collections import Counter, OrderedDict

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from pe import IMAGE_BASE, load_sections, to_offset

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)

SYMBOLS = os.path.join(ROOT, "data", "symbols.json")
OUT = os.path.join(ROOT, "data", "hierarchy.json")
EXE = os.environ.get("MM2_EXE", r"C:\Users\xshxd\OneDrive\Desktop\PC Games\MM2\midtown2.exe")

MAX_SLOTS = 512  # far beyond any real vtable; a runaway read means the terminator logic failed



def main():
    with open(SYMBOLS, encoding="utf-8") as f:
        syms = json.load(f)

    data, sections = load_sections(EXE)

    # address -> owning class, for every function in the binary
    owner = {}
    for s in syms:
        if s["code"] and s.get("class"):
            owner[s["rva"]] = s["class"].split("::")[0]

    code_lo = min((s["rva"] for s in syms if s["code"]), default=0)
    code_hi = max((s["rva"] + (s["size"] or 0) for s in syms if s["code"]), default=0)

    vfts = [s for s in syms if s["kind"] == "vftable" and s.get("class")]
    vfts.sort(key=lambda s: s["rva"])

    # Vftables sit back to back in .rdata, and the next one's entries are themselves valid
    # function pointers - so "stop at the first non-function" runs straight into the neighbour.
    # The next vftable symbol is a hard ceiling.
    ceiling = {}
    for i, v in enumerate(vfts):
        nxt = vfts[i + 1]["rva"] if i + 1 < len(vfts) else None
        ceiling[v["rva"]] = (nxt - v["rva"]) // 4 if nxt else MAX_SLOTS

    print("reading %d vftables" % len(vfts))

    result = OrderedDict()
    unresolved = 0

    for v in vfts:
        cls = v["class"].split("::")[0]
        off = to_offset(sections, v["rva"] - IMAGE_BASE)
        if off is None:
            continue

        limit = min(ceiling.get(v["rva"], MAX_SLOTS), MAX_SLOTS)

        slots = []
        for i in range(limit):
            raw = struct.unpack_from("<I", data, off + i * 4)[0]

            # Anything pointing into .text is a slot. An entry with no symbol is still real - a
            # pure virtual resolves to _purecall, and thunks are not in the map - so only a value
            # outside the code section ends the table.
            if not (code_lo <= raw < code_hi):
                break

            slots.append((raw, owner.get(raw)))

        contributors = Counter(o for _, o in slots if o)
        others = [c for c in contributors if c != cls]

        # The owner of the earliest non-overridden slot is the DEEPEST ancestor, not the
        # immediate parent - asCamera's slot 0 belongs to asCullable even though asCamera derives
        # from asNode, which derives from asCullable in turn. C++ needs the immediate parent, so
        # first_base is kept only as evidence and the parent is resolved in a second pass, once
        # every class's vtable length is known.
        first_base = None
        for _, o in slots:
            if o and o != cls:
                first_base = o
                break

        if not others:
            unresolved += 1

        result[cls] = OrderedDict(
            vftable_rva=v["rva"],
            slots=len(slots),
            own_slots=contributors.get(cls, 0),
            inherited=OrderedDict(sorted(((c, n) for c, n in contributors.items() if c != cls),
                                         key=lambda kv: -kv[1])),
            first_base=first_base,
        )

    # Second pass: pick the IMMEDIATE parent out of the ancestors contributing slots.
    #
    # Neither "owns the earliest slot" nor "has the longest vtable" works. The first returns the
    # deepest ancestor (asCamera's slot 0 belongs to asCullable, two levels up). The second fails
    # whenever a class overrides without adding virtuals, which leaves it the same vtable length
    # as its base. Contribution count fails too: aiCableCar derives from aiVehicle, but most of
    # its slots are aiObstacle's, because aiVehicle overrides only a few.
    #
    # What does hold is that the immediate parent is the MOST DERIVED of the candidates - the one
    # that has all the others among its own ancestors.
    for cls, r in result.items():
        cands = list(r["inherited"])
        best, best_score = None, -1

        for c in cands:
            inherited = result.get(c, {}).get("inherited", {})
            score = sum(1 for other in cands if other != c and other in inherited)

            if score > best_score:
                best, best_score = c, score
            elif score == best_score and best is not None:
                # Tie: prefer the one contributing more slots, which is the nearer of the two.
                if r["inherited"][c] > r["inherited"][best]:
                    best = c

        r["base"] = best or r["first_base"]

    with open(OUT, "w", encoding="utf-8") as f:
        json.dump(result, f, indent=1)

    sized = [r for r in result.values() if r["slots"]]
    print("wrote %s" % OUT)
    print("  vftables read      : %d" % len(result))
    print("  mean slots         : %.1f" % (sum(r["slots"] for r in sized) / max(len(sized), 1)))
    print("  with a known base  : %d" % sum(1 for r in result.values() if r["first_base"]))
    print("  no base found      : %d" % unresolved)

    print("\n  most common bases:")
    bases = Counter(r["first_base"] for r in result.values() if r["first_base"])
    for b, n in bases.most_common(15):
        print("    %-28s %3d" % (b, n))


if __name__ == "__main__":
    main()
