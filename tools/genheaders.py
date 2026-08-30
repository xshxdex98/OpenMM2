# -*- coding: utf-8 -*-
"""Generate OpenMM2 class headers from data/symbols.json.

Shape is Open1560's, because the goal is that a Midtown Madness 2 port reads like the Midtown
Madness 1 one: GPL header, `#pragma once`, alphabetical forward declarations, then the class with
the exact MSVC mangled symbol commented above every declaration. That comment is the documentation
- it is the only thing tying a C++ declaration back to an address in midtown2.exe.

Everything starts as ARTS_IMPORT: still the original code, merely callable. A member becomes
ARTS_EXPORT when it has actually been reimplemented.

Members are NOT emitted. A linker map lists statics and functions, never instance layout, so
offsets have to come from the binary. Emitting a guessed layout would be worse than none, because
the whole point of member order is that it matches memory.
"""
import json
import os
import re
import sys

NEWLINE = chr(10)
from collections import OrderedDict

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)

SYMBOLS = os.path.join(ROOT, "data", "symbols.json")
HIERARCHY = os.path.join(ROOT, "data", "hierarchy.json")
LAYOUTS = os.path.join(ROOT, "data", "layouts.json")
PORTED = os.path.join(ROOT, "data", "ported.json")
MM2TYPES = os.path.join(ROOT, "data", "mm2types.json")
PARAM_NAMES = os.path.join(ROOT, "data", "param_names.json")
OUTDIR = os.path.join(ROOT, "code", "midtown2")

GPL = """/*
    OpenMM2 - An Open Source Re-Implementation of Midtown Madness 2
    Copyright (C) 2026 The OpenMM2 contributors

    Structure, conventions and the arts/core runtime follow Open1560 by Brick,
    an Open Source Re-Implementation of Midtown Madness 1. Copyright (C) 2020 Brick.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
"""

# MSVC's readable spelling -> the arts primitive vocabulary. core/primitives.h is the whole type
# vocabulary the codebase is allowed to use; `int`/`float`/`bool` never appear in game code.
PRIMITIVES = OrderedDict([
    # Windows typedefs, mapped to the project's own fixed-width names. These carry real sizes and
    # must map to something the same width, or the class they appear in changes size and its
    # check_size assertion starts failing for reasons that have nothing to do with the layout.
    # Windows and DirectX pointer typedefs. These are NAMES for pointer types, and leaving
    # them alone made the generator forward-declare the typedef itself and then use it by
    # value. Mapping them to the pointer form lets the ordinary forward-declaration path
    # handle them - `struct IDirectSound;` - which is what it already does elsewhere.
    # Direct3D 7 VALUE types. gfxLight is D3DLIGHT7 field for field, holding these by value,
    # so a forward declaration cannot serve - they need real types of the right size. The
    # layouts are fixed by the D3D7 ABI: D3DVALUE is a float, D3DVECTOR is three of them (the
    # same twelve bytes as Vector3), D3DLIGHTTYPE is an enum and so four bytes, and
    # D3DCOLORVALUE is four floats - defined in core/arts.h beside GUID.
    # Handles and void pointers. Both are pointer-width and neither has any structure this
    # code needs, so void* is the honest spelling.
    ("LPVOID", "void*"),
    ("LPSTR", "char*"),
    ("HFONT", "void*"),
    ("D3DCOLORVALUE", "D3DCOLORVALUE"),
    ("D3DLIGHTTYPE", "i32"),
    ("D3DVECTOR", "D3DVECTOR"),
    ("D3DVALUE", "f32"),
    ("LPDIRECTSOUNDBUFFER", "IDirectSoundBuffer*"),
    ("LPDIRECTSOUND", "IDirectSound*"),
    ("LPDIRECTDRAWSURFACE7", "IDirectDrawSurface7*"),
    ("LPDIRECTDRAW7", "IDirectDraw7*"),
    # DPID is DirectPlay's player id - a DWORD, not a pointer.
    ("DPID", "u32"),
    ("BOOL", "i32"),
    ("HRESULT", "i32"),
    ("DWORD", "u32"),
    ("WORD", "u16"),
    ("BYTE", "u8"),
    ("UINT", "u32"),
    ("ULONG", "u32"),
    ("LONG", "i32"),
    ("unsigned __int64", "u64"),
    ("unsigned short", "u16"),
    ("unsigned char", "u8"),
    ("unsigned long", "ulong"),
    ("unsigned int", "u32"),
    ("signed char", "i8"),
    ("__int64", "i64"),
    ("double", "f64"),
    ("float", "f32"),
    ("short", "i16"),
    ("long", "ilong"),
    # NOT b32. The mangled name is the contract with the linker, and `bool` mangles as `_N`
    # while b32 (an i32 here) mangles as `H` - so declaring b32 would produce a symbol that
    # does not match, the link would quietly keep the assembly version, and the C++ would
    # never run. It is also a real 1-byte bool in this binary, so the width differs too.
    # Midtown Madness 1 used int-as-bool, which is why Open1560 can spell it b32 and we cannot.
    ("bool", "bool"),
    ("int", "i32"),
])

TAGS = re.compile(r"\b(class|struct|union|enum)\s+")
IDENT = re.compile(r"\b([A-Za-z_]\w*(?:::\w+)*)\b")

# Names that are types in a signature but never need a forward declaration.
BUILTIN = set(PRIMITIVES.values()) | {
    "void", "char", "bool", "int", "float", "double", "short", "long", "unsigned", "signed",
    "const", "volatile", "__int64", "operator", "static", "virtual",
    # Defined in core/arts.h, which every generated header includes. Forward-declaring it as well
    # is a redefinition against the typedef.
    "GUID", "D3DCOLORVALUE", "D3DVECTOR",
    # Calling conventions read as identifiers but are keywords. They were filtered by accident
    # while forward declarations required a leading letter; once underscore-prefixed names were
    # allowed - which _GUID and _DDPIXELFORMAT need - "struct __stdcall;" appeared and MSVC
    # reported it as a missing tag name, which points nowhere near the cause.
    "__stdcall", "__cdecl", "__fastcall", "__thiscall", "__vectorcall", "__ptr64",
    "struct", "class", "union", "enum", "public", "private", "protected", "inline",
}


