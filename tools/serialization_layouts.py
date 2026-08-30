# -*- coding: utf-8 -*-
"""Recover member layouts from serialization / reset / dump functions.

A linker map has no member offsets, and 347 classes (47% of the code) are blocked on them.
Constructors and scattered member reads give offsets but no names and no widths. Serialization
does better: a function that walks an object field by field states, in one line each, the offset,
the width AND - for datParser - the original 1999 field name.

Four idioms are mined, in descending order of strength:

  datParser::AddRecord(p, TYPE, "Name", this + N, count, cb)
        offset + width + NAME. TYPE is an enum whose widths are read off datParser::Read
        (0x4A7CA0) and confirmed against datParser::Write (0x4A7F70) - they agree.

  Class::FileIO(this + N, subparser)
        a sub-object of a known class at byte N.

  Stream::Read/Write(s, this + N, LEN)
        offset + exact width, in declaration order, but no name.

  memset(this + N, 0, LEN)
        one line naming a whole block's extent.

Everything is byte-offset arithmetic through hex-rays' `this`, whose declared pointee decides the
scale: `float *this; this + 69` is byte 276, not byte 69. Getting that wrong shifts every field of
the class, so the scale is taken from the signature and any expression whose scale cannot be
proven is dropped rather than guessed.

Output: data/layouts_from_serialization.json. Sizes are LOWER BOUNDS.
"""
import json
import os
import re
from collections import OrderedDict, defaultdict

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
PSEUDO = os.path.join(ROOT, "MM2_RE_KIT", "MM2_PSEUDOCODE")
OUT = os.path.join(ROOT, "data", "layouts_from_serialization.json")

# ---------------------------------------------------------------------------
# datParser type enum. Read straight off datParser::Read's switch - each case shows the store
# width and the pointer advance, so this is not a guess.
DAT_TYPE = {
    0: ("i8", 1, "string"),      # GetToken into a char buffer of ValueSize bytes
    1: ("u8", 1, "bool"),        # *Data++ = GetInt() != 0
    2: ("i8", 1, "char"),        # *Data++ = GetInt()
    3: ("i16", 2, "short"),      # *(_WORD *)Data = GetInt(); Data += 2
    4: ("i32", 4, "int"),        # *(_DWORD *)Data = GetInt(); Data += 4
    5: ("f32", 4, "float"),      # *(float *)Data = GetFloat(); Data += 4
    6: ("Vector2", 8, "Vector2"),
    7: ("Vector3", 12, "Vector3"),
    8: ("Vector4", 16, "Vector4"),
    9: (None, None, "subparser"),
}

# A cast's pointee width. This is the whole ballgame: `(char *)this + 8` is byte 8 and
# `(_DWORD *)this + 8` is byte 32.
PTR_WIDTH = {
    "char": 1, "_BYTE": 1, "unsigned char": 1, "signed char": 1, "__int8": 1,
    "unsigned __int8": 1, "uint8_t": 1, "int8_t": 1, "BYTE": 1, "bool": 1, "void": 1,
    "_WORD": 2, "__int16": 2, "unsigned __int16": 2, "short": 2, "unsigned short": 2,
    "uint16_t": 2, "int16_t": 2, "WORD": 2, "wchar_t": 2,
    "_DWORD": 4, "int": 4, "unsigned int": 4, "__int32": 4, "unsigned __int32": 4,
    "uint32_t": 4, "int32_t": 4, "long": 4, "unsigned long": 4, "DWORD": 4, "float": 4,
    "_QWORD": 8, "__int64": 8, "unsigned __int64": 8, "double": 8, "int64_t": 8,
}

# Scalar (non-pointer) casts: `(int)this + 8` is byte arithmetic.
INT_CASTS = {"int", "unsigned int", "__int32", "unsigned __int32", "DWORD", "_DWORD",
             "size_t", "unsigned long", "long", "uintptr_t"}

SEP = "// --------------------------------------------------------------------------"
EA = re.compile(r"^//\s*ea=0x([0-9A-Fa-f]+)\s+size=0x([0-9A-Fa-f]+)")


# ---------------------------------------------------------------------------
def split_functions(path):
    """-> [{'comments': [...], 'ea': int, 'size': int, 'body': str}]"""
    with open(path, encoding="utf-8", errors="replace") as f:
        text = f.read()

    out = []
    for chunk in text.split(SEP):
        lines = chunk.split("\n")
        ea = size = None
        comments = []
        body_start = 0
        for i, ln in enumerate(lines):
            m = EA.match(ln.strip())
            if m:
                ea, size = int(m.group(1), 16), int(m.group(2), 16)
                body_start = i + 1
                break
            if ln.strip().startswith("//"):
                comments.append(ln.strip()[2:].strip())
        if ea is None:
            continue
        out.append(dict(comments=comments, ea=ea, size=size,
                        body="\n".join(lines[body_start:])))
    return out


def signature(body):
    """hex-rays' declaration, rejoined - it is wrapped across lines when it is long."""
    lines = body.split("\n")
    i = 0
    while i < len(lines) and (not lines[i].strip() or lines[i].strip().startswith("//")):
        i += 1
    if i >= len(lines) or "(" not in lines[i]:
        return None
    buf, depth = [], 0
    while i < len(lines):
        ln = lines[i]
        buf.append(ln.strip())
        depth += ln.count("(") - ln.count(")")
        if depth <= 0 and "(" in " ".join(buf):
            break
        i += 1
        if len(buf) > 40:
            return None
    return " ".join(buf)


