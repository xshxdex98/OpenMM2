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

class Matrix34;
class Stream;
struct __m128;

class dgPath
{
public:
    // ??0dgPath@@QAE@PBD@Z
    ARTS_IMPORT dgPath(const char* arg1);

    // ??1dgPath@@QAE@XZ
    ARTS_IMPORT ~dgPath();

    // ?Enumerate@dgPath@@QBEXP6AXPBDABVMatrix34@@_N@ZMM@Z
    ARTS_IMPORT void Enumerate(void (__cdecl* arg1)(const char*,const Matrix34&,bool), f32 arg2, f32 arg3) const;

    // ?Load@dgPath@@SAPAV1@PAVStream@@@Z
    ARTS_IMPORT static dgPath* Load(Stream* arg1);

    // ?SetName@dgPath@@QAEXPBD@Z
    ARTS_IMPORT void SetName(const char* arg1);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 Name[32]; // 0x000
    __m128* pVector4s; // 0x020
    u32 field_24; // 0x024
    u32 field_28; // 0x028
    u32 field_2C; // 0x02C
    u32 field_30; // 0x030
    f32 field_34; // 0x034
};

check_size(dgPath, 0x38);
