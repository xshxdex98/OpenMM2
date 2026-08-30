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
class aiVehicleSpline;

class aiGoalCollision : public aiGoal
{
public:
    // ??0aiGoalCollision@@QAE@PAVaiRailSet@@PAVaiVehicleSpline@@@Z
    ARTS_IMPORT aiGoalCollision(aiRailSet* arg1, aiVehicleSpline* arg2);

    // ??1aiGoalCollision@@QAE@XZ
    ARTS_IMPORT ~aiGoalCollision();

    // ?Context@aiGoalCollision@@QAEHXZ
    ARTS_IMPORT i32 Context();

    // ?Init@aiGoalCollision@@UAEXXZ
    ARTS_IMPORT virtual void Init();

    // ?Priority@aiGoalCollision@@QAEHXZ
    ARTS_IMPORT i32 Priority();

    // ?Reset@aiGoalCollision@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Update@aiGoalCollision@@UAEXXZ
    ARTS_IMPORT virtual void Update();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i32 field_4; // 0x004
    i32 field_8; // 0x008
    i32 field_C; // 0x00C
};

// ??_7aiGoalCollision@@6B@
// vtable at 0x005B5C14
check_size(aiGoalCollision, 0x10);
