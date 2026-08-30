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

class MArrayEntry;
class Vector4;

class MArray
{
public:
    // ??0MArray@@QAE@XZ
    ARTS_IMPORT MArray();

    // ??1MArray@@QAE@XZ
    ARTS_IMPORT ~MArray();

    // ?AddMenuData@MArray@@QAEXHAAVVector4@@PAD@Z
    ARTS_IMPORT void AddMenuData(i32 arg1, Vector4& arg2, char* arg3);

    // ?Flush@MArray@@QAEXXZ
    ARTS_IMPORT void Flush();

    // ?Init@MArray@@QAEXH@Z
    ARTS_IMPORT void Init(i32 arg1);

    // ?Read@MArray@@QAEXPAD@Z
    ARTS_EXPORT void Read(char* arg1);

    // ?RetrieveMenuData@MArray@@QAEHHAAM000@Z
    ARTS_IMPORT i32 RetrieveMenuData(i32 arg1, f32& arg2, f32& arg3, f32& arg4, f32& arg5);

    // ?Write@MArray@@QAEXPAD@Z
    ARTS_IMPORT void Write(char* arg1);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    MArrayEntry* pData; // 0x000
    u32 MaxSize; // 0x004
    u32 Size; // 0x008
};

check_size(MArray, 0xC);
