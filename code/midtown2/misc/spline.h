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

#include "arts7/asnode.h"

class Spline : public asNode
{
public:
    // ??0Spline@@QAE@XZ
    ARTS_IMPORT Spline();

    // ??1Spline@@UAE@XZ
    ARTS_IMPORT virtual ~Spline();

    // ?Update@Spline@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?CalcCoeff@Spline@@QAEXXZ
    ARTS_IMPORT void CalcCoeff();

    // ?FixTimeStop@Spline@@QAEXXZ
    ARTS_IMPORT void FixTimeStop();

    // ?Init@Spline@@QAEXPAMH@Z
    ARTS_IMPORT void Init(f32* arg1, i32 arg2);

    // ?InRange@Spline@@QAEHXZ
    ARTS_IMPORT i32 InRange();

    // ?Print@Spline@@QAEXH@Z
    ARTS_IMPORT void Print(i32 arg1);

    // ?SetGoal@Spline@@QAEXPAMM@Z
    ARTS_IMPORT void SetGoal(f32* arg1, f32 arg2);

    // ?SetValue@Spline@@QAEXPAM@Z
    ARTS_IMPORT void SetValue(f32* arg1);

    // ?Solve@Spline@@QAEXM@Z
    ARTS_IMPORT void Solve(f32 arg1);

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i32 Count; // 0x018
    f32* field_1C; // 0x01C
    f32* field_20; // 0x020
    f32* field_24; // 0x024
    f32* field_28; // 0x028
    f32* field_2C; // 0x02C
    f32* field_30; // 0x030
    f32* field_34; // 0x034
    f32* field_38; // 0x038
    f32* field_3C; // 0x03C
    f32* field_40; // 0x040
    f32* field_44; // 0x044
    f32 field_48; // 0x048
    f32 field_4C; // 0x04C
    f32 field_50; // 0x050
    f32 field_54; // 0x054
    f32 field_58; // 0x058
    f32 field_5C; // 0x05C
    f32 field_60; // 0x060
    f32 field_64; // 0x064
    i32 field_68; // 0x068
};

// ??_7Spline@@6B@
// vtable at 0x005B51B8
check_size(Spline, 0x6C);
