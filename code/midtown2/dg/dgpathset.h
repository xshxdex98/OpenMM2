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

class dgPath;
class dgPathSet_vtbl;

class dgPathSet
{
public:
    // ??0dgPathSet@@QAE@XZ
    ARTS_IMPORT dgPathSet();

    // ??1dgPathSet@@UAE@XZ
    ARTS_IMPORT virtual ~dgPathSet();

    // ?Kill@dgPathSet@@QAEXXZ
    ARTS_IMPORT void Kill();

    // ?Load@dgPathSet@@QAE_NPBD0@Z
    ARTS_IMPORT bool Load(const char* arg1, const char* arg2);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    dgPath [192]* Paths; // 0x004
    u8[764] pad_8; // 0x008
    i32 PathCount; // 0x304
    i32 unk0; // 0x308
    i32 unk1; // 0x30C
};

// ??_7dgPathSet@@6B@
// vtable at 0x005B1B0C
check_size(dgPathSet, 0x310);
