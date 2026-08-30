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

#include "aiambientvehicleaudio.h"
#include "airailset.h"
#include "aivehicle.h"

class AudImpact;
class Matrix34;
class Vector3;
class aiPath;

class aiVehicleSpline : public aiVehicle
{
public:
    // ??0aiVehicleSpline@@QAE@XZ
    ARTS_IMPORT aiVehicleSpline();

    // ??1aiVehicleSpline@@QAE@XZ
    ARTS_IMPORT ~aiVehicleSpline();

    // ?AvoidPlayerCollision@aiVehicleSpline@@QAEXH@Z
    ARTS_IMPORT void AvoidPlayerCollision(i32 arg1);

    // ?BackBumperDistance@aiVehicleSpline@@UAEMXZ
    ARTS_IMPORT virtual f32 BackBumperDistance();

    // ?CurrentLane@aiVehicleSpline@@UAEHXZ
    ARTS_IMPORT virtual i32 CurrentLane();

    // ?CurrentRdVert@aiVehicleSpline@@UAEHXZ
    ARTS_IMPORT virtual i32 CurrentRdVert();

    // ?CurrentRoadId@aiVehicleSpline@@UAEHXZ
    ARTS_IMPORT virtual i32 CurrentRoadId();

    // ?CurrentRoadIdx@aiVehicleSpline@@UAEHPAPAVaiPath@@QA_NPAH@Z
    ARTS_IMPORT virtual i32 CurrentRoadIdx(aiPath** arg1, bool*const arg2, i32* arg3);

    // ?DetectPlayerCollision@aiVehicleSpline@@QAEHH@Z
    ARTS_IMPORT i32 DetectPlayerCollision(i32 arg1);

    // ?DetectPlayerForwardCollision@aiVehicleSpline@@QAEHH@Z
    ARTS_IMPORT i32 DetectPlayerForwardCollision(i32 arg1);

    // ?DetectPlayerTargetPtCollision@aiVehicleSpline@@QAEHH@Z
    ARTS_IMPORT i32 DetectPlayerTargetPtCollision(i32 arg1);

    // ?DetectPlayerZoneCollision@aiVehicleSpline@@QAEHH@Z
    ARTS_IMPORT i32 DetectPlayerZoneCollision(i32 arg1);

    // ?DistanceToIntersection@aiVehicleSpline@@QAEMXZ
    ARTS_IMPORT f32 DistanceToIntersection();

    // ?DistanceToVehicle@aiVehicleSpline@@QAEMPAV1@@Z
    ARTS_IMPORT f32 DistanceToVehicle(aiVehicleSpline* arg1);

    // ?DrawId@aiVehicleSpline@@UAEXXZ
    ARTS_IMPORT virtual void DrawId();

    // ?Dump@aiVehicleSpline@@QAEXXZ
    ARTS_IMPORT void Dump();

    // ?FrontBumperDistance@aiVehicleSpline@@UAEMXZ
    ARTS_IMPORT virtual f32 FrontBumperDistance();

    // ?GetAudImpactPtr@aiVehicleSpline@@UAEPAVAudImpact@@XZ
    ARTS_IMPORT virtual AudImpact* GetAudImpactPtr();

    // ?GetMatrix@aiVehicleSpline@@UAEAAVMatrix34@@XZ
    ARTS_IMPORT virtual Matrix34& GetMatrix();

    // ?Impact@aiVehicleSpline@@UAEXH@Z
    ARTS_IMPORT virtual void Impact(i32 arg1);

    // ?InAccident@aiVehicleSpline@@UAEHXZ
    ARTS_IMPORT virtual i32 InAccident();

    // ?Init@aiVehicleSpline@@QAEXPADH@Z
    ARTS_IMPORT void Init(char* arg1, i32 arg2);

    // ?IsAmbientBlockingPlayer@aiVehicleSpline@@QAEHH@Z
    ARTS_IMPORT i32 IsAmbientBlockingPlayer(i32 arg1);

    // ?IsThePlayerInFrontOfMe@aiVehicleSpline@@QAEHH@Z
    ARTS_IMPORT i32 IsThePlayerInFrontOfMe(i32 arg1);

    // ?LSideDistance@aiVehicleSpline@@UAEMXZ
    ARTS_IMPORT virtual f32 LSideDistance();

    // ?PlayHorn@aiVehicleSpline@@UAEXMM@Z
    ARTS_IMPORT virtual void PlayHorn(f32 arg1, f32 arg2);

    // ?Position@aiVehicleSpline@@UAEXAAVVector3@@@Z
    ARTS_IMPORT virtual void Position(Vector3& arg1);

    // ?ReplayDebug@aiVehicleSpline@@UAEXXZ
    ARTS_IMPORT virtual void ReplayDebug();

    // ?Reset@aiVehicleSpline@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?ResetReactTicks@aiVehicleSpline@@QAEXXZ
    ARTS_IMPORT void ResetReactTicks();

    // ?RSideDistance@aiVehicleSpline@@UAEMXZ
    ARTS_IMPORT virtual f32 RSideDistance();

    // ?Speed@aiVehicleSpline@@UAEMXZ
    ARTS_IMPORT virtual f32 Speed();

    // ?StopVoice@aiVehicleSpline@@UAEXXZ
    ARTS_IMPORT virtual void StopVoice();

    // ?TotLength@aiVehicleSpline@@QAEMXZ
    ARTS_IMPORT f32 TotLength();

    // ?Type@aiVehicleSpline@@UAEHXZ
    ARTS_IMPORT virtual i32 Type();

    // ?Update@aiVehicleSpline@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?UpdateObstacleMap@aiVehicleSpline@@QAEXXZ
    ARTS_IMPORT void UpdateObstacleMap();

    // ?WaitTime@aiVehicleSpline@@QAEHXZ
    ARTS_IMPORT i32 WaitTime();

public:
    // Members from 0x010; everything below that belongs to aiVehicle.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    aiRailSet field_10; // 0x010
    u8 field_90[32]; // 0x090
    i32 field_B0; // 0x0B0
    i32 field_B4; // 0x0B4
    i32 field_B8; // 0x0B8
    i32 field_BC; // 0x0BC
    i32 field_C0; // 0x0C0
    i32 field_C4; // 0x0C4
    i32 field_C8; // 0x0C8
    i32 field_CC; // 0x0CC
    i32 field_D0; // 0x0D0
    i32 field_D4; // 0x0D4
    u8 field_D8[24]; // 0x0D8
    u16 TotReactTicks; // 0x0F0
    u8 CurReactTicks[14]; // 0x0F2
    aiAmbientVehicleAudio field_100; // 0x100
};

// ??_7aiVehicleSpline@@6B@
// vtable at 0x005B5B1C
check_size(aiVehicleSpline, 0x18C);
