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

class mmVehInfo_vtbl;

class mmVehInfo
{
public:
    // ??0mmVehInfo@@QAE@XZ
    ARTS_IMPORT mmVehInfo();

    // ??1mmVehInfo@@UAE@XZ
    ARTS_IMPORT virtual ~mmVehInfo();

    // ?ComputeTuningCRC@mmVehInfo@@QAEXXZ
    ARTS_IMPORT void ComputeTuningCRC();

    // ?GetTuningCRC@mmVehInfo@@QAEKXZ
    ARTS_IMPORT ulong GetTuningCRC();

    // ?HasColorVariations@mmVehInfo@@QAEHXZ
    ARTS_IMPORT i32 HasColorVariations();

    // ?IsValid@mmVehInfo@@QAEHXZ
    ARTS_IMPORT i32 IsValid();

    // ?Load@mmVehInfo@@QAEHPAD@Z
    ARTS_IMPORT i32 Load(char* getter);

    // ?Print@mmVehInfo@@QAEXXZ
    ARTS_IMPORT void Print();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    char[40] baseName; // 0x004
    char[80] description; // 0x02C
    char[100] colors; // 0x07C
    i32 flags; // 0x0E0
    i32 order; // 0x0E4
    i32 isValid; // 0x0E8
    i32 unlockScore; // 0x0EC
    i32 unlockFlags; // 0x0F0
    i32 isLocked; // 0x0F4
    i32 horsepower; // 0x0F8
    i32 topSpeed; // 0x0FC
    i32 durability; // 0x100
    i32 mass; // 0x104
    i32 rewardFlags; // 0x108
    i32 unk_10C; // 0x10C
    f32 scoringBias; // 0x110
    f32 uiDist; // 0x114
    f32 unk_118; // 0x118
    f32 unk_11C; // 0x11C
    u32 tuningCrc; // 0x120
};

// ??_7mmVehInfo@@6B@
// vtable at 0x005B5248
check_size(mmVehInfo, 0x124);
