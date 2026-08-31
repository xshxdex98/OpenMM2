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
class vehCar;

class aiVehiclePlayer : public aiVehicle
{
public:
    // ??0aiVehiclePlayer@@QAE@XZ
    ARTS_IMPORT aiVehiclePlayer();

    // ??1aiVehiclePlayer@@QAE@XZ
    ARTS_IMPORT ~aiVehiclePlayer();

    // ?Position@aiVehiclePlayer@@UAEXAAVVector3@@@Z
    ARTS_IMPORT virtual void Position(Vector3& arg1);

    // ?Speed@aiVehiclePlayer@@UAEMXZ
    ARTS_IMPORT virtual f32 Speed();

    // ?CurrentRoadIdx@aiVehiclePlayer@@UAEHPAPAVaiPath@@QA_NPAH@Z
    ARTS_IMPORT virtual i32 CurrentRoadIdx(aiPath** arg1, bool*const arg2, i32* arg3);

    // ?CurrentRdVert@aiVehiclePlayer@@UAEHXZ
    ARTS_IMPORT virtual i32 CurrentRdVert();

    // ?Update@aiVehiclePlayer@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@aiVehiclePlayer@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Type@aiVehiclePlayer@@UAEHXZ
    ARTS_EXPORT virtual i32 Type();

    // ?GetMatrix@aiVehiclePlayer@@UAEAAVMatrix34@@XZ
    ARTS_IMPORT virtual Matrix34& GetMatrix();

    // ?FrontBumperDistance@aiVehiclePlayer@@UAEMXZ
    ARTS_IMPORT virtual f32 FrontBumperDistance();

    // ?BackBumperDistance@aiVehiclePlayer@@UAEMXZ
    ARTS_IMPORT virtual f32 BackBumperDistance();

    // ?LSideDistance@aiVehiclePlayer@@UAEMXZ
    ARTS_IMPORT virtual f32 LSideDistance();

    // ?RSideDistance@aiVehiclePlayer@@UAEMXZ
    ARTS_IMPORT virtual f32 RSideDistance();

    // ?CurrentLane@aiVehiclePlayer@@UAEHXZ
    ARTS_EXPORT virtual i32 CurrentLane();

    // ?CurrentRoadId@aiVehiclePlayer@@UAEHXZ
    ARTS_IMPORT virtual i32 CurrentRoadId();

    // ?DrawId@aiVehiclePlayer@@UAEXXZ
    ARTS_EXPORT virtual void DrawId();

    // ?Attach@aiVehiclePlayer@@QAEXPAVvehCar@@@Z
    ARTS_IMPORT void Attach(vehCar* arg1);

public:
    // Members from 0x010; everything below that belongs to aiVehicle.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 pad_10[32]; // 0x010
};

// ??_7aiVehiclePlayer@@6B@
// vtable at 0x005B55AC
check_size(aiVehiclePlayer, 0x30);
