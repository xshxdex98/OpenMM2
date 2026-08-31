# -*- coding: utf-8 -*-
"""Parse MM2_RE_KIT/_ALL_TYPES.h into data/layouts.json.

A linker map lists functions and statics but never member offsets, so instance layout was the one
thing the map could not give us. The community IDB has it: 11,240 struct layouts with byte offsets
and partially recovered field names.

Two things this feeds:

  check_size(Class, 0xNNN)  - the guard that pins layout, currently 0x0 in every header
  members, in offset order  - which is mandatory, because the original code and the remaining asm
                              read them at fixed byte offsets

IDA's type vocabulary is not ours, so types are mapped to arts primitives BY WIDTH. That matters
more than it looks: a 1-byte IDA `bool` must not become `b32`, which is four bytes and would move
every field after it.
"""
import json
import os
import re
import sys
from collections import OrderedDict

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)

TYPES = os.environ.get("MM2_TYPES",
                       os.path.join(ROOT, "MM2_RE_KIT", "MM2_PSEUDOCODE", "_ALL_TYPES.h"))
OUT = os.environ.get("MM2_LAYOUTS_OUT", os.path.join(ROOT, "data", "layouts.json"))

HEAD = re.compile(r"^(struct|union)\s+(\S+)\s+//\s+sizeof=0x([0-9A-Fa-f]+)\s*$")
MEMBER = re.compile(r"^\s*/\*\s*0x([0-9A-Fa-f]+)\s*\*/\s*(.+?)\s*;\s*$")

# IDA spelling -> (arts primitive, width in bytes). Width is what keeps the layout intact.
SCALARS = {
    "unsigned __int8": ("u8", 1), "_BYTE": ("u8", 1), "uint8_t": ("u8", 1),
    "unsigned char": ("u8", 1), "BYTE": ("u8", 1), "UCHAR": ("u8", 1),
    "char": ("i8", 1), "__int8": ("i8", 1), "int8_t": ("i8", 1), "signed char": ("i8", 1),
    "bool": ("u8", 1), "_BOOL1": ("u8", 1),  # one byte in the original; never widen to b32
    "unsigned __int16": ("u16", 2), "WORD": ("u16", 2), "uint16_t": ("u16", 2),
    "unsigned short": ("u16", 2), "USHORT": ("u16", 2), "wchar_t": ("u16", 2),
    "__int16": ("i16", 2), "short": ("i16", 2), "int16_t": ("i16", 2), "_WORD": ("i16", 2),
    "unsigned __int32": ("u32", 4), "DWORD": ("u32", 4), "uint32_t": ("u32", 4),
    "unsigned int": ("u32", 4), "UINT": ("u32", 4), "ULONG": ("u32", 4),
    "unsigned long": ("u32", 4), "_DWORD": ("u32", 4), "_BOOL4": ("b32", 4),
    "int": ("i32", 4), "__int32": ("i32", 4), "int32_t": ("i32", 4), "long": ("i32", 4),
    "LONG": ("i32", 4), "HRESULT": ("i32", 4),
    "float": ("f32", 4), "double": ("f64", 8),
    "unsigned __int64": ("u64", 8), "uint64_t": ("u64", 8), "_QWORD": ("u64", 8),
    "__int64": ("i64", 8), "int64_t": ("i64", 8),
}

ARRAY = re.compile(r"^(.*?)\s*\[(\d+)\]$")


# IDA SOMETIMES OVER-QUALIFIES A TYPE WITH ITS OWN OWNER, REPEATEDLY. _ALL_TYPES.h:52801 spells
# mmGame's player pointer `struct mmGame::mmGame::mmGame::mmGame::mmPlayer *`, which is not a type
# and is not what the class holds - mmPlayer is a top-level class in the same subsystem.
#
# Requiring the qualifier to repeat is what keeps this from touching a genuine nested name: a real
# Owner::Nested appears once, and one occurrence is left alone.
REPEATED_SCOPE = re.compile(r"(\w+)::(?:::)+")