def map_type(text):
    """`class aiPath * *` -> `aiPath**`, `char const *` -> `const char*`, `int` -> `i32`."""
    if not text:
        return "void"

    t = TAGS.sub("", text).strip()

    # Primitives FIRST. The east-const rewrite below matches a single word before `const`, so
    # on a two-word type it splits it: `unsigned short const *` became `unsigned const short*`,
    # and only then did short -> i16 run, giving `unsigned const i16*` - not a type. Mapping
    # first collapses the type to one word, after which the rewrite is safe.
    for msvc, arts in PRIMITIVES.items():
        t = re.sub(r"\b%s\b" % re.escape(msvc), arts, t)

    # MSVC writes east const; the codebase writes west const.
    t = re.sub(r"\b(\w+)\s+const\b", r"const \1", t)
    t = re.sub(r"\s*\*\s*", "*", t)
    t = re.sub(r"\s*&\s*", "&", t)
    t = re.sub(r"\s+", " ", t).strip()

    # `aiPath *` reads as `aiPath*`, but keep the space in `const char*`.
    return t


def referenced_types(sig_parts):
    """(by_value, by_reference) sets of class names a declaration mentions.

    The split matters: a type used by value must be COMPLETE at the point of declaration, so it
    needs an include. A pointer or reference only needs a forward declaration. Forward declaring
    everything is what makes generated headers fail to compile.
    """
    by_value, by_ref = set(), set()

    for part in sig_parts:
        if not part:
            continue

        text = TAGS.sub("", part)
        indirect = ("*" in text) or ("&" in text)

        for m in IDENT.finditer(text):
            name = m.group(1)

            if "::" in name:
                # A nested type such as `audManager::AUDTYPE`. The mangling preserved the original
                # 1999 enum names, which is worth having - but the enum lives inside its owner, so
                # the owner's header is required whether or not the parameter is indirect.
                by_value.add(name.split("::")[0])
                continue

            if name in BUILTIN:
                continue
            if not re.match(r"^[A-Za-z_]", name):
                continue

            (by_ref if indirect else by_value).add(name)

    return by_value, by_ref


def COMPILER_GENERATED(sym):
    """True for symbols the compiler emitted, not the programmer.

    `scalar deleting destructor', `vector deleting destructor', vftables and adjustor thunks all
    show up as public symbols but have no source-level declaration. They stay in symbols.json
    because they carry information - 222 scalar deleting destructors means 222 polymorphic
    classes, and an adjustor thunk only exists under multiple inheritance - but declaring one
    would be nonsense.
    """
    return "`" in (sym.get("demangled") or "") or "`" in (sym.get("name") or "")


def base_of(cls):
    """The immediate base class.

    Two independent sources: MM2Hook's type set states the parent outright, and the vftable
    recovery infers it. They agree on 110 of 120 shared classes. MM2Hook wins the rest, because
    every disagreement is a case where the real parent has no vftable of its own and so is
    invisible to the byte-level method.
    """
    stated = (MM2T.get("bases") or {}).get(cls)
    if stated and stated != cls:
        return stated

    inferred = (HIER.get(cls) or {}).get("base")
    return inferred if inferred and inferred != cls else None


def usable_members(cls, info):
    """The members of `cls` that can actually be declared, in recorded order.

    A MEMBER RECORDED WITH NEITHER NAME NOR TYPE IS NORMALLY UNUSABLE, and dropping it is right for
    the ten thousand entries that are method signatures or anonymous-union spellings. A few do name
    a real type in `raw` and were simply never typed, and dropping one of those costs the class
    exactly that type's width. mmNetPath is 0x70 with `Matrix44` at offset 0 and named members only
    from 0x40 up: the discarded entry was all 64 of its missing bytes, and that same 64 propagated
    to mmNetObject, mmGameMulti, mmMultiCircuit, mmMultiRace and mmMultiCR, each of which embeds
    one of the others by value.

    THE ANCESTOR TEST IS WHAT MAKES THIS SAFE, and without it the rule would do net harm. Of the 32
    raw-only members naming a known type, 30 spell the class's own base and one more spells a
    grandparent (mmCompRoster's `asNode`, reached through mmCompBase). Those are flattened base
    stubs; inheritance already lays those bytes down, so reviving one would emit the base twice.
    Only a type appearing nowhere in the ancestor chain can be a genuine member - today that is
    mmNetPath's Matrix44 and nothing else.

    Both the member emitter and the include scanner read this, because a revived member needs its
    defining header included just as much as it needs declaring.
    """
    out = []
    for m in info.get("members") or []:
        if m.get("name") and m.get("type"):
            out.append(m)
            continue

        raw = (m.get("raw") or "").strip()
        raw_size = size_of(raw) if raw else None
        if raw_size and raw != cls and raw not in ancestors_of(cls):
            out.append(dict(m, name="field_%X" % m["offset"], type=raw,
                            width=raw_size, count=0))
    return out


def ancestors_of(cls):
    """Every class `cls` inherits from, nearest first.

    Used to tell a flattened base stub from a real member: the recovery writes both as an entry at
    offset 0 naming a class, and only the ancestor test separates them.
    """
    out = []
    seen = {cls}
    base = base_of(cls)
    while base and base not in seen:
        seen.add(base)
        out.append(base)
        base = base_of(base)
    return out


def size_of(cls):
    """Instance size, for check_size. The IDB's own layouts first, MM2Hook's sizeof second."""
    info = LAYOUT.get(cls)
    if info:
        return info["size"]

    return (MM2T.get("sizes") or {}).get(cls)


# Every spelling a recovery might produce for the vtable pointer. Matched case-insensitively
# because the sources disagree: the IDA types say "vTable", datParser recovery says "vtable",
# and a member that slips through is emitted as a real field on a class that already has an
# implicit vptr - two pointers, and a sizeof that no longer matches the original.
# {owner: {nested type names}} - every `Owner::Nested` anything refers to, filled in main().
# A nested type can only be declared inside its owner, so this is what lets that happen.
NESTED = {}

# Nested names MSVC mangles as W4<Name>@1@, i.e. nested enums. Anything else nested is a struct.
NESTED_ENUMS = set()

# {class: {method names}} - filled in main() before any header is written, so emit_members can
# tell when a recovered field name collides with a real method.
METHOD_NAMES = {}

VPTR_NAMES = {"vtable", "vfptr", "__vftable", "vtbl", "vptr"}


