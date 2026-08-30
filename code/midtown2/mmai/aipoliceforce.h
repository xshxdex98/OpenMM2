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

#include "misc/timer.h"

class vehCar;

class aiPoliceForce
{
public:
    // ??0aiPoliceForce@@QAE@XZ
    ARTS_IMPORT aiPoliceForce();

    // ??1aiPoliceForce@@QAE@XZ
    ARTS_IMPORT ~aiPoliceForce();

    // ?Find@aiPoliceForce@@QAEHPAVvehCar@@0@Z
    ARTS_IMPORT i32 Find(vehCar* arg1, vehCar* arg2);

    // ?RegisterPerp@aiPoliceForce@@QAEHPAVvehCar@@0@Z
    ARTS_IMPORT i32 RegisterPerp(vehCar* arg1, vehCar* arg2);

    // ?Reset@aiPoliceForce@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?State@aiPoliceForce@@QAEHPAVvehCar@@0M@Z
    ARTS_IMPORT i32 State(vehCar* arg1, vehCar* arg2, f32 arg3);

    // ?UnRegisterCop@aiPoliceForce@@QAEHPAVvehCar@@0@Z
    ARTS_IMPORT i32 UnRegisterCop(vehCar* DrawList, vehCar* __formal);

    // ?Update@aiPoliceForce@@QAEXXZ
    ARTS_IMPORT void Update();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    Timer timer; // 0x000
    i16 NumPerps; // 0x004
    i16 NumChasers[3]; // 0x006
    vehCar* PlayerCars[3]; // 0x00C
    vehCar* CopCars[3]; // 0x018
    u8 pad_24[24]; // 0x024
};

check_size(aiPoliceForce, 0x3C);