def split_args(s):
    """Top-level comma split, respecting parens, brackets, template angles and strings.

    `->` must not be read as a closing angle bracket. That bug silently truncated every
    `&this->Member` argument list, so the stack is explicit and `->` is skipped whole.
    """
    args, stack, cur, i, instr = [], [], [], 0, False
    while i < len(s):
        c = s[i]
        if instr:
            cur.append(c)
            if c == "\\":
                if i + 1 < len(s):
                    cur.append(s[i + 1])
                    i += 1
            elif c == '"':
                instr = False
        elif c == "-" and i + 1 < len(s) and s[i + 1] == ">":
            cur.append("->")
            i += 1
        elif c == '"':
            instr = True
            cur.append(c)
        elif c in "([{":
            stack.append(c)
            cur.append(c)
        elif c in ")]}":
            if stack:
                stack.pop()
            cur.append(c)
        elif c == "<":
            stack.append(c)
            cur.append(c)
        elif c == ">":
            if stack and stack[-1] == "<":
                stack.pop()
            cur.append(c)
        elif c == "," and not stack:
            args.append("".join(cur).strip())
            cur = []
        else:
            cur.append(c)
        i += 1
    args.append("".join(cur).strip())
    return args


def call_sites(flat, name):
    """Yield the argument list of every `name(...)` call in `flat` (whitespace-collapsed body)."""
    for m in re.finditer(re.escape(name) + r"\s*\(", flat):
        i = m.end()
        depth, instr, start = 1, False, i
        while i < len(flat) and depth:
            c = flat[i]
            if instr:
                if c == "\\":
                    i += 1
                elif c == '"':
                    instr = False
            elif c == '"':
                instr = True
            elif c == "(":
                depth += 1
            elif c == ")":
                depth -= 1
            i += 1
        if depth == 0:
            yield split_args(flat[start:i - 1])


# ---------------------------------------------------------------------------
THIS_PARAM = re.compile(r"\(\s*(.*?)\s*(\**)\s*(\w+)\s*[,)]")


def this_scale(sig):
    """Byte width of one step of `p + 1` for the first parameter. None if unprovable.

    `char *this`  -> 1.  `float *this` -> 4.  `AudioOptions *this` -> unknown, so None: bare
    `this + N` on a class pointer would be N * sizeof(class) and must not be read as bytes.
    """
    if not sig:
        return None, None
    open_paren = sig.find("(")
    if open_paren < 0:
        return None, None
    args = split_args(sig[open_paren + 1:sig.rfind(")")] if sig.rfind(")") > open_paren
                      else sig[open_paren + 1:])
    if not args or not args[0]:
        return None, None
    first = args[0].strip()
    m = re.match(r"^(?:const\s+|struct\s+|volatile\s+)*(.+?)\s*(\*+)\s*(\w+)$", first)
    if m:
        base, stars, name = m.group(1).strip(), m.group(2), m.group(3)
        base = re.sub(r"^(struct|union|enum|const|volatile)\s+", "", base).strip()
        if len(stars) > 1:
            return name, 4          # pointer-to-pointer: one step is a pointer
        # None for a class type: bare `this + N` cannot be scaled, but `(_DWORD *)this + N` can,
        # so the name is still registered.
        return name, PTR_WIDTH.get(base)
    m = re.match(r"^(?:const\s+)?(.+?)\s+(\w+)$", first)
    if m and m.group(1).strip() in INT_CASTS:
        return m.group(2), 1        # `int this` - byte arithmetic
    return None, None


CAST = re.compile(r"^\(\s*(?:const\s+|struct\s+|volatile\s+)*"
                  r"([A-Za-z_][\w:< >,]*?)\s*(\*?)\s*\)\s*(.*)$")


def resolve(expr, bases, fields=None):
    """`(char *)this + 8` -> 8 for bases={'this': 4}. None when the scale is not provable.

    Only *address* forms are accepted. Anything with a leading `*` is a read through a member
    pointer and lands somewhere else entirely, so it is rejected outright.

    `fields` (name -> offset) resolves `&this->Member`, which is what hex-rays emits once IDA
    already has the struct. That path recovers nothing new; it exists so the control group can
    be scored.
    """
    e = " ".join(expr.split())
    if not e or e.startswith("*"):
        return None

    scale = None
    # peel casts, keeping the innermost pointer cast that has a width
    while True:
        m = CAST.match(e)
        if not m:
            break
        base, star, rest = m.group(1).strip(), m.group(2), m.group(3).strip()
        if star:
            w = PTR_WIDTH.get(base)
            if w is None:
                return None          # cast to a class pointer - one step is sizeof(class)
            scale = w
        elif base in INT_CASTS:
            scale = 1
        else:
            return None
        e = rest

    if fields:
        m = re.match(r"^&\s*(\w+)\s*->\s*(\w+)$", e)
        if m and m.group(1) in bases and m.group(2) in fields:
            return fields[m.group(2)]
        m = re.match(r"^&\s*(\w+)\s*->\s*(\w+)\s*\[\s*(\d+)\s*\]$", e)
        if m and m.group(1) in bases and m.group(2) in fields:
            return fields[m.group(2)]

    m = re.match(r"^&?\s*(\w+)\s*(?:\+\s*(0[xX][0-9A-Fa-f]+|\d+)\s*)?$", e)
    if m:
        name, off = m.group(1), int(m.group(2) or "0", 0)
    else:
        m = re.match(r"^&\s*(\w+)\s*\[\s*(0[xX][0-9A-Fa-f]+|\d+)\s*\]$", e)
        if not m:
            return None
        name, off = m.group(1), int(m.group(2), 0)

    if name not in bases:
        return None
    if off == 0:
        return 0                     # the object's own address: no scaling involved
    if scale is None:
        scale = bases[name]
    if scale is None:
        return None
    return off * scale


