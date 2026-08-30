# -*- coding: utf-8 -*-
"""Emit data/ghidra_symbols.tsv for the Ghidra import script.

Ghidra's own analysis of a stripped 1999 binary names everything FUN_004c8af0. We already know
all 10,608 real names and their addresses, so applying them BEFORE decompiling is what turns the
output from unreadable into reviewable: every call site shows a real class and method name.

Addresses are the corrected ones - see the 0x308 .rdata note in tools/pe.py.
"""
import json
import os

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)

with open(os.path.join(ROOT, "data", "symbols.json"), encoding="utf-8") as f:
    syms = json.load(f)

out = os.path.join(ROOT, "data", "ghidra_symbols.tsv")
n = 0

with open(out, "w", encoding="utf-8", newline="\n") as f:
    f.write("va\tmangled\tsize\tcode\tclass\tname\n")
    for s in syms:
        f.write("%08X\t%s\t%d\t%d\t%s\t%s\n" % (
            s["rva"], s["mangled"], s.get("size") or 0, 1 if s["code"] else 0,
            s.get("class") or "", s.get("name") or ""))
        n += 1

print("wrote %s (%d symbols)" % (out, n))
