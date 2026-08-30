# -*- coding: utf-8 -*-
"""Build OpenMM2.ico from the icon inside the retail binary.

The built executable has had no icon since the harness was stood up: the linker writes its own
.rsrc holding only the DPI manifest, so retail's ICON, GROUP_ICON and VERSION entries are simply
absent and Windows draws the generic application icon.

The shipped icon is one 32x32 image at 16 colours, which is all anyone needed in 1999. Handing
that to a 1440p desktop and letting Windows scale it produces a blurred mess, so the larger sizes
are generated here instead, where the resampling can be chosen:

  * upscales use NEAREST at exact integer multiples (2x, 4x, 8x), which reproduces the original
    pixel art exactly rather than inventing detail that was never drawn;
  * the one downscale, to 16x16, uses LANCZOS, because averaging is genuinely the right answer
    when discarding information.

Sizes that are not integer multiples are skipped rather than fudged - Windows picks the nearest
larger entry and scales down, which looks better than an unevenly resampled source.

Entries are written as PNG rather than BMP. The ICO format has allowed that since Vista and it
keeps 256x256 from costing 256 KB, and nothing that can run this project predates it.

Pillow cannot do this itself: its ICO writer ignores append_images and re-derives the size list
with its own filter, which is exactly the choice this script exists to make.

Usage: py tools/mkicon.py
"""
import io
import os
import struct
import sys

import pefile
from PIL import Image

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)

RETAIL = os.environ.get(
    "MM2_RETAIL", "C:/Users/xshxd/OneDrive/Desktop/PC Games/MM2/midtown2.exe")
OUT = os.path.join(HERE, "openmm2.ico")

RT_ICON = 3
RT_GROUP_ICON = 14


def retail_icon():
    """The largest icon image in the retail binary, as a Pillow image."""
    pe = pefile.PE(RETAIL)

    icons, groups = {}, {}

    for entry in pe.DIRECTORY_ENTRY_RESOURCE.entries:
        for res in entry.directory.entries:
            data = res.directory.entries[0].data.struct
            blob = pe.get_data(data.OffsetToData, data.Size)

            if entry.struct.Id == RT_ICON:
                icons[res.struct.Id] = blob
            elif entry.struct.Id == RT_GROUP_ICON:
                groups[res.struct.Id] = blob

    if not groups or not icons:
        sys.exit("no icon in %s" % RETAIL)

    group = next(iter(groups.values()))
    count = struct.unpack_from("<H", group, 4)[0]

    # Rebuild a single-image .ico in memory. Pillow cannot read a bare RT_ICON blob, which is a
    # DIB without the file header the format otherwise always carries.
    best, best_px = None, -1

    for i in range(count):
        w, h, _cc, _rv, planes, bits, size, nid = struct.unpack_from("<BBBBHHIH", group, 6 + 14 * i)
        px = (w or 256) * (h or 256)

        if px > best_px and nid in icons:
            best, best_px = (w, h, planes, bits, icons[nid]), px

    w, h, planes, bits, blob = best
    ico = io.BytesIO()
    ico.write(struct.pack("<HHH", 0, 1, 1))
    ico.write(struct.pack("<BBBBHHII", w, h, 0, 0, planes, bits, len(blob), 6 + 16))
    ico.write(blob)
    ico.seek(0)

    image = Image.open(ico)
    image.load()
    return image.convert("RGBA")


def main():
    source = retail_icon()
    side = source.size[0]
    print("retail icon: %dx%d" % source.size)

    images = {side: source}

    # 16x16 is the one size Windows will not derive acceptably on its own, because shrinking is
    # where a nearest-neighbour pick drops whole features rather than blending them.
    if side > 16:
        images[16] = source.resize((16, 16), Image.LANCZOS)

    for factor in (2, 4, 8):
        target = side * factor

        if target <= 256:
            images[target] = source.resize((target, target), Image.NEAREST)

    order = sorted(images)
    encoded = []

    for size in order:
        buf = io.BytesIO()
        images[size].save(buf, format="PNG", optimize=True)
        encoded.append(buf.getvalue())

    offset = 6 + 16 * len(order)
    out = io.BytesIO()
    out.write(struct.pack("<HHH", 0, 1, len(order)))

    for size, png in zip(order, encoded):
        # A 256-wide entry records its width as 0; the field is a single byte and 256 does not fit.
        dim = 0 if size >= 256 else size
        out.write(struct.pack("<BBBBHHII", dim, dim, 0, 0, 1, 32, len(png), offset))
        offset += len(png)

    for png in encoded:
        out.write(png)

    with open(OUT, "wb") as f:
        f.write(out.getvalue())

    print("wrote %s (%d bytes)" % (OUT, os.path.getsize(OUT)))
    print("sizes: %s" % ", ".join("%dx%d" % (s, s) for s in order))
    return 0


if __name__ == "__main__":
    sys.exit(main())