def base_pointers(sig, body):
    """{name: step_width} for `this` and any local proven to alias it."""
    this_name, this_w = this_scale(sig)
    if this_name is None:
        return {}
    bases = {this_name: this_w}   # width may be None: only explicitly-cast forms then resolve

    # locals assigned straight from this: `v3 = this;` / `v3 = (char *)this;`
    for m in re.finditer(r"^\s*(\w+)\s*=\s*(.+?);\s*$", body, re.M):
        lhs, rhs = m.group(1), m.group(2).strip()
        if lhs in bases:
            continue
        r = resolve(rhs, bases)
        if r != 0:
            continue
        # width of the alias = the width its own declaration implies
        d = re.search(r"^\s*(?:const\s+|struct\s+)*(.+?)\s*(\*+)\s*" + re.escape(lhs)
                      + r"\s*;", body, re.M)
        if d:
            b = re.sub(r"^(struct|union|enum|const|volatile)\s+", "", d.group(1).strip())
            w = 4 if len(d.group(2)) > 1 else PTR_WIDTH.get(b)
        else:
            w = None
        if w is not None:
            bases[lhs] = w
    return bases


# ---------------------------------------------------------------------------
def flat(body):
    return " ".join(body.split())


# ---------------------------------------------------------------------------
# Format strings. `Displayf("Heading: %f", this[7])` is the single richest line in the binary:
# it pairs a byte offset with the name the 1999 programmer gave the field. A Dump() that walks
# an object prints most of it, in order, with a label each.

PRINTERS = ("Displayf", "Printf", "Warningf", "Errorf", "Quitf", "Msgf", "Printer", "printf")

# The functions where a printed label is a field name rather than a sentence.
DUMPY = re.compile(r"(Dump|Print|Debug|Display|Stats|Report)", re.I)

SPEC = re.compile(r"%[-+ #0]*[\d*]*(?:\.[\d*]+)?(?:hh|h|ll|l|L|z|j|t|I64|I32)?([diuoxXfFeEgGcsp%])")

CONV_TYPE = {"d": ("i32", 4), "i": ("i32", 4), "u": ("u32", 4), "o": ("u32", 4),
             "x": ("u32", 4), "X": ("u32", 4), "f": ("f32", 4), "F": ("f32", 4),
             "e": ("f32", 4), "E": ("f32", 4), "g": ("f32", 4), "G": ("f32", 4),
             "c": ("i8", 1), "s": ("i8*", 4), "p": ("void*", 4)}

READ_CAST = re.compile(r"^\*\s*\(\s*(?:const\s+|volatile\s+)*([A-Za-z_][\w ]*?)\s*\*\s*\)\s*(.*)$")


def resolve_read(expr, bases, fields=None):
    """Offset and width of a member *read*: `this[7]`, `*((__int16 *)this + 4)`, `this->F`."""
    e = " ".join(expr.split())
    width = None

    # peel value casts that are not derefs: `(__int16)this[24]`
    while True:
        m = re.match(r"^\(\s*(?:const\s+|unsigned\s+|signed\s+)*([A-Za-z_][\w ]*?)\s*\)\s*(\*|\w|\().*$",
                     e)
        if not m:
            break
        t = m.group(1).strip()
        if t not in PTR_WIDTH or "*" in e[:e.index(")") + 1].replace("(", ""):
            break
        rest = e[e.index(")") + 1:].strip()
        if not rest:
            break
        e = rest

    # `*(TYPE *)EXPR`
    m = READ_CAST.match(e)
    if m:
        base = re.sub(r"^(struct|union|enum|const|volatile)\s+", "", m.group(1).strip())
        width = PTR_WIDTH.get(base)
        inner = m.group(2).strip()
        if inner.startswith("(") and inner.endswith(")"):
            inner = inner[1:-1].strip()
        off = resolve(inner, bases, fields)
        if off is None or width is None:
            return None, None
        return off, width

    # `*((TYPE *)this + N)` was normalised above only when the parens sit outside; handle the
    # common `*((_DWORD *)this + 4)` shape directly.
    m = re.match(r"^\*\s*\(\s*(.*)\s*\)$", e)
    if m:
        inner = m.group(1).strip()
        c = CAST.match(inner)
        if c and c.group(2):
            width = PTR_WIDTH.get(c.group(1).strip())
        off = resolve(inner, bases, fields)
        if off is None:
            return None, None
        return off, width

    # `*this`
    m = re.match(r"^\*\s*(\w+)$", e)
    if m and m.group(1) in bases:
        return 0, bases[m.group(1)]

    # `this[N]`
    m = re.match(r"^(\w+)\s*\[\s*(0[xX][0-9A-Fa-f]+|\d+)\s*\]$", e)
    if m and m.group(1) in bases:
        w = bases[m.group(1)]
        if w is None:
            return None, None
        return int(m.group(2), 0) * w, w

    if fields:
        m = re.match(r"^(\w+)\s*->\s*(\w+)$", e)
        if m and m.group(1) in bases and m.group(2) in fields:
            return fields[m.group(2)], None

    return None, None


