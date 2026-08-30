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

class aiGoalRegainRail : public aiGoal
{
public:
    // ??0aiGoalRegainRail@@QAE@PAVaiRailSet@@PAVaiVehicleAmbient@@@Z
    ARTS_IMPORT aiGoalRegainRail(aiRailSet* arg1, aiVehicleAmbient* arg2);

    // ??1aiGoalRegainRail@@QAE@XZ
    ARTS_IMPORT ~aiGoalRegainRail();

    // ?Dump@aiGoalRegainRail@@QAEXXZ
    ARTS_IMPORT void Dump();

    // ?Init@aiGoalRegainRail@@UAEXXZ
    ARTS_IMPORT virtual void Init();

    // ?ReplayDebug@aiGoalRegainRail@@QAEXXZ
    ARTS_IMPORT void ReplayDebug();

    // ?Reset@aiGoalRegainRail@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Update@aiGoalRegainRail@@UAEXXZ
    ARTS_IMPORT virtual void Update();
};

// ??_7aiGoalRegainRail@@6B@
// vtable at 0x005B5BDC
// check_size(aiGoalRegainRail, 0x24); // size known, members are not - cannot verify
