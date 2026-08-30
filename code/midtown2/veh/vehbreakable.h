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

#include "lvl/lvlinstance.h"
#include "vector7/matrix34.h"

class modStatic;

class vehBreakable
{
public:
    // ??0vehBreakable@@QAE@PAPAVmodStatic@@AAVMatrix34@@HHH@Z
    ARTS_IMPORT vehBreakable(modStatic** arg1, Matrix34& arg2, i32 arg3, i32 arg4, i32 arg5);

    // ?Add@vehBreakable@@QAEXPAV1@@Z
    ARTS_IMPORT void Add(vehBreakable* arg1);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    bool IsAttached; // 0x000
    u8 pad_1[3]; // 0x001
    i32 Flags; // 0x004
    Matrix34 Matrix; // 0x008
    u8 pad_C[44]; // 0x00C
    lvlInstance::GeomTableEntry* Model; // 0x038
    i32 BangerDataIndex; // 0x03C
    i32 GeometryIndex; // 0x040
    lvlInstance* Instance; // 0x044
    vehBreakable* Next; // 0x048
};

check_size(vehBreakable, 0x4C);
