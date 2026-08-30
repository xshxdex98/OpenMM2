# -*- coding: utf-8 -*-
"""Select a resolution in MM2's gfxconf.dat.

The game enumerates modes once, caches the result here, and then uses a stored index. Patching the
advertised cap and adding 1440p to dgVoodoo's mode list makes 2560x1440 *available*; this is what
makes it *selected*. Without it the game keeps running at whatever index it saved, which is 640x480
on a fresh detect.

The format, decoded from the file itself:

    +0x00  'GFX5' magic, then version, then adapter count
    ...    per adapter, a 0x270-byte record - the same size as gfxInterface in docs/gfx_layouts.md
    +0x60  video memory and friends
    +0x64  mode count
    +0x68  the selected mode index
    +0x7C  the mode table: {u16 width, u16 height, u16 depth, u16 flags} per entry

MM2 keeps two choices - one for the frontend and one for the game view (the log prints
"SetupResChoices - interfaceChoice = 0, gfxInterfaceChoice = 1"), so both adapter records matter.
"""
import os
import struct
import sys

CONF = os.environ.get(
    "MM2_GFXCONF", "C:/Users/xshxd/OneDrive/Desktop/PC Games/MM2/gfxconf.dat")

ADAPTER_STRIDE = 0x270
COUNT_OFF = 0x64
CHOICE_OFF = 0x68
TABLE_OFF = 0x7C
ENTRY = 8


def main():
    if len(sys.argv) < 3:
        sys.exit("usage: setres.py <width> <height>")

    want_w, want_h = int(sys.argv[1]), int(sys.argv[2])

    if not os.path.exists(CONF):
        sys.exit("no gfxconf.dat at %s - run the game once so it detects modes" % CONF)

    data = bytearray(open(CONF, "rb").read())
    if bytes(data[:4]) != b"GFX5":
        sys.exit("not a gfxconf.dat (magic is %r)" % bytes(data[:4]))

    adapters = struct.unpack_from("<I", data, 8)[0]
    changed = 0

    for a in range(adapters):
        base = a * ADAPTER_STRIDE
        count = struct.unpack_from("<I", data, base + COUNT_OFF)[0]
        if not 0 < count < 256:
            continue

        found = None
        for i in range(count):
            w, h, d, f = struct.unpack_from("<4H", data, base + TABLE_OFF + i * ENTRY)
            if w == want_w and h == want_h:
                found = i
                break

        if found is None:
            print("adapter %d: %dx%d is not in its %d enumerated modes" % (a, want_w, want_h, count))
            continue

        was = struct.unpack_from("<I", data, base + CHOICE_OFF)[0]
        struct.pack_into("<I", data, base + CHOICE_OFF, found)
        print("adapter %d: choice %d -> %d (%dx%d)" % (a, was, found, want_w, want_h))
        changed += 1

    if not changed:
        sys.exit("nothing changed - is the mode enumerated? delete gfxconf.dat and run once")

    open(CONF, "wb").write(bytes(data))
    print("wrote %s" % CONF)
    return 0


if __name__ == "__main__":
    sys.exit(main())
