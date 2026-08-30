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
class aiPath;
class vehCar;

class aiVehiclePlayer : public aiVehicle
{
public:
    // ??0aiVehiclePlayer@@QAE@XZ
    ARTS_IMPORT aiVehiclePlayer();

    // ??1aiVehiclePlayer@@QAE@XZ
    ARTS_IMPORT ~aiVehiclePlayer();

    // ?Attach@aiVehiclePlayer@@QAEXPAVvehCar@@@Z
    ARTS_IMPORT void Attach(vehCar* arg1);

    // ?BackBumperDistance@aiVehiclePlayer@@UAEMXZ
    ARTS_IMPORT virtual f32 BackBumperDistance();

    // ?CurrentLane@aiVehiclePlayer@@UAEHXZ
    ARTS_IMPORT virtual i32 CurrentLane();

    // ?CurrentRdVert@aiVehiclePlayer@@UAEHXZ
    ARTS_IMPORT virtual i32 CurrentRdVert();

    // ?CurrentRoadId@aiVehiclePlayer@@UAEHXZ
    ARTS_IMPORT virtual i32 CurrentRoadId();

    // ?CurrentRoadIdx@aiVehiclePlayer@@UAEHPAPAVaiPath@@QA_NPAH@Z
    ARTS_IMPORT virtual i32 CurrentRoadIdx(aiPath** arg1, bool*const arg2, i32* arg3);

    // ?DrawId@aiVehiclePlayer@@UAEXXZ
    ARTS_IMPORT virtual void DrawId();

    // ?FrontBumperDistance@aiVehiclePlayer@@UAEMXZ
    ARTS_IMPORT virtual f32 FrontBumperDistance();

    // ?GetMatrix@aiVehiclePlayer@@UAEAAVMatrix34@@XZ
    ARTS_IMPORT virtual Matrix34& GetMatrix();

    // ?LSideDistance@aiVehiclePlayer@@UAEMXZ
    ARTS_IMPORT virtual f32 LSideDistance();

    // ?Position@aiVehiclePlayer@@UAEXAAVVector3@@@Z
    ARTS_IMPORT virtual void Position(Vector3& arg1);

    // ?Reset@aiVehiclePlayer@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?RSideDistance@aiVehiclePlayer@@UAEMXZ
    ARTS_IMPORT virtual f32 RSideDistance();

    // ?Speed@aiVehiclePlayer@@UAEMXZ
    ARTS_IMPORT virtual f32 Speed();

    // ?Type@aiVehiclePlayer@@UAEHXZ
    ARTS_IMPORT virtual i32 Type();

    // ?Update@aiVehiclePlayer@@UAEXXZ
    ARTS_IMPORT virtual void Update();
};

// ??_7aiVehiclePlayer@@6B@
// vtable at 0x005B55AC
// check_size(aiVehiclePlayer, 0x30); // size known, members are not - cannot verify
