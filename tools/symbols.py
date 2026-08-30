# -*- coding: utf-8 -*-
"""Turn midtown2.map into structured symbol data.

The linker map that shipped with Midtown Madness 2 is the whole reason this project is possible.
It carries 9,142 mangled MSVC symbols, each with an address, and a mangled name is not just a
name: it encodes access, virtualness, calling convention, return type and every parameter. So the
map alone yields most of a class declaration for 525 classes without decompiling anything.

Symbols are listed in address order, which also gives us function SIZE for free - the next
symbol's address minus this one. Size is the best cheap proxy for "how hard is this to
reimplement", so it drives the porting order.

Output is data/symbols.json, consumed by genheaders.py.
"""
import json
import os
import re
import subprocess
import sys
from collections import OrderedDict

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from mangled import qualified as qualified_name
from pe import load_sections, va_from_section

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)

MAP = os.environ.get("MM2_MAP", r"C:\Users\xshxd\OneDrive\Desktop\PC Games\MM2\midtown2.map")
OUT = os.path.join(ROOT, "data", "symbols.json")

UNDNAME = os.environ.get(
    "UNDNAME",
    r"C:\Program Files\Microsoft Visual Studio\18\Community\VC\Tools\MSVC"
    r"\14.51.36231\bin\Hostx64\x86\undname.exe")

# " 0001:00000010       ?ProgressRect@@YAXHHHHI@Z  00401010     <unknown>"
LINE = re.compile(r"^\s*([0-9a-fA-F]{4}):([0-9a-fA-F]{8})\s+(\S+)\s+([0-9a-fA-F]{8})\s+(.*?)\s*$")

# undname prints a block per symbol; the final line is the readable form.
UNDNAME_OUT = re.compile(r'^is :- "(.*)"$')

CODE_SECTION = 1  # .text, per the map header


def read_map(path):
    """Every public symbol, in address order, with the section it lives in."""
    syms = []
    started = False

    with open(path, "r", encoding="utf-8", errors="replace") as f:
        for line in f:
            if "Publics by Value" in line:
                started = True
                continue
            if not started:
                continue

            m = LINE.match(line)
            if not m:
                continue

            section, offset, name, rva, lib = m.groups()
            syms.append(OrderedDict(
                mangled=name,
                section=int(section, 16),
                offset=int(offset, 16),
                rva=int(rva, 16),
                lib=lib,
            ))

    return syms


def demangle(names, batch=180):
    """Mangled -> full signature, via undname.exe.

    Batched because a process launch per symbol is ~9,000 launches. 180 keeps the command line
    well under the Windows limit even for the long template-ish names.
    """
    out = {}

    for i in range(0, len(names), batch):
        chunk = names[i:i + batch]
        try:
            res = subprocess.run([UNDNAME] + chunk, capture_output=True, text=True, timeout=120)
        except (OSError, subprocess.SubprocessError) as exc:
            sys.exit("undname failed: %s" % exc)

        got = [m.group(1) for m in
               (UNDNAME_OUT.match(l.strip()) for l in res.stdout.splitlines()) if m]

        # undname emits exactly one "is :-" per input, in order. If that ever stops being true we
        # would silently misalign every name after the gap, so refuse rather than guess.
        if len(got) != len(chunk):
            sys.exit("undname returned %d results for %d symbols (batch at %d)"
                     % (len(got), len(chunk), i))

        out.update(zip(chunk, got))
        sys.stderr.write("\r  demangled %d/%d" % (min(i + batch, len(names)), len(names)))

    sys.stderr.write("\n")
    return out


CALLCONV = re.compile(r"\b(__thiscall|__cdecl|__stdcall|__fastcall)\b")
ACCESS = re.compile(r"^(public|protected|private):\s*")


