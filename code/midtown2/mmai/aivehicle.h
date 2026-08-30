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

class aiVehicle : public aiObstacle
{
public:
    // ??0aiVehicle@@QAE@XZ
    ARTS_IMPORT aiVehicle();

    // ??1aiVehicle@@QAE@XZ
    ARTS_IMPORT ~aiVehicle();

    // ?Init@aiVehicle@@QAEXH@Z
    ARTS_IMPORT void Init(i32 arg1);

    // ?IsBlockingTarget@aiVehicle@@UAEMABVVector3@@0MM@Z
    ARTS_IMPORT virtual f32 IsBlockingTarget(const Vector3& arg1, const Vector3& arg2, f32 arg3, f32 arg4);

    // ?PreAvoid@aiVehicle@@UAEXABVVector3@@0MAAV2@1@Z
    ARTS_IMPORT virtual void PreAvoid(const Vector3& arg1, const Vector3& arg2, f32 arg3, Vector3& arg4, Vector3& arg5);

    // ?ReplayDebug@aiVehicle@@UAEXXZ
    ARTS_IMPORT virtual void ReplayDebug();

    // ?Reset@aiVehicle@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Update@aiVehicle@@UAEXXZ
    ARTS_IMPORT virtual void Update();

public:
    // Members from 0x008; everything below that belongs to aiObstacle.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u16 VehicleId; // 0x008
    u16 field_A; // 0x00A
    aiVehicle* Next; // 0x00C
};

// ??_7aiVehicle@@6B@
// vtable at 0x005B59E8
check_size(aiVehicle, 0x10);
