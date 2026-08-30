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

#include "mmjaxis.h"

define_dummy_symbol(mmgame_mmjaxis);

// The whole class - seven functions and 256 bytes of machine code at 0x00530230-0x00530323 - read
// off the instruction bytes of midtown2.exe with capstone. One joystick axis: a raw reading is
// mapped into [-1, +1] by Normalize, and Capture reports whether it has moved far enough from a
// snapshot to count as a deliberate flick.
//
// THE LAYOUT WAS RETYPED FOR THIS. tools/ctor_layouts.py had all seven offsets and the 0x1C size
// right but called the first four `i32`, because a constructor storing a dword of zero cannot say
// whether the dword is an integer or a float, and it had 0x10/0x14 as one opaque eight-byte blob
// because no constructor writes them at all. Every function below reads those offsets with x87
// instructions, which settles it; the derivation and the witness for each field are in
// data/layouts_round4.json. Offsets and size are unchanged, so this is a retyping and not a new
// layout - nothing in the binary moves.
//
// FLOAT FIDELITY. Every computation here is a single unbroken x87 chain with exactly one `fstp`
// at the end, so intermediates never round to f32 on the way through. They are written as f64 for
// the same reason vector7/vector2.cpp is: with f32 operands the compiler would round at each step
// and the result would differ in the low bits from what the 1999 build produces.

// ??0mmJaxis@@QAE@XZ
mmJaxis::mmJaxis()
{
    CaptureOrigin = 0.0f;
    Value = 0.0f;
    Range = 0.0f;
    Min = 0.0f;
    CaptureEnabled = 0;

    // CaptureLo and CaptureHi are deliberately NOT initialised. The constructor writes five dwords
    // - 0x00, 0x04, 0x08, 0x0C and 0x18 - and stops; the capture window is set by ResetCapture and
    // is garbage until something calls it. Capture reads both, so an mmJaxis that was constructed
    // and never reset compares against whatever the allocator last left there. That is the
    // original's behaviour and reproducing it is the job - see Trap 12.
}

// ??1mmJaxis@@QAE@XZ
mmJaxis::~mmJaxis()
{
    // A bare `ret`.
}

// ?Capture@mmJaxis@@QAEHXZ
i32 mmJaxis::Capture()
{
    if (!CaptureEnabled)
        return 0;

    f64 moved = f64 {Value} - CaptureOrigin;

    // `fcom` then `test ah, 1` - C0 alone. C0 is set both by "less than" AND by "unordered", so a
    // NaN reading takes this branch in the original. Written `!(moved >= CaptureLo)` rather than
    // `moved < CaptureLo` precisely to keep that: the plain form is false for a NaN and would fall
    // through to return 0 instead of -1. This is the exact shape of the deliberate change that got
    // the Matrix33 round quarantined, so it is spelled out rather than tidied.
    if (!(moved >= CaptureLo))
        return -1;

    // `fcomp` then `test ah, 0x41` - C0 or C3, so less-or-equal (or unordered) falls through to 0.
    // Unordered cannot reach here, having been taken above, so this one is an ordinary `>`.
    if (moved > CaptureHi)
        return 1;

    return 0;
}

// ?Normalize@mmJaxis@@QAEXM@Z
void mmJaxis::Normalize(f32 arg1)
{
    // 2 * (v - Min) / Range - 1, which is [Min, Min+Range] mapped onto [-1, +1]. The 1.0f comes
    // from .rdata at 0x005B0364, confirmed by its bit pattern 0x3F800000.
    f64 normalized = f64 {arg1} - Min;

    normalized = normalized + normalized;
    normalized = normalized / Range;

    Value = static_cast<f32>(normalized - 1.0);
}

// ?NormalizePOV@mmJaxis@@QAEXK@Z
void mmJaxis::NormalizePOV(ulong arg1)
{
    // `cmp ax, 0xFFFF` - only the LOW SIXTEEN BITS are tested, so DirectInput's centred POV of
    // 0xFFFFFFFF matches and so does a plain 0x0000FFFF. Kept as a 16-bit comparison.
    if (static_cast<u16>(arg1) == 0xFFFF)
    {
        Value = -1.0f;

        return;
    }

    // `fild qword ptr` over the argument zero-extended to 64 bits, so this is an unsigned load and
    // not a signed one. (Range - v) / Range, with Range doing duty as the POV's full sweep.
    Value = static_cast<f32>((f64 {Range} - static_cast<f64>(arg1)) / Range);
}

// ?ResetCapture@mmJaxis@@QAEXXZ
void mmJaxis::ResetCapture()
{
    // 0xBE000000 and 0x3E000000, verified by bit pattern: a symmetric eighth of full deflection
    // either side of wherever the axis is sitting right now.
    CaptureLo = -0.125f;
    CaptureOrigin = Value;
    CaptureHi = 0.125f;
}

// ?SetRange@mmJaxis@@QAEXMM@Z
void mmJaxis::SetRange(f32 arg1, f32 arg2)
{
    Value = 0.0f;
    Min = arg2;
    Range = arg1 - arg2;
}
