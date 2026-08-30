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

#include "arts7/asbirthrule.h"
#include "arts7/asparticles.h"
#include "dg/dgphysentity.h"
#include "ph/phinertialcs.h"
#include "ph/phsleep.h"

class Base_vtbl;
struct ImpactCBData;
class aiVehicleInstance;
class datCallback;
class lvlInstance;

class aiVehicleActive : public dgPhysEntity
{
public:
    // ??0aiVehicleActive@@QAE@XZ
    ARTS_IMPORT aiVehicleActive();

    // ??1aiVehicleActive@@UAE@XZ
    ARTS_IMPORT virtual ~aiVehicleActive();

    // ?Update@aiVehicleActive@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?PostUpdate@aiVehicleActive@@UAEXXZ
    ARTS_IMPORT virtual void PostUpdate();

    // ?GetICS@aiVehicleActive@@UAEPAVphInertialCS@@XZ
    ARTS_IMPORT virtual phInertialCS* GetICS();

    // ?GetInst@aiVehicleActive@@UAEPAVlvlInstance@@XZ
    ARTS_IMPORT virtual lvlInstance* GetInst();

    // ?DetachMe@aiVehicleActive@@UAEXXZ
    ARTS_IMPORT virtual void DetachMe();

    // ?RequiresTerrainCollision@aiVehicleActive@@UAE_NXZ
    ARTS_IMPORT virtual bool RequiresTerrainCollision();

    // ?Attach@aiVehicleActive@@QAEXPAVaiVehicleInstance@@@Z
    ARTS_IMPORT void Attach(aiVehicleInstance* arg1);

    // ?BottomedOut@aiVehicleActive@@QAEHXZ
    ARTS_IMPORT i32 BottomedOut();

    // ?Detach@aiVehicleActive@@QAEXXZ
    ARTS_IMPORT void Detach();

    // ?Impact@aiVehicleActive@@SAXPAV1@PAUImpactCBData@@@Z
    ARTS_IMPORT static void Impact(aiVehicleActive* arg1, ImpactCBData* arg2);

    // ?Reset@aiVehicleActive@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?UpdateDamage@aiVehicleActive@@QAEXXZ
    ARTS_IMPORT void UpdateDamage();

public:
    // Members from 0x258; everything below that belongs to dgPhysEntity.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    f32 CurDamage; // 0x2C8
    f32 MaxDamage; // 0x2CC
    i32 field_2D0; // 0x2D0
    lvlInstance* Instance; // 0x2D4
    phInertialCS ICS; // 0x2D8
    u8[432] pad_2DC; // 0x2DC
    phSleep Sleep; // 0x48C
    u8[76] pad_490; // 0x490
    vehWeelCheap WheelFrontLeft; // 0x4DC
    u8[388] pad_4E0; // 0x4E0
    vehWeelCheap WheelFrontRight; // 0x664
    u8[388] pad_668; // 0x668
    vehWeelCheap WheelBackLeft; // 0x7EC
    u8[388] pad_7F0; // 0x7F0
    vehWeelCheap WheelBackRight; // 0x974
    u8[388] pad_978; // 0x978
};

// ??_7aiVehicleActive@@6B@
// vtable at 0x005B5974
check_size(aiVehicleActive, 0xAFC);