def ident(label):
    """`Num Paths` -> `NumPaths`; `-Ped` -> `Ped`; `StartPos - x` -> `StartPos`."""
    s = label.strip()
    s = re.split(r"[,;]", s)[-1]                     # keep the last clause
    s = s.strip(" \t:=-.'\"[](){}<>/|*\\")
    s = re.sub(r"\s*-\s*[xyzwXYZW]$", "", s)         # `StartPos - x` is one vector
    s = re.sub(r"\s*\(.*?\)\s*$", "", s)
    if not s or not re.match(r"^[A-Za-z]", s):
        return None
    if len(s) > 40 or len(s) < 2:
        return None
    if not re.match(r"^[A-Za-z][A-Za-z0-9 _]*$", s):
        return None
    parts = [p for p in re.split(r"[ _]+", s) if p]
    if len(parts) > 4:
        return None                                  # a sentence, not a field name
    return "".join(p[0].upper() + p[1:] for p in parts)


def mine_format(f, bases, fields, fn):
    """Findings from printf-style calls whose format string labels a member."""
    out = []
    strong = bool(DUMPY.search(" ".join(fn["comments"])))

    for p in PRINTERS:
        for args in call_sites(f, p):
            if len(args) < 2:
                continue
            fmt = args[0].strip()
            m = re.match(r'^"((?:[^"\\]|\\.)*)"$', fmt)
            if not m:
                continue
            text = m.group(1)
            specs = [(mm.start(), mm.end(), mm.group(1)) for mm in SPEC.finditer(text)
                     if mm.group(1) != "%"]
            if not specs or len(specs) != len(args) - 1:
                continue

            prev_end = 0
            pending = None      # (offset, width, name) of the last real field, for %f, %f, %f
            for i, (s0, s1, conv) in enumerate(specs):
                label = text[prev_end:s0]
                prev_end = s1
                off, w = resolve_read(args[i + 1], bases, fields)
                name = ident(label)
                ctype, cw = CONV_TYPE.get(conv, (None, None))

                if off is None:
                    pending = None
                    continue

                if name is None and pending is not None and conv in "fFeEgG":
                    # `Position: %f, %f, %f` - a vector, not three fields
                    po, pw, pn, pc, pt = pending
                    if off == po + pc * 4:
                        pending = (po, pw, pn, pc + 1, pt)
                        continue
                    pending = None
                    continue
                if name is None:
                    pending = None
                    continue

                if pending:
                    out.append(fmt_member(pending, fn, strong))
                width = w or cw
                pending = (off, width, name, 1, ctype)

            if pending:
                out.append(fmt_member(pending, fn, strong))
    return out


def fmt_member(pending, fn, strong):
    off, width, name, comps, ctype = pending
    if comps == 3:
        return dict(offset=off, type="Vector3", name=name, count=0, width=12, unit=12,
                    kind="format" if strong else "format-weak", fn=fn["ea"])
    if comps == 2:
        return dict(offset=off, type="Vector2", name=name, count=0, width=8, unit=8,
                    kind="format" if strong else "format-weak", fn=fn["ea"])
    # the cast fixes the width; the conversion fixes whether it is integer or float
    if ctype == "f32" and width != 4:
        ctype = None
    if ctype in (None, "i32", "u32") and width in (1, 2):
        ctype = {1: "i8", 2: "i16"}[width]
    return dict(offset=off, type=ctype, name=name, count=0, width=width, unit=width,
                kind="format" if strong else "format-weak", fn=fn["ea"])


