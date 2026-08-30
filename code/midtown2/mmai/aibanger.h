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

#include "aiobstacle.h"

class Vector3;
class aiPath;
class lvlInstance;

class aiBanger : public aiObstacle
{
public:
    // ??0aiBanger@@QAE@PAVlvlInstance@@HH@Z
    ARTS_IMPORT aiBanger(lvlInstance* arg1, i32 arg2, i32 arg3);

    // ??1aiBanger@@QAE@XZ
    ARTS_IMPORT ~aiBanger();

    // ?BreakThreshold@aiBanger@@UAEMXZ
    ARTS_IMPORT virtual f32 BreakThreshold();

    // ?CurrentRdVert@aiBanger@@UAEHXZ
    ARTS_IMPORT virtual i32 CurrentRdVert();

    // ?CurrentRoadIdx@aiBanger@@UAEHPAPAVaiPath@@QA_NPAH@Z
    ARTS_IMPORT virtual i32 CurrentRoadIdx(aiPath** arg1, bool*const arg2, i32* arg3);

    // ?Draw@aiBanger@@QAEXXZ
    ARTS_IMPORT void Draw();

    // ?Drivable@aiBanger@@UAEHXZ
    ARTS_IMPORT virtual i32 Drivable();

    // ?IsBlockingTarget@aiBanger@@UAEMABVVector3@@0MM@Z
    ARTS_IMPORT virtual f32 IsBlockingTarget(const Vector3& arg1, const Vector3& arg2, f32 arg3, f32 arg4);

    // ?Position@aiBanger@@UAEXAAVVector3@@@Z
    ARTS_IMPORT virtual void Position(Vector3& arg1);

    // ?PreAvoid@aiBanger@@UAEXABVVector3@@0MAAV2@1@Z
    ARTS_IMPORT virtual void PreAvoid(const Vector3& arg1, const Vector3& arg2, f32 arg3, Vector3& arg4, Vector3& arg5);

    // ?Radius@aiBanger@@QAEMXZ
    ARTS_IMPORT f32 Radius();

    // ?Speed@aiBanger@@UAEMXZ
    ARTS_IMPORT virtual f32 Speed();

public:
    // Members from 0x008; everything below that belongs to aiObstacle.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    lvlInstance* Instance; // 0x008
    u16 IntersectionId; // 0x00C
    u16 Type; // 0x00E
};

// ??_7aiBanger@@6B@
// vtable at 0x005B5B84
check_size(aiBanger, 0x10);
