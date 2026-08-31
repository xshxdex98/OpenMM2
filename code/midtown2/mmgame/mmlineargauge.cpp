/*
    OpenMM2 - An Open Source Re-Implementation of Midtown Madness 2

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

// The HUD's bar gauges - the speedometer and tachometer backing, and anything else drawn as a
// bitmap filled proportionally along one axis. mmSlidingGauge derives from it.
//
// All five functions of the class, so nothing of mmLinearGauge is left in game.asm. It is not
// polymorphic: the destructor mangles ??1mmLinearGauge@@QAE@XZ, `Q` for public non-virtual, and
// the class has no vftable, so none of the handover machinery is involved.
//
// The two MM2:: entries the kit lists beside these - getMinValue at 0x005A9290 and setDestY at
// 0x005AA280 - are MM2Hook's Lua bindings, outside the class's own code and with bodies that do
// not correspond to their names. They are not part of this class and are not ported.
//
// MinValue and MaxValue are POINTERS TO the values, not the values: the gauge is wired to whatever
// floats it should track and reads them each frame. That is why Draw dereferences twice.

#include "mmlineargauge.h"

#include "gfx/gfxbitmap.h"
#include "gfx/gfxpipeline.h"
#include "misc/freefuncs.h"
#include "mmgame/mmexternalview.h"

// ??0mmLinearGauge@@QAE@XZ - 0x0043EE40
//
// Clears the two bitmap pointers and the destination, and deliberately not the rest: MinValue,
// MaxValue, DimensionType and ExternalView are left holding whatever the allocation held, because
// Init writes all four before anything reads them.
mmLinearGauge::mmLinearGauge()
{
    DestX = 0;
    DestY = 0;
    BitmapGauge = nullptr;
    BitmapLabel = nullptr;
}

// ??1mmLinearGauge@@QAE@XZ - 0x0043EE60
//
// One byte in the binary - a bare `ret`. It does NOT release BitmapGauge, though Init does; the
// bitmaps outlive the gauge in the node cache.
mmLinearGauge::~mmLinearGauge()
{}

// ?Init@mmLinearGauge@@QAEXPADPAM1HPAVmmExternalView@@@Z - 0x0043EE70
//
// Points the gauge at the two floats it tracks, then swaps its bitmap.
//
// THE RELEASE IS A HAND-ROLLED REFCOUNT, not a call to any Release method: decrement, and destroy
// only on reaching zero. gfxBitmap's destructor is private in midtown2.exe, which is what
// Access_Destroy in gfxbitmap.h exists to reach - the same split the original has, where the
// destructor and operator delete are two separate calls.
void mmLinearGauge::Init(char* arg1, f32* arg2, f32* arg3, i32 arg4, mmExternalView* arg5)
{
    gfxBitmap* previous = BitmapGauge;

    MinValue = arg2;
    MaxValue = arg3;
    DimensionType = arg4;
    ExternalView = arg5;

    if (previous)
    {
        if (--previous->RefCount == 0)
        {
            previous->Access_Destroy();
            operator delete(previous);
        }
    }

    BitmapGauge = nodeGetBitmap(arg1, false, false);
}

// ?InitOverlay@mmLinearGauge@@QAEXPAD@Z - 0x0043EED0
//
// The label drawn over the gauge. No release of the previous one, unlike Init - the original
// simply overwrites it.
void mmLinearGauge::InitOverlay(char* arg1)
{
    BitmapLabel = nodeGetBitmap(arg1, false, false);
}

// ?Draw@mmLinearGauge@@QAEXXZ - 0x0043EEF0
//
// Blits the gauge bitmap cropped to the fraction MinValue/MaxValue, then the label over it.
// DimensionType picks the axis: non-zero fills horizontally and crops the width, zero fills
// vertically and crops the height.
//
// THE FRACTION IS COMPUTED IN DOUBLE AND TRUNCATED, and both halves matter:
//
//     v5 = (__int64)(*MinValue / *MaxValue * (double)Width);
//
// the extent is widened to double before the multiply - x87 under /arch:IA32 - and the result is
// truncated toward zero rather than rounded. A fraction that lands below one pixel therefore comes
// out as 0, and the `if (v5)` below skips the blit entirely rather than drawing a zero-width one.
// Reproduced rather than tidied: it is why an empty gauge draws nothing at all.
//
// Width and Height are u16 on gfxBitmap, so they widen to i32 before the multiply either way.
void mmLinearGauge::Draw()
{
    gfxBitmap* gauge = BitmapGauge;
    i32 extent = DimensionType ? gauge->Width : gauge->Height;
    i32 filled = static_cast<i32>(static_cast<f64>(*MinValue) / *MaxValue * static_cast<f64>(extent));

    if (filled)
    {
        i32 x = DestX + ExternalView->DestX;
        i32 y = DestY + ExternalView->DestY;

        if (DimensionType)
            gfxPipeline::CopyBitmap(x, y, gauge, 0, 0, filled, gauge->Height, true);
        else
            gfxPipeline::CopyBitmap(x, y, gauge, 0, 0, gauge->Width, filled, true);
    }

    if (BitmapLabel)
    {
        gfxPipeline::CopyBitmap(DestX + ExternalView->DestX, DestY + ExternalView->DestY,
            BitmapLabel, 0, 0, BitmapLabel->Width, BitmapLabel->Height, true);
    }
}