def mine_function(fn, cls, fields=None):
    """-> list of findings for one function."""
    sig = signature(fn["body"])
    bases = base_pointers(sig, fn["body"])
    if not bases:
        return []
    f = flat(fn["body"])
    found = []

    # ---- datParser::AddRecord(parser, TYPE, "Name", data, count, cb)
    for args in call_sites(f, "datParser::AddRecord"):
        if len(args) < 5:
            continue
        m = re.match(r"^\(?[\w:* ]*\)?\s*(\d+)$", args[1].strip())
        if not m:
            continue
        code = int(m.group(1))
        if code not in DAT_TYPE:
            continue
        nm = re.match(r'^"(.*)"$', args[2].strip())
        if not nm:
            continue
        off = resolve(args[3], bases, fields)
        if off is None:
            continue
        cnt = args[4].strip()
        cnt = int(cnt) if cnt.isdigit() else 1
        ctype, width, _ = DAT_TYPE[code]
        if ctype is None:
            continue
        found.append(dict(offset=off, type=ctype, name=nm.group(1), count=(cnt if cnt > 1 else 0),
                          width=width * cnt, unit=width, kind="datParser", fn=fn["ea"]))

    # ---- datParser::AddValue(parser, "Name", data, cb)   (float and int overloads)
    for args in call_sites(f, "datParser::AddValue"):
        if len(args) < 3:
            continue
        nm = re.match(r'^"(.*)"$', args[1].strip())
        if not nm:
            continue
        off = resolve(args[2], bases, fields)
        if off is None:
            continue
        # the float overload is 0x406850, the int one 0x45FC00; the pseudocode does not say
        # which, but the pointee cast usually does.
        ctype = "i32" if re.search(r"\(\s*int\s*\*\s*\)", args[2]) else "f32"
        found.append(dict(offset=off, type=ctype, name=nm.group(1), count=0, width=4, unit=4,
                          kind="datParser", fn=fn["ea"], note="AddValue overload assumed float"
                          if ctype == "f32" else None))

    # ---- Class::FileIO(this + N, subparser)  -> a sub-object at N
    for m in re.finditer(r"\b(\w+)::FileIO\s*\(", f):
        sub = m.group(1)
        if sub == cls:
            continue
        i = m.end()
        depth, j = 1, i
        while j < len(f) and depth:
            if f[j] == "(":
                depth += 1
            elif f[j] == ")":
                depth -= 1
            j += 1
        args = split_args(f[i:j - 1])
        if not args:
            continue
        off = resolve(args[0], bases, fields)
        if off is None:
            continue
        found.append(dict(offset=off, type=sub, name=None, count=0, width=None, unit=None,
                          kind="subobject", fn=fn["ea"]))

    # ---- Stream::Read/Write(stream, this + N, LEN)
    for which in ("Stream::Read", "Stream::Write"):
        for args in call_sites(f, which):
            if len(args) != 3:
                continue
            off = resolve(args[1], bases, fields)
            if off is None:
                continue
            ln = args[2].strip()
            lm = re.match(r"^(0[xX][0-9A-Fa-f]+|\d+)u?$", ln)
            if not lm:
                continue
            n = int(lm.group(1), 0)
            if n == 0 or n > 0x4000:
                continue
            found.append(dict(offset=off, type=None, name=None, count=0, width=n, unit=n,
                              kind="stream", fn=fn["ea"]))

    # ---- memset(this + N, 0, LEN) -> a block's extent
    for args in call_sites(f, "memset"):
        if len(args) != 3:
            continue
        off = resolve(args[0], bases, fields)
        if off is None:
            continue
        ln = re.match(r"^0[xX]([0-9A-Fa-f]+)u?$|^(\d+)u?$", args[2].strip())
        if not ln:
            continue
        n = int(ln.group(1), 16) if ln.group(1) else int(ln.group(2))
        if n == 0 or n > 0x40000:
            continue
        found.append(dict(offset=off, type=None, name=None, count=0, width=n, unit=n,
                          kind="memset", fn=fn["ea"]))

    # ---- memcpy(this + N, src, LEN) / memcpy(dst, this + N, LEN) -> a copied block's extent
    for which in ("qmemcpy", "memcpy"):
        for args in call_sites(f, which):
            if len(args) != 3:
                continue
            lm = re.match(r"^(0[xX][0-9A-Fa-f]+|\d+)u?$", args[2].strip())
            if not lm:
                continue
            n = int(lm.group(1), 0)
            if n == 0 or n > 0x4000:
                continue
            for side in (0, 1):
                off = resolve(args[side], bases, fields)
                if off is None:
                    continue
                found.append(dict(offset=off, type=None, name=None, count=0, width=n, unit=n,
                                  kind="memcpy", fn=fn["ea"]))

    # ---- Displayf("Heading: %f", this[7])  -> the field's ORIGINAL 1999 NAME
    found.extend(mine_format(f, bases, fields, fn))

    return found


# ---------------------------------------------------------------------------
def mine_class(cls, path, fields=None):
    per_fn = []
    for fn in split_functions(path):
        got = mine_function(fn, cls, fields)
        if got:
            per_fn.append((fn, got))
    return per_fn


def norm_class(sym):
    c = sym.get("class")
    return c.split("::")[0] if c else None


# ---------------------------------------------------------------------------
# MM2Hook's sizeof set is the one source here that is genuinely independent of the IDB, so it is
# what the results are checked against. Two of its entries are not sizes at all:
#
#   a stub - aiPedestrian is recorded as 2 bytes, which no class with 43 methods can be
#   a base placeholder - mmPlayerData is 136 and so is its base mmInfoBase; camTrackCS is 272
#     and so is its base camCarCS. MM2Hook never modelled the derived class's own fields, so its
#     number is the base's size wearing the derived class's name.
#
# Treating either as a ceiling would reject correct layouts, so both are detected and dropped.

def real_size(cls, mm2size, base_of):
    n = mm2size.get(cls)
    if not n or n < 8:
        return None, "stub"
    b = base_of.get(cls)
    if b and mm2size.get(b) == n:
        return None, "base placeholder"
    return n, None


