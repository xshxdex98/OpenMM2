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

#include "vector7/vector3.h"

class lvlIntersectionPoint;
class phBound;
class phPolygon;

class phIntersection
{
public:
    // ??0phIntersection@@QAE@XZ
    ARTS_IMPORT phIntersection();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    lvlIntersectionPoint IntersectionPoint; // 0x000
    u8 pad_4[32]; // 0x004
    i32 PolyIndex; // 0x024
    i32 field_28; // 0x028
    i32 field_2C; // 0x02C
    i32 field_30; // 0x030
    Vector3 StartPos; // 0x034
    u8 pad_38[8]; // 0x038
    Vector3 EndPos; // 0x040
    u8 pad_44[8]; // 0x044
    Vector3 field_4C; // 0x04C
    u8 pad_50[8]; // 0x050
    phPolygon* Poly; // 0x058
    i32 MaterialIndex; // 0x05C
    i32 field_60; // 0x060
    i32 field_64; // 0x064
    i32 field_68; // 0x068
    phBound* Bound; // 0x06C
    f32 field_70; // 0x070
    i32 field_74; // 0x074
    f32 field_78; // 0x078
    Vector3 field_7C; // 0x07C
    u8 pad_80[8]; // 0x080
    Vector3 field_88; // 0x088
    u8 pad_8C[8]; // 0x08C
    f32 field_94; // 0x094
    i16 Flags; // 0x098
    u8 pad_9A[2]; // 0x09A
};

check_size(phIntersection, 0x9C);
