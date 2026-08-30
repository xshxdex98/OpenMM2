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

#include "arts7/asnode.h"

class datParser;

class aiVehicleData : public asNode
{
public:
    // ??0aiVehicleData@@QAE@XZ
    ARTS_IMPORT aiVehicleData();

    // ??1aiVehicleData@@UAE@XZ
    ARTS_IMPORT virtual ~aiVehicleData();

    // ?FileIO@aiVehicleData@@UAEXAAVdatParser@@@Z
    ARTS_IMPORT virtual void FileIO(datParser& arg1);

    // ?GetClassName@aiVehicleData@@UAEPADXZ
    ARTS_IMPORT virtual char* GetClassName();

    // ?GetDirName@aiVehicleData@@UAEPBDXZ
    ARTS_IMPORT virtual const char* GetDirName();

    // ?SetFricElas@aiVehicleData@@QAEXXZ
    ARTS_IMPORT void SetFricElas();
};

// ??_7aiVehicleData@@6B@
// vtable at 0x005B5C20
// check_size(aiVehicleData, 0xBC); // size known, members are not - cannot verify
