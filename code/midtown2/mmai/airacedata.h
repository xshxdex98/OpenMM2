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

struct dgStatePack;

class aiRaceData
{
public:
    // ??0aiRaceData@@QAE@PAD0PBUdgStatePack@@@Z
    ARTS_IMPORT aiRaceData(char* arg1, char* arg2, const dgStatePack* arg3);

    // ??1aiRaceData@@UAE@XZ
    ARTS_IMPORT virtual ~aiRaceData();

    // ?SaveData@aiRaceData@@QAEXXZ
    ARTS_IMPORT void SaveData();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 field_4[144]; // 0x004
    u8 field_94; // 0x094
    u8 field_95[3]; // 0x095
    f32 field_98; // 0x098
    u16 field_9C; // 0x09C
    u16 field_9E; // 0x09E
    u16 field_A0; // 0x0A0
    u16 field_A2; // 0x0A2
    u16 field_A4; // 0x0A4
    u8 field_A6[2]; // 0x0A6
    i32 field_A8; // 0x0A8
    i32 field_AC; // 0x0AC
    i32 field_B0; // 0x0B0
    i32 field_B4; // 0x0B4
    i32 field_B8; // 0x0B8
    i32 field_BC; // 0x0BC
    i32 field_C0; // 0x0C0
    i32 field_C4; // 0x0C4
};

check_size(aiRaceData, 0xC8);