def aggregate(cls, per_fn, by_rva):
    members = {}
    conflicts = []
    evidence = set()
    fns = set()
    unverified = 0

    rank = {"datParser": 4, "format": 3, "subobject": 2, "stream": 1,
            "format-weak": 1, "memset": 0, "memcpy": 0}

    for fn, found in per_fn:
        sym = by_rva.get(fn["ea"])
        if sym is None or norm_class(sym) != cls:
            unverified += len(found)
            continue                          # mislabelled, or not a method of this class
        for f in found:
            fns.add(fn["ea"])
            evidence.add(f["kind"])
            off = f["offset"]
            prev = members.get(off)
            if prev is None:
                members[off] = dict(f, sites=1, source_fn=fn["ea"], source=sym["demangled"])
                continue
            prev["sites"] += 1
            if prev["name"] and f["name"] and prev["name"] != f["name"]:
                conflicts.append("0x%X named both %s and %s" % (off, prev["name"], f["name"]))
                continue
            if rank[f["kind"]] > rank[prev["kind"]] or (prev["name"] is None and f["name"]):
                members[off] = dict(f, sites=prev["sites"], source_fn=fn["ea"],
                                    source=sym["demangled"])

    if not members:
        return None

    ordered = [members[o] for o in sorted(members)]

    # Two members cannot overlap. This is the one contradiction the method can see from the
    # inside, and it is what separates a layout that tiles from one that does not.
    overlaps = 0
    for a, b in zip(ordered, ordered[1:]):
        w = a.get("width")
        if not w:
            continue
        # a bulk read or a memset legitimately spans several members
        if a["kind"] in ("stream", "memset", "memcpy") and w > 8:
            continue
        if a["offset"] + w > b["offset"]:
            overlaps += 1
            conflicts.append("0x%X (%s, %s bytes) overlaps 0x%X (%s)"
                             % (a["offset"], a["name"] or "-", w, b["offset"], b["name"] or "-"))

    lower = max((m["offset"] + (m.get("width") or 4)) for m in ordered)
    own = [m["offset"] for m in ordered if m["offset"] or m["kind"] != "subobject"]
    first_own = min(own) if own else 0

    return dict(members=ordered, conflicts=conflicts, evidence=sorted(evidence),
                fns=sorted(fns), lower=lower, first_own=first_own, overlaps=overlaps,
                named=len([m for m in ordered if m["name"]]), unverified=unverified)


def grade(agg, size_ceiling, base_floor):
    """CONFIRMED / PROBABLE / WEAK, and the corroborations that decided it."""
    checks = []
    bad = agg["overlaps"] > 0

    if size_ceiling:
        if agg["lower"] <= size_ceiling:
            checks.append("fits MM2Hook sizeof 0x%X (lower bound 0x%X)"
                          % (size_ceiling, agg["lower"]))
        else:
            checks.append("EXCEEDS MM2Hook sizeof 0x%X (lower bound 0x%X)"
                          % (size_ceiling, agg["lower"]))
            bad = True

    if base_floor is not None:
        if agg["first_own"] >= base_floor:
            checks.append("first own field 0x%X is at or past base end 0x%X"
                          % (agg["first_own"], base_floor))
        else:
            checks.append("first own field 0x%X is INSIDE the base (ends 0x%X)"
                          % (agg["first_own"], base_floor))

    named_ev = {"datParser", "format"} & set(agg["evidence"])
    if bad:
        return "WEAK", checks
    if named_ev and agg["named"] >= 2:
        return "CONFIRMED", checks
    if named_ev or len([m for m in agg["members"] if m["kind"] != "memset"]) >= 3:
        return "PROBABLE", checks
    return "WEAK", checks


# ---------------------------------------------------------------------------
GAP = re.compile(r"^(field|gap)_?[0-9A-Fa-f]+$")

SCALAR = re.compile(r"^(i8|u8|i16|u16|i32|u32|i64|u64|f32|f64|b32)$")


def opaque(m):
    """True when the IDB member is a container the offset could legitimately point inside.

    Two shapes: an anonymous `u8 field_XX[122]` filler, which is the IDB saying "unidentified
    bytes"; and a nested class member such as `phBoundPolygonal Base`, whose own fields are real
    fields at real offsets that the IDB simply did not flatten. A finding inside either is finer
    resolution than the IDB has. A finding inside a *scalar* member is a genuine contradiction.
    """
    t = m.get("type") or ""
    if not SCALAR.match(t) and not t.endswith("*"):
        return True                                   # a nested struct or class
    if GAP.match(m.get("name") or "") and (m.get("count") or 0) > 1:
        return True                                   # an anonymous byte filler
    return False


