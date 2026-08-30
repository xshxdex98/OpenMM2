# -*- coding: utf-8 -*-
"""Re-key Ghidra's decompiler output by mangled name, into the shape MM2_RE_KIT uses.

tools/ghidra/ExportDecomp.java writes one block per function, headed by whatever name Ghidra holds
for it. That is the DEMANGLED name for almost everything - `mmLinearGauge::Init`, `__setargv` -
because the importer demangles on load, so 10,425 of 11,105 functions landed in one `_other` file
and only 680 could be matched by mangled name.

Re-running the export with better naming would cost another full decompile. It is not needed: every
block carries `ea=0x…`, and data/symbols.json maps an address to the mangled name the linker map
gave it. So this re-keys what is already on disk.

Output is <out>/<Class>/<Class>.c with a `// <mangled>` line above each body - byte for byte the
convention MM2_RE_KIT/MM2_PSEUDOCODE already uses, so every tool and every habit built around the
kit reads this without changing.

WHERE IT WRITES, AND WHY THAT MATTERS. Into MM2_RE_KIT/GHIDRA_DECOMP, inside the directory
.gitignore excludes wholesale. This is derived from a copyrighted binary; it is never committed,
and every contributor regenerates their own - the same rule as game.asm.

    py tools/ghidra/... (see ExportDecomp.java)   # produces C:/mm2ghidra/out/decomp
    py tools/decomp_index.py                      # re-keys it into MM2_RE_KIT/GHIDRA_DECOMP
"""
import collections
import glob
import json
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)

SRC = os.environ.get("MM2_DECOMP_RAW", r"C:\mm2ghidra\out\decomp")
OUT = os.path.join(ROOT, "MM2_RE_KIT", "GHIDRA_DECOMP")

SEPARATOR = "// ------------------------------------------------------------------"
EA = re.compile(r"^// ea=0x([0-9A-Fa-f]{8})\s")


def class_of(mangled):
    """The class a mangled name belongs to, or _free for a function that belongs to none."""
    if not mangled.startswith("?"):
        return "_other"
    at = mangled.find("@@")
    if at < 0:
        return "_other"
    head = mangled[1:at]
    sep = head.find("@")
    if sep < 0:
        return "_free"
    cls = head[sep + 1:].split("@")[0]
    return re.sub(r"[^A-Za-z0-9_]", "_", cls) or "_free"


def blocks(path):
    """Yield each function block from one exported file."""
    with open(path, encoding="utf-8", errors="replace") as f:
        current = None
        for line in f:
            if line.rstrip("\n") == SEPARATOR:
                if current:
                    yield current
                current = []
                continue
            if current is not None:
                current.append(line.rstrip("\n"))
        if current:
            yield current


def main():
    symbols = os.path.join(ROOT, "data", "symbols.json")
    if not os.path.exists(symbols):
        sys.exit("no data/symbols.json - run tools/symbols.py first")

    with open(symbols, encoding="utf-8") as f:
        by_addr = {s["rva"]: s["mangled"] for s in json.load(f) if s.get("code")}

    files = sorted(glob.glob(os.path.join(SRC, "*", "*.c")))
    if not files:
        sys.exit("no decompiler output under %s - run ExportDecomp.java first" % SRC)

    grouped = collections.defaultdict(list)
    total = unnamed = 0

    for path in files:
        for block in blocks(path):
            total += 1
            addr = None
            for line in block[:4]:
                m = EA.match(line)
                if m:
                    addr = int(m.group(1), 16)
                    break

            mangled = by_addr.get(addr) if addr is not None else None
            if mangled is None:
                # No entry in the linker map: a thunk, a nullsub, or something Ghidra found that
                # was never a named function. Kept rather than dropped - the body is still the only
                # decompilation of those bytes - but parked where it cannot be mistaken for a
                # method of some class.
                unnamed += 1
                mangled = "sub_%08X" % addr if addr is not None else "sub_UNKNOWN"

            body = [ln for ln in block if not ln.startswith("// ea=0x")]
            while body and not body[0].strip():
                body.pop(0)

            grouped[class_of(mangled)].append(
                (addr if addr is not None else 0, mangled, body))

    os.makedirs(OUT, exist_ok=True)
    for cls, items in grouped.items():
        items.sort(key=lambda t: t[0])
        d = os.path.join(OUT, cls)
        os.makedirs(d, exist_ok=True)
        with open(os.path.join(d, cls + ".c"), "w", encoding="utf-8", newline="\n") as w:
            w.write("// ===== %s - %d functions, decompiled by Ghidra =====\n" % (cls, len(items)))
            for addr, mangled, body in items:
                w.write("\n" + SEPARATOR + "\n")
                w.write("// %s\n" % mangled)
                w.write("// ea=0x%08X\n\n" % addr)
                w.write("\n".join(body).rstrip() + "\n")

    named = total - unnamed
    print("blocks read            : %d" % total)
    print("  keyed to a mangled name: %d" % named)
    print("  no linker-map entry    : %d  (kept as sub_ADDRESS)" % unnamed)
    print("classes written        : %d" % len(grouped))
    print("wrote %s" % OUT)


if __name__ == "__main__":
    main()
