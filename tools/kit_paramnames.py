# -*- coding: utf-8 -*-
"""Recover parameter names from the kit's function signatures into data/param_names.json.

A linker map gives a parameter's TYPE - that is what the mangled name encodes - but never its
name, so tools/genheaders.py has nothing to call them and emits arg1, arg2, arg3. Measured against
Brick's tree with CODE_STANDARD/style_scan.py, that is this codebase's largest style divergence:
91% of his parameters are snake_case names against 7% of ours, with 93% of ours still argN.

The kit prints each function's real signature above its body:

    void __thiscall aiIntersection::AddRoad(aiIntersection *this, aiPath *road)

Most of those names are Hex-Rays placeholders (a1, a2) and are skipped, but 1,780 across 1,157
functions are real - `road`, `fillColor`, `width`, `height`, `lhs`, `rhs` - and a real name in a
declaration is worth more than a whole comment explaining what argN meant.

THIS DOES NOT CLOSE THE GAP AND IS NOT MEANT TO. Brick's 91% comes from naming parameters by hand
while porting, which is part of the work rather than something to automate. This only ensures that
where the name was already recovered, it is not thrown away.

ARITY IS CHECKED BEFORE ANY NAME IS USED. The mangled name is the authority on how many parameters
a function takes; the kit signature is recovered and can disagree, usually where Hex-Rays inferred
a different calling convention. Naming parameters from a signature with a different count would
attach names to the wrong types, which is worse than argN because it reads as if someone checked.
So a disagreement drops the whole entry.
"""
import glob
import json
import os
import re
import sys
from collections import OrderedDict

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
sys.path.insert(0, HERE)

PSEUDO = os.path.join(ROOT, "MM2_RE_KIT", "MM2_PSEUDOCODE")
OUT = os.path.join(ROOT, "data", "param_names.json")

MANGLED = re.compile(r"^// (\?\??[^\s]+)$")
SIGNATURE = re.compile(
    r"^[A-Za-z_].*?\b__(?:thiscall|cdecl|stdcall|fastcall|usercall)\s+.*?\((.*)\)\s*$")
PLACEHOLDER = re.compile(r"^a\d+$")

# C++ keywords and arts type names a parameter must not be called, in case a signature is parsed
# badly enough that the type ends up in the name position.
RESERVED = {
    "int", "float", "double", "char", "short", "long", "unsigned", "signed", "void", "bool",
    "const", "struct", "class", "union", "enum", "this", "operator", "return", "case",
    "i8", "i16", "i32", "i64", "u8", "u16", "u32", "u64", "f32", "f64",
}


def parameter_names(text):
    """Names from one signature's argument list, with None where the kit had a placeholder.

    `this` is dropped: it is the implicit receiver of a __thiscall, not a declared parameter, and
    the mangled name does not count it either.
    """
    out = []
    for arg in (a.strip() for a in text.split(",")):
        if not arg:
            continue
        parts = arg.replace("*", " ").replace("&", " ").split()
        name = parts[-1] if parts else ""
        # An unnamed parameter - `int` alone, or an array suffix - has nothing to take.
        if len(parts) < 2 or name == "this":
            if name != "this":
                out.append(None)
            continue
        if PLACEHOLDER.match(name) or name in RESERVED or not re.fullmatch(r"[A-Za-z_]\w*", name):
            out.append(None)
        else:
            out.append(name)
    return out


def main():
    if not os.path.isdir(PSEUDO):
        sys.exit("no %s - the kit is gitignored; every contributor generates their own" % PSEUDO)

    found = OrderedDict()
    pending = None
    files = 0

    for path in sorted(glob.glob(os.path.join(PSEUDO, "*", "*.c"))):
        files += 1
        with open(path, encoding="latin-1", errors="replace") as f:
            for line in f:
                line = line.rstrip("\n")
                m = MANGLED.match(line)
                if m:
                    pending = m.group(1)
                    continue
                if pending is None:
                    continue
                s = SIGNATURE.match(line)
                if not s:
                    continue
                names = parameter_names(s.group(1))
                if any(names):
                    found[pending] = names
                pending = None

    named = sum(1 for v in found.values() for n in v if n)
    print("pseudocode files scanned      : %d" % files)
    print("functions with a named parameter: %d" % len(found))
    print("named parameters              : %d" % named)

    with open(OUT, "w", encoding="utf-8") as f:
        json.dump(found, f, indent=1)
    print("wrote %s" % OUT)


if __name__ == "__main__":
    main()
