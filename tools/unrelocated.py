# -*- coding: utf-8 -*-
"""Count .rdata/.data addresses in .text that were never relocated.

This is the metric that actually tracks progress toward a running binary, and it is measurable
without launching anything.

midtown2.exe has no relocation table, so every absolute address baked into the code has to be
recognised and rewritten by ExportAsm. Whatever it misses keeps its retail value, and because
.rdata and .data are displaced in the linked image, each miss is a pointer into the wrong memory.

The failures are silent by construction: the bytes match retail EXACTLY - that is what being
missed means - so a byte-for-byte comparison against the original reports them as correct. Only
this check sees them.

Run against a build to see how many remain, overall and on the startup path.
"""
import json
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)

sys.path.insert(0, HERE)
from pe import IMAGE_BASE  # noqa: E402
from verify_data import sections  # noqa: E402

RETAIL = os.environ.get("MM2_EXE", r"C:\Users\xshxd\OneDrive\Desktop\PC Games\MM2\midtown2.exe")
BUILT = os.path.join(ROOT, "build", "OpenMM2.exe")

# The order Main actually calls these in, so a regression shows up where it happens.
STARTUP = [
    (0x004011E0, "Main"),
    (0x00402180, "CreateGameMutex"),
    (0x004021D0, "CheckGlobalMemory"),
    (0x00402250, "CheckDiskSpace"),
    (0x004026A0, "FirstRunEula"),
    (0x004ABE00, "gfxAutoDetect"),
    (0x004AC030, "AutoDetectCallback"),
    (0x00401690, "InitDirectDraw"),
    (0x004016D0, "MainPhase"),
]


def main():
    _, retail = sections(RETAIL)
    _, built = sections(BUILT)

    rb = retail[".text"]["bytes"]
    bb = built[".text"]["bytes"]
    va = retail[".text"]["va"] + IMAGE_BASE

    lo = retail[".rdata"]["va"] + IMAGE_BASE
    hi = retail[".data"]["va"] + IMAGE_BASE + retail[".data"]["rsz"]

    with open(os.path.join(ROOT, "data", "symbols.json"), encoding="utf-8") as f:
        sizes = {s["rva"]: (s.get("size") or 0) for s in json.load(f)}

    def count(start, length):
        n = 0
        off = start - va
        for i in range(max(0, length - 4)):
            r = struct.unpack_from("<I", rb, off + i)[0]
            if not (lo <= r < hi):
                continue
            if struct.unpack_from("<I", bb, off + i)[0] == r:
                n += 1
        return n

    print("unrelocated .rdata/.data addresses on the startup path:")
    total_start = 0
    for addr, name in STARTUP:
        n = count(addr, sizes.get(addr, 256))
        total_start += n
        flag = "" if n == 0 else "   <-"
        print("   %-20s %3d%s" % (name, n, flag))

    print("\n   startup path total: %d" % total_start)

    whole = count(va, min(len(rb), len(bb)))
    print("   whole .text        : %d" % whole)

    return 0 if total_start == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
