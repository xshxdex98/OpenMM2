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

#include "core/arts.h"

define_dummy_symbol(agigl_glbitmap);

// gfxBitmap ON TOP OF A GL TEXTURE.
//
// A gfxBitmap was a system-memory DirectDraw surface whose pixel format was a copy of the render
// target's - 565 or 555 or 32-bit, whatever the display mode happened to be - with the source
// colour key set to black. Every CopyBitmap was a sysmem-to-vidmem BltFast honouring that key.
//
// Here every bitmap is RGBA8 and the key becomes a real alpha channel at upload time. That removes
// the strangest piece of the original: gfxBitmap::Load8888 bumps opaque black to (1,1,1) so that
// deliberate black is not keyed out, and that hack exists only because the key is a colour rather
// than an alpha. With alpha there is nothing to protect black from.
//
// Guarded twice, like glgfxpipeline.cpp - gfx/gfxbitmap.h and gfx/gfximage.h do not currently
// compile. See docs/opengl_plan.md, "Generated headers that do not compile".
#ifdef ARTS_AGIGL

#    include "data7/printer.h"

#    include "gfx/gfxbitmap.h"
#    include "gfx/gfximage.h"

#    include "glcontext.h"
#    include "glsurface.h"

// gfxImage::gfxImageFormat. The generated header declares the enum's existence but not its
// enumerators, so the values are repeated here from MM2_RE_KIT/MM2_PSEUDOCODE/gfxImage/gfxImage.h.
static constexpr u8 kImageARGB_8888 = 1;
static constexpr u8 kImageRGB_0888 = 2;
static constexpr u8 kImageARGB_1555 = 3;
static constexpr u8 kImageRGB_0555 = 4;
static constexpr u8 kImagePalette8 = 5;
static constexpr u8 kImagePalette4 = 6;

// Source alpha below this becomes fully transparent. The original tests `< 64` in
// gfxBitmap::Load8888 before collapsing the texel to the colour key, so the threshold is its.
static constexpr u8 kAlphaKeyThreshold = 64;

// Expands a 5-bit channel to 8 bits. Replicating the top three bits rather than shifting left by
// three is what makes 0x1F reach 0xFF instead of 0xF8 - without it every bitmap is subtly dark and
// white is never quite white.
static u8 agiExpand5(u32 value)
{
    u32 v = value & 0x1F;

    return static_cast<u8>((v << 3) | (v >> 2));
}

static u8 agiExpand6(u32 value)
{
    u32 v = value & 0x3F;

    return static_cast<u8>((v << 2) | (v >> 4));
}

