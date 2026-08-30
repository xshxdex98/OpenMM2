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

class Vector3;
class aiPath;
class lvlInstance;

class aiPedestrian
{
public:
    // ??0aiPedestrian@@QAE@XZ
    ARTS_IMPORT aiPedestrian();

    // ??1aiPedestrian@@QAE@XZ
    ARTS_IMPORT ~aiPedestrian();

    // ?DrawDebug@aiPedestrian@@QAEXXZ
    ARTS_IMPORT void DrawDebug();

    // ?Dump@aiPedestrian@@QAEXXZ
    ARTS_IMPORT void Dump();

    // ?Go@aiPedestrian@@QAEXXZ
    ARTS_IMPORT void Go();

    // ?Init@aiPedestrian@@QAEXPADHH@Z
    ARTS_IMPORT void Init(char* arg1, i32 arg2, i32 arg3);

    // ?QuickDump@aiPedestrian@@QAEXXZ
    ARTS_IMPORT void QuickDump();

    // ?ReplayDebug@aiPedestrian@@QAEXXZ
    ARTS_IMPORT void ReplayDebug();

    // ?Reset@aiPedestrian@@QAEXPAVaiPath@@H@Z
    ARTS_IMPORT void Reset(aiPath* arg1, i32 arg2);

    // ?Reset@aiPedestrian@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?Stop@aiPedestrian@@QAEXXZ
    ARTS_IMPORT void Stop();

    // ?Update@aiPedestrian@@QAEXXZ
    ARTS_IMPORT void Update();

private:
    // ?Accident@aiPedestrian@@AAEHXZ
    ARTS_IMPORT i32 Accident();

    // ?Anticipate@aiPedestrian@@AAEXXZ
    ARTS_IMPORT void Anticipate();

    // ?Avoid@aiPedestrian@@AAEXPAM@Z
    ARTS_IMPORT void Avoid(f32* arg1);

    // ?AvoidBanger@aiPedestrian@@AAEXPAVlvlInstance@@M@Z
    ARTS_IMPORT void AvoidBanger(lvlInstance* arg1, f32 arg2);

    // ?AvoidObstacle@aiPedestrian@@AAEXABVVector3@@M@Z
    ARTS_IMPORT void AvoidObstacle(const Vector3& arg1, f32 arg2);

    // ?AvoidPedCollision@aiPedestrian@@AAEXPAV1@M@Z
    ARTS_IMPORT void AvoidPedCollision(aiPedestrian* arg1, f32 arg2);

    // ?AvoidPlayer@aiPedestrian@@AAEXAAVVector3@@M@Z
    ARTS_IMPORT void AvoidPlayer(Vector3& arg1, f32 arg2);

    // ?CalcCurve@aiPedestrian@@AAEXHHM@Z
    ARTS_IMPORT void CalcCurve(i32 arg1, i32 arg2, f32 arg3);

    // ?ComputeCurve@aiPedestrian@@AAEXAAVVector3@@000@Z
    ARTS_IMPORT void ComputeCurve(Vector3& arg1, Vector3& arg2, Vector3& arg3, Vector3& arg4);

    // ?CrossStreet@aiPedestrian@@AAEXXZ
    ARTS_IMPORT void CrossStreet();

    // ?DetectBangerCollision@aiPedestrian@@AAEHPAPAVlvlInstance@@PAM@Z
    ARTS_IMPORT i32 DetectBangerCollision(lvlInstance** arg1, f32* arg2);

    // ?DetectPedCollision@aiPedestrian@@AAEHMPAPAV1@PAM@Z
    ARTS_IMPORT i32 DetectPedCollision(f32 arg1, aiPedestrian** arg2, f32* arg3);

    // ?DetectPlayerAnticipate@aiPedestrian@@AAEHPAM0@Z
    ARTS_IMPORT i32 DetectPlayerAnticipate(f32* arg1, f32* arg2);

    // ?DetectPlayerCollision@aiPedestrian@@AAEHPAM@Z
    ARTS_IMPORT i32 DetectPlayerCollision(f32* arg1);

    // ?DetectPlayerForwardCollision@aiPedestrian@@AAEHPAM0@Z
    ARTS_IMPORT i32 DetectPlayerForwardCollision(f32* arg1, f32* arg2);

    // ?GetRoadToLeft@aiPedestrian@@AAEPAVaiPath@@XZ
    ARTS_IMPORT aiPath* GetRoadToLeft();

    // ?GetRoadToRight@aiPedestrian@@AAEPAVaiPath@@XZ
    ARTS_IMPORT aiPath* GetRoadToRight();

    // ?PickNextRdSeg@aiPedestrian@@AAEPAVaiPath@@XZ
    ARTS_IMPORT aiPath* PickNextRdSeg();

    // ?PreCrossStreet@aiPedestrian@@AAEXXZ
    ARTS_IMPORT void PreCrossStreet();

    // ?RoadDistance@aiPedestrian@@AAEMXZ
    ARTS_IMPORT f32 RoadDistance();

    // ?SetNextRoad@aiPedestrian@@AAEPAVaiPath@@XZ
    ARTS_IMPORT aiPath* SetNextRoad();

    // ?SolvePosition@aiPedestrian@@AAEXAAVVector3@@M@Z
    ARTS_IMPORT void SolvePosition(Vector3& arg1, f32 arg2);

    // ?SolveRoadSegment@aiPedestrian@@AAEXM@Z
    ARTS_IMPORT void SolveRoadSegment(f32 arg1);

    // ?SolveTargetPoint@aiPedestrian@@AAEXM@Z
    ARTS_IMPORT void SolveTargetPoint(f32 arg1);

    // ?TimeToCollision@aiPedestrian@@AAEMM@Z
    ARTS_IMPORT f32 TimeToCollision(f32 arg1);

    // ?UpcomingAccident@aiPedestrian@@AAEHXZ
    ARTS_IMPORT i32 UpcomingAccident();

    // ?WaitCrossStreet@aiPedestrian@@AAEXXZ
    ARTS_IMPORT void WaitCrossStreet();

    // ?Wander@aiPedestrian@@AAEXXZ
    ARTS_IMPORT void Wander();
};

// check_size(aiPedestrian, 0x2); // size known, members are not - cannot verify
