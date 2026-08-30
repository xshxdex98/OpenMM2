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

class aiRouteNode
{
public:
    // ??0aiRouteNode@@QAE@XZ
    ARTS_IMPORT aiRouteNode();

    // ?ReplayDebug@aiRouteNode@@QAEXXZ
    ARTS_IMPORT void ReplayDebug();

    // ?Reset@aiRouteNode@@QAEXXZ
    ARTS_IMPORT void Reset();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i32 unk_0x00; // 0x000
    Vector3 Position; // 0x004
    u8[8] pad_8; // 0x008
    f32 Angle; // 0x010
    i32 unk_14; // 0x014
    i16 unk_18; // 0x018
    i16 RoadVertexIndex; // 0x01A
    i16 unk_1C; // 0x01C
    i16 unk_1E; // 0x01E
    i16 unk_20; // 0x020
    i16 unk_22; // 0x022
};

check_size(aiRouteNode, 0x24);