def emit_members(cls, is_polymorphic):
    """Member declarations, in memory-offset order, plus the check_size value.

    Three things make this less mechanical than it looks.

    The IDB's structs are FLAT: a derived class's dump repeats every field of its base inline. C++
    inheritance already lays the base down first, so emitting both would place the base's fields
    twice and move everything after them. Where the base's size is known, its bytes are skipped
    and the class is declared `: public Base`; where it is not, the flat layout is emitted with no
    base, because flat is still layout-correct.

    The vtable pointer is explicit in the dump and implicit in C++. Declaring it on a class that
    also has virtual methods would produce two.

    Returns (lines, size, ok) - ok is False when the members do not reconcile with sizeof, which
    is a signal not to trust the result rather than something to paper over.
    """
    info = LAYOUT.get(cls)
    if not info:
        return [], None, True

    size = info["size"]
    members = usable_members(cls, info)

    skip_to = 0
    base = base_of(cls)
    base_info = LAYOUT.get(base) if base else None
    base_size = base_info["size"] if base_info else (size_of(base) if base else None)

    if base_size:
        skip_to = base_size
    elif (is_polymorphic and members and members[0]["offset"] == 0
            and (members[0].get("width") or 0) == 4 and not members[0].get("count")):
        # THE FIRST FOUR BYTES OF A POLYMORPHIC CLASS ARE THE VPTR, whatever the recovery called
        # them. The compiler emits one itself, so those bytes must be skipped or the class comes
        # out four bytes too large and every member sits four bytes late.
        #
        # This used to require the member to be NAMED like a vptr - vtable, vfptr, vtbl - which is
        # the spelling only some sources use. Where a recovery had called it field_0 or unk_0x00 it
        # was emitted as a real field on top of the compiler's own pointer, and DirSnd, dgPhysEntity,
        # eqEventQ and the four aiGoal classes were each exactly +4. Their names were the only thing
        # wrong with them.
        #
        # THE WIDTH TEST IS NOT OPTIONAL. Dropping any offset-0 member took 116 bytes off
        # dgPhysEntity, whose first member is a flattened base struct, not a pointer - it went from
        # four bytes too large to a hundred and twelve too small. A vptr is exactly one pointer, so
        # only a four-byte scalar at offset 0 can be one.
        skip_to = 4

    lines = []
    used_names = set()
    for m in members:
        # A MEMBER OF UNKNOWN WIDTH KEEPS THE OLD, SIMPLER TEST. Plenty of entries record no
        # width - an embedded struct, mostly - and treating that as zero makes a member sitting
        # exactly ON the base boundary look like it ends there, so it was dropped. asRoot's
        # `Matrix34 Matrix` is at 0x18 with no width and asNode is 0x18 bytes, so the matrix
        # vanished and a class that had always passed started failing. Only a member whose extent
        # is actually known can be reasoned about as straddling.
        span = (m.get("width") or 0) * (m["count"] or 1) if m.get("width") else 0

        if not span:
            if m["offset"] < skip_to:
                continue
            m_end = m["offset"]
        else:
            m_end = m["offset"] + span
            if m_end <= skip_to:
                continue

        if span and m["offset"] < skip_to:
            # Straddles the base boundary. The part below it belongs to the base and is already
            # laid down by inheritance; the part above belongs to this class and would otherwise
            # be lost. Kept as padding because only its extent is known, not its meaning.
            m = dict(m, name="pad_%X" % skip_to, offset=skip_to, type="u8",
                     count=m_end - skip_to, width=1)

        # A data member cannot share a name with a member function. phBound declares both an
        # IsOffset() method and an IsOffset field; the method name comes from the linker map and
        # the field name from a recovery, so the field yields. The trailing underscore is this
        # codebase's marker for a name we chose rather than one from 1999.
        if m["name"] in METHOD_NAMES.get(cls, ()):
            m = dict(m, name=m["name"] + "_")

        # A repeated name is silently fatal: the compiler keeps the first declaration, drops the
        # second, and the class is short by exactly that member's width. Suffixing with the offset
        # keeps both fields and says where the survivor came from.
        if m["name"] in used_names:
            m = dict(m, name="%s_%X" % (m["name"], m["offset"]))
        used_names.add(m["name"])

        # Member types go through the same mapping as signature types. They did not, so a member
        # declared BOOL stayed BOOL - a Windows typedef nothing here declares - and the class then
        # failed its check_size, which reads like a layout error rather than an undeclared type.
        mtype = map_type(m["type"])

        # A function pointer puts the name INSIDE the declarator:
        #     void (__cdecl* OnSparkAdded)(asSparkInfo*, asSparkPos*)
        # never `void (__cdecl*)(asSparkInfo*, asSparkPos*) OnSparkAdded`, which is not a
        # declaration at all. declare() already did this for PARAMETERS; members reached the
        # same shape and were emitted flat, so asBirthRule had never compiled.
        fp = re.match(r"^(.*\(\s*(?:__stdcall|__cdecl|__fastcall|__thiscall)?\s*"
                      r"(?:[A-Za-z_]\w*::)*\*)(\)\(.*)$", mtype)
        if fp:
            decl = "%s%s%s" % (fp.group(1), m["name"], fp.group(2))
        else:
            decl = "%s %s" % (mtype, m["name"])
        if m.get("count"):
            decl += "[%d]" % m["count"]

        lines.append("    %s; // 0x%03X" % (decl, m["offset"]))

    kept = [m for m in members if m["offset"] >= skip_to]
    ok = not kept or kept[-1]["offset"] < size

    return lines, size, ok


def vtable_slots():
    """{mangled: slot index} for every method that occupies a vftable slot.

    A symbol's index is the same in every table it appears in, because a derived table repeats its
    base's layout before adding to it, so one flat map is enough.
    """
    global _VTABLE_SLOTS
    if _VTABLE_SLOTS is None:
        _VTABLE_SLOTS = {}
        path = os.path.join(ROOT, "data", "vtable_order.json")
        if os.path.exists(path):
            with open(path, encoding="utf-8") as f:
                for entry in json.load(f).values():
                    for slot in (entry.get("slots") or []):
                        m = slot.get("symbol")
                        if m and m not in _VTABLE_SLOTS:
                            _VTABLE_SLOTS[m] = slot["index"]
    return _VTABLE_SLOTS


_VTABLE_SLOTS = None


