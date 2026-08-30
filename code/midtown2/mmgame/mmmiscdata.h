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

#include "mminfobase.h"
#include "mmrecord.h"

class mmMiscData : public mmInfoBase
{
public:
    // ??0mmMiscData@@QAE@XZ
    ARTS_IMPORT mmMiscData();

    // ??1mmMiscData@@UAE@XZ
    ARTS_IMPORT virtual ~mmMiscData();

    // ?Close@mmMiscData@@QAEHXZ
    ARTS_IMPORT i32 Close();

    // ?GetRecord@mmMiscData@@QAE?AVmmRecord@@HHHH@Z
    ARTS_IMPORT mmRecord GetRecord(i32 arg1, i32 arg2, i32 arg3, i32 arg4);

    // ?Init@mmMiscData@@QAEHPADHHH@Z
    ARTS_IMPORT i32 Init(char* arg1, i32 arg2, i32 arg3, i32 arg4);

    // ?NewRecord@mmMiscData@@QAEHHPADMH0HH@Z
    ARTS_IMPORT i32 NewRecord(i32 arg1, char* arg2, f32 arg3, i32 arg4, char* arg5, i32 arg6, i32 arg7);

    // ?Open@mmMiscData@@QAEHPAD@Z
    ARTS_IMPORT i32 Open(char* arg1);

    // ?Reset@mmMiscData@@QAEXXZ
    ARTS_IMPORT void Reset();

private:
    // ?GetFileOffset@mmMiscData@@AAEJHHHH@Z
    ARTS_IMPORT ilong GetFileOffset(i32 arg1, i32 arg2, i32 arg3, i32 arg4);

public:
    // Members from 0x088; everything below that belongs to mmInfoBase.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 field_88[4]; // 0x088
    u8 field_8C[4]; // 0x08C
    u8 field_90[4]; // 0x090
    u8 field_94[4]; // 0x094
    mmRecord field_98; // 0x098
    u8 field_120[128]; // 0x120
    u8 field_1A0[4]; // 0x1A0
};

// ??_7mmMiscData@@6B@
// vtable at 0x005B5208
check_size(mmMiscData, 0x1A4);
