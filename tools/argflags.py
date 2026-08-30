# -*- coding: utf-8 -*-
"""Every command-line option midtown2.exe actually parses.

MM2's options are not documented anywhere and are not guessable: they are string literals pushed
immediately before a call to one of the datArgParser entry points. This walks .text, finds those
calls, and reads the pushed string back out of .rdata/.data - so the list is what the binary does,
not what a readme claims.

Written to answer a specific question - whether the game can be launched straight into gameplay, so
that the selected resolution can be measured rather than inferred, since the frontend is hardcoded
to 640x480 - but the full list is worth having on its own for the moddability work.
"""
import json
import os
import struct
import sys

import capstone

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
sys.path.insert(0, HERE)

from pe import IMAGE_BASE
from verify_data import sections

RETAIL = os.environ.get(
    "MM2_RETAIL", "C:/Users/xshxd/OneDrive/Desktop/PC Games/MM2/midtown2.exe")

# The parser entry points, from data/symbols.json.
PARSERS = {
    0x004C6190: "Exists(name)",
    0x004C61C0: "Get(name, i, int&)",
    0x004C6210: "Get(name, i, float&)",
    0x004C6260: "Get(name, i, char**)",
    0x004C62A0: "GetNum(name)",
}


def main():
    _, secs = sections(RETAIL)

    def read_string(addr):
        for s in secs.values():
            va = s["va"] + IMAGE_BASE
            if va <= addr < va + len(s["bytes"]):
                b = s["bytes"]
                i = addr - va
                end = b.find(b"\0", i)
                if end < 0 or end - i > 64:
                    return None
                text = b[i:end]
                if text and all(0x20 <= c < 0x7F for c in text):
                    return text.decode("ascii")
        return None

    text = secs[".text"]
    base = text["va"] + IMAGE_BASE
    code = bytes(text["bytes"])

    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True

    # A pushed string stays live across a few instructions before the call, because other arguments
    # are pushed after it. Keeping a short window of recent pushes is enough and avoids decoding
    # the whole function.
    recent = []
    found = {}

    for ins in md.disasm(code, base):
        if ins.mnemonic == "push" and ins.op_str.startswith("0x"):
            try:
                recent.append(int(ins.op_str, 16))
            except ValueError:
                pass
            recent = recent[-4:]
        elif ins.mnemonic == "call":
            try:
                target = int(ins.op_str, 16)
            except ValueError:
                recent = []
                continue
            if target in PARSERS:
                for value in reversed(recent):
                    name = read_string(value)
                    if name:
                        found.setdefault(name, set()).add(PARSERS[target])
                        break
            recent = []
        elif ins.mnemonic in ("ret", "jmp"):
            recent = []

    print("command-line options parsed by midtown2.exe: %d\n" % len(found))
    for name in sorted(found):
        print("  -%-24s %s" % (name, ", ".join(sorted(found[name]))))

    out = os.path.join(ROOT, "docs", "command_line.md")
    with open(out, "w", encoding="utf-8", newline="\n") as f:
        f.write("# Command-line options\n\n")
        f.write("Extracted by `tools/argflags.py` from the calls themselves - each is a string\n")
        f.write("literal pushed immediately before a `datArgParser` call. This is what the binary\n")
        f.write("parses, not what any readme claims.\n\n")
        f.write("| Option | Read as |\n|---|---|\n")
        for name in sorted(found):
            f.write("| `-%s` | %s |\n" % (name, ", ".join(sorted(found[name]))))
    print("\nwrote %s" % out)
    return 0


if __name__ == "__main__":
    sys.exit(main())
