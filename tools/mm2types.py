# -*- coding: utf-8 -*-
"""Mine the `MM2::` namespace in the IDB type dump for sizes, offsets and vtable slot names.

The dump carries two type sets for the same game. The plain names (`struct aiPath`) are the IDB's
own recovered layouts, with full member lists. The `MM2::`-prefixed ones come from MM2Hook, the
community mod framework, and look different:

    struct MM2::aiVehiclePhysics  // sizeof=0x9770
    {
        MM2::aiVehicle ;                            <- base class, embedded at 0
        hook::Field<38544,float> _brake;            <- offset is the TEMPLATE ARGUMENT
    };

The `/* 0x00NN */` comment on those lines is the index within the hook wrapper, not the field
offset, so reading it would be wrong by a wide margin. The real offset is the first template
parameter.

This set is sparse on member names but has `sizeof` for 152 classes the IDB has no plain layout
for, which is exactly what `check_size` needs. It also carries 156 `_vtbl` structs naming virtual
methods in slot order - an independent check on the hierarchy recovered from vftable bytes.
"""
import json
import os
import re
from collections import OrderedDict

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)

TYPES = os.environ.get("MM2_TYPES",
                       os.path.join(ROOT, "MM2_RE_KIT", "MM2_PSEUDOCODE", "_ALL_TYPES.h"))
OUT = os.path.join(ROOT, "data", "mm2types.json")

HEAD = re.compile(r"^struct MM2::(\w+)\s+//\s+sizeof=0x([0-9A-Fa-f]+)\s*$")
FIELD = re.compile(r"hook::Field<\s*(\d+)\s*,\s*(.+?)\s*>\s+(\w+)\s*;")
BASE = re.compile(r"^\s*/\*\s*0x0000\s*\*/\s*MM2::(\w+)\s*;\s*$")
VSLOT = re.compile(r"^\s*/\*\s*0x([0-9A-Fa-f]+)\s*\*/\s*.*?\)\s*(\w+)\s*;\s*$")
# The IDB's own layout style: the offset really is in the comment here. Anchored with four
# hex digits so it cannot match the VSLOT form above, whose comments are slot offsets.
PLAIN = re.compile(r"^\s*/\*\s*0x([0-9A-Fa-f]{4})\s*\*/\s+(.+?)\s+(\w+)\s*(\[\d+\])?\s*;\s*$")

# THE KIT MISSPELLS ONE CLASS, and the misspelling is faithfully recovered rather than introduced
# here: `MM2::vehWeelCheap` is how the community IDB's MM2Hook-derived type set writes it, and it
# appears that way in _ALL_TYPES.h and in the kit's own inventory.
#
# The retail binary disagrees, and the linker map is the authority on a name. All eight mangled
# symbols in data/symbols.json spell it vehWheelCheap - ??0vehWheelCheap@@QAE@XZ at 0x0059D790,
# ?Update@vehWheelCheap@@UAEXXZ at 0x0059D970 - and vehWeelCheap appears in no symbol at all. The
# kit's Ghidra side agrees; only the MM2Hook side is wrong.
#
# Corrected HERE, where the kit is read, rather than in a data file. A data patch would be undone
# the next time anyone runs mm2types.py, and fixing it at the source keeps sizes, fields, bases and
# every member type string consistent in one place - which matters because genheaders.size_of()
# reads MM2T["sizes"] directly. Left uncorrected it produced TWO headers for one class: an empty
# vehwheelcheap.h holding the methods, and a vehweelcheap.h holding the layout under a name the
# linker has never heard of.
TYPOS = {"vehWeelCheap": "vehWheelCheap"}


def despell(text):
    """The kit's spelling of a class name, corrected to the binary's."""
    for wrong, right in TYPOS.items():
        text = text.replace(wrong, right)
    return text