def sort_key(sym):
    """Constructor, destructor, virtuals IN VTABLE ORDER, then strictly alphabetical.

    The alphabetical rule was measured across 298 upstream headers at 0.87 mean sortedness - it is
    a real rule, and an unusual enough one that matching it is most of what makes a header look
    native. It cannot apply to virtuals, and this is not a style question.

    MSVC assigns vtable slots from the order virtuals are DECLARED. Sorting them alphabetically
    therefore builds a table whose entries are in the wrong places, and every original call
    dispatching through it lands on the wrong method - with no link error, no crash at the call
    site, and nothing to point at afterwards. Aud3DObject is the clearest case in the binary: the
    required order is AssignSounds, UnAssignSounds, UpdateAudio, Update, SetNon3DParams,
    Set3DParams, and alphabetical puts four of those six in the wrong slot.

    The destructor keeps its place at rank 1 rather than being sorted with the other virtuals,
    because that is both the house style and correct: measured over the whole binary, a deleting
    destructor sits at slot 0 in 231 of the 231 classes that have one. Where a class has a virtual
    destructor, declaring it first is exactly what the table wants.

    A virtual absent from vtable_order.json sorts alphabetically among the non-virtuals, which is
    the old behaviour and the safe fallback when the table for a class was never recovered.
    """
    kind = sym["kind"]
    if kind in ("constructor", "destructor"):
        rank = 0 if kind == "constructor" else 1
        return (rank, 0, (sym.get("name") or "").lstrip("~").lower(), sym.get("mangled") or "")

    slot = vtable_slots().get(sym.get("mangled")) if sym.get("virtual") else None
    if slot is not None:
        return (2, slot, "", sym.get("mangled") or "")

    return (3, 0, (sym.get("name") or "").lower(), sym.get("mangled") or "")


PARAMS = {}


def shadowing_members(cls):
    """Member names in `cls` or any ancestor that would hide a type of the same name."""
    names = set()
    seen = set()
    while cls and cls not in seen:
        seen.add(cls)
        info = LAYOUT.get(cls)
        if info:
            for m in info.get("members") or []:
                if m.get("name"):
                    names.add(m["name"])
        cls = base_of(cls)
    return names


def unshadow(text, shadowed):
    """Qualify any type name in `text` that a member of the enclosing class would hide."""
    if not shadowed or not text:
        return text
    return re.sub(r"\b([A-Za-z_]\w*)\b",
                  lambda m: "::" + m.group(1) if m.group(1) in shadowed else m.group(1),
                  text)


def param_name(sym, index, arity):
    """The recovered name for parameter `index`, or argN.

    Parameter names are not in the linker map - it encodes types, not names - so these come from
    the kit's own signatures via tools/kit_paramnames.py. Where none was recovered the old argN is
    used, so a header is never left with an unnamed parameter.

    THE ARITY MUST AGREE. The mangled name is the authority on how many parameters a function
    takes, and a recovered signature can disagree - usually where Hex-Rays inferred a different
    calling convention. Names from a signature of a different length would land on the wrong types
    and read as though someone had checked them, so the whole entry is ignored instead.
    """
    names = PARAMS.get(sym.get("mangled"))
    if names and len(names) == arity:
        picked = names[index]
        if picked:
            return picked
    return "arg%d" % (index + 1)


def declare(sym):
    """One declaration, with its mangled-symbol comment above it."""
    lines = ["    // %s" % sym["mangled"]]

    if not sym.get("size"):
        pass  # size is unknown for the last symbol in a section; not worth a marker

    # ARTS_EXPORT means this one is ours now: the C++ replaces the original, and tools/asm.py
    # strips its PROC out of game.asm so the linker resolves here instead.
    parts = ["ARTS_EXPORT" if sym["mangled"] in DONE else "ARTS_IMPORT"]

    if sym.get("static"):
        parts.append("static")
    if sym.get("virtual"):
        parts.append("virtual")

    kind = sym["kind"]

    shadowed = shadowing_members(sym.get("class") or "")

    if kind not in ("constructor", "destructor"):
        parts.append(unshadow(map_type(sym.get("type")), shadowed))

    args = []
    for i, p in enumerate(sym.get("params") or []):
        # A function-pointer parameter puts its name inside the parentheses, not after them.
        # "i32 (__stdcall*)(...) arg2" is not a declaration; "i32 (__stdcall* arg2)(...)" is.
        ty = unshadow(map_type(p), shadowed)

        # Varargs take no name. "const char* arg1, ... arg2" is not a declaration.
        if ty.strip() in ("...", "..."):
            args.append("...")
            continue

        # The `(?:\w+::)*` is for POINTER-TO-MEMBER parameters, which datCallback is full of.
        # Their type reads `void (__thiscall Base::*)(void*, void*)`, so the star is preceded by a
        # class qualifier and the original pattern - which allowed only a calling convention there -
        # did not match. The name was then appended after the declarator, producing
        # `void (__thiscall Base::*)(void*,void*) arg1`, which is not a declaration: data7/
        # datcallback.h did not compile, and neither did anything that included it.
        m = re.match(
            r"^(.*\(\s*(?:__stdcall|__cdecl|__fastcall|__thiscall)?\s*(?:[A-Za-z_]\w*::)*\*)(\)\(.*)$", ty)
        pname = param_name(sym, i, len(sym.get("params") or []))
        if m:
            args.append("%s %s%s" % (m.group(1), pname, m.group(2)))
        else:
            args.append("%s %s" % (ty, pname))

    name = sym.get("name") or "UNKNOWN"

    # A pointer-to-array static puts its name inside the parentheses too, and its extent after
    # them. The `type` field truncates at the open paren - `?sm_Buffers@Stream@@0PAY0BAAA@DA` has
    # type "char (*" and nothing else - so the extent has to come from the demangled name, which
    # spells it in full: "private: static char (* Stream::sm_Buffers)[4096]". Without this the
    # generated line was `static char (* sm_Buffers;`, which does not parse.
    if kind == "data" and parts[-1].rstrip().endswith("(*"):
        m = re.search(r"\)\s*((?:\[\d+\])+)\s*$", sym.get("demangled") or "")

        if m:
            parts[-1] = parts[-1].rstrip()
            parts.append("%s)%s" % (name, m.group(1)))
            lines.append("    %s;" % " ".join(parts))

            return lines

    decl = "%s(%s)" % (name, ", ".join(args))

    parts.append(decl)

    line = " ".join(parts)

    if sym.get("const"):
        line += " const"

    lines.append("    %s;" % line)
    return lines


def array_pointer_decl(mangled, name, ty):
    """`char (*sm_Buffers)[4096]` - a pointer to an array, with the name inside the parentheses.

    The demangler that produced data/symbols.json truncates these to "char (*", losing the extent,
    so emitting "<type> <name>" gives `char (* sm_Buffers;` and the header does not parse. The
    extent is still in the mangled name: PAY0<extent>@<type> is pointer-to-array, and MSVC writes a
    number as hex nibbles spelled A..P and terminated by '@' - so Stream's PAY0BAAA@DA is
    1,0,0,0 = 0x1000 = a pointer to char[4096], which matches the 4096-byte buffer size the stream
    code sets up.
    """
    m = re.search(r"PAY0([A-P]+)@(.)", mangled or "")
    if not m:
        return None

    extent = 0
    for ch in m.group(1):
        extent = extent * 16 + (ord(ch) - ord("A"))

    elem = {"D": "char", "E": "u8", "F": "i16", "G": "u16",
            "H": "i32", "I": "u32", "M": "f32", "N": "f64"}.get(m.group(2))
    if not elem or not extent:
        return None

    return "%s (*%s)[%d]" % (elem, name, extent)


