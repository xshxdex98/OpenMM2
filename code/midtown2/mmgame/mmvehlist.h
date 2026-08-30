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

class mmVehInfo;

class mmVehList
{
public:
    // ??0mmVehList@@QAE@XZ
    ARTS_IMPORT mmVehList();

    // ??1mmVehList@@UAE@XZ
    ARTS_IMPORT virtual ~mmVehList();

    // ?GetVehicleID@mmVehList@@QAEHPAD@Z
    ARTS_IMPORT i32 GetVehicleID(char* renderer);

    // ?GetVehicleInfo@mmVehList@@QAEPAVmmVehInfo@@H@Z
    ARTS_IMPORT mmVehInfo* GetVehicleInfo(i32 arg1);

    // ?GetVehicleInfo@mmVehList@@QAEPAVmmVehInfo@@PAD@Z
    ARTS_IMPORT mmVehInfo* GetVehicleInfo(char* arg1);

    // ?Init@mmVehList@@QAEXH@Z
    ARTS_IMPORT void Init(i32 arg1);

    // ?Load@mmVehList@@QAEXPAD@Z
    ARTS_IMPORT void Load(char* arg1);

    // ?LoadAll@mmVehList@@QAEXXZ
    ARTS_IMPORT void LoadAll();

    // ?Print@mmVehList@@QAEXXZ
    ARTS_IMPORT void Print();

    // ?SetDefaultVehicle@mmVehList@@QAEXPAD@Z
    ARTS_IMPORT void SetDefaultVehicle(char* arg1);
};

// ??_7mmVehList@@6B@
// vtable at 0x005B5204
// check_size(mmVehList, 0x10); // size known, members are not - cannot verify
