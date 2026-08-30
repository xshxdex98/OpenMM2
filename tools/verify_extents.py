# -*- coding: utf-8 -*-
"""No ported function may swallow something else that is referenced from data.

A function is stripped by its recorded extent, and that extent comes from the symbol map, which
gives the distance to the next known symbol. When a function the map does not know about sits
inside that gap, "the whole function" is bigger than the function - and stripping it deletes the
neighbour.

That is not hypothetical. `?Extend@Vector3@@QAEXABV1@M@Z` is recorded as 272 bytes at 0x004C0620,
and a static initialiser begins at 0x004C0670, fifty bytes in. Porting Extend replaced the
initialiser with padding, so the CRT's _initterm called straight into the middle of the
reimplementation and the game died before printing its second line. Every byte-level gate passed:
the bytes inside a ported region are *expected* to differ, which is exactly where the deleted
function was.

The check is simple and does not depend on knowing where functions begin: if any dword anywhere in
.rdata or .data points strictly inside a ported function's range, something outside is reaching
into the middle of it, and stripping it is unsafe. That covers initialiser tables, jump tables and
vftable slots alike.

Pointing at a function's first byte is normal and allowed - that is an ordinary reference to the
function itself, which the linker retargets to the reimplementation.
"""
import json
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
sys.path.insert(0, HERE)

from pe import IMAGE_BASE
from verify_data import sections

RETAIL = os.environ.get(
    "MM2_RETAIL", "C:/Users/xshxd/OneDrive/Desktop/PC Games/MM2/midtown2.exe")


def main():
    with open(os.path.join(ROOT, "data", "ported.json"), encoding="utf-8") as f:
        ported = set(json.load(f).get("ported", []))

    with open(os.path.join(ROOT, "data", "symbols.json"), encoding="utf-8") as f:
        syms = json.load(f)

    by_name = {s["mangled"]: s for s in syms if s.get("code") and s.get("size")}

    _, retail = sections(RETAIL)

    # Every dword in the initialised data sections, as a set of values.
    targets = set()
    for name in (".rdata", ".data"):
        s = retail.get(name)
        if not s:
            continue
        b = s["bytes"]
        for i in range(0, len(b) - 4, 4):
            v = struct.unpack_from("<I", b, i)[0]
            if 0x400000 < v < 0x600000:
                targets.add(v)

    bad = []
    checked = 0

    for m in sorted(ported):
        f = by_name.get(m)
        if not f:
            continue
        checked += 1
        start, size = f["rva"], f["size"]
        # Strictly inside: the first byte is an ordinary reference to the function itself.
        hits = sorted(v for v in targets if start < v < start + size)
        if hits:
            bad.append((m, start, size, hits))

    print("  ported functions checked : %d" % checked)

    if not bad:
        print("  PASS: no ported function has data pointing into the middle of it")
        return 0

    print("  UNSAFE TO PORT           : %d" % len(bad))
    for m, start, size, hits in bad:
        print("    %s" % m)
        print("      0x%08X + %d bytes, but data points at %s"
              % (start, size, ", ".join("0x%08X (+0x%X)" % (v, v - start) for v in hits[:4])))
    print("  FAIL: stripping these deletes something else that is still referenced")
    return 1


if __name__ == "__main__":
    sys.exit(main())