def unqualify(text):
    """Drop a scope qualifier that IDA repeated - see REPEATED_SCOPE."""
    return REPEATED_SCOPE.sub("", text)


def map_member(text):
    """`_BYTE[32] gap54` -> ('u8', 'gap54', 32). `aiPedestrian * Peds` -> ('aiPedestrian*', 'Peds', 0)."""
    text = text.strip()

    # An anonymous nested union is emitted with no member name at all.
    if not text or text.endswith(("union", "struct")):
        return None

    count = 0

    m = re.match(r"^(.*?)\[(\d+)\]\s+(\S+)$", text)
    if m:
        # `_BYTE[32] gap54` - IDA puts the dimension on the type
        base, count, name = m.group(1).strip(), int(m.group(2)), m.group(3)
    else:
        m = re.match(r"^(.*?)\s+(\S+)\[(\d+)\]$", text)
        if m:
            # `float Foo[4]` - dimension on the name
            base, name, count = m.group(1).strip(), m.group(2), int(m.group(3))
        else:
            parts = text.rsplit(" ", 1)
            if len(parts) != 2:
                return None
            base, name = parts[0].strip(), parts[1]

    # Pointers keep their pointee spelling; only the depth matters for width.
    stars = 0
    while base.endswith("*"):
        stars += 1
        base = base[:-1].strip()
    while name.startswith("*"):
        stars += 1
        name = name[1:]

    base = re.sub(r"^(struct|union|enum|const|volatile)\s+", "", base).strip()

    if stars:
        # Map the pointee too, so `WORD *` becomes `u16*` rather than keeping IDA's spelling.
        pointee = SCALARS[base][0] if base in SCALARS else base
        ctype, width = pointee + "*" * stars, 4  # 32-bit target
    elif base in SCALARS:
        ctype, width = SCALARS[base]
    else:
        ctype, width = base, None  # a struct member; width resolved later if we know the type

    if not re.match(r"^[A-Za-z_]\w*$", name):
        return None

    return ctype, name, count, width


# IDA's auto-generated member names - gap54, char18, dword30, pvoid80 - encode nothing but the
# offset, and half of them contradict the type they carry (`float char18`). Brick's convention for
# a member whose only known property is its offset is field_<HEXOFFSET>, so normalise to that. A
# name a human actually chose is left exactly as it is: it is recovered 1999 provenance.
AUTO_NAME = re.compile(
    r"^(gap|char|byte|word|dword|qword|float|double|pvoid|unk|off|stru|asc|xmmword|field)"
    r"_?([0-9A-Fa-f]+)$")


def normalise(name, offset):
    m = AUTO_NAME.match(name)

    if not m:
        return name

    try:
        if int(m.group(2), 16) != offset:
            return name  # the suffix is not this member's offset, so it may mean something
    except ValueError:
        return name

    return "field_%X" % offset


def parse(path):
    """{name: {kind, size, members: [...]}} for every struct and union in the dump."""
    out = OrderedDict()
    cur = None

    with open(path, encoding="utf-8", errors="replace") as f:
        for line in f:
            line = line.rstrip("\n")

            m = HEAD.match(line)
            if m:
                kind, name, size = m.group(1), m.group(2), int(m.group(3), 16)
                cur = OrderedDict(kind=kind, size=size, members=[])
                out[name] = cur
                continue

            if line.startswith("};"):
                cur = None
                continue

            if cur is None:
                continue

            m = MEMBER.match(line)
            if not m:
                continue

            offset = int(m.group(1), 16)
            parsed = map_member(m.group(2))

            if parsed is None:
                cur["members"].append(OrderedDict(
                    offset=offset, type=None, name=None, count=0, raw=m.group(2)))
                continue

            ctype, name, count, width = parsed
            name = normalise(name, offset)
            cur["members"].append(OrderedDict(
                offset=offset, type=ctype, name=name, count=count, width=width))

    return out


