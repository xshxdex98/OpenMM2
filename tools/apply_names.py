# -*- coding: utf-8 -*-
"""Apply recovered 1999 field names onto the layouts we already trust.

Several recoveries turn up real field names - `datParser::AddValue` registrations state them
outright, and debug format strings print them. Those names are worth having: `Flags` and
`NumLanes` tell a porter what a field is for, `field_2C` tells them only where it lives.

This is deliberately the SAFEST possible use of those recoveries. It never adds a member, never
removes one, and never changes an offset or a size. It only renames a member that is already in
`data/layouts.json`, at an offset the recovery agrees on exactly, whose current name is a
placeholder we generated. So it cannot affect `sizeof`, cannot affect `check_size`, and cannot
move a field - the failure modes that make merging a sparse layout dangerous simply do not apply.

Two rules keep it honest:

  - Only `field_XX` placeholders are replaced. A name already recovered from the 1999 symbols is
    provenance and is never overwritten by an inference.
  - A recovered name landing on an offset that is NOT a member start is reported, not applied.
    That is a disagreement between two sources and it deserves a human, not a silent rename.
"""
import argparse
import glob
import json
import os
import re
import sys
from collections import OrderedDict

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)

LAYOUTS = os.path.join(ROOT, "data", "layouts.json")

PLACEHOLDER = re.compile(r"^field_[0-9A-Fa-f]+$")


def recoveries():
    """Every layouts_from_*.json, newest last so later sources win ties."""
    out = []
    for path in sorted(glob.glob(os.path.join(ROOT, "data", "layouts_from_*.json"))):
        with open(path, encoding="utf-8") as f:
            data = json.load(f)
        out.append((os.path.basename(path), data.get("layouts", data)))
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--write", action="store_true", help="apply; without it, nothing is modified")
    args = ap.parse_args()

    with open(LAYOUTS, encoding="utf-8") as f:
        layouts = json.load(f)

    sources = recoveries()
    if not sources:
        print("no data/layouts_from_*.json to apply")
        return 0

    print("sources: %s" % ", ".join(name for name, _ in sources))

    renamed = 0
    conflicts = []
    skipped_real = 0
    examples = []

    for source, recovered in sources:
        for cls, info in recovered.items():
            if cls not in layouts or not isinstance(info, dict):
                continue

            known = layouts[cls].get("members") or []
            by_offset = {m.get("offset"): m for m in known}

            for m in info.get("members") or []:
                name = m.get("name")
                offset = m.get("offset")

                if not name or offset is None:
                    continue
                if PLACEHOLDER.match(name):
                    continue  # the recovery has no name either

                # A vtable pointer is not a field. C++ emits it implicitly for any class with
                # virtuals, so declaring one explicitly gives the object TWO and a wrong sizeof.
                # genheaders already drops offset 0 on a polymorphic class; renaming field_0 to
                # "vtable" would only have carried it past that filter, which spells it vTable.
                if offset == 0 and name.lower() in ("vtable", "vfptr", "vptr", "vtbl", "__vftable"):
                    continue

                target = by_offset.get(offset)

                if target is None:
                    # The recovery names an offset our layout has no member at. Two sources
                    # disagreeing about the shape of a class is worth a look, not a silent fix.
                    conflicts.append((source, cls, offset, name))
                    continue

                current = target.get("name") or ""

                if not PLACEHOLDER.match(current):
                    if current != name:
                        skipped_real += 1
                    continue

                target["name"] = name
                target["name_source"] = source
                renamed += 1

                if len(examples) < 12:
                    examples.append("%s::%s -> %s (0x%X)" % (cls, current, name, offset))

    print()
    print("renamed placeholders          : %d" % renamed)
    print("kept (already a real name)    : %d" % skipped_real)
    print("offsets with no member here   : %d" % len(conflicts))

    if examples:
        print()
        for e in examples:
            print("   %s" % e)

    if conflicts:
        print()
        print("recovered names at offsets our layout has no member at:")
        for source, cls, offset, name in conflicts[:12]:
            print("   %-28s 0x%-5X %-24s (%s)" % (cls, offset, name, source))
        if len(conflicts) > 12:
            print("   ... and %d more" % (len(conflicts) - 12))

    if not args.write:
        print("\ndry run; pass --write to apply")
        return 0

    with open(LAYOUTS, "w", encoding="utf-8") as f:
        json.dump(layouts, f, indent=1)

    print("\nwrote %s" % LAYOUTS)
    return 0


if __name__ == "__main__":
    sys.exit(main())
