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

class NetStartArray
{
public:
    // ??0NetStartArray@@QAE@XZ
    ARTS_IMPORT NetStartArray();

    // ??1NetStartArray@@QAE@XZ
    ARTS_IMPORT ~NetStartArray();

    // ?AssignOpenIndex@NetStartArray@@QAEHK@Z
    ARTS_IMPORT i32 AssignOpenIndex(ulong arg1);

    // ?Clear@NetStartArray@@QAEXXZ
    ARTS_IMPORT void Clear();

    // ?ClearIndex@NetStartArray@@QAEXK@Z
    ARTS_IMPORT void ClearIndex(ulong arg1);

    // ?GetIndex@NetStartArray@@QAEHK@Z
    ARTS_IMPORT i32 GetIndex(ulong arg1);

    // ?Init@NetStartArray@@QAEXPAK@Z
    ARTS_IMPORT void Init(ulong* arg1);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u32 Slots[10]; // 0x000
};

check_size(NetStartArray, 0x28);