def access_of(mangled, cls):
    """public / protected / private, read out of the mangled name.

    MSVC encodes access in the mangling, so a declaration in the wrong section produces a different
    symbol and simply fails to link. gfxPipeline::m_iWidth is `?m_iWidth@gfxPipeline@@0HA` - the 0
    means private static - while the header declared it public and emitted `@@2HA`. Nothing
    resolved it, and the error named the C++ spelling rather than the mismatch.

    Static data uses a digit: 0 private, 1 protected, 2 public. Member functions use a letter, in
    blocks of six: A-F private, I-N protected, Q-V public.

    A CONSTRUCTOR OR DESTRUCTOR spells the class name immediately after ??0 / ??1, with no leading
    @ - `??1gfxBitmap@@AAE@XZ`, not `??1@gfxBitmap@@AAE@XZ` - so the marker below cannot match and
    the lookup used to fall through to its public default. That is not cosmetic: gfxBitmap's
    constructor and destructor are both private (AAE), and declaring them public made the compiler
    emit ??1gfxBitmap@@QAE@XZ, a symbol nothing in midtown2.exe defines. Any ported member that
    destroyed a gfxBitmap - gfxBitmap::Create(gfxImage*, bool) does, on a failed load - would have
    failed to link with an error naming the C++ spelling rather than the mismatch.
    """
    if not mangled:
        return "public"

    leaf = cls.split("::")[-1]

    marker = "@%s@@" % leaf
    i = mangled.find(marker)
    if i >= 0:
        j = i + len(marker)
    elif mangled.startswith("??0%s@@" % leaf) or mangled.startswith("??1%s@@" % leaf):
        j = len("??0%s@@" % leaf)
    else:
        return "public"

    c = mangled[j:j + 1]
    if c in "012":
        return {"0": "private", "1": "protected", "2": "public"}[c]
    if c in "ABCDEF":
        return "private"
    if c in "IJKLMN":
        return "protected"
    return "public"


