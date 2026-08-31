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

#include "aivehicle.h"

class Matrix34;
class Vector3;
class aiObstacle_vtbl;
class aiPath;

class aiCableCar : public aiVehicle
{
public:
    // ??0aiCableCar@@QAE@XZ
    ARTS_IMPORT aiCableCar();

    // ??1aiCableCar@@QAE@XZ
    ARTS_IMPORT ~aiCableCar();

    // ?Position@aiCableCar@@UAEXAAVVector3@@@Z
    ARTS_IMPORT virtual void Position(Vector3& arg1);

    // ?Speed@aiCableCar@@UAEMXZ
    ARTS_IMPORT virtual f32 Speed();

    // ?CurrentRoadIdx@aiCableCar@@UAEHPAPAVaiPath@@QA_NPAH@Z
    ARTS_IMPORT virtual i32 CurrentRoadIdx(aiPath** arg1, bool*const arg2, i32* arg3);

    // ?CurrentRdVert@aiCableCar@@UAEHXZ
    ARTS_IMPORT virtual i32 CurrentRdVert();

    // ?Update@aiCableCar@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@aiCableCar@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Type@aiCableCar@@UAEHXZ
    ARTS_IMPORT virtual i32 Type();

    // ?GetMatrix@aiCableCar@@UAEAAVMatrix34@@XZ
    ARTS_IMPORT virtual Matrix34& GetMatrix();

    // ?FrontBumperDistance@aiCableCar@@UAEMXZ
    ARTS_IMPORT virtual f32 FrontBumperDistance();

    // ?BackBumperDistance@aiCableCar@@UAEMXZ
    ARTS_IMPORT virtual f32 BackBumperDistance();

    // ?LSideDistance@aiCableCar@@UAEMXZ
    ARTS_IMPORT virtual f32 LSideDistance();

    // ?RSideDistance@aiCableCar@@UAEMXZ
    ARTS_IMPORT virtual f32 RSideDistance();

    // ?CurrentLane@aiCableCar@@UAEHXZ
    ARTS_IMPORT virtual i32 CurrentLane();

    // ?CurrentRoadId@aiCableCar@@UAEHXZ
    ARTS_IMPORT virtual i32 CurrentRoadId();

    // ?DrawId@aiCableCar@@UAEXXZ
    ARTS_IMPORT virtual void DrawId();

    // ?DetermineSister@aiCableCar@@QAEXXZ
    ARTS_IMPORT void DetermineSister();

    // ?Init@aiCableCar@@QAEXPADHHF@Z
    ARTS_IMPORT void Init(char* arg1, i32 arg2, i32 arg3, i16 arg4);

private:
    // ?CheckForObstacles@aiCableCar@@AAE_NPAM@Z
    ARTS_IMPORT bool CheckForObstacles(f32* arg1);

    // ?DetermineNextLink@aiCableCar@@AAE_NPAVaiPath@@PAPAV2@FPAF@Z
    ARTS_IMPORT bool DetermineNextLink(aiPath* DrawList, aiPath** cull_rect, i16 uv, i16* prim);

    // ?DistanceToIntersection@aiCableCar@@AAEMXZ
    ARTS_IMPORT f32 DistanceToIntersection();

    // ?OkayToEnterIntersection@aiCableCar@@AAE_NM@Z
    ARTS_IMPORT bool OkayToEnterIntersection(f32 arg1);

    // ?SolvePositionAndOrientation@aiCableCar@@AAEXXZ
    ARTS_IMPORT void SolvePositionAndOrientation();

    // ?SolveRailType@aiCableCar@@AAEXXZ
    ARTS_IMPORT void SolveRailType();

    // ?SolveVelocity@aiCableCar@@AAEXXZ
    ARTS_IMPORT void SolveVelocity();

    // ?UpdateObstacleMap@aiCableCar@@AAEXXZ
    ARTS_IMPORT void UpdateObstacleMap();

public:
    // Members from 0x010; everything below that belongs to aiVehicle.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 pad_10[372]; // 0x010
};

// ??_7aiCableCar@@6B@
// vtable at 0x005B5604
check_size(aiCableCar, 0x184);
