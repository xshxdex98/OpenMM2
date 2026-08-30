# -*- coding: utf-8 -*-
"""Every DirectInput GUID pointer must be relocated.

This is a gate rather than a note because the failure it catches is invisible to the others.
verify_text asks whether a byte differs from retail and whether the difference is explained; an
unrelocated pointer does not differ from retail at all. It is *correct-looking* and wrong, and it
cost this project the mouse: IDirectInputDevice::SetDataFormat validates every pguid it is handed,
so one stale entry makes it return E_INVALIDARG and input never initialises. The symptom is a crash
on the first click, nowhere near the cause.

A DIDATAFORMAT states its own dwNumObjs and rgodf, so nothing here is inferred. Find the header by
its two fixed ABI sizes, walk exactly that many entries at exactly that stride, and require every
non-NULL pguid to have moved. NULL is normal - mouse buttons and joystick POVs leave it null.
"""
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
BUILT = os.path.join(ROOT, "build", "OpenMM2.exe")

SIZEOF_DIDATAFORMAT = 24
SIZEOF_DIOBJECTDATAFORMAT = 16


def read_dword(secs, addr):
    for s in secs.values():
        va = s["va"] + IMAGE_BASE
        if va <= addr < va + len(s["bytes"]) - 3:
            return struct.unpack_from("<I", s["bytes"], addr - va)[0]
    return None


def find_formats(secs):
    """Every DIDATAFORMAT header, as (address, numObjs, rgodf)."""
    out = []
    for name in (".rdata", ".data"):
        s = secs.get(name)
        if not s:
            continue
        b, va = s["bytes"], s["va"] + IMAGE_BASE
        for i in range(0, len(b) - SIZEOF_DIDATAFORMAT, 4):
            size, obj_size, flags, _data_size, num, rgodf = struct.unpack_from("<6I", b, i)
            if size != SIZEOF_DIDATAFORMAT or obj_size != SIZEOF_DIOBJECTDATAFORMAT:
                continue
            if flags not in (1, 2) or not 0 < num <= 1024 or rgodf < 0x400000:
                continue
            out.append((va + i, num, rgodf))
    return out


def main():
    if not os.path.exists(BUILT):
        print("no build to check")
        return 0

    _, retail = sections(RETAIL)
    _, built = sections(BUILT)

    formats = find_formats(retail)
    if not formats:
        print("FAIL: no DIDATAFORMAT found - the scan is broken, not the binary")
        return 1

    total = stale = 0
    bad = []

    for addr, num, rgodf in formats:
        for k in range(num):
            entry = rgodf + k * SIZEOF_DIOBJECTDATAFORMAT
            want = read_dword(retail, entry)
            if want is None or want < 0x400000:
                continue                      # NULL pguid, or outside any emitted section
            total += 1
            if read_dword(built, entry) == want:
                stale += 1
                bad.append((addr, k, entry, want))

    print("  DirectInput data formats: %d" % len(formats))
    print("  non-NULL GUID pointers  : %d" % total)

    if not stale:
        print("  PASS: every GUID pointer was relocated")
        return 0

    print("  UNRELOCATED             : %d" % stale)
    for addr, k, entry, want in bad[:12]:
        print("    format 0x%08X entry [%d] at 0x%08X still points at 0x%08X"
              % (addr, k, entry, want))
    if len(bad) > 12:
        print("    ... and %d more" % (len(bad) - 12))
    print("  FAIL: SetDataFormat will reject these and input will not initialise")
    return 1


if __name__ == "__main__":
    sys.exit(main())