def emit_class(cls, syms, index, subsys):
    """The full header text for one class."""
    functions = [s for s in syms if s["kind"] in
                 ("function", "constructor", "destructor", "operator") and s["code"]
                 and not COMPILER_GENERATED(s)]
    # A `..._SEH` symbol is MSVC's exception-handling metadata for a function, not a member of the
    # class. There are 681 of them, and each one carries the mangled name of the function it belongs
    # to, so emitting them as data produced `static void Insert;` sitting next to the real
    # `Insert()` - "this use of 'void' is not valid", then a redefinition error. They describe a
    # function that is already declared; there is nothing to emit.
    statics = [s for s in syms
               if s["kind"] == "data" and not s["mangled"].endswith("_SEH")]
    vftable = [s for s in syms if s["kind"] == "vftable"]

    functions.sort(key=sort_key)

    # A type used by value needs its header; a pointer or reference only needs a forward
    # declaration. Includes go in two alphabetical groups, cross-subsystem then same-directory.
    val, ref = set(), set()
    for s in functions:
        v, r = referenced_types([s.get("type")] + (s.get("params") or []))
        val |= v
        ref |= r
    for s in statics:
        v, r = referenced_types([s.get("type")])
        val |= v
        ref |= r

    # Member types too. Leaving these out meant a member like `FixedHashEntry* FixedHashEntrys;`
    # was emitted with nothing declaring FixedHashEntry, and the compiler reported it as "missing
    # ';' before '*'" - which reads like a broken generator rather than a missing declaration, and
    # then cascaded into the class's check_size failing for no real reason.
    info = LAYOUT.get(cls)
    if info:
        for m in usable_members(cls, info):
            if m.get("type"):
                v, r = referenced_types([map_type(m["type"])])
                val |= v
                ref |= r

    base = base_of(cls)
    if base:
        val.add(base)

    own = {cls, cls.split("::")[0]}
    val -= own
    ref -= own | val  # a type needed by value anywhere is included, so never also forward declared

    includes, unknown = set(), set()
    for name in val:
        path = index.get(name)
        if path:
            includes.add(path)
        else:
            unknown.add(name)

    here = directory(cls, subsys) + "/"
    cross = sorted(p for p in includes if not p.startswith(here))
    local = sorted(p.split("/")[-1] for p in includes if p.startswith(here))

    out = [GPL.rstrip(), "", "#pragma once", "",
           '#include "core/arts.h"', ""]

    if cross:
        out += ['#include "%s"' % p for p in cross]
        out.append("")

    if local:
        out += ['#include "%s"' % p for p in local]
        out.append("")

    # A leading underscore is a perfectly ordinary type name - _DDPIXELFORMAT and _GUID are the
    # DirectDraw and COM structs. Requiring isalpha() dropped their forward declarations, and the
    # header then failed on "syntax error: identifier '_DDPIXELFORMAT'".
    # Enums at file scope, mangled W4<name>@@ rather than W4<name>@1@. gfxDrawMode is one, and
    # without this it is simply an undeclared identifier in every signature that takes one.
    global_enums = set()
    for s in functions + statics:
        for m in re.finditer(r"W4([A-Za-z_][A-Za-z0-9_]*)@@", s.get("mangled") or ""):
            global_enums.add(m.group(1))
    # A MEMBER can be an enum too, and its type never appears in a mangled function name, so the
    # scan above misses it entirely: dgStatePack holds a GameMode and a SkillLevel, gfxLight a
    # dltType, and each was simply an undeclared identifier. The IDB records what kind of type each
    # name is, so ask it rather than guessing from the spelling.
    if info:
        for m in info.get("members") or []:
            base = map_type(m.get("type") or "").replace("*", "").strip()
            if base and TYPE_TAGS.get(base) == "enum":
                global_enums.add(base)

    global_enums -= own

    if global_enums:
        out += ["enum %s : i32;" % e for e in sorted(global_enums)]
        out.append("")

    # Must be a real identifier: the demangler occasionally yields fragments, and "class ;" is a
    # syntax error that says nothing about where it came from.
    IDENT = re.compile(r"^[A-Za-z_][A-Za-z0-9_]*$")

    # A by-value type with no header of its own fell through both arms of the include loop above
    # and was silently dropped - and because `ref -= own | val` ran first, being needed by value
    # ALSO deleted the forward declaration its by-pointer uses would have produced. One
    # `union eqEvent` parameter on mmInput::BuildCaptureIO is what left eqEvent undeclared in 22
    # headers.
    #
    # A forward declaration is enough here: an incomplete type is only forbidden as a parameter of a
    # function DEFINITION, and these headers emit nothing but declarations.
    #
    # Only for names the binary actually states a class-key for. The rest (BOOL, HWND, LPVOID,
    # D3DVALUE) are typedefs from headers this build does not have, and guessing `class` for one
    # would emit a declaration that mangles differently from the binary - the very failure
    # STRUCT_TAGS exists to prevent. Dropping those is still the honest answer.
    orphans = {u for u in unknown if TYPE_TAGS.get(u) in ("class", "struct", "union")}

    fwd = sorted(r for r in (ref | orphans) if r and IDENT.match(r))
    fwd = [r for r in fwd if r not in global_enums]
    if fwd:
        # The class-key comes from the binary, not from a guess - see class_key/STRUCT_TAGS.
        out += ["%s %s;" % (class_key(r), r) for r in fwd]
        out.append("")

    # Nested classes are declared inside their outer class; emit the leaf name only.
    leaf = cls.split("::")[-1]

    base = base_of(cls)

    if base:
        out.append("class %s : public %s" % (leaf, base))
    else:
        out.append("class %s" % leaf)
    out.append("{")
    out.append("public:")

    # Nested types anything refers to as Owner::Nested. Declared here because that is the only
    # place they CAN be declared - a nested name cannot be forward-declared from outside its owner.
    # A declaration is sufficient for the pointers and references these are used as.
    for nested in sorted(NESTED.get(leaf, ())):
        if nested in NESTED_ENUMS or TYPE_TAGS.get(nested) == "enum":
            out.append("    enum %s : i32;" % nested)
        else:
            out.append("    struct %s;" % nested)
    if NESTED.get(leaf):
        out.append("")

    # Nested enums, declared opaquely.
    #
    # MSVC mangles an enum parameter as W4<name>@<scope>@, so `?Create@gfxImage@@SAPAV1@HHW4gfx    # ImageFormat@1@0H@Z` says gfxImageFormat is an enum nested in this class. Nothing recovers its
    # enumerators, and none are needed: an opaque declaration with a fixed underlying type is a
    # complete type, so it can be passed and returned by value. Without it the parameter type is
    # simply undeclared and the header does not compile.
    nested_enums = set()
    for s in functions + statics:
        for m in re.finditer(r"W4([A-Za-z_][A-Za-z0-9_]*)@1@", s.get("mangled") or ""):
            nested_enums.add(m.group(1))

    if nested_enums:
        for name in sorted(nested_enums):
            out.append("    enum %s : i32;" % name)
        out.append("")

    # Grouped by the access the mangled name says they have, because access is part of the symbol.
    # Everything was emitted public before, which is right for most of the binary and silently
    # wrong for the rest: gfxPipeline::m_iWidth is `?m_iWidth@gfxPipeline@@0HA`, private, and
    # declaring it public produced `@@2HA` - a symbol that does not exist and does not link.
    for section in ("public", "protected", "private"):
        chunk = []

        for s in functions:
            if access_of(s.get("mangled"), cls) == section:
                chunk.append(NEWLINE.join(declare(s)))

        for s in statics:
            if access_of(s.get("mangled"), cls) != section:
                continue
            decl = array_pointer_decl(s.get("mangled"), s["name"], s.get("type"))
            if decl is None:
                decl = "%s %s" % (map_type(s.get("type")), s["name"])
            chunk.append(("    // %s" + NEWLINE + "    ARTS_IMPORT static %s;")
                         % (s["mangled"], decl))

        # A private static still has to be reachable from ported code outside the class.
        #
        # Access is part of the mangled name, so these cannot simply be declared public: that
        # produces a different symbol and does not link. Declaring them correctly and adding an
        # accessor keeps both true at once - the storage keeps its real decorated name, and the
        # class itself is allowed to hand out a reference to it. The accessor is inline and static,
        # so it contributes no storage, no vtable and no symbol.
        # A private destructor still has to be reachable from ported code outside the class.
        #
        # 1999 inlined the release idiom at every call site - decrement the refcount, and if it hits
        # zero destruct and free - so there is no ?Release@gfxImage@@ symbol to import, and the
        # destructor itself is private. Reimplemented code therefore cannot destroy the object at
        # all. Making the destructor public is not an option: access is part of the mangled name, so
        # ??1gfxImage@@AAE@XZ would become @QAE and stop matching what the binary defines.
        #
        # An accessor keeps both true, the same way Access_ does for private statics. It is inline
        # and contributes no symbol, and it deliberately does NOT free the memory or touch the
        # refcount - that logic differs per class and belongs at the call site, where it can be
        # written to match the original instruction for instruction.
        if section in ("private", "protected"):
            dtors = [s for s in functions
                     if s["kind"] == "destructor" and access_of(s.get("mangled"), cls) == section]
            if dtors:
                leaf = cls.split("::")[-1]
                out.append("")
                out.append("public:")
                out.append("    // The destructor is %s in midtown2.exe; this reaches it without" % section)
                out.append("    // changing its mangled name. It does not free the storage.")
                out.append("    void Access_Destroy() { this->~%s(); }" % leaf)

        if section in ("private", "protected") and statics:
            # A pointer-to-array needs its name inside the parentheses, and so would a reference
            # to one - `char (*&Access())[4096]`. Not worth generating; skip them rather than emit
            # a return type that does not parse.
            acc = [s for s in statics
                   if access_of(s.get("mangled"), cls) == section and s.get("type")
                   and "(*" not in (s.get("type") or "")
                   and array_pointer_decl(s.get("mangled"), s["name"], s.get("type")) is None]
            if acc:
                out.append("")
                out.append("public:")
                for s in acc:
                    out.append("    static %s& Access_%s() { return %s; }"
                               % (map_type(s.get("type")), s["name"], s["name"]))

        if not chunk:
            continue

        if section != "public":
            out.append("")
            out.append("%s:" % section)

        out.append((NEWLINE + NEWLINE).join(chunk))

    is_poly = any(s.get("virtual") for s in syms)
    mlines, msize, mok = emit_members(cls, is_poly)

    if mlines:
        out.append("")

        # Instance members are always public.
        #
        # They used to inherit whichever access section the loop above emitted last, which is
        # decided by the class's STATICS - so gfxBitmap, whose only static is the private
        # sm_First, ended up with Name, Width, Height and Surface private, while gfxTexture, which
        # has no private static, got the same fields public. That difference is an accident of the
        # generator, not something the binary says.
        #
        # An instance member's name is not part of any mangled symbol, so its access specifier has
        # no effect on what links. What it does affect is whether ported code outside the class can
        # read a field that the original machine code reads from everywhere: ProgressCB, mmHUD,
        # RestoreFocus and gfxPipeline::CopyBitmap all take gfxBitmap::Width straight off the
        # object. Emitting these private makes those functions impossible to port for no gain.
        out.append("public:")

        base_note = base_of(cls)
        if base_note and LAYOUT.get(base_note):
            out.append("    // Members from 0x%03X; everything below that belongs to %s."
                       % (LAYOUT[base_note]["size"], base_note))
        out.append("    // Offset order is mandatory - the original code reads these at fixed offsets.")
        out += mlines

    out.append("};")
    out.append("")

    if vftable:
        out.append("// %s" % vftable[0]["mangled"])
        out.append("// vtable at 0x%08X" % vftable[0]["rva"])

    # Instance size is not in a linker map; it comes from the IDB type library via layouts.json.
    # A class the IDB has no layout for keeps an unfilled guard rather than an invented number.
    # A live check_size is only meaningful when the members are known.
    #
    # MM2Hook models static-only classes (gfxPipeline, datArgParser, datTimeManager...) as empty
    # structs, so its sizeof is 1. Emitting check_size(gfxPipeline, 0x1) against a header that
    # declares no members gives sizeof == 1 on both sides: the assertion passes and certifies
    # nothing, which is worse than having no assertion at all. 32 classes were in that state.
    #
    # So the guard is live only with a real member layout. A size we know but cannot verify is
    # recorded as a comment, where it is useful to a human and cannot lull the compiler.
    if mlines:
        out.append("check_size(%s, 0x%X);%s" % (leaf, msize, "" if mok else "  // UNRECONCILED"))
    else:
        known = size_of(cls)
        if known:
            out.append("// check_size(%s, 0x%X); // size known, members are not - cannot verify"
                       % (leaf, known))
        else:
            out.append("// check_size(%s, 0x0); // TODO: no layout in the IDB type library" % leaf)
    out.append("")

    return "\n".join(out)


