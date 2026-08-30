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

class Vector3;

class mmNetPath
{
public:
    // ??0mmNetPath@@QAE@XZ
    ARTS_IMPORT mmNetPath();

    // ??1mmNetPath@@QAE@XZ
    ARTS_IMPORT ~mmNetPath();

    // ?Compute@mmNetPath@@QAEXAAVVector3@@000@Z
    ARTS_IMPORT void Compute(Vector3& arg1, Vector3& arg2, Vector3& arg3, Vector3& arg4);

    // ?Reset@mmNetPath@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?Solve@mmNetPath@@QAEXAAVVector3@@0M@Z
    ARTS_IMPORT void Solve(Vector3& arg1, Vector3& arg2, f32 arg3);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i32 field_40; // 0x040
    i32 field_44; // 0x044
    i32 field_48; // 0x048
    i32 field_4C; // 0x04C
    i32 field_50; // 0x050
    i32 field_54; // 0x054
    i32 field_58; // 0x058
    i32 field_5C; // 0x05C
    i32 field_60; // 0x060
    i32 field_64; // 0x064
    i32 field_68; // 0x068
    i32 field_6C; // 0x06C
};

check_size(mmNetPath, 0x70);
