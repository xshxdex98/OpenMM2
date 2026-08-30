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

class lvlSegmentInfo;
class phSegment;

class lvlSegment : public phSegment
{
public:
    // ?CalculateInfo@lvlSegment@@QAEXXZ
    ARTS_IMPORT void CalculateInfo();

    // ?Set@lvlSegment@@QAEXABVVector3@@0HPAVlvlSegmentInfo@@@Z
    ARTS_IMPORT void Set(const Vector3& arg1, const Vector3& arg2, i32 arg3, lvlSegmentInfo* arg4);

public:
    // Members from 0x01C; everything below that belongs to phSegment.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    bool IsVertical; // 0x01C
    u8[3] pad_1D; // 0x01D
    f32 InvLength; // 0x020
    lvlSegmentInfo* SegmentInfo; // 0x024
};

check_size(lvlSegment, 0x28);
