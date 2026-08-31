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


# CARRIER PRECEDENCE, STATED RATHER THAN INHERITED FROM THE FILENAMES.
#
# This loop used to iterate sorted(glob(...)), so alphabetical order WAS the precedence rule, and
# it favoured the weaker source. layouts_from_ctors.json sorts before layouts_round4.json, inserts
# first, and round4 is then refused against it - costing mmJaxis seven recovered 1999 names and
# retyping six floats as i32. Two more classes, camTrackCS and mmTimer, are correct today only
# because `c` sorts before `k`, and for those the loss would be a SIZE.
#
# Ranked by evidential strength, from what each file says about itself:
#
#   1. hand-read off the binary - a decoded instruction stream, not a tool's output. Within this
#      tier order is chronological supersession and must be written down, not derived.
#   2. layouts_kit.json - the recovered IDA/MM2Hook type library. Authoritative for NAMES and
#      TYPES, and the only source with 1999 field names for whole classes.
#   3. layouts_from_datparser.json - names from datParser::AddRecord string literals, genuine 1999
#      artifacts, but partial by construction so it can only ever contribute names.
#   4. layouts_from_serialization.json - Hex-Rays derived, with its own internal evidence ranking.
#   5. layouts_from_ctors.json - constructor machine code. Recovers OFFSETS and SIZES and nothing
#      else: every field comes out i32 field_NN whatever it holds, as its own _note says.
#
# THE LADDER INVERTS FOR SIZE. layouts_from_ctors outranks layouts_kit there, and provably: the
# kit records camTrackCS as 0x110, which is merely sizeof(camCarCS), its base, and mmTimer as
# 0x18, which is sizeof(asNode). mmHUD constructs three mmTimers at stride 0x30, not 0x18. A
# ctors size beats a kit size; a kit member list beats a ctors member list. Collapsing the two
# into one ladder regresses camTrackCS to 0x110 and mmTimer to 0x18.
ORDER = [
    "layouts_round5.json",
    "layouts_corrections.json",
    "layouts_round4.json",
    "layouts_gfx_round3.json",
    "layouts_gfx_round2.json",
    "layouts_gfx_recovered.json",
    "layouts_vector7_round3.json",
    "layouts_short_fix.json",
    "layouts_kit.json",
    "layouts_from_datparser.json",
    "layouts_from_serialization.json",
    "layouts_from_ctors.json",
]

# Sources whose SIZE outranks the kit's, per the inversion above.
SIZE_OVER_KIT = {"layouts_from_ctors.json"}


def carriers():
    """Every data/layouts_*.json, in declared precedence order.

    A carrier on disk that nobody has ranked is an error rather than something to place by its
    filename - that silent placement is the whole defect this replaces.
    """
    found = {os.path.basename(x) for x in glob.glob(os.path.join(ROOT, "data", "layouts_*.json"))}
    found.discard("layouts.json")

    unranked = sorted(found - set(ORDER))
    if unranked:
        sys.exit("unranked layout carriers: %s\n"
                 "Add each to ORDER in tools/merge_layouts.py, deliberately." % ", ".join(unranked))

    return [(n, os.path.join(ROOT, "data", n)) for n in ORDER if n in found]


def rank(name):
    return ORDER.index(name) if name in ORDER else len(ORDER)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--write", action="store_true", help="apply; without it nothing is modified")
    args = ap.parse_args()

    with open(LAYOUTS, encoding="utf-8") as f:
        layouts = json.load(f)

    before = len(layouts)
    merged, rejected = [], []

    # Which carrier supplied each entry, so a later one can be compared against it by RANK rather
    # than by which happened to run first.
    owner = {}

    for name, path in carriers():

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
                    # Rank applies here too. Two carriers can both be hand-reads, and without this
                    # the later one simply overwrote the earlier regardless of which supersedes
                    # which - layouts_corrections silently replacing layouts_round5's account of
                    # dgUnhitMtxBangerInstance, losing the evidence that its 16-byte gap is NOT
                    # attested by any instruction.
                    if cls in owner and rank(name) >= rank(owner[cls]):
                        rejected.append((cls, name, "outranked by %s" % owner[cls]))
                        continue
                    if existing.get("size") != info.get("size"):
                        rejected.append((cls, name, "correction says sizeof=0x%X, layouts.json "
                                         "says 0x%X" % (info.get("size") or 0,
                                                        existing.get("size") or 0)))
                        continue
                    layouts[cls] = info
                    owner[cls] = name
                    merged.append((cls, name, info.get("size"), len(info.get("members") or [])))
                    continue

                # AN INCUMBENT FROM ANOTHER CARRIER IS NOT THE IDA TYPE LIBRARY, and rule 1 was
                # refusing against it as though it were. Compare by declared rank instead. The
                # size lock below still applies, so this can improve names and types and can never
                # move a size.
                if cls in owner:
                    if rank(name) >= rank(owner[cls]):
                        rejected.append((cls, name, "outranked by %s" % owner[cls]))
                        continue
                    if existing.get("size") != info.get("size"):
                        rejected.append((cls, name, "%s says sizeof=0x%X, %s says 0x%X"
                                         % (name, info.get("size") or 0, owner[cls],
                                            existing.get("size") or 0)))
                        continue

                    members = info.get("members") or []
                    ok, why = tiles(members, info.get("size"))
                    if not ok:
                        rejected.append((cls, name, why))
                        continue

                    entry = OrderedDict(existing)
                    entry["members"] = members
                    entry["source"] = name
                    if info.get("evidence"):
                        entry["evidence"] = info["evidence"]
                    layouts[cls] = entry
                    owner[cls] = name
                    merged.append((cls, name, info.get("size"), len(members)))
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

            merged_entry = OrderedDict(
                kind=info.get("kind", "struct"),
                size=size,
                members=members,
                source=name,
                confidence="CONFIRMED",
            )

            # KEEP THE EVIDENCE. A carrier that documents where its layout came from holds the only
            # record of why the class looks the way it does, and rebuilding the entry from five
            # fixed keys threw it away - so a hand-read landing here lost its provenance, while one
            # that merely CORRECTS an existing entry kept it. That asymmetry is an accident of which
            # branch runs, not a decision anyone made.
            if info.get("evidence"):
                merged_entry["evidence"] = info["evidence"]

            layouts[cls] = merged_entry
            owner[cls] = name
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
