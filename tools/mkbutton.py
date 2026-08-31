"""
Generate a MM2 UI bitmap button in the exact style of an existing one.

MM2's menu buttons are not text drawn at runtime - `UIMenu::AddBMButton` takes a BITMAP name and
the label is painted into the image. `texture/veh_show.tga` is 230x140: four 35-pixel states
stacked vertically, in the order the widget indexes them - normal, highlighted, pressed, disabled.

So a new button that looks like an existing one is made by taking that file, erasing the label from
each of the four states, and painting a new one in the colour that state uses. Everything else -
the frame, the glow, the gradient, the disabled wash - is preserved exactly, because it is the same
pixels.

THE LABEL COLOUR IS SAMPLED, NOT GUESSED. Each state paints its text differently (white on the
first two, dark blue when pressed, grey when disabled), so the replacement text is drawn in the
colour actually taken from the brightest (or darkest) run of the original label in that state. That
keeps a new button consistent with the rest of the menu even though nothing here knows what the
palette is supposed to be.
"""
import os
import sys

from PIL import Image, ImageDraw, ImageFont
import numpy as np

SRC = sys.argv[1] if len(sys.argv) > 1 else r"C:\Users\xshxd\AppData\Local\Temp\ui\texture\veh_show.tga"
DST = sys.argv[2] if len(sys.argv) > 2 else r"C:\Users\xshxd\OneDrive\Desktop\PC Games\MM2\texture\veh_phys.tga"
TEXT = sys.argv[3] if len(sys.argv) > 3 else "PHYSICS OPTIONS"

STATES = 4


def pick_font(height):
    """A bold condensed sans, as close to the original label as the system provides."""
    for name in ("arialbd.ttf", "ARIALBD.TTF", "seguisb.ttf", "tahomabd.ttf", "verdanab.ttf"):
        for d in (r"C:\Windows\Fonts",):
            p = os.path.join(d, name)
            if os.path.exists(p):
                try:
                    return ImageFont.truetype(p, height)
                except Exception:
                    pass
    return ImageFont.load_default()


def main():
    im = Image.open(SRC).convert("RGB")
    w, h = im.size
    sh = h // STATES
    a = np.asarray(im).astype(int)

    out = im.copy()
    d = ImageDraw.Draw(out)

    for s in range(STATES):
        y0 = s * sh
        band = a[y0:y0 + sh]

        # The label occupies the middle of the band; the frame lives at the edges. Work inside a
        # margin so the frame is never touched.
        mx, my = 14, 8
        inner = band[my:sh - my, mx:w - mx]

        # Background = the most common colour in the inner area (the label is a minority of it).
        flat = inner.reshape(-1, 3)
        colours, counts = np.unique(flat, axis=0, return_counts=True)
        bg = tuple(int(v) for v in colours[counts.argmax()])

        # Label colour = whichever extreme is furthest from the background. On the light states the
        # text is brighter than the plate; on the pressed state it is darker.
        lum = flat.sum(axis=1)
        bg_lum = sum(bg)
        hi = flat[lum.argmax()]
        lo = flat[lum.argmin()]
        fg = tuple(int(v) for v in (hi if abs(int(hi.sum()) - bg_lum) > abs(int(lo.sum()) - bg_lum) else lo))

        # Erase the old label, then paint the new one centred in the same band.
        d.rectangle([mx, y0 + my, w - mx - 1, y0 + sh - my - 1], fill=bg)

        size = sh - 2 * my - 2
        while size > 6:
            font = pick_font(size)
            tw = d.textlength(TEXT, font=font)
            if tw <= (w - 2 * mx) - 8:
                break
            size -= 1

        bbox = d.textbbox((0, 0), TEXT, font=font)
        tx = (w - (bbox[2] - bbox[0])) // 2 - bbox[0]
        ty = y0 + (sh - (bbox[3] - bbox[1])) // 2 - bbox[1]
        d.text((tx, ty), TEXT, font=font, fill=fg)

        print("   state %d  bg=%s  text=%s  size=%d" % (s, bg, fg, size))

    os.makedirs(os.path.dirname(DST), exist_ok=True)
    # TGA, uncompressed 24-bit - the same shape as the file it was made from.
    out.save(DST)
    out.save(os.path.splitext(DST)[0] + ".png")
    print("wrote %s (%dx%d, %d states)" % (DST, w, h, STATES))


if __name__ == "__main__":
    main()