def parse():
    sizes = OrderedDict()
    fields = OrderedDict()
    bases = OrderedDict()
    vtables = OrderedDict()

    cur = None
    cur_vtbl = None

    with open(TYPES, encoding="utf-8", errors="replace") as f:
        for line in f:
            line = line.rstrip("\n")

            m = HEAD.match(line)
            if m:
                name, size = despell(m.group(1)), int(m.group(2), 16)

                if name.endswith("_vtbl"):
                    cur, cur_vtbl = None, name[:-5]
                    vtables[cur_vtbl] = []
                else:
                    cur, cur_vtbl = name, None
                    sizes[name] = size
                    fields.setdefault(name, [])
                continue

            if line.startswith("};"):
                cur = cur_vtbl = None
                continue

            if cur_vtbl is not None:
                m = VSLOT.match(line)
                if m:
                    vtables[cur_vtbl].append(
                        OrderedDict(slot=int(m.group(1), 16) // 4, name=m.group(2)))
                continue

            if cur is None:
                continue

            m = BASE.match(line)
            if m and despell(m.group(1)) != cur:
                bases[cur] = despell(m.group(1))
                continue

            m = FIELD.search(line)
            if m:
                fields[cur].append(OrderedDict(
                    offset=int(m.group(1)), type=despell(m.group(2)), name=m.group(3)))
                continue

            # The IDB's own recovered layouts, written with the offset in the comment:
            #
            #     /* 0x002C */ float LeftPositionPtrDistSqr;
            #     /* 0x004C */ MM2::Vector3 * LeftPositionPtr;
            #
            # Only the hook::Field form above was being read, so these were skipped entirely - 386
            # structs and 4,582 members, 4,328 of them carrying a REAL 1999 name rather than
            # field_NN. That is the most valuable thing in the kit and it was going unused, which
            # is why classes like Aud3DObject emitted `i32 field_1C` for what the IDB knows as
            # `float MaxDropOffSqr`.
            #
            # Unlike the hook::Field form, HERE THE COMMENT IS THE REAL OFFSET. The warning at the
            # top of this file applies only to the MM2Hook wrapper, where the comment is an index
            # into the wrapper and the offset is the template argument.
            m = PLAIN.match(line)
            if m:
                offset = int(m.group(1), 16)
                ftype, name, array = despell(m.group(2).strip()), m.group(3), m.group(4)
                if array:
                    ftype += array
                fields[cur].append(OrderedDict(offset=offset, type=ftype, name=name))

    return sizes, fields, bases, vtables


def main():
    sizes, fields, bases, vtables = parse()

    print("MM2:: namespace:")
    print("  classes with a size : %d" % len(sizes))
    print("  hook::Field members : %d" % sum(len(v) for v in fields.values()))
    print("  base classes        : %d" % len(bases))
    print("  vtables             : %d (%d named slots)"
          % (len(vtables), sum(len(v) for v in vtables.values())))

    data = OrderedDict(sizes=sizes, fields=fields, bases=bases, vtables=vtables)

    with open(OUT, "w", encoding="utf-8") as f:
        json.dump(data, f, indent=1)

    print("wrote %s" % OUT)

    # Cross-check the bases against the hierarchy recovered from vftable bytes. These are wholly
    # independent - one is MM2Hook's hand-maintained type set, the other is read out of the
    # binary - so agreement is real evidence and disagreement is worth knowing about.
    hier_path = os.path.join(ROOT, "data", "hierarchy.json")
    if not os.path.exists(hier_path):
        return

    with open(hier_path, encoding="utf-8") as f:
        hier = json.load(f)

    agree = differ = 0
    examples = []

    for cls, base in bases.items():
        ours = (hier.get(cls) or {}).get("base")
        if not ours:
            continue

        if ours == base:
            agree += 1
        else:
            differ += 1
            if len(examples) < 10:
                examples.append("%s: vftables say %s, MM2Hook says %s" % (cls, ours, base))

    print("\n  base class agreement vs vftable recovery: %d agree, %d differ" % (agree, differ))
    for e in examples:
        print("    %s" % e)


if __name__ == "__main__":
    main()