# Class prefix -> directory. Midtown Madness 2 renamed most of the Midtown Madness 1 subsystems
# (agi became gfx/dg, mmcity became lvl, mmcar became veh, mmphysics became ph) and it mixes
# prefixed classes with plain PascalCase ones, so both spellings are mapped here.
DIRS = OrderedDict([
    ("mm", "mmgame"), ("ai", "mmai"), ("ph", "ph"), ("veh", "veh"),
    ("gfx", "gfx"), ("dg", "dg"), ("lvl", "lvl"), ("as", "arts7"),
    ("aud", "audio"), ("cam", "cam"), ("dat", "data7"), ("giz", "giz"),
    ("cr", "cr"), ("city", "city"), ("ped", "ped"), ("mod", "mod"),
    ("io", "io"), ("lt", "lt"), ("sdl", "sdl"), ("eq", "eventq7"),
    ("mem", "memory"), ("ui", "ui"), ("net", "net"), ("zip", "stream"),
    ("par", "data7"), ("fx", "fx"), ("ptx", "ptx"), ("vgl", "vgl"),
    ("mc", "mc"), ("sf", "sf"), ("cc", "cc"), ("win", "pcwindis"),
])

PASCAL_DIRS = [
    (re.compile(r"^(Matrix|Vector|Quat)"), "vector7"),
    (re.compile(r"^(Aud|DMusic|Audio|Echo|DirSnd|Sound)"), "audio"),
    (re.compile(r"^(UI|Dialog|Menu|PU[A-Z]|Control|Options|About|Card|Race|Host|Main|Net[A-Z]|Vehicle|Graphics|Crash)"), "ui"),
    (re.compile(r"^Age"), "age"),
    (re.compile(r"^(CArrayList|CFileStream|CMemStream|CLoader|Stream|HashTable|string)"), "stream"),
]


def directory(cls, subsystem):
    if subsystem in DIRS:
        return DIRS[subsystem]

    leaf = cls.split("::")[0]
    for pattern, d in PASCAL_DIRS:
        if pattern.match(leaf):
            return d

    return "misc"


def filename(cls):
    return cls.split("::")[0].lower() + ".h"


HIER = {}
LAYOUT = {}
MM2T = {}
DONE = set()


# Type names the retail mangling spells `U` (struct) rather than `V` (class).
#
# This is not cosmetic. The mangled name is the contract with the linker, and a forward
# declaration is what decides which letter the compiler emits: declaring `class LocString;` makes
# mmText::CreateLocFont mangle as ?CreateLocFont@mmText@@SAPAXPBVLocString@@H@Z, while game.asm
# defines ?CreateLocFont@mmText@@SAPAXPBULocString@@H@Z. The C++ compiles, the object file links
# against the wrong name, and tools/build.py quarantines it - which is how this was found.
#
# The set is DERIVED rather than listed, from every mangled name in data/symbols.json: a
# `U<Name>@@` occurrence means the 1999 compiler saw a struct. Deriving it is self-calibrating
# where a hand-written list would rot, and it also picks up HWND__, gfxStridedData,
# gfxVertexBuffer, coreFileMethods and the rest without anyone having to notice them one by one.
STRUCT_TAGS = set()

MANGLED_STRUCT = re.compile(r"U([A-Za-z_][A-Za-z0-9_]*)@@")


# The class-key the 1999 compiler saw, read out of the demangled signature text rather than out of
# the mangled name. MSVC spells a union `T`, and a `T([A-Za-z_]\w*)@@` regex is unusable: it also
# matches the tail of every class name beginning with T - VTokenizer@@ yields "okenizer", 86 times.
# MANGLED_STRUCT has the same defect and it fires: in ...@@UAEXPAUNETSYS_MSG@@@Z the leftmost `U` is
# the one in UAEX, so NETSYS_MSG never reaches STRUCT_TAGS and is emitted `class`, which mangles
# PAVNETSYS_MSG@@ against a binary that says PAUNETSYS_MSG@@.
#
# The demangled text has no such ambiguity: parameters are spelled "class mmIO *", "union eqEvent".
# The (?!\s*::) guard is required - without it "enum gfxImage::gfxImageFormat" registers gfxImage as
# an enum. With it, all 413 names learned this way have exactly one stated key.
SIGNATURE_TAG = re.compile(r"\b(class|struct|union|enum)\s+([A-Za-z_][A-Za-z0-9_]*)\b(?!\s*::)")

