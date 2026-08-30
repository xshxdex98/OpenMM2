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

#include "data7/printer.h"
#include "mmtextnode.h"

#include "gfx/gfxbitmap.h"
#include "ui/uiscale.h"

define_dummy_symbol(mmgame_mmtextnode);

// sizeof(mmTextData). The original spells it as the multiply `((n << 4) + n) << 4`, which is
// n * 17 * 16; the struct is a 256-byte text buffer plus four dwords of position, font and flags.
#define MM_TEXT_DATA_SIZE 272

// The 0xFFFFFF00 the original stores into HiglightColor - the constructor leaves the field alone
// and Init is what sets it.
#define MM_TEXT_DEFAULT_HIGHLIGHT 0xFFFFFF00u

static ARTS_FORCEINLINE i32 ClampToRange(i32 value, i32 limit)
{
    if (value < 0)
        return 0;

    return (value > limit) ? limit : value;
}

// Transcribed from the disassembly of ?Init@mmTextNode@@QAEXHHHHHH@Z at 0x00532750. The Hex-Rays
// output for it in MM2_RE_KIT is legible but every screen dimension in it is mislabelled as an
// std::function constructor, which is gfxPipeline::Access_m_iHeight() (docs/ui_scaling.md).
//
// This is the INTEGER overload, the one that takes authored pixels. The float overload at
// 0x00532840 takes 0..1 fractions and sizes its bitmap off the live screen, so it is already
// proportional and is left alone. mmDropDown::InitString at 0x004F2358 is the only caller of this
// one in the whole binary.
//
// THE CHANGE, and it is the same one as in the CSV loaders. The original clamps the four pixel
// arguments against the LIVE screen and then divides the position by that same live screen size,
// which the draw path multiplies straight back - so a text node authored at (100, 40) sits at
// pixel (100, 40) whatever the resolution, and its bitmap is the authored pixel size no matter
// how large the glyphs going into it are.
//
// Here the clamp is against the 640x480 design space, the position goes through uiScale, and the
// bitmap is created at the SCALED size. That last part is what pairs with mmText::CreateLocFont:
// once fonts scale with the vertical resolution, a design-sized bitmap would clip the text it is
// being asked to hold. The bitmap is now in screen pixels, so mmTextNode::Cull's 1:1 blit stays
// correct and gfxPipeline::CopyBitmap must NOT resample it.
//
// At exactly 640x480 every value below is the value the original computes.

// ?Init@mmTextNode@@QAEXHHHHHH@Z
void mmTextNode::Init(i32 arg1, i32 arg2, i32 arg3, i32 arg4, i32 arg5, i32 arg6)
{
    // arg6 is dead in the original too - it is pushed, never read, and the function returns with
    // `ret 0x18`, so it cannot be dropped from the signature.
    (void)arg6;

    EntryCount = 0;

    // THIS OVERLOAD IS HANDED LIVE-SCREEN PIXELS, NOT AUTHORED ONES, AND THE COMMENT ABOVE USED
    // TO CLAIM OTHERWISE.
    //
    // The design-space treatment that was here - clamp against 640x480, map the position through
    // uiScale, create the bitmap at the scaled size - rested on the premise that mmDropDown passes
    // authored pixels. It does not. Logging the arguments at the only call site gives values like
    // (1212, 756) and (0, 911) with the screen at 1920x1080: those are already screen pixels, and
    // the extent 615x48 is already scaled to match.
    //
    // The mistake was invisible for as long as the frontend ran at 640x480, because uiScale's
    // factor was then exactly one and every line of it was the identity. Moving the frontend to the
    // chosen resolution turned it on, and 1212 promptly clamped to 639 and 911 to 479 - the
    // dropdown list was positioned and cropped into nothing, which is why it stopped appearing
    // while its arrows still worked.
    //
    // So this is the ORIGINAL again: clamp to the live screen, and divide by the same live screen
    // the draw path multiplies back. Nothing is lost by it - the caller has already done the
    // scaling this was trying to do a second time.
    const i32 screen_w = gfxPipeline::Access_m_iWidth();
    const i32 screen_h = gfxPipeline::Access_m_iHeight();

    i32 x = ClampToRange(arg1, screen_w - 1);
    i32 y = ClampToRange(arg2, screen_h - 1);
    i32 width = ClampToRange(arg3, screen_w - x);
    i32 height = ClampToRange(arg4, screen_h - y);

    Pos.x = static_cast<f32>(x) / static_cast<f32>(screen_w);
    Pos.y = static_cast<f32>(y) / static_cast<f32>(screen_h);

    MaxEntries = static_cast<u32>(arg5);

    void* entries = operator new(MM_TEXT_DATA_SIZE * MaxEntries);

    pTextEntries = static_cast<mmTextData*>(entries);

    // The zero-fill is written through a volatile pointer ON PURPOSE. MSVC recognises a plain
    // zeroing loop and lowers it to a call to _memset - and _memset does not exist as a symbol
    // anywhere in midtown2.exe, because the 1999 compiler inlined it as the `rep stosd` this
    // function actually contains. See docs/ported_freefuncs.md; the same trap is why several
    // otherwise trivial free functions are still ARTS_IMPORT.
    volatile u32* fill = static_cast<volatile u32*>(entries);
    u32 dwords = (MM_TEXT_DATA_SIZE / 4) * MaxEntries;

    for (u32 i = 0; i < dwords; ++i)
        fill[i] = 0;

    // Raw extent, like the position above: the caller has already scaled both. The scaled canvas
    // that was briefly here made the dropdown list three times its proper size, and did NOT bring
    // the text back - which is what proved the blank text was never about canvas size.
    Bitmap = gfxBitmap::Create(width, height, false);
    Bitmap->Clear(0);

    HiglightColor = MM_TEXT_DEFAULT_HIGHLIGHT;
}