def split_params(text):
    """Split a parameter list on top-level commas.

    Naive splitting breaks on `class Bar<int,int>` and on function-pointer parameters like
    `void (__cdecl*)(int,int)`, both of which occur in this map.
    """
    parts, depth, cur = [], 0, ""

    for ch in text:
        if ch in "<([":
            depth += 1
        elif ch in ">)]":
            depth -= 1

        if ch == "," and depth == 0:
            parts.append(cur.strip())
            cur = ""
        else:
            cur += ch

    if cur.strip():
        parts.append(cur.strip())

    return [] if parts == ["void"] else parts


def split_qualified(name):
    """`aiIntersection::SinkPaths` -> ("aiIntersection", "SinkPaths").

    Splits on the LAST `::` that is not inside template brackets, so a nested class keeps its
    full outer path as the class name.
    """
    depth, cut = 0, -1

    for i, ch in enumerate(name):
        if ch == "<":
            depth += 1
        elif ch == ">":
            depth -= 1
        elif ch == ":" and depth == 0 and name[i:i + 2] == "::":
            cut = i

    if cut < 0:
        return None, name

    return name[:cut], name[cut + 2:]


def parse(mangled, demangled):
    """Break a demangled signature into the pieces a declaration needs."""
    sym = OrderedDict(mangled=mangled, demangled=demangled)
    text = demangled.strip()

    m = ACCESS.match(text)
    if m:
        sym["access"] = m.group(1)
        text = text[m.end():]
    else:
        sym["access"] = "public"

    sym["static"] = False
    sym["virtual"] = False

    while True:
        if text.startswith("static "):
            sym["static"] = True
            text = text[7:]
        elif text.startswith("virtual "):
            sym["virtual"] = True
            text = text[8:]
        else:
            break

    m = CALLCONV.search(text)

    if not m:
        # No calling convention means this is not a function: a vftable, a global, or a static
        # member. Those still matter - a vftable address is how we recover the virtual layout.
        sym["kind"] = "data"

        q = qualified_name(mangled)
        if q and q[1]:
            sym["class"], sym["name"] = q[0], q[1]
        else:
            sym["class"], sym["name"] = split_qualified(text)

        # Whatever precedes the qualified name is the declared type. For a function-pointer
        # global the name sits inside the parentheses, so cut on the name rather than on a space.
        full = "%s::%s" % (sym["class"], sym["name"]) if sym["class"] else (sym["name"] or "")
        at = text.find(full) if full else -1
        sym["type"] = text[:at].strip() or None if at > 0 else None

        if "`vftable'" in text:
            sym["kind"] = "vftable"
        elif "`vbtable'" in text:
            sym["kind"] = "vbtable"
        elif "`string'" in text:
            sym["kind"] = "string"

        return sym

    sym["kind"] = "function"
    sym["callconv"] = m.group(1)
    sym["type"] = text[:m.start()].strip() or None  # empty for constructors and destructors

    rest = text[m.end():].strip()

    open_paren = rest.find("(")
    if open_paren < 0:
        sym["kind"] = "unparsed"
        return sym

    qual_text = rest[:open_paren].strip()

    # Walk to the matching close paren rather than using rfind: a function-pointer parameter
    # carries its own parens and the trailing `const` sits after the real one.
    depth, close = 0, -1
    for i in range(open_paren, len(rest)):
        if rest[i] == "(":
            depth += 1
        elif rest[i] == ")":
            depth -= 1
            if depth == 0:
                close = i
                break

    if close < 0:
        sym["kind"] = "unparsed"
        return sym

    sym["params"] = split_params(rest[open_paren + 1:close])
    sym["const"] = "const" in rest[close + 1:]

    q = qualified_name(mangled)
    if q and q[1]:
        cls, name = q[0], q[1]
    else:
        cls, name = split_qualified(qual_text)

    sym["class"] = cls
    sym["name"] = name

    if cls:
        base = cls.split("::")[-1]
        if name == base:
            sym["kind"] = "constructor"
        elif name == "~" + base:
            sym["kind"] = "destructor"
        elif name.startswith("operator"):
            sym["kind"] = "operator"
    elif name.startswith("operator"):
        sym["kind"] = "operator"

    return sym