// Converts one gfxImage into the tightly packed RGBA8 buffer a bitmap upload wants, folding the
// black colour key into the alpha channel on the way.
//
// Returns false for a format that has to be expanded earlier - which today means only the paletted
// ones, because GL_COLOR_INDEX and the paletted-texture extensions are gone from every modern
// driver. gfxLoadTexImage already has an expansion for its non-expand path and that is the code to
// lift; see docs/opengl_plan.md.
static bool agiGLConvertToRGBA8(const gfxImage* image, u8* dst)
{
    i32 width = image->Width;
    i32 height = image->Height;
    i32 pitch = image->BytesPerRow;
    const u8* src_base = static_cast<const u8*>(image->pImageData);

    for (i32 y = 0; y < height; ++y)
    {
        const u8* src = src_base + (y * pitch);
        u8* out = dst + (y * width * 4);

        switch (image->Type)
        {
            case kImageARGB_8888: {
                for (i32 x = 0; x < width; ++x, src += 4, out += 4)
                {
                    out[0] = src[0];
                    out[1] = src[1];
                    out[2] = src[2];

                    // The original collapses a nearly transparent texel to black so the colour key
                    // catches it. Writing the alpha directly says the same thing without losing
                    // the colour, and lets opaque black stay black.
                    out[3] = (src[3] < kAlphaKeyThreshold) ? 0 : 0xFF;
                }

                break;
            }

            case kImageRGB_0888: {
                for (i32 x = 0; x < width; ++x, src += 3, out += 4)
                {
                    out[0] = src[0];
                    out[1] = src[1];
                    out[2] = src[2];

                    // No alpha in the source, so black is the key - which is what
                    // DDBLTFAST_SRCCOLORKEY did with a key of 0 on every one of these.
                    out[3] = (src[0] == 0 && src[1] == 0 && src[2] == 0) ? 0 : 0xFF;
                }

                break;
            }

            case kImageARGB_1555: {
                for (i32 x = 0; x < width; ++x, src += 2, out += 4)
                {
                    u32 texel = static_cast<u32>(src[0]) | (static_cast<u32>(src[1]) << 8);

                    out[0] = agiExpand5(texel >> 10);
                    out[1] = agiExpand5(texel >> 5);
                    out[2] = agiExpand5(texel);
                    out[3] = (texel & 0x8000) ? 0xFF : 0;
                }

                break;
            }

            case kImageRGB_0555: {
                // THE NAME LIES, AND THE ORIGINAL PROVES IT. gfxTexture::Load picks its row
                // converter from the created surface's masks: for a 555 surface it runs the
                // 565-to-555 down shift `(w & 0x1F) | ((w >> 1) & 0x7FE0)`, and for a 565 surface
                // it runs a plain memcpy. A converter that shifts green down by one to reach 555
                // can only be reading 565, and a memcpy into a 565 surface can only be writing
                // 565. So the payload behind RGB_0555 is 5-6-5.
                for (i32 x = 0; x < width; ++x, src += 2, out += 4)
                {
                    u32 texel = static_cast<u32>(src[0]) | (static_cast<u32>(src[1]) << 8);

                    out[0] = agiExpand5(texel >> 11);
                    out[1] = agiExpand6(texel >> 5);
                    out[2] = agiExpand5(texel);
                    out[3] = (texel == 0) ? 0 : 0xFF;
                }

                break;
            }

            case kImagePalette8:
            case kImagePalette4: return false;

            default: Errorf("agiGLConvertToRGBA8: unknown gfxImage type %d", image->Type); return false;
        }
    }

    return true;
}

// The GL half of gfxBitmap::Create. Returns the surface SLOT, not a gfxBitmap: allocating and
// constructing the object is identical on both paths, so gfx/gfxbitmap.cpp does it once after the
// branch rather than each arm doing it separately and having to agree.
void* agiGLBitmapCreate(i32 width, i32 height)
{
    // A ZERO DIMENSION IS NOT AN ERROR HERE, because it was not one for DirectDraw.
    //
    // mmTextNode::Init asks for a 328x0 bitmap when a text node has nothing to lay out yet, and
    // DirectDraw handed back a surface with no pixels rather than failing. Refusing returns null
    // from gfxBitmap::Create, and none of its forty-odd callers check - mmTextNode::Init goes
    // straight into `mov ecx, eax / call gfxBitmap::Clear` and faults reading Height through a
    // null this, several frames away from the actual cause.
    //
    // So an empty request gets a 1x1 texture. The object exists, Width and Height still report
    // what the caller asked for because gfxBitmap stores them itself, and nothing is drawn from a
    // bitmap with no area anyway.
    const i32 w = (width > 0) ? width : 1;
    const i32 h = (height > 0) ? height : 1;

    return agiGLSurfaceCreate(w, h, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, 1);
}

