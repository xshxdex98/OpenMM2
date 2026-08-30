/*
    OpenMM2 - An Open Source Re-Implementation of Midtown Madness 2
    Copyright (C) 2026 The OpenMM2 contributors

    Structure, conventions and the arts/core runtime follow Open1560 by Brick,
    an Open Source Re-Implementation of Midtown Madness 1. Copyright (C) 2020 Brick.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "gfxbitmap.h"
#ifdef ARTS_AGIGL
#    include "agigl/glstub.h" // agiGLProbeVerbose
#endif

#include "data7/printer.h"
#include "gfximage.h"
#include "origfallback.h"
#include "gfxpipeline.h"
#include "ui/uiscale.h"

#ifdef ARTS_AGIGL
#    include "agigl/glblit.h"
#    include "agigl/glswitch.h"
#endif

define_dummy_symbol(gfx_gfxbitmap);

// gfxImage::Type, the gfxImageFormat enum. ARGB_8888 = 1 and RGB_0888 = 2 are the only two
// formats gfxBitmap has ever handled: gfxBitmap::Load compares image->Type against exactly these
// and calls Errorf("gfxBitmap::Load - Unexpected surface combination.") otherwise. They are also
// the only two gfxImage::Scale handles - it answers anything else with
// "gfxImage::Scale - Only 24 bit and 32 bit images are handled." The two domains coincide, which
// is what makes Scale the right resampler for this path and not merely a convenient one.
#define GFX_IMAGE_ARGB_8888 1
#define GFX_IMAGE_RGB_0888 2

// FIX 3 FROM docs/ui_scaling.md, DONE AT LOAD RATHER THAN AT BLIT.
//
// The blit was the wrong place and the previous pass through this work was right to refuse it.
// Roughly forty callers of gfxPipeline::CopyBitmap compute their layout in assembly from the
// UNSCALED gfxBitmap::Width - mmHUD advances across a row of digits with `v4 += bmp->Width`, and
// mmSlider, mmNumber, mmSpeedIndicator and mmIcons all do the same - so a blit that drew each
// piece three times larger while the caller still advanced one width would make every multi-part
// HUD element overlap itself.
//
// Resampling when the bitmap is created makes Width and Height report the scaled size, and all
// forty keep working with no change at all. Two functions in the original already demonstrate
// that this is the shape the engine expects: ProgressCB blits the loading screen with
// `CopyBitmap(0, 0, bmp, 0, 0, bmp->Width, bmp->Height, 0)` and RestoreFocus centres it with
// `(m_iWidth - bmp->Width) >> 1`. Both follow a resized bitmap for free.
//
// MM2 DOES THIS ITSELF, which is the strongest evidence that it is safe. PUMenuBase's constructor
// at 0x0050CC90 is `gfxLoadImage` -> `gfxImage::Scale(image, m_iWidth, m_iHeight)` ->
// `gfxBitmap::Create(image, 0)`, and then reads the resulting bitmap's Width and Height back to
// derive its own fractional size. This function is that sequence, applied to every named UI
// bitmap instead of to one popup background.
//
// WHY THIS OVERLOAD AND NOT THE OTHER. gfxBitmap::Create(i32, i32, bool) takes an explicit pixel
// size from a caller that has already decided what it wants - text node bitmaps, render scratch,
// PUMenuBase::CreateDummyBitmap, mmText::CreateFitBitmap - and must not be second-guessed.
// mmTextNode::Init in particular already sizes its bitmap through uiScale, so scaling it again
// here would square the factor. This overload is the one that means "a decoded image file", and
// every path into it is UI art: gfxGetBitmap (so every nodeGetBitmap caller - mmHUD, sfPointer,
// UIBMButton, UIIcon, mmSlider, asCamera::SetUnderlay and thirty more) plus the two loading
// screens.

static bool ScaledImageSize(const gfxImage* image, i32& out_width, i32& out_height)
{
    if (UI_SCALE_BITMAPS == 0)
        return false;

    // Palettised images are rejected by gfxBitmap::Load anyway, and gfxImage::Scale would walk
    // them as if they were direct colour.
    if (image->PaletteType != 0)
        return false;

    if (image->Type != GFX_IMAGE_ARGB_8888 && image->Type != GFX_IMAGE_RGB_0888)
        return false;

    // gfxImage::Scale divides by (width - 1) and (height - 1).
    if (image->Width < 2 || image->Height < 2)
        return false;

    i32 width = static_cast<i32>(image->Width);
    i32 height = static_cast<i32>(image->Height);

    out_width = uiScale::PixelsW(width);
    out_height = uiScale::PixelsH(height);

    // Only ever enlarge, and only when there is something to gain. At 640x480 both factors are
    // exactly one, so this returns false and the function below is the original byte for byte -
    // which is the cheapest place to catch a transcription mistake in the rest of it.
    if (out_width < width || out_height < height)
        return false;

    if (out_width == width && out_height == height)
        return false;

    // Never build a UI surface larger than the screen. This is what keeps an image that was
    // already authored at or above the design width from being blown up again - PUMenuBase has
    // called gfxImage::Scale to exactly the screen size before it gets here, and its image must
    // pass straight through - and it bounds the memory a single bitmap can take.
    if (out_width > gfxPipeline::Access_m_iWidth() || out_height > gfxPipeline::Access_m_iHeight())
        return false;

    return true;
}

// GROW THE SOURCE BUFFER BY ONE ROW AND ONE PIXEL BEFORE gfxImage::Scale READS IT.
//
// gfxImage::Scale is a bilinear resampler and it reads one sample beyond the image on both axes.
// For each output row it takes `v8 = pImageData + BytesPerRow * (acc >> 15)` and
// `v11 = v8 + BytesPerRow`, and on the last output row `acc >> 15` is Height - 1, so v11 is row
// Height - one row past the end. Within a row it reads pixel `(x >> 15) + 1`, which on the last
// column is pixel Width.
//
// That is the original's own behaviour, and PUMenuBase has been living with it since 1999: the
// bottom row of a scaled image is interpolated against whatever follows the allocation. It gets
// away with it because it does this once, for a popup background. Doing it for every UI bitmap in
// the game raises a one-row cosmetic artifact into a real chance of reading an unmapped page,
// because a 640x480x32 image is 1.2MB and an allocation that size comes straight from the heap
// manager rather than from a bucket with slack behind it.
//
// So the read is made legal instead: the buffer is reallocated with one extra row plus one extra
// pixel, the last row is duplicated into the pad, and Scale's edge sample lands on a clamped copy
// of the edge. BytesPerRow is deliberately NOT changed - the pad is at the end of the buffer, not
// inside every row, which is enough because the column overread of row N lands inside row N + 1.
// Scale ends by calling operator delete on pImageData, so it frees this buffer and not the
// original one.
static void PadImageForScale(gfxImage* image)
{
    u32 pitch = image->BytesPerRow;
    u32 rows = image->Height;

    if (pitch == 0 || rows == 0)
        return;

    u32 used = pitch * rows;

    // One row for the vertical overread, plus eight bytes so the column overread of the pad row
    // itself stays inside the allocation. Four would do for ARGB_8888; eight is free.
    u32 total = used + pitch + 8;

    void* padded = operator new(total);

    if (padded == nullptr)
        return;

    const u8* source = static_cast<const u8*>(image->pImageData);

    // The copies go through a volatile pointer, the same idiom mmTextNode::Init uses for its
    // zero fill. MSVC recognises a plain byte copy loop and lowers it to a call to _memcpy, and
    // this translation unit has no declaration of memcpy - deliberately, because <cstring>
    // declares the CRT entry points with _ACRTIMP and the link runs -NODEFAULTLIB against the
    // game's own 1999 CRT (see the note at the top of misc/miscfuncs.cpp). _memcpy does exist in
    // midtown2.exe, unlike _memset, so the lowered call would in fact resolve - but not depending
    // on that is one less thing to be right about, and this runs once per bitmap at load.
    volatile u8* dest = static_cast<volatile u8*>(padded);

    for (u32 i = 0; i < used; ++i)
        dest[i] = source[i];

    const u8* last_row = source + used - pitch;

    for (u32 i = 0; i < pitch; ++i)
        dest[used + i] = last_row[i];

    for (u32 i = 0; i < 8; ++i)
        dest[used + pitch + i] = 0;

    operator delete(image->pImageData);

    image->pImageData = padded;
}

// Transcribed from the disassembly of ?Create@gfxBitmap@@SAPAV1@PAVgfxImage@@_N@Z at 0x004AE480.
// The original is the last two paragraphs of this function on their own: make a surface the size
// of the image, load the image into it, and destroy the bitmap if the load is refused.
//
// The scaled path is written so that it can only ever be taken when it is certain to work. The
// surface is created FIRST, at the scaled size, and the image is resampled only once that has
// succeeded - because gfxImage::Scale frees the source pixels as it goes, so there is no way back
// from it. If the larger surface cannot be created, the image is still untouched and the original
// path runs exactly as it always did.

// ?Create@gfxBitmap@@SAPAV1@PAVgfxImage@@_N@Z
gfxBitmap* gfxBitmap::Create(gfxImage* arg1, bool arg2)
{
    i32 scaled_width = 0;
    i32 scaled_height = 0;

    gfxBitmap* bitmap = nullptr;

    if (ScaledImageSize(arg1, scaled_width, scaled_height))
    {
        // The surface first, the resample second. gfxImage::Scale frees the source pixels as it
        // goes, so once it has run there is no unscaled image left to fall back to. Creating the
        // larger surface up front means the only way to reach Scale is with somewhere to put the
        // result, and a driver that refuses the size costs nothing but a scaled-up bitmap.
        bitmap = gfxBitmap::Create(scaled_width, scaled_height, arg2);

        if (bitmap)
        {
            PadImageForScale(arg1);

            arg1->Scale(scaled_width, scaled_height);
        }
    }

    // The original, from here down.
    if (bitmap == nullptr)
        bitmap = gfxBitmap::Create(arg1->Width, arg1->Height, arg2);

    if (bitmap)
    {
        if (!bitmap->Load(arg1))
        {
            // The destructor and the constructor are both private in the binary - AAE, not QAE -
            // which is why gfx/gfxbitmap.h declares them in a private section and why this call
            // has to live inside a member of gfxBitmap.
            bitmap->~gfxBitmap();

            operator delete(bitmap);

            return nullptr;
        }
    }

    return bitmap;
}

// THE THREE gfxBitmap MEMBERS OF SET A.
//
// Each is a -gl branch and nothing else. The else arm is not a transcription: it is a call into
// the ORIGINAL 1999 machine code, which tools/asm.py kept under an alias instead of padding away
// because these symbols are listed in data/keep_original.json. See gfx/origfallback.h.
//
// That is deliberate and it is the stricter of the two options, not the lazier one. Transcribing
// gfxBitmap::Load faithfully means porting the surface lock, four pixel-format converters chosen
// by bit depth and mask, the row loop and the unlock - and the rule these functions have to obey
// is that with -gl absent the process does EXACTLY what it does today. Running the original code
// satisfies that by construction; a transcription only satisfies it if it is perfect, and there
// would be no way to tell the difference until something looked subtly wrong on screen.
//
// When one of these grows a real transcription, its entry comes out of keep_original.json and the
// ArtsOrig call goes with it.

// ?Create@gfxBitmap@@SAPAV1@HH_N@Z
gfxBitmap* gfxBitmap::Create(i32 arg1, i32 arg2, bool arg3)
{
#ifdef ARTS_AGIGL
    if (agiGLEnabled())
    {
        // arg3 is unused here exactly as it is in the original, which never reads [ebp+10h] on any
        // path. gfxBitmap::Create(gfxImage*, bool) passes its own flag straight through and nothing
        // downstream looks at it.
        (void) arg3;

        void* slot = agiGLBitmapCreate(arg1, arg2);

        if (slot == nullptr)
        {
            // Worth shouting about rather than returning quietly. Not one of the forty-odd callers
            // checks this for null - mmTextNode::Init goes straight into
            // `mov ecx, eax / call gfxBitmap::Clear` - so a null here is not a handled failure, it
            // is a crash a few instructions later with nothing pointing back to this line.
            Errorf("gfxBitmap::Create(%d, %d) - the GL surface could not be created", arg1, arg2);

            return nullptr;
        }

        // Allocation and construction are identical on both paths - operator new(0x14) then the
        // private two-argument constructor - so they happen here rather than inside the GL half.
        gfxBitmap* bitmap = new gfxBitmap(arg1, arg2);

        bitmap->Surface = static_cast<IDirectDrawSurface7*>(slot);

        return bitmap;
    }
#endif

    return ArtsOrigBitmapCreate(arg1, arg2, arg3);
}

// ?Load@gfxBitmap@@QAE_NPAVgfxImage@@@Z
bool gfxBitmap::Load(gfxImage* image)
{
#ifdef ARTS_AGIGL
    if (agiGLEnabled())
        // DIAGNOSTIC: what each loaded bitmap actually is.
        //
        // mmHUD::Cull positions its digits as m_iWidth/2 - 3*digitWidth - colonWidth, reading those
        // widths from the gfxBitmap objects it loaded as "digi_%d" and "digi_colon". The shipped
        // assets are DIGI_0.tga at 21x32 and DIGI_COLON.tga at 10x32, so at the 4x scale this build
        // uses the sum should be about 292 and the row should start near x=988. It starts near x=38,
        // which needs a sum around 1242 - so the widths these objects report are not the digits'.
        {
            static i32 shown = 0;

            // Name is NOT set yet at Load time - gfxGetBitmap assigns it afterwards - so requiring
            // it logged nothing at all. The digits are identifiable by shape instead: DIGI_0.tga is
            // 21x32 authored, so 21x32 here means the object carries the authored size and 84x128
            // means the loader has written the 4x scaled size back into it.
            if (shown < 60 && agiGLProbeVerbose())
            {
                ++shown;

                Displayf("BITMAP %ux%u", Width, Height);
            }
        }

        return agiGLBitmapLoad(Surface, image, Width, Height);
#endif

    return ArtsOrigBitmapLoad(this, image);
}

// ?Clear@gfxBitmap@@QAEXI@Z
void gfxBitmap::Clear(u32 fillColor)
{
#ifdef ARTS_AGIGL
    if (agiGLEnabled())
    {
        agiGLBitmapClear(Surface, fillColor, Width, Height);

        return;
    }
#endif

    ArtsOrigBitmapClear(this, fillColor);
}