def main():
    if not os.path.exists(TYPES):
        raise SystemExit("not found: %s" % TYPES)

    types = parse(TYPES)
    print("parsed %d layouts from %s" % (len(types), os.path.basename(TYPES)))

    with open(os.path.join(ROOT, "data", "symbols.json"), encoding="utf-8") as f:
        syms = json.load(f)

    ours = set(s["class"].split("::")[0] for s in syms if s.get("class"))
    matched = {k: v for k, v in types.items() if k in ours}

    print("  classes we know about   : %d" % len(ours))
    print("  with a layout in the IDB: %d" % len(matched))
    print("  missing a layout        : %d" % len(ours - set(matched)))

    unnamed = sum(1 for v in matched.values() for m in v["members"] if not m.get("name"))
    fields = sum(len(v["members"]) for v in matched.values())
    gaps = sum(1 for v in matched.values() for m in v["members"]
               if (m.get("name") or "").startswith(("gap", "field_")))

    print("  members total           : %d" % fields)
    print("  still unnamed (gap/field): %d (%.0f%%)" % (gaps, 100.0 * gaps / max(fields, 1)))
    print("  unparsed member lines   : %d" % unnamed)

    # THIS FILE IS NOT A PURE FUNCTION OF _ALL_TYPES.h, AND THIS SCRIPT CANNOT REBUILD IT.
    #
    # data/layouts.json is a merge: the kit parsed here, plus every data/layouts_*.json carrier,
    # plus 28 sizes read by hand out of the retail binary. Nothing restores that last group -
    # data/layouts_short_fix.json holds them with `size: null` DELIBERATELY, so merge_layouts.py
    # cannot auto-apply a size that has no member list behind it, and its own _meta says so.
    # Overwriting the merged file therefore silently reverts every one of them.
    #
    # Measured: a full replay of layouts.py -> merge_layouts --write -> apply_names --write over an
    # unchanged tree changes 38 entries, 29 of them sizes. Every one of the 29 that an independent
    # source can arbitrate is CORRECT in the committed file and WRONG after regeneration.
    # HostRaceMenu is 0x1B8, which the 1999 compiler emitted as `operator new(440)` immediately
    # before the constructor call; regeneration makes it 0x270, because the kit embeds a bogus
    # RaceMenuBase at offset 0. 28 of the 29 would fail build gate 2 loudly. gfxMaterial would not,
    # having no clean allocation site to measure against, and would revert in silence.
    #
    # So the committed file is the authority and this script is one of its inputs. It refuses to
    # act like the generator unless told to in as many words.
    if os.path.exists(OUT) and "--force" not in sys.argv:
        print("")
        print("REFUSING to overwrite %s" % OUT)
        print("")
        print("  That file is a MERGE, not this script's output. It carries 28 hand-read sizes")
        print("  nothing here can restore. Regenerating changes 38 entries, and every size it")
        print("  changes is one the binary contradicts. See data/layouts_short_fix.json.")
        print("")
        print("  Parse the kit without touching it:  MM2_LAYOUTS_OUT=<path> py tools/layouts.py")
        print("  Overwrite anyway, deliberately:     py tools/layouts.py --force")
        return

    with open(OUT, "w", encoding="utf-8") as f:
        json.dump(types, f, indent=1)

    print("wrote %s" % OUT)

    # Cross-check the vtable-derived hierarchy against the layouts. A derived class embeds its
    # base at offset 0, so the base must not be larger than the derived class. Two independent
    # sources disagreeing here would mean one of them is wrong.
    hier_path = os.path.join(ROOT, "data", "hierarchy.json")
    if os.path.exists(hier_path):
        with open(hier_path, encoding="utf-8") as f:
            hier = json.load(f)

        checked = bad = 0
        for cls, info in hier.items():
            base = info.get("first_base")
            if not base or cls not in matched or base not in matched:
                continue
            checked += 1
            if matched[base]["size"] > matched[cls]["size"]:
                bad += 1
                if bad <= 8:
                    print("    ! %s (0x%X) < base %s (0x%X)" %
                          (cls, matched[cls]["size"], base, matched[base]["size"]))

        print("\n  hierarchy vs layout: %d pairs checked, %d contradictions" % (checked, bad))


if __name__ == "__main__":
    main()
