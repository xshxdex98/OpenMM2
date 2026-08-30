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

class mmCityInfo;

class mmCityList
{
public:
    // ??0mmCityList@@QAE@XZ
    ARTS_IMPORT mmCityList();

    // ??1mmCityList@@UAE@XZ
    ARTS_IMPORT virtual ~mmCityList();

    // ?GetCityID@mmCityList@@QAEHPAD@Z
    ARTS_IMPORT i32 GetCityID(char* arg1);

    // ?GetCityInfo@mmCityList@@QAEPAVmmCityInfo@@H@Z
    ARTS_IMPORT mmCityInfo* GetCityInfo(i32 arg1);

    // ?GetCityInfo@mmCityList@@QAEPAVmmCityInfo@@PAD@Z
    ARTS_IMPORT mmCityInfo* GetCityInfo(char* arg1);

    // ?GetCurrentCity@mmCityList@@QAEPAVmmCityInfo@@XZ
    ARTS_IMPORT mmCityInfo* GetCurrentCity();

    // ?Init@mmCityList@@QAEXH@Z
    ARTS_IMPORT void Init(i32 arg1);

    // ?Load@mmCityList@@QAEXPAD@Z
    ARTS_IMPORT void Load(char* arg1);

    // ?LoadAll@mmCityList@@QAEXXZ
    ARTS_IMPORT void LoadAll();

    // ?Print@mmCityList@@QAEXXZ
    ARTS_IMPORT void Print();

    // ?SetCurrentCity@mmCityList@@QAEXH@Z
    ARTS_IMPORT void SetCurrentCity(i32 arg1);

    // ?SetCurrentCity@mmCityList@@QAEXPAD@Z
    ARTS_IMPORT void SetCurrentCity(char* arg1);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i32 field_4; // 0x004
    i32 field_8; // 0x008
    i32 field_C; // 0x00C
};

// ??_7mmCityList@@6B@
// vtable at 0x005B5200
check_size(mmCityList, 0x10);
