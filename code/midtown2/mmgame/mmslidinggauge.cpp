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

// A gauge whose bitmap SLIDES behind a fixed window, rather than being revealed a bit at a time.
// mmLinearGauge crops what it draws to the filled fraction; this keeps the drawn size constant at
// BitmapDimension and moves the source offset instead. That difference is the entire class.
//
// READ FROM THE DISASSEMBLY, NOT THE KIT. MM2_RE_KIT has pseudocode for the destructor alone -
// Draw and Init are in the 28.7% of functions it does not cover - so both were decoded from
// midtown2.exe directly. The offsets below are quoted from that reading so the next person can
// check them without repeating it.
//
// Two calls the disassembly resolves, both confirmed against data/symbols.json:
//     0x004AB4C0  ?CopyBitmap@gfxPipeline@@SAXHHPAVgfxBitmap@@HHHH_N@Z
//     0x00581608  __ftol - the float-to-int helper, so every cast below truncates toward zero

#include "mmslidinggauge.h"

#include "gfx/gfxbitmap.h"
#include "gfx/gfxpipeline.h"
#include "mmgame/mmexternalview.h"

// ??1mmSlidingGauge@@QAE@XZ - 0x00431C10
//
// `jmp 0x0043EE60` and nothing else - a thunk straight to mmLinearGauge::~mmLinearGauge, which is
// itself a bare ret. An empty body reproduces it: the compiler emits the base call.
mmSlidingGauge::~mmSlidingGauge()
{}

// ?Init@mmSlidingGauge@@QAEXPADPAM1HPAVmmExternalView@@H@Z - 0x0043EFB0
//
//     mov [ecx+0x20], eax        ; the sixth argument, straight into BitmapDimension
//     push .. push .. call 0x0043EE70
//
// Takes the extra sixth argument for itself and hands the other five to the base unchanged. `this`
// is passed through in ecx without adjustment, which is correct because mmLinearGauge sits at
// offset 0.
void mmSlidingGauge::Init(char* arg1, f32* arg2, f32* arg3, i32 arg4, mmExternalView* arg5, i32 arg6)
{
    BitmapDimension = arg6;

    mmLinearGauge::Init(arg1, arg2, arg3, arg4, arg5);
}

// ?Draw@mmSlidingGauge@@QAEXXZ - 0x0043EFE0
//
// The travel is the bitmap's extent along the chosen axis MINUS the window, because the window is
// what stays on screen:
//
//     eax = DimensionType ? BitmapGauge->Width : BitmapGauge->Height
//     fld [MinValue]; fdiv [MaxValue]      ; the fraction
//     sub eax, ebx                         ; ebx is BitmapDimension
//     fimul [ebp-4]                        ; fraction * (extent - BitmapDimension), as an INTEGER
//     call __ftol                          ; truncated toward zero
//
// then the offset moves along the SOURCE while the drawn size stays BitmapDimension:
//
//     DimensionType != 0   CopyBitmap(x, y, gauge, offset, 0, BitmapDimension, gauge->Height, 1)
//     DimensionType == 0   CopyBitmap(x, y, gauge, 0, offset, gauge->Width, BitmapDimension, 1)
//
// That is the opposite of mmLinearGauge::Draw, which leaves the source at 0,0 and varies the size.
//
// The fraction is computed on the x87 stack and multiplied by an integer via `fimul`, so the
// widening is inherent rather than a decompiler artifact - written here as an f64 multiply against
// the integer travel, which is the same operation under /arch:IA32.
//
// Unlike the base there is NO `if (filled)` guard: a zero offset still draws, because the window
// is a fixed size and drawing it at offset 0 is the correct empty state.
void mmSlidingGauge::Draw()
{
    gfxBitmap* gauge = BitmapGauge;
    i32 extent = DimensionType ? gauge->Width : gauge->Height;
    i32 travel = extent - BitmapDimension;
    i32 offset = static_cast<i32>(static_cast<f64>(*MinValue) / *MaxValue * travel);

    i32 x = DestX + ExternalView->DestX;
    i32 y = DestY + ExternalView->DestY;

    if (DimensionType)
        gfxPipeline::CopyBitmap(x, y, gauge, offset, 0, BitmapDimension, gauge->Height, true);
    else
        gfxPipeline::CopyBitmap(x, y, gauge, 0, offset, gauge->Width, BitmapDimension, true);

    // The label overlay is identical to mmLinearGauge's - same source rectangle, same origin.
    if (BitmapLabel)
    {
        gfxPipeline::CopyBitmap(DestX + ExternalView->DestX, DestY + ExternalView->DestY,
            BitmapLabel, 0, 0, BitmapLabel->Width, BitmapLabel->Height, true);
    }
}
