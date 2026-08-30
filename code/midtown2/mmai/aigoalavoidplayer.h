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

#include "aigoal.h"

class aiRailSet;
class aiVehicleAmbient;

class aiGoalAvoidPlayer : public aiGoal
{
public:
    // ??0aiGoalAvoidPlayer@@QAE@PAVaiRailSet@@PAVaiVehicleAmbient@@@Z
    ARTS_IMPORT aiGoalAvoidPlayer(aiRailSet* arg1, aiVehicleAmbient* arg2);

    // ??1aiGoalAvoidPlayer@@QAE@XZ
    ARTS_IMPORT ~aiGoalAvoidPlayer();

    // ?Context@aiGoalAvoidPlayer@@QAEHXZ
    ARTS_IMPORT i32 Context();

    // ?Dump@aiGoalAvoidPlayer@@QAEXXZ
    ARTS_IMPORT void Dump();

    // ?Init@aiGoalAvoidPlayer@@UAEXXZ
    ARTS_IMPORT virtual void Init();

    // ?Priority@aiGoalAvoidPlayer@@QAEHXZ
    ARTS_IMPORT i32 Priority();

    // ?ReplayDebug@aiGoalAvoidPlayer@@QAEXXZ
    ARTS_IMPORT void ReplayDebug();

    // ?Reset@aiGoalAvoidPlayer@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Update@aiGoalAvoidPlayer@@UAEXXZ
    ARTS_IMPORT virtual void Update();

private:
    // ?AvoidPlayer@aiGoalAvoidPlayer@@AAEXXZ
    ARTS_IMPORT void AvoidPlayer();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i32 field_4; // 0x004
    u8 field_8[16]; // 0x008
    i32 field_18; // 0x018
    i32 field_1C; // 0x01C
};

// ??_7aiGoalAvoidPlayer@@6B@
// vtable at 0x005B5BB8
check_size(aiGoalAvoidPlayer, 0x20);
