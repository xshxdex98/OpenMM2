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

#pragma once

#include "core/arts.h"

class mmJaxis
{
public:
    // ??0mmJaxis@@QAE@XZ
    ARTS_EXPORT mmJaxis();

    // ??1mmJaxis@@QAE@XZ
    ARTS_EXPORT ~mmJaxis();

    // ?Capture@mmJaxis@@QAEHXZ
    ARTS_EXPORT i32 Capture();

    // ?Normalize@mmJaxis@@QAEXM@Z
    ARTS_EXPORT void Normalize(f32 arg1);

    // ?NormalizePOV@mmJaxis@@QAEXK@Z
    ARTS_EXPORT void NormalizePOV(ulong arg1);

    // ?ResetCapture@mmJaxis@@QAEXXZ
    ARTS_EXPORT void ResetCapture();

    // ?SetRange@mmJaxis@@QAEXMM@Z
    ARTS_EXPORT void SetRange(f32 arg1, f32 arg2);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    f32 CaptureOrigin; // 0x000
    f32 Value; // 0x004
    f32 Range; // 0x008
    f32 Min; // 0x00C
    f32 CaptureLo; // 0x010
    f32 CaptureHi; // 0x014
    i32 CaptureEnabled; // 0x018
};

check_size(mmJaxis, 0x1C);
