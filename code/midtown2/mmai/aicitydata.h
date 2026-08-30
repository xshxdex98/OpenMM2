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

class aiCityData
{
public:
    // ??0aiCityData@@QAE@PAD0@Z
    ARTS_IMPORT aiCityData(char* arg1, char* arg2);

    // ??1aiCityData@@UAE@XZ
    ARTS_IMPORT virtual ~aiCityData();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 field_4; // 0x004
    u8 field_5[3]; // 0x005
    f32 field_8; // 0x008
    i32 field_C; // 0x00C
    u16 field_10; // 0x010
    u8 field_12[2]; // 0x012
    i32 field_14; // 0x014
    i32 field_18; // 0x018
    i32 field_1C; // 0x01C
    i32 field_20; // 0x020
    i32 field_24; // 0x024
    i32 field_28; // 0x028
    void* field_2C; // 0x02C
    i32 field_30; // 0x030
};

// ??_7aiCityData@@6B@
// vtable at 0x005B59E4
check_size(aiCityData, 0x34);