def control_group(by_rva, known):
    """Score the extractor on the 188 classes whose layout is already known.

    Only *arithmetic* findings count - `&this->Member` resolves through the very layout being
    tested, so scoring it would be circular and it is excluded.

    An offset landing inside an anonymous `field_XX` byte filler is not a miss. The IDB models
    everything it could not identify as one long `u8[]`; a finding there is finer resolution
    than the IDB has, not a disagreement with it. Only landing inside a member the IDB actually
    typed, or past its sizeof, is a contradiction.
    """
    hit = refine = wrong_inside = wrong_past = 0
    span_ok = span_bad = 0
    detail = []

    for cls in sorted(os.listdir(PSEUDO)):
        if cls not in known:
            continue
        path = os.path.join(PSEUDO, cls, cls + ".c")
        if not os.path.isfile(path):
            continue
        lay = known[cls]
        if len(lay["members"]) < 3:
            continue                      # degenerate: one opaque base, discriminates nothing
        size = lay.get("size") or 0
        offs = sorted(m["offset"] for m in lay["members"])
        by_off = {m["offset"]: m for m in lay["members"]}
        end = {}
        for i, o in enumerate(offs):
            end[o] = offs[i + 1] if i + 1 < len(offs) else size

        def container(o):
            for a in offs:
                if a <= o < end[a]:
                    return a
            return None

        for fn, found in mine_class(cls, path):
            sym = by_rva.get(fn["ea"])
            if sym is None or norm_class(sym) != cls:
                continue
            for f in found:
                o = f["offset"]
                if o in by_off:
                    hit += 1
                    w = f.get("width")
                    if w and w > 8 and f["kind"] in ("stream", "memset", "memcpy"):
                        # a bulk region: it should stop on a member boundary or at the end
                        if (o + w) in by_off or (size and o + w == size):
                            span_ok += 1
                        else:
                            span_bad += 1
                            detail.append("%s region 0x%X+%d ends mid-member" % (cls, o, w))
                elif size and o >= size:
                    wrong_past += 1
                    detail.append("%s 0x%X past IDB sizeof 0x%X (%s)" % (cls, o, size, f["kind"]))
                else:
                    a = container(o)
                    km = by_off.get(a) if a is not None else None
                    if km and opaque(km):
                        refine += 1
                    else:
                        wrong_inside += 1
                        detail.append("%s 0x%X inside %s : %s (%s)"
                                      % (cls, o, km.get("name") if km else "?",
                                         km.get("type") if km else "?", f["kind"]))
    return dict(hit=hit, refine=refine, wrong_inside=wrong_inside, wrong_past=wrong_past,
                span_ok=span_ok, span_bad=span_bad, detail=detail)


