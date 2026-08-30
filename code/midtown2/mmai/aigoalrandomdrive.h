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

class Vector3;
class aiRailSet;
class aiVehicleSpline;

class aiGoalRandomDrive : public aiGoal
{
public:
    // ??0aiGoalRandomDrive@@QAE@PAVaiRailSet@@PAVaiVehicleSpline@@@Z
    ARTS_IMPORT aiGoalRandomDrive(aiRailSet* arg1, aiVehicleSpline* arg2);

    // ??1aiGoalRandomDrive@@QAE@XZ
    ARTS_IMPORT ~aiGoalRandomDrive();

    // ?Init@aiGoalRandomDrive@@UAEXXZ
    ARTS_IMPORT virtual void Init();

    // ?Reset@aiGoalRandomDrive@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Update@aiGoalRandomDrive@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Context@aiGoalRandomDrive@@QAEHXZ
    ARTS_IMPORT i32 Context();

    // ?Dump@aiGoalRandomDrive@@QAEXXZ
    ARTS_IMPORT void Dump();

    // ?Priority@aiGoalRandomDrive@@QAEHXZ
    ARTS_IMPORT i32 Priority();

    // ?ReplayDebug@aiGoalRandomDrive@@QAEXXZ
    ARTS_IMPORT void ReplayDebug();

    // ?SolvePosition@aiGoalRandomDrive@@QAEXAAVVector3@@M@Z
    ARTS_IMPORT void SolvePosition(Vector3& arg1, f32 arg2);

private:
    // ?AnyVehiclesComingThisWay@aiGoalRandomDrive@@AAEHXZ
    ARTS_IMPORT i32 AnyVehiclesComingThisWay();

    // ?AvoidCollision@aiGoalRandomDrive@@AAEXPAVaiVehicleSpline@@M@Z
    ARTS_IMPORT void AvoidCollision(aiVehicleSpline* arg1, f32 arg2);

    // ?ChangeLanes@aiGoalRandomDrive@@AAEXXZ
    ARTS_IMPORT void ChangeLanes();

    // ?OkayToEnterIntersection@aiGoalRandomDrive@@AAEHM@Z
    ARTS_IMPORT i32 OkayToEnterIntersection(f32 arg1);

    // ?SolveLane@aiGoalRandomDrive@@AAEXXZ
    ARTS_IMPORT void SolveLane();

    // ?SolveRailType@aiGoalRandomDrive@@AAE_NXZ
    ARTS_IMPORT bool SolveRailType();

    // ?SolveVelocity@aiGoalRandomDrive@@AAEXXZ
    ARTS_IMPORT void SolveVelocity();

    // ?SpeedLimit@aiGoalRandomDrive@@AAEMXZ
    ARTS_IMPORT f32 SpeedLimit();

    // ?UpcomingAccident@aiGoalRandomDrive@@AAEHXZ
    ARTS_IMPORT i32 UpcomingAccident();
};

// ??_7aiGoalRandomDrive@@6B@
// vtable at 0x005B5BF4
// check_size(aiGoalRandomDrive, 0x14); // size known, members are not - cannot verify
