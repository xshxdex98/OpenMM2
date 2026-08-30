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

class aiCityData_vtbl;

class aiCityData
{
public:
    // ??0aiCityData@@QAE@PAD0@Z
    ARTS_IMPORT aiCityData(char* DrawList, char* __formal);

    // ??1aiCityData@@UAE@XZ
    ARTS_IMPORT virtual ~aiCityData();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    unsigned __int8[48] _buffer; // 0x004
};

// ??_7aiCityData@@6B@
// vtable at 0x005B59E4
check_size(aiCityData, 0x34);
