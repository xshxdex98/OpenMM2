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

class mmCityInfo
{
public:
    // ??0mmCityInfo@@QAE@XZ
    ARTS_EXPORT mmCityInfo();

    // ??1mmCityInfo@@UAE@XZ
    ARTS_EXPORT virtual ~mmCityInfo();

    // ?Load@mmCityInfo@@QAEHPAD@Z
    ARTS_IMPORT i32 Load(char* arg1);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i8 localisedName[40]; // 0x004
    i8 mapName[40]; // 0x02C
    i8 raceDir[40]; // 0x054
    i8* blitzNames; // 0x07C
    i8* checkpointNames; // 0x080
    i8* circuitNames; // 0x084
    i32 isValid; // 0x088
    i32 blitzCount; // 0x08C
    i32 checkpointCount; // 0x090
    i32 circuitCount; // 0x094
    i32 _unk[2]; // 0x098
};

// ??_7mmCityInfo@@6B@
// vtable at 0x005B524C
check_size(mmCityInfo, 0xA0);
