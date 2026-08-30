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

class aiVehicleActive;
class aiVehicleInstance;

class aiVehicleManager : public asNode
{
public:
    // ??0aiVehicleManager@@QAE@XZ
    ARTS_IMPORT aiVehicleManager();

    // ??1aiVehicleManager@@UAE@XZ
    ARTS_IMPORT virtual ~aiVehicleManager();

    // ?AddVehicleDataEntry@aiVehicleManager@@QAEHPAD@Z
    ARTS_IMPORT i32 AddVehicleDataEntry(char* arg1);

    // ?Attach@aiVehicleManager@@QAEPAVaiVehicleActive@@PAVaiVehicleInstance@@@Z
    ARTS_IMPORT aiVehicleActive* Attach(aiVehicleInstance* arg1);

    // ?Detach@aiVehicleManager@@QAEXPAVaiVehicleActive@@@Z
    ARTS_IMPORT void Detach(aiVehicleActive* arg1);

    // ?Init@aiVehicleManager@@QAEXPAD@Z
    ARTS_IMPORT void Init(char* arg1);

    // ?Reset@aiVehicleManager@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?SaveEntry@aiVehicleManager@@QAEXXZ
    ARTS_IMPORT void SaveEntry();

    // ?Update@aiVehicleManager@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Instance@aiVehicleManager@@2PAV1@A
    ARTS_IMPORT static aiVehicleManager* Instance;

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 field_18[6016]; // 0x018
    i32 field_1798; // 0x1798
    i32 field_179C; // 0x179C
    u8 field_17A0[90108]; // 0x17A0
    u16 field_1779C; // 0x1779C
    u8 field_1779E[2]; // 0x1779E
    i32 field_177A0; // 0x177A0
};

// ??_7aiVehicleManager@@6B@
// vtable at 0x005B599C
check_size(aiVehicleManager, 0x177A4);
