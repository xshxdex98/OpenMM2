# -*- coding: utf-8 -*-
"""Turn the IDB's own recovered layouts into data/layouts_kit.json.

data/layouts.json currently carries a mixture: a few classes typed by hand, and a large number
inferred from what their constructors write - which yields correct OFFSETS and nothing else. That
is why Aud3DObject emitted

    i32 field_14;   // 0x014
    i32 field_18;   // 0x018
    i32 field_1C;   // 0x01C

for what the IDB knows as

    float MinDropOffSqr;
    float MaxDropOffSqr;
    float field_1c;

The names and types were in MM2_RE_KIT all along; tools/mm2types.py simply never read that form of
member declaration. It does now, and this converts what it produces into the shape merge_layouts
expects.

WHY TYPES MATTER AND NOT JUST NAMES. A constructor that stores -1.0f into a float field is one
assignment. Against a field declared i32 it is a bit-cast, which is both wrong to read and wrong to
write - and there is no way to tell from the offsets alone which of two adjacent 4-byte fields is
a float. Porting a class cleanly needs the type, so an inferred layout is not a substitute for the
recovered one.

WIDTH IS DERIVED, NOT LOOKED UP. Each member's width is the distance to the next member, and the
last one runs to the end of the struct. That needs no table of type sizes, and it makes the tiling
merge_layouts insists on true by construction rather than by hope. Where a member's declared type
is narrower than the space before the next one - a `bool` with three bytes of padding after it -
the remainder is emitted as an explicit padding array, because a member list with a hole in it is
exactly what merge_layouts refuses.
"""
import json
import os
import re
import sys
from collections import OrderedDict

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)

MM2TYPES = os.path.join(ROOT, "data", "mm2types.json")
OUT = os.path.join(ROOT, "data", "layouts_kit.json")

# The IDB's spelling -> the arts primitive vocabulary in core/primitives.h. Ordered because the
# longer spellings must be tried before the shorter ones they contain.
PRIMITIVES = [
    ("unsigned __int64", "u64"),
    ("unsigned __int32", "u32"),
    ("unsigned __int16", "u16"),
    ("unsigned __int8", "u8"),
    ("unsigned short", "u16"),
    ("unsigned char", "u8"),
    ("unsigned int", "u32"),
    ("signed char", "i8"),
    ("__int64", "i64"),
    ("__int32", "i32"),
    ("__int16", "i16"),
    ("__int8", "i8"),
    ("double", "f64"),
    ("float", "f32"),
    ("short", "i16"),
    ("char", "i8"),
    ("bool", "bool"),
    ("int", "i32"),
    ("void", "void"),
]

# The natural width of each, so a member can be checked against the space it actually occupies.
WIDTH = {"u64": 8, "i64": 8, "f64": 8, "u32": 4, "i32": 4, "f32": 4, "u16": 2, "i16": 2,
         "u8": 1, "i8": 1, "bool": 1}

ARRAY = re.compile(r"^(.*?)\[(\d+)\]$")


def map_type(text):
    """`MM2::Vector3 *` -> `Vector3*`, `unsigned __int8` -> `u8`, `float` -> `f32`."""
    t = text.strip()
    t = t.replace("MM2::", "")
    t = re.sub(r"\b(struct|class|union|enum)\s+", "", t)
    stars = t.count("*")
    t = t.replace("*", "").strip()

    for ida, arts in PRIMITIVES:
        if t == ida:
            t = arts
            break

    return t + "*" * stars