// The GL half of gfxBitmap::Load. Width and height come from the bitmap, which this no longer
// has a pointer to.
bool agiGLBitmapLoad(void* slot, gfxImage* arg1, i32 Width, i32 Height)
{
    if (arg1 == nullptr)
        return false;

    // The original rejects a size mismatch and any paletted image outright, and so does this. A
    // bitmap is created at the image's size by the only caller that matters, so a mismatch here
    // means something went wrong upstream and silently rescaling would hide it.
    if (arg1->Width != Width || arg1->Height != Height || arg1->PaletteType != 0)
    {
        Errorf("gfxBitmap::Load - %dx%d image into a %dx%d bitmap, palette type %d", arg1->Width, arg1->Height, Width,
            Height, arg1->PaletteType);

        return false;
    }

    agiGLSurface* surface = agiGLSurfaceFromSlot(slot);

    if (surface == nullptr)
        return false;

    usize pixel_count = static_cast<usize>(Width) * static_cast<usize>(Height);
    u8* rgba = new u8[pixel_count * 4];

    bool converted = agiGLConvertToRGBA8(arg1, rgba);

    if (converted)
        agiGLSurfaceUpload(surface, 0, rgba, Width * 4);

    delete[] rgba;

    return converted;
}

// The GL half of gfxBitmap::Clear.
void agiGLBitmapClear(void* slot, u32 arg1, i32 Width, i32 Height)
{
    agiGLSurface* surface = agiGLSurfaceFromSlot(slot);

    if (surface == nullptr)
        return;

    // THE GDI DIB IS CLEARED TOO, AND FORGETTING IT MADE EVERY STRING ACCUMULATE.
    //
    // A surface that has produced a device context has TWO copies of its pixels: the GL texture,
    // and the DIB section GDI draws into, which agiGLStubSurfaceReleaseDC uploads over the top.
    // Clearing only the texture is therefore not a clear at all - the next ReleaseDC puts the DIB
    // straight back, with every string ever drawn into it still there. On the vehicle screen that
    // showed as "Mini Cooper Classic" printed over "Ford Mustang Fastback" and "Fiery Red" over
    // "Slate Grey".
    //
    // Cleared to the same colour, in the DIB's own BGRA order, and then uploaded - so the two
    // copies agree and the next GDI draw starts from a blank canvas.
    if (surface->DibPixels != nullptr)
    {
        const usize dib_count = static_cast<usize>(surface->Width) * static_cast<usize>(surface->Height);

        const u8 a = static_cast<u8>((arg1 >> 24) & 0xFF);
        const u8 r = static_cast<u8>((arg1 >> 16) & 0xFF);
        const u8 g = static_cast<u8>((arg1 >> 8) & 0xFF);
        const u8 b = static_cast<u8>(arg1 & 0xFF);

        for (usize i = 0; i < dib_count; ++i)
        {
            u8* px = surface->DibPixels + (i * 4);

            px[0] = b;
            px[1] = g;
            px[2] = r;
            px[3] = a;
        }

        agiGLSurfaceUpload(surface, 0, surface->DibPixels, surface->Width * 4);

        return;
    }

    // The original is a DDBLT_COLORFILL over the whole surface, and arg1 is a value in the
    // surface's own pixel format. Here it is ARGB, matching every other colour in the reimplemented
    // path; mmText::CreateFitBitmap is the one caller and it passes 0, which means the same thing
    // either way.
    usize pixel_count = static_cast<usize>(Width) * static_cast<usize>(Height);
    u8* rgba = new u8[pixel_count * 4];

    u8 r = static_cast<u8>((arg1 >> 16) & 0xFF);
    u8 g = static_cast<u8>((arg1 >> 8) & 0xFF);
    u8 b = static_cast<u8>(arg1 & 0xFF);
    u8 a = static_cast<u8>((arg1 >> 24) & 0xFF);

    for (usize i = 0; i < pixel_count; ++i)
    {
        rgba[(i * 4) + 0] = r;
        rgba[(i * 4) + 1] = g;
        rgba[(i * 4) + 2] = b;
        rgba[(i * 4) + 3] = a;
    }

    agiGLSurfaceUpload(surface, 0, rgba, Width * 4);

    delete[] rgba;
}

#endif