TYPE_TAGS = {}


def learn_struct_tags(syms):
    for s in syms:
        for m in MANGLED_STRUCT.finditer(s.get("mangled") or ""):
            STRUCT_TAGS.add(m.group(1))

        for text in [s.get("demangled") or "", s.get("type") or ""] + list(s.get("params") or []):
            for key, name in SIGNATURE_TAG.findall(text):
                TYPE_TAGS[name] = key


def class_key(name):
    """`struct` or `class`, whichever the binary says."""
    # An underscore-prefixed name is a C struct from the Windows headers - _GUID, _DDPIXELFORMAT -
    # and those are structs whether or not any mangled name happens to mention them.
    stated = TYPE_TAGS.get(name)
    if stated in ("class", "struct", "union"):
        return stated

    if name.startswith("_") or name in STRUCT_TAGS:
        return "struct"

    return "class"


def main():
    global HIER, LAYOUT, MM2T, DONE

    with open(SYMBOLS, encoding="utf-8") as f:
        syms = json.load(f)

    learn_struct_tags(syms)

    if os.path.exists(HIERARCHY):
        with open(HIERARCHY, encoding="utf-8") as f:
            HIER = json.load(f)
    else:
        print("no hierarchy.json; run tools/vtables.py first - headers will have no base classes")

    if os.path.exists(LAYOUTS):
        with open(LAYOUTS, encoding="utf-8") as f:
            LAYOUT = json.load(f)
    else:
        print("no layouts.json; run tools/layouts.py - headers will have no members or sizes")

    if os.path.exists(PORTED):
        with open(PORTED, encoding="utf-8") as f:
            DONE = set(json.load(f).get("ported", []))
        print("%d symbols marked ARTS_EXPORT" % len(DONE))

    if os.path.exists(MM2TYPES):
        with open(MM2TYPES, encoding="utf-8") as f:
            MM2T = json.load(f)

    if os.path.exists(PARAM_NAMES):
        with open(PARAM_NAMES, encoding="utf-8") as f:
            PARAMS.update(json.load(f))
        print("%d functions have a recovered parameter name" % len(PARAMS))

    classes = {}
    nested_skipped = 0
    for s in syms:
        cls = s.get("class")
        if not cls:
            continue
        if "::" in cls:
            # A member of a NESTED class. Filing it under the outer class produces nonsense -
            # ??0TerrainContact@phInertialCS@@ became `void phInertialCS();` - and a nested class
            # needs a declaration inside its owner, which this generator does not write. Skipped
            # rather than misfiled; see docs/harness.md.
            nested_skipped += 1
            continue
        classes.setdefault(cls, []).append(s)
    if nested_skipped:
        print("%d symbols belong to nested classes and are not emitted" % nested_skipped)

    # A class that owns no functions still needs a header if something inherits from it. phSegment
    # is pure data, owns no symbols, and so was never written - leaving lvlSegment with an
    # undefined base even though phSegment's layout is complete and CONFIRMED.
    for cls in list(classes):
        base = base_of(cls)
        while base and base not in classes and LAYOUT.get(base):
            classes[base] = []
            base = base_of(base)

    for cls, group in classes.items():
        METHOD_NAMES[cls] = {g.get("name") for g in group
                             if g.get("code") and g.get("name")}

    # Every Owner::Nested named by a return type, a parameter or a member.
    # MSVC spells a nested enum W4<Name>@1@ in the mangled name. That is the only reliable way
    # to tell one from a nested struct here, since TYPE_TAGS holds no nested names.
    for s2 in syms:
        for m in re.finditer(r"W4([A-Za-z_]\w*)@1@", s2.get("mangled") or ""):
            NESTED_ENUMS.add(m.group(1))

    nested_re = re.compile(r"\b([A-Za-z_]\w*)::([A-Za-z_]\w*)\b")
    def note_nested(text):
        for m in nested_re.finditer(TAGS.sub("", text or "")):
            owner, name = m.group(1), m.group(2)
            if owner in classes and name != owner:
                NESTED.setdefault(owner, set()).add(name)

    for s2 in syms:
        note_nested(s2.get("type"))
        for prm in (s2.get("params") or []):
            note_nested(prm)
    for cls2, info2 in LAYOUT.items():
        if isinstance(info2, dict):
            for m2 in info2.get("members") or []:
                note_nested(m2.get("type"))

    # HAND-WRITTEN HEADERS THIS GENERATOR CANNOT PRODUCE, BUT MUST STILL BE ABLE TO INCLUDE.
    #
    # A type used BY VALUE needs its definition, not a forward declaration. The resolution below
    # only knows about classes this generator writes itself, so a by-value member whose type is
    # hand-written fell through to a forward declaration and the header then failed to compile with
    # "uses undefined class".
    #
    # gfxRenderState is the case in point: it holds `gfxRenderStateData State;` by value at offset
    # 0, gfxRenderStateData has no methods so no symbols and therefore no generated header, and the
    # result was that gfxrenderstate.h could not be compiled at all - which is why nothing had ever
    # defined a gfxRenderState member function, and why agigl/glpacket.cpp reaches RSTATE through
    # an incomplete type and a hand-computed byte offset instead of naming a member.
    HANDWRITTEN = {
        "gfxRenderStateData": "gfx/gfxrenderstatedata.h",
    }

    # class -> "dir/file.h", so an include can be resolved before anything is written
    index = dict(HANDWRITTEN)
    for cls, group in classes.items():
        if any(s["code"] for s in group):
            index[cls] = "%s/%s" % (directory(cls, group[0]["subsystem"]), filename(cls))

    written = 0
    per_dir = {}

    for cls, group in sorted(classes.items()):
        if not any(s["code"] for s in group):
            continue  # data-only: nothing to declare yet

        d = directory(cls, group[0]["subsystem"])
        path = os.path.join(OUTDIR, d, filename(cls))
        os.makedirs(os.path.dirname(path), exist_ok=True)

        with open(path, "w", encoding="utf-8", newline="\n") as f:
            f.write(emit_class(cls, group, index, group[0]["subsystem"]))

        written += 1
        per_dir[d] = per_dir.get(d, 0) + 1

    print("wrote %d headers under %s" % (written, OUTDIR))
    for d, n in sorted(per_dir.items(), key=lambda kv: -kv[1]):
        print("   %-12s %3d" % (d, n))


if __name__ == "__main__":
    main()