def convert(cls, fields, size):
    """One class's member list, or (None, reason) if it cannot be trusted."""
    members = [f for f in fields if f.get("name") and f.get("offset") is not None]
    if not members:
        return None, "no members"

    members.sort(key=lambda f: f["offset"])

    # A duplicate offset means two names for the same storage - a union, or a mistake. Either way
    # the list no longer tiles, and guessing which one to keep is not this tool's business.
    seen = set()
    for f in members:
        if f["offset"] in seen:
            return None, "duplicate offset 0x%X" % f["offset"]
        seen.add(f["offset"])

    if members[0]["offset"] != 0:
        return None, "first member is at 0x%X, not 0" % members[0]["offset"]
    if members[-1]["offset"] >= size:
        return None, "member at 0x%X is outside sizeof=0x%X" % (members[-1]["offset"], size)

    out = []
    for i, f in enumerate(members):
        off = f["offset"]
        end = members[i + 1]["offset"] if i + 1 < len(members) else size
        space = end - off
        if space <= 0:
            return None, "member at 0x%X has no space" % off

        name = f["name"]
        ctype = map_type(f["type"])

        # The vtable pointer is spelled the way the rest of data/layouts.json spells it, so a
        # reader does not meet two conventions for the same thing.
        if off == 0 and name.upper() in ("VTBL", "VFTABLE", "VTABLE"):
            name, ctype = "vtable", "void*"

        m = ARRAY.match(ctype)
        if m:
            base, count = map_type(m.group(1)), int(m.group(2))
            natural = WIDTH.get(base, 4) * count
        else:
            natural = 4 if ctype.endswith("*") else WIDTH.get(ctype, 4)

        if natural > space:
            return None, "%s is %d bytes but only %d are free at 0x%X" % (name, natural, space, off)

        out.append(OrderedDict(name=name, offset=off, type=ctype, width=natural, src="kit"))

        # Trailing padding, so the list tiles. A hole is what merge_layouts refuses, and rightly.
        if natural < space:
            out.append(OrderedDict(name="pad_%X" % (off + natural), offset=off + natural,
                                   type="u8[%d]" % (space - natural), width=space - natural,
                                   src="kit-padding"))

    return out, None


def main():
    if not os.path.exists(MM2TYPES):
        sys.exit("no %s - run tools/mm2types.py first" % MM2TYPES)

    with open(MM2TYPES, encoding="utf-8") as f:
        kit = json.load(f)

    sizes = kit.get("sizes") or {}
    fields = kit.get("fields") or {}

    # What the binary actually hands to operator new, from tools/verify_sizes.py --json. The kit's
    # sizeof is a recovered value and can be wrong: it says vehSiren is 0x164 where every call site
    # allocates 0x44. Believing it would declare a class three hundred bytes larger than the object
    # really is, and the build's own size gate says why that matters - a ported new() would
    # under-allocate and the still-assembly code would write past the end.
    #
    # So the kit is checked against the binary rather than trusted, using the same evidence the
    # gate uses. Regenerate with:  py tools/verify_sizes.py --json > data/measured_sizes.json
    measured = {}
    measured_path = os.path.join(ROOT, "data", "measured_sizes.json")
    if os.path.exists(measured_path):
        with open(measured_path, encoding="utf-8") as f:
            measured = json.load(f).get("measured") or {}
    else:
        print("WARNING: no data/measured_sizes.json - kit sizes will not be cross-checked")

    layouts, rejected = OrderedDict(), []
    for cls in sorted(fields):
        size = sizes.get(cls)
        if not size:
            rejected.append((cls, "no sizeof"))
            continue

        real = measured.get(cls)
        if real is not None and real != size:
            rejected.append((cls, "kit says sizeof=0x%X, the binary allocates 0x%X"
                             % (size, real)))
            continue

        members, why = convert(cls, fields[cls], size)
        if members is None:
            rejected.append((cls, why))
            continue

        layouts[cls] = OrderedDict(kind="class", size=size, members=members,
                                   source="MM2_RE_KIT", confidence="CONFIRMED")

    named = sum(1 for c in layouts for m in layouts[c]["members"]
                if not m["name"].startswith(("field_", "pad_")))
    print("classes converted : %d" % len(layouts))
    print("members           : %d (%d with a recovered name)"
          % (sum(len(v["members"]) for v in layouts.values()), named))
    print("rejected          : %d" % len(rejected))

    counts = {}
    for _, why in rejected:
        key = re.sub(r"0x[0-9A-F]+|\d+", "N", why)
        counts[key] = counts.get(key, 0) + 1
    for why, n in sorted(counts.items(), key=lambda kv: -kv[1])[:8]:
        print("    %-52s %d" % (why, n))

    with open(OUT, "w", encoding="utf-8") as f:
        json.dump(OrderedDict(layouts=layouts), f, indent=1)
    print("wrote %s" % OUT)


if __name__ == "__main__":
    main()
