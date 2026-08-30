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

#include "aisubwayaudio.h"
#include "aivehicle.h"

class Matrix34;
class Vector3;
class aiPath;

class aiSubway : public aiVehicle
{
public:
    // ??0aiSubway@@QAE@XZ
    ARTS_IMPORT aiSubway();

    // ??1aiSubway@@QAE@XZ
    ARTS_IMPORT ~aiSubway();

    // ?BackBumperDistance@aiSubway@@UAEMXZ
    ARTS_IMPORT virtual f32 BackBumperDistance();

    // ?CurrentLane@aiSubway@@UAEHXZ
    ARTS_IMPORT virtual i32 CurrentLane();

    // ?CurrentRdVert@aiSubway@@UAEHXZ
    ARTS_IMPORT virtual i32 CurrentRdVert();

    // ?CurrentRoadId@aiSubway@@UAEHXZ
    ARTS_IMPORT virtual i32 CurrentRoadId();

    // ?CurrentRoadIdx@aiSubway@@UAEHPAPAVaiPath@@QA_NPAH@Z
    ARTS_IMPORT virtual i32 CurrentRoadIdx(aiPath** arg1, bool*const arg2, i32* arg3);

    // ?DrawId@aiSubway@@UAEXXZ
    ARTS_IMPORT virtual void DrawId();

    // ?FrontBumperDistance@aiSubway@@UAEMXZ
    ARTS_IMPORT virtual f32 FrontBumperDistance();

    // ?GetMatrix@aiSubway@@UAEAAVMatrix34@@XZ
    ARTS_IMPORT virtual Matrix34& GetMatrix();

    // ?Init@aiSubway@@QAEXPADHHHHHPAV1@1@Z
    ARTS_IMPORT void Init(char* arg1, i32 arg2, i32 arg3, i32 arg4, i32 arg5, i32 arg6, aiSubway* arg7, aiSubway* arg8);

    // ?LSideDistance@aiSubway@@UAEMXZ
    ARTS_IMPORT virtual f32 LSideDistance();

    // ?Position@aiSubway@@UAEXAAVVector3@@@Z
    ARTS_IMPORT virtual void Position(Vector3& arg1);

    // ?Reset@aiSubway@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?RSideDistance@aiSubway@@UAEMXZ
    ARTS_IMPORT virtual f32 RSideDistance();

    // ?Speed@aiSubway@@UAEMXZ
    ARTS_IMPORT virtual f32 Speed();

    // ?Type@aiSubway@@UAEHXZ
    ARTS_IMPORT virtual i32 Type();

    // ?Update@aiSubway@@UAEXXZ
    ARTS_IMPORT virtual void Update();

private:
    // ?ComputeXZCurve@aiSubway@@AAEXAAVVector3@@000@Z
    ARTS_IMPORT void ComputeXZCurve(Vector3& arg1, Vector3& arg2, Vector3& arg3, Vector3& arg4);

    // ?DetermineNextLink@aiSubway@@AAE_NPAVaiPath@@PAPAV2@@Z
    ARTS_IMPORT bool DetermineNextLink(aiPath* arg1, aiPath** arg2);

    // ?DistanceToIntersection@aiSubway@@AAEMXZ
    ARTS_IMPORT f32 DistanceToIntersection();

    // ?OkayToEnterIntersection@aiSubway@@AAE_NM@Z
    ARTS_IMPORT bool OkayToEnterIntersection(f32 arg1);

    // ?ReverseDirection@aiSubway@@AAEXXZ
    ARTS_IMPORT void ReverseDirection();

    // ?SolvePositionAndOrientation@aiSubway@@AAEXXZ
    ARTS_IMPORT void SolvePositionAndOrientation();

    // ?SolveRailType@aiSubway@@AAEXXZ
    ARTS_IMPORT void SolveRailType();

    // ?SolveVelocity@aiSubway@@AAEXXZ
    ARTS_IMPORT void SolveVelocity();

    // ?SolveXZCurve@aiSubway@@AAEXAAVVector3@@0M@Z
    ARTS_IMPORT void SolveXZCurve(Vector3& arg1, Vector3& arg2, f32 arg3);

    // ?UpdateObstacleMap@aiSubway@@AAEXXZ
    ARTS_IMPORT void UpdateObstacleMap();

public:
    // Members from 0x010; everything below that belongs to aiVehicle.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i32 field_10; // 0x010
    i32 field_14; // 0x014
    u8 field_18[12]; // 0x018
    i32 field_24; // 0x024
    i32 field_28; // 0x028
    i32 field_2C; // 0x02C
    i32 field_30; // 0x030
    i32 field_34; // 0x034
    i32 field_38; // 0x038
    i32 field_3C; // 0x03C
    i32 field_40; // 0x040
    i32 field_44; // 0x044
    u8 field_48[116]; // 0x048
    Timer field_BC; // 0x0BC
    u8 field_C0[8]; // 0x0C0
    aiSubwayAudio field_C8; // 0x0C8
};

// ??_7aiSubway@@6B@
// vtable at 0x005B5744
check_size(aiSubway, 0x14C);
