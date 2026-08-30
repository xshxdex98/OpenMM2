# -*- coding: utf-8 -*-
"""Check every __thiscall symbol's `ret imm16` against the parameter list its name encodes.

A mangled name is a contract about the stack: MSVC turns the parameter list into a fixed
`ret imm16` in the callee. So if the map says one argument and the machine code pops two, the
symbol CANNOT be reimplemented - declaring it the way the name demands produces a function that
pops four bytes where the assembly's callers pushed eight, and the stack is off by four from then
on. There is no diagnostic, and the failure surfaces a subsystem away from vector7.

Six symbols in Vector3 are in exactly that state; see Trap 14 in docs/port_order.md. This script
is how to find the next one before writing code against it, not after.

SIZE COMES FROM THE KIT, NOT THE MAP. The map's size is distance-to-the-next-symbol, so it
absorbs unnamed static constructors and helpers (Trap 7) and the "last ret" would then belong to
somebody else's function. MM2_RE_KIT's _INDEX.csv carries IDA's real function extents, so that is
used wherever it has an entry for the address.

WHAT IT DOES NOT KNOW. The size a parameter occupies is read out of the DEMANGLED text, and a
class passed by value occupies its own sizeof, which the demangled text does not give. Rather than
guess, any symbol with a by-value class parameter is reported as SKIPPED rather than as a
mismatch - so a clean run means "nothing suspicious among the ones I can decide", not "everything
is fine". Functions that end in a tail call rather than a `ret` are skipped for the same reason.

    py tools/verify_arity.py            # summary plus the confident mismatches
    py tools/verify_arity.py --all      # also list what was skipped and why
"""
import csv
import json
import os
import re
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)

sys.path.insert(0, HERE)
from pe import IMAGE_BASE, load_sections, to_offset

SYMS = os.path.join(ROOT, "data", "symbols.json")
KIT_INDEX = os.path.join(ROOT, "MM2_RE_KIT", "MM2_PSEUDOCODE", "_INDEX.csv")
EXE = os.environ.get("MM2_EXE", r"C:\mm2ghidra\bin\midtown2.exe")

# Types whose stack footprint is unambiguous from the demangled spelling alone.
SCALAR4 = ("int", "unsigned int", "float", "long", "unsigned long", "short", "unsigned short",
           "char", "unsigned char", "signed char", "bool", "void")
SCALAR8 = ("double", "__int64", "unsigned __int64", "long double")


def split_params(demangled):
    """The top-level parameter list, or None if there is no parenthesised list."""
    m = re.search(r"\((.*)\)", demangled)
    if not m:
        return None

    inner = m.group(1).strip()
    if inner in ("", "void"):
        return []

    depth = 0
    parts = []
    cur = ""
    for ch in inner:
        if ch in "(<":
            depth += 1
        elif ch in ")>":
            depth -= 1
        if ch == "," and depth == 0:
            parts.append(cur)
            cur = ""
        else:
            cur += ch
    parts.append(cur)
    return [p.strip() for p in parts]


def stack_bytes(params):
    """Bytes the parameters occupy, or None when one of them cannot be sized confidently."""
    total = 0
    for p in params:
        base = p.replace("const", "").replace("volatile", "").strip()
        if base.endswith("&") or base.endswith("*"):
            total += 4
        elif base in SCALAR8:
            total += 8
        elif base in SCALAR4:
            total += 4
        elif base.startswith("enum "):
            total += 4
        else:
            # A class or struct by value. Its sizeof is not in the demangled text.
            return None
    return total


def terminating_ret(code):
    """The imm16 of the function's final `ret`, 0 for a bare `ret`, or None if it ends otherwise."""
    body = code.rstrip(b"\x90")
    if not body:
        return None
    if body[-1] == 0xC3:
        return 0
    if len(body) >= 3 and body[-3] == 0xC2:
        return struct.unpack_from("<H", body, len(body) - 2)[0]
    return None


def kit_sizes():
    """ea -> IDA's function size, for the addresses the RE kit decompiled."""
    if not os.path.exists(KIT_INDEX):
        return {}

    sizes = {}
    with open(KIT_INDEX, newline="", encoding="utf-8", errors="replace") as f:
        for row in csv.DictReader(f):
            sizes[int(row["ea"], 16)] = int(row["size"])
    return sizes


def main():
    show_all = "--all" in sys.argv

    if not os.path.exists(EXE):
        sys.exit("binary not found: %s (set MM2_EXE)" % EXE)

    data, sections = load_sections(EXE)

    def read(va, n):
        off = to_offset(sections, va - IMAGE_BASE)
        return data[off:off + n] if off is not None else None

    with open(SYMS, encoding="utf-8") as f:
        symbols = json.load(f)

    real_size = kit_sizes()

    agree = 0
    skipped = []
    mismatched = []

    for s in symbols:
        if not s.get("code"):
            continue

        demangled = s.get("demangled") or ""
        if "__thiscall" not in demangled or "..." in demangled:
            continue

        size = real_size.get(s["rva"]) or s.get("size") or 0
        if not 0 < size <= 8192:
            skipped.append((s, "no usable size"))
            continue

        params = split_params(demangled)
        if params is None:
            skipped.append((s, "no parameter list"))
            continue

        expected = stack_bytes(params)
        if expected is None:
            skipped.append((s, "by-value class parameter"))
            continue

        code = read(s["rva"], size)
        if code is None:
            skipped.append((s, "outside a mapped section"))
            continue

        actual = terminating_ret(code)
        if actual is None:
            skipped.append((s, "does not end in a ret"))
            continue

        # A function returning a class by value takes a hidden return pointer as its first
        # argument, so it pops four more than its named parameters. The demangled return type is
        # the only hint that this is happening, and it sits before the __thiscall keyword.
        head = demangled.split("__thiscall")[0]
        hidden = not any(t in head for t in SCALAR4 + SCALAR8)

        if actual == expected or (hidden and actual == expected + 4):
            agree += 1
        else:
            mismatched.append((s, expected, actual))

    print("__thiscall symbols checked: %d agree, %d MISMATCH, %d skipped"
          % (agree, len(mismatched), len(skipped)))

    if mismatched:
        print("\nname says one stack size, the code pops another - these cannot be ported as")
        print("declared; see Trap 14 in docs/port_order.md:")
        for s, expected, actual in sorted(mismatched, key=lambda r: r[0]["rva"]):
            print("  0x%06X  name=%2d code=%2d  %s" % (s["rva"], expected, actual, s["mangled"]))

    if show_all:
        print("\nskipped:")
        for s, why in sorted(skipped, key=lambda r: r[0]["rva"]):
            print("  0x%06X  %-26s %s" % (s["rva"], why, s["mangled"]))

    return 0


if __name__ == "__main__":
    sys.exit(main())