PREFIX = re.compile(r"^([a-z]{2,4})(?=[A-Z])")


def subsystem(cls, name):
    """The lowercase prefix an ARTS class carries: mmGameManager -> mm, aiPath -> ai.

    Angel Studios used this prefix as the module marker, so it is what decides which directory a
    class lands in. Anything without one goes to `misc` for a human to place.
    """
    probe = cls or name

    if not probe:
        return "misc"

    m = PREFIX.match(probe)
    return m.group(1) if m else "misc"


def main():
    if not os.path.exists(MAP):
        sys.exit("map not found: %s" % MAP)

    print("reading %s" % MAP)
    syms = read_map(MAP)
    print("  %d public symbols" % len(syms))

    # Sizes: distance to the next symbol in the same section. The map is emitted in address order
    # but do not rely on that - sort explicitly.
    syms.sort(key=lambda s: (s["section"], s["offset"]))

    for i, s in enumerate(syms):
        nxt = syms[i + 1] if i + 1 < len(syms) else None
        if nxt and nxt["section"] == s["section"]:
            s["size"] = nxt["offset"] - s["offset"]
        else:
            s["size"] = None  # last in its section, unknowable from the map alone

    # Recompute every address from `section:offset` against the real PE section table. The map's
    # own Rva+Base column is wrong for .rdata by exactly 0x308, which pointed vftable reads at the
    # wrong table entirely, so it is kept only to report the discrepancy.
    exe = os.environ.get("MM2_EXE", MAP[:-4] + ".exe")
    if os.path.exists(exe):
        _, sections = load_sections(exe)
        deltas = {}

        for s in syms:
            va = va_from_section(sections, s["section"], s["offset"])
            if va is None:
                continue
            deltas.setdefault(s["section"], set()).add(s["rva"] - va)
            s["map_rva"] = s["rva"]
            s["rva"] = va

        for sec in sorted(deltas):
            d = deltas[sec]
            if d != {0}:
                print("  section %d: map Rva+Base is off by %s" %
                      (sec, ", ".join("0x%X" % x for x in sorted(d))))
    else:
        print("  %s not found; using the map's Rva+Base column unverified" % exe)

    names = [s["mangled"] for s in syms]
    print("demangling via undname")
    table = demangle(names)

    out = []
    for s in syms:
        parsed = parse(s["mangled"], table[s["mangled"]])
        parsed["rva"] = s["rva"]
        parsed["map_rva"] = s.get("map_rva", s["rva"])
        parsed["size"] = s["size"]
        parsed["section"] = s["section"]
        parsed["code"] = (s["section"] == CODE_SECTION)
        parsed["subsystem"] = subsystem(parsed.get("class"), parsed.get("name"))
        out.append(parsed)

    classes = {}
    for s in out:
        if s.get("class"):
            classes.setdefault(s["class"], []).append(s)

    os.makedirs(os.path.dirname(OUT), exist_ok=True)
    with open(OUT, "w", encoding="utf-8") as f:
        json.dump(out, f, indent=1)

    subs = {}
    for s in out:
        subs[s["subsystem"]] = subs.get(s["subsystem"], 0) + 1

    kinds = {}
    for s in out:
        kinds[s["kind"]] = kinds.get(s["kind"], 0) + 1

    code_bytes = sum(s["size"] or 0 for s in out if s["code"])

    print("\nwrote %s" % OUT)
    print("  classes      : %d" % len(classes))
    print("  code bytes    : %d (%.2f MB)" % (code_bytes, code_bytes / 1048576.0))
    print("\n  by kind:")
    for k, n in sorted(kinds.items(), key=lambda kv: -kv[1]):
        print("    %-14s %5d" % (k, n))
    print("\n  by subsystem:")
    for k, n in sorted(subs.items(), key=lambda kv: -kv[1])[:24]:
        print("    %-10s %5d" % (k, n))


if __name__ == "__main__":
    main()
