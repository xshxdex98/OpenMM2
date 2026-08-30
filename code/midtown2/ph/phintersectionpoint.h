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

class Matrix34;

class phIntersectionPoint
{
public:
    // ??0phIntersectionPoint@@QAE@XZ
    ARTS_IMPORT phIntersectionPoint();

    // ?Set@phIntersectionPoint@@QAEXABVVector3@@0MME@Z
    ARTS_IMPORT void Set(const Vector3& arg1, const Vector3& arg2, f32 arg3, f32 arg4, u8 arg5);

    // ?Transform@phIntersectionPoint@@QAEXPBVMatrix34@@@Z
    ARTS_IMPORT void Transform(const Matrix34* arg1);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    Vector3 Point; // 0x000
    u8 pad_4[8]; // 0x004
    Vector3 Normal; // 0x00C
    u8 pad_10[8]; // 0x010
    f32 NormalizedDistance; // 0x018
    f32 Penetration; // 0x01C
    u8 IntersectResult; // 0x020
    u8 pad_21[3]; // 0x021
};

check_size(phIntersectionPoint, 0x24);
