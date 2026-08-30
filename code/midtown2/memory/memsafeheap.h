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

class memMemoryAllocator;

class memSafeHeap
{
public:
    // ??0memSafeHeap@@QAE@XZ
    ARTS_IMPORT memSafeHeap();

    // ??1memSafeHeap@@QAE@XZ
    ARTS_IMPORT ~memSafeHeap();

    // ?Init@memSafeHeap@@QAEXPAVmemMemoryAllocator@@I_N11@Z
    ARTS_IMPORT void Init(memMemoryAllocator* arg1, u32 arg2, bool arg3, bool arg4, bool arg5);

    // ?Kill@memSafeHeap@@QAEXXZ
    ARTS_IMPORT void Kill();

    // ?Restart@memSafeHeap@@QAEXXZ
    ARTS_IMPORT void Restart();

protected:
    // ?Activate@memSafeHeap@@IAEXXZ
    ARTS_IMPORT void Activate();

    // ?Deactivate@memSafeHeap@@IAEXXZ
    ARTS_IMPORT void Deactivate();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    memMemoryAllocator* Allocator; // 0x000
    LPVOID Heap; // 0x004
    void* CommitedData; // 0x008
    u32 SomeAlignmentThingy; // 0x00C
    u32 AllocSize; // 0x010
    u8 bool14; // 0x014
    u8 bool15; // 0x015
    u8 CheckAlloc; // 0x016
    u8 field_17; // 0x017
};

check_size(memSafeHeap, 0x18);
