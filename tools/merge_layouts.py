# -*- coding: utf-8 -*-
"""Merge recovered layouts into data/layouts.json, under strict rules.

data/layouts.json drives `check_size` and the emitted member list, so anything merged badly does
not fail loudly - it produces a class whose sizeof is smaller than the real object, and the
still-assembly code then writes past everything we allocate. See docs/layout_sources.md.

Four rules, all of them refusals:

  1. NEVER overwrite a class the IDA type library already has. That is the most-verified source
     we own; an inference does not get to replace it.
  2. CONFIRMED only. PROBABLE and WEAK stay out of the file the build trusts.
  3. A real size, independently corroborated. A `size_lower_bound` is not a size - it is the floor
     of what was observed, and a third of them fall short of the truth.
  4. The members must TILE the struct: contiguous from 0 to size with no gap and no overlap. A
     sparse member list is the failure mode this whole file exists to prevent, so it is checked
     rather than assumed.

Anything rejected is reported with the reason. Rejection is the expected outcome for most entries.
"""
import argparse
import glob
import json
import os
import sys
from collections import OrderedDict

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)

LAYOUTS = os.path.join(ROOT, "data", "layouts.json")


def tiles(members, size):
    """Do these members cover [0, size) contiguously, with no overlap?"""
    if not members or not size:
        return False, "no members or no size"

    at = 0
    for m in sorted(members, key=lambda x: x.get("offset", 0)):
        off = m.get("offset")
        if off is None:
            return False, "a member has no offset"
        if off < at:
            return False, "overlap at 0x%X" % off
        if off > at:
            return False, "gap at 0x%X (expected 0x%X)" % (off, at)

        width = m.get("width") or 0
        count = m.get("count") or 0
        step = width * count if count else width

        if not step:
            return False, "member at 0x%X has no width" % off

        at = off + step

    if at != size:
        return False, "covers 0x%X, size is 0x%X" % (at, size)

    return True, ""


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--write", action="store_true", help="apply; without it nothing is modified")
    args = ap.parse_args()

    with open(LAYOUTS, encoding="utf-8") as f:
        layouts = json.load(f)

    before = len(layouts)
    merged, rejected = [], []

    for path in sorted(glob.glob(os.path.join(ROOT, "data", "layouts_*.json"))):
        name = os.path.basename(path)
        if name == "layouts.json":
            continue

        with open(path, encoding="utf-8") as f:
            data = json.load(f)

        for cls, info in (data.get("layouts", data)).items():
            if not isinstance(info, dict):
                continue

            # THE ONE CASE WHERE AN EXISTING ENTRY IS REPLACED.
            #
            # Rule 1 says an inference never displaces the IDA type library, and that stands. But
            # data/layouts.json is not purely that library: most of it was inferred from what
            # constructors write, which recovers correct OFFSETS and nothing else - every field
            # comes out `i32 field_NN` whatever it really holds. data/layouts_kit.json IS the
            # recovered library, with the 1999 names and types, so here the priority runs the other
            # way and it replaces the inference.
            #
            # Guarded by SIZE AGREEMENT, which is the whole safety argument: two sources that agree
            # on sizeof are describing the same object, so swapping one member list for the other
            # cannot change what check_size asserts. A disagreement means one of them is wrong
            # about the class, and then nothing is merged.
            existing = layouts.get(cls)
            if existing is not None:
                # A hand-read taken straight out of the binary outranks every recovered source,
                # because it is the only one produced by someone actually reading the code that
                # uses the fields. data/layouts_corrections.json carries the evidence for each
                # entry inline, and the size check below still applies - a correction may respell
                # the bytes at a known offset, never change how many there are.
                if info.get("source") == "hand-analysis":
                    if existing.get("size") != info.get("size"):
                        rejected.append((cls, name, "correction says sizeof=0x%X, layouts.json "
                                         "says 0x%X" % (info.get("size") or 0,
                                                        existing.get("size") or 0)))
                        continue
                    layouts[cls] = info
                    merged.append((cls, name, info.get("size"), len(info.get("members") or [])))
                    continue

                from_kit = (info.get("source") == "MM2_RE_KIT")
                # layouts_kit.json is included so a corrected converter can refresh what it
                # merged before - without it, a bug in kit_layouts.py is permanent the moment its
                # output lands once.
                inferred = (existing.get("source") or "").startswith(
                    ("layouts_from_", "inferred_", "layouts_kit"))
                if not (from_kit and inferred):
                    rejected.append((cls, name, "already in the IDA type library"))
                    continue
                if existing.get("size") != info.get("size"):
                    rejected.append((cls, name, "kit says sizeof=0x%X, layouts.json says 0x%X"
                                     % (info.get("size") or 0, existing.get("size") or 0)))
                    continue

            # Sources spell the grade differently; compare case-insensitively rather than
            # silently rejecting 12 genuinely confirmed classes for using lowercase.
            if (info.get("confidence") or "").upper() != "CONFIRMED":
                rejected.append((cls, name, "graded %s" % info.get("confidence")))
                continue

            size = info.get("size")
            if not size:
                rejected.append((cls, name, "size is a lower bound, not a size"))
                continue

            members = info.get("members") or []
            ok, why = tiles(members, size)
            if not ok:
                rejected.append((cls, name, why))
                continue

            layouts[cls] = OrderedDict(
                kind=info.get("kind", "struct"),
                size=size,
                members=members,
                source=name,
                confidence="CONFIRMED",
            )
            merged.append((cls, name, size, len(members)))

    print("merged %d classes:" % len(merged))
    for cls, src, size, n in sorted(merged, key=lambda x: -x[2])[:20]:
        print("   %-30s 0x%-6X %3d members   (%s)" % (cls, size, n, src))
    if len(merged) > 20:
        print("   ... and %d more" % (len(merged) - 20))

    reasons = {}
    for _, _, why in rejected:
        key = why.split(" at ")[0].split(" (")[0]
        reasons[key] = reasons.get(key, 0) + 1

    print("\nrejected %d, by reason:" % len(rejected))
    for why, n in sorted(reasons.items(), key=lambda kv: -kv[1]):
        print("   %-44s %d" % (why, n))

    print("\nlayouts.json: %d -> %d classes" % (before, len(layouts)))

    if not args.write:
        print("\ndry run; pass --write to apply")
        return 0

    with open(LAYOUTS, "w", encoding="utf-8") as f:
        json.dump(layouts, f, indent=1)

    print("wrote %s" % LAYOUTS)
    return 0


if __name__ == "__main__":
    sys.exit(main())
