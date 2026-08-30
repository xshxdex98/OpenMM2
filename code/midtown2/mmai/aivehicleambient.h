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

#include "aivehiclespline.h"

class aiVehicleAmbient : public aiVehicleSpline
{
public:
    // ??0aiVehicleAmbient@@QAE@XZ
    ARTS_IMPORT aiVehicleAmbient();

    // ??1aiVehicleAmbient@@QAE@XZ
    ARTS_IMPORT ~aiVehicleAmbient();

    // ?Update@aiVehicleAmbient@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@aiVehicleAmbient@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Type@aiVehicleAmbient@@UAEHXZ
    ARTS_IMPORT virtual i32 Type();

    // ?DrawId@aiVehicleAmbient@@UAEXXZ
    ARTS_IMPORT virtual void DrawId();

    // ?ReplayDebug@aiVehicleAmbient@@UAEXXZ
    ARTS_IMPORT virtual void ReplayDebug();

    // ?Impact@aiVehicleAmbient@@UAEXH@Z
    ARTS_IMPORT virtual void Impact(i32 arg1);

    // ?DrawBBox@aiVehicleAmbient@@QAEXF@Z
    ARTS_IMPORT void DrawBBox(i16 arg1);

    // ?Dump@aiVehicleAmbient@@QAEXXZ
    ARTS_IMPORT void Dump();

    // ?Init@aiVehicleAmbient@@QAEXPADH@Z
    ARTS_IMPORT void Init(char* DrawList, i32 __formal);

public:
    // Members from 0x18C; everything below that belongs to aiVehicleSpline.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 field_18C[4]; // 0x18C
    i32 field_190; // 0x190
    i32 field_194; // 0x194
    i32 field_198; // 0x198
    i32 field_19C; // 0x19C
};

// ??_7aiVehicleAmbient@@6B@
// vtable at 0x005B589C
check_size(aiVehicleAmbient, 0x1A0);
