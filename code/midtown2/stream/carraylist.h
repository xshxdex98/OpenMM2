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

class Stream;

class CArrayList
{
public:
    // ??0CArrayList@@QAE@XZ
    ARTS_IMPORT CArrayList();

    // ??1CArrayList@@QAE@XZ
    ARTS_IMPORT ~CArrayList();

    // ?Append@CArrayList@@QAEXH@Z
    ARTS_IMPORT void Append(i32 arg1);

    // ?ReadBinary@CArrayList@@QAEXPAVStream@@@Z
    ARTS_IMPORT void ReadBinary(Stream* arg1);

    // ?SaveBinary@CArrayList@@QAEXPAVStream@@@Z
    ARTS_IMPORT void SaveBinary(Stream* arg1);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u16 Size; // 0x000
    u32 Data; // 0x004
};

check_size(CArrayList, 0x8);