# ---------------------------------------------------------------------------
def main():
    with open(os.path.join(ROOT, "data", "symbols.json"), encoding="utf-8") as f:
        syms = json.load(f)
    by_rva = {}
    for s in syms:
        by_rva.setdefault(s["rva"], s)

    code_bytes = defaultdict(int)
    for s in syms:
        c = s.get("class")
        if c and s.get("code"):
            code_bytes[c.split("::")[0]] += s.get("size") or 0

    with open(os.path.join(ROOT, "data", "layouts.json"), encoding="utf-8") as f:
        known = json.load(f)
    with open(os.path.join(ROOT, "data", "mm2types.json"), encoding="utf-8") as f:
        mm2 = json.load(f)
    mm2size = mm2.get("sizes", {})
    base_of = dict(mm2.get("bases", {}))
    hier_path = os.path.join(ROOT, "data", "hierarchy.json")
    if os.path.exists(hier_path):
        with open(hier_path, encoding="utf-8") as f:
            for c, info in json.load(f).items():
                base_of.setdefault(c, info.get("base") or info.get("first_base"))

    cg = control_group(by_rva, known)
    tot = cg["hit"] + cg["refine"] + cg["wrong_inside"] + cg["wrong_past"]
    print("CONTROL GROUP - classes that already have an IDB layout, arithmetic findings only")
    print("  exact hit on a known member start   : %d / %d (%.1f%%)"
          % (cg["hit"], tot, 100.0 * cg["hit"] / max(tot, 1)))
    print("  inside an anonymous u8[] filler     : %d  (refines the IDB, not a disagreement)"
          % cg["refine"])
    print("  inside a member the IDB typed       : %d  WRONG" % cg["wrong_inside"])
    print("  past the IDB sizeof                 : %d  WRONG" % cg["wrong_past"])
    print("  bulk regions ending on a boundary   : %d ok, %d not" % (cg["span_ok"], cg["span_bad"]))
    for d in cg["detail"][:20]:
        print("    ! " + d)
    print()

    out = OrderedDict()
    layouts = OrderedDict()
    agree = disagree = floor_ok = floor_bad = 0

    for cls in sorted(os.listdir(PSEUDO)):
        path = os.path.join(PSEUDO, cls, cls + ".c")
        if not os.path.isfile(path) or cls not in code_bytes:
            continue
        per_fn = mine_class(cls, path)
        if not per_fn:
            continue
        agg = aggregate(cls, per_fn, by_rva)
        if agg is None:
            continue

        ceiling, why = real_size(cls, mm2size, base_of)
        base = base_of.get(cls)
        floor, _ = real_size(base, mm2size, base_of) if base else (None, None)
        # a floor only means anything when the class actually has members past offset 0
        if floor and agg["first_own"] == 0:
            floor = None

        conf, checks = grade(agg, ceiling, floor)
        if ceiling:
            if agg["lower"] <= ceiling:
                agree += 1
            else:
                disagree += 1
        if floor:
            if agg["first_own"] >= floor:
                floor_ok += 1
            else:
                floor_bad += 1

        entry = OrderedDict()
        entry["kind"] = "struct"
        entry["confidence"] = conf
        entry["already_known"] = cls in known
        entry["code_bytes"] = code_bytes[cls]
        entry["size"] = ceiling
        entry["size_confirmed_by"] = "MM2Hook sizeof" if ceiling else None
        entry["size_note"] = ("MM2Hook's %s for this class is a %s and was not used as a ceiling"
                              % (mm2size.get(cls), why)) if why else None
        entry["size_lower_bound"] = agg["lower"]
        entry["size_lower_bound_hex"] = "0x%X" % agg["lower"]
        entry["base"] = base
        entry["base_size"] = floor
        # How much of the object this actually accounts for. A class is only portable once every
        # byte the remaining assembly touches is declared, so a high confidence with 12% coverage
        # is a good start and not a finished layout.
        spans = [(0, floor)] if floor else []
        spans += [(m["offset"], m["offset"] + (m.get("width") or 4)) for m in agg["members"]]
        covered, last = 0, -1
        for a, b in sorted(spans):
            a = max(a, last)
            if b > a:
                covered += b - a
                last = b
        entry["bytes_accounted_for"] = covered
        entry["highest_byte_touched"] = agg["lower"]
        entry["coverage_pct"] = round(100.0 * covered / ceiling, 1) if ceiling else None
        entry["evidence"] = agg["evidence"]
        entry["corroboration"] = checks
        entry["source_functions"] = ["0x%X" % a for a in agg["fns"]]
        entry["named_fields"] = agg["named"]
        entry["conflicts"] = agg["conflicts"] + [c for c in checks
                                                 if "EXCEEDS" in c or "INSIDE" in c]
        entry["dropped_unverified_findings"] = agg["unverified"]
        entry["members"] = [
            OrderedDict(offset=m["offset"], offset_hex="0x%X" % m["offset"], type=m["type"],
                        name=m["name"] or ("field_%X" % m["offset"]),
                        recovered_name=bool(m["name"]), count=m["count"], width=m["width"],
                        evidence=m["kind"], source_fn="0x%X" % m["source_fn"], sites=m["sites"])
            for m in agg["members"]]
        layouts[cls] = entry

    out["_meta"] = OrderedDict(
        generated_by="tools/serialization_layouts.py",
        source="MM2_RE_KIT/MM2_PSEUDOCODE/<Class>/<Class>.c, every source function verified by "
               "address against data/symbols.json (the retail linker map)",
        schema="data/layouts.json plus per-class confidence/evidence/corroboration and a "
               "per-member evidence kind and source function",
        WARNING="'size_lower_bound' is a FLOOR, never a size. Only 'size' (present when MM2Hook "
                "corroborates) may be fed to check_size(). Pinning a struct too small is a "
                "silent corruption.",
        datParser_type_enum={str(k): v[2] for k, v in DAT_TYPE.items()},
        evidence_kinds=OrderedDict(
            datParser="datParser::AddRecord/AddValue - offset, width and the 1999 field NAME",
            format="a printf-style call inside a Dump/Print/Debug method - offset, width, NAME",
            **{"format-weak": "a printf-style call somewhere else - the label may be prose"},
            subobject="Base::FileIO(this + N, ...) - a sub-object of a known class at N",
            stream="Stream::Read/Write(s, this + N, LEN) - offset and exact width, no name",
            memset="memset(this + N, 0, LEN) - a block's extent only",
            memcpy="memcpy/qmemcpy touching this + N - a copied block's extent only"),
        control_group=OrderedDict(
            exact_hits=cg["hit"], refines_an_unnamed_filler=cg["refine"],
            inside_a_typed_member=cg["wrong_inside"], past_idb_sizeof=cg["wrong_past"],
            total=tot, accuracy=round(100.0 * cg["hit"] / max(tot, 1), 1),
            regions_ending_on_a_boundary=cg["span_ok"], regions_not=cg["span_bad"]),
        independent_checks=OrderedDict(
            mm2hook_sizeof_agrees=agree, mm2hook_sizeof_disagrees=disagree,
            base_size_floor_agrees=floor_ok, base_size_floor_disagrees=floor_bad),
    )
    out["layouts"] = layouts

    with open(OUT, "w", encoding="utf-8") as f:
        json.dump(out, f, indent=1)

    new = [c for c, e in layouts.items() if not e["already_known"]]
    print("INDEPENDENT CHECKS (MM2Hook sizeof, which does not come from the IDB)")
    print("  lower bound fits the sizeof         : %d classes agree, %d disagree"
          % (agree, disagree))
    print("  first own field at or past base end : %d agree, %d disagree" % (floor_ok, floor_bad))
    print()
    print("wrote %s" % OUT)
    print("  classes with findings   : %d (%d already had a layout, %d new)"
          % (len(layouts), len(layouts) - len(new), len(new)))
    for g in ("CONFIRMED", "PROBABLE", "WEAK"):
        cg2 = [c for c in new if layouts[c]["confidence"] == g]
        print("  %-9s : %3d classes, %8d bytes of code, %4d named fields"
              % (g, len(cg2), sum(layouts[c]["code_bytes"] for c in cg2),
                 sum(layouts[c]["named_fields"] for c in cg2)))
    return out


if __name__ == "__main__":
    main()
