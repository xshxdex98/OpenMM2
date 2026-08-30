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

class vehCar;

class aiPoliceOfficer
{
public:
    // ??0aiPoliceOfficer@@QAE@XZ
    ARTS_IMPORT aiPoliceOfficer();

    // ??1aiPoliceOfficer@@QAE@XZ
    ARTS_IMPORT ~aiPoliceOfficer();

    // ?DrawId@aiPoliceOfficer@@QAEXXZ
    ARTS_IMPORT void DrawId();

    // ?DrawRouteThroughTraffic@aiPoliceOfficer@@QAEXXZ
    ARTS_IMPORT void DrawRouteThroughTraffic();

    // ?Init@aiPoliceOfficer@@QAEXH@Z
    ARTS_IMPORT void Init(i32 arg1);

    // ?InPersuit@aiPoliceOfficer@@QAEHXZ
    ARTS_IMPORT i32 InPersuit();

    // ?PerpEscapes@aiPoliceOfficer@@QAEX_N@Z
    ARTS_IMPORT void PerpEscapes(bool arg1);

    // ?Reset@aiPoliceOfficer@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?StartSiren@aiPoliceOfficer@@QAEXXZ
    ARTS_IMPORT void StartSiren();

    // ?StopSiren@aiPoliceOfficer@@QAEXXZ
    ARTS_IMPORT void StopSiren();

    // ?Update@aiPoliceOfficer@@QAEXXZ
    ARTS_IMPORT void Update();

private:
    // ?ApprehendPerpetrator@aiPoliceOfficer@@AAEXXZ
    ARTS_IMPORT void ApprehendPerpetrator();

    // ?Barricade@aiPoliceOfficer@@AAEXXZ
    ARTS_IMPORT void Barricade();

    // ?Block@aiPoliceOfficer@@AAEXXZ
    ARTS_IMPORT void Block();

    // ?Collision@aiPoliceOfficer@@AAEHPAVvehCar@@@Z
    ARTS_IMPORT i32 Collision(vehCar* arg1);

    // ?DetectPerpetrator@aiPoliceOfficer@@AAEXXZ
    ARTS_IMPORT void DetectPerpetrator();

    // ?FollowPerpetrator@aiPoliceOfficer@@AAEXXZ
    ARTS_IMPORT void FollowPerpetrator();

    // ?Fov@aiPoliceOfficer@@AAEHPAVvehCar@@@Z
    ARTS_IMPORT i32 Fov(vehCar* arg1);

    // ?HitMe@aiPoliceOfficer@@AAEHPAVvehCar@@@Z
    ARTS_IMPORT i32 HitMe(vehCar* arg1);

    // ?IsPerpACop@aiPoliceOfficer@@AAEHPAVvehCar@@@Z
    ARTS_IMPORT i32 IsPerpACop(vehCar* arg1);

    // ?OffRoad@aiPoliceOfficer@@AAEHPAVvehCar@@@Z
    ARTS_IMPORT i32 OffRoad(vehCar* arg1);

    // ?Push@aiPoliceOfficer@@AAEXXZ
    ARTS_IMPORT void Push();

    // ?Speeding@aiPoliceOfficer@@AAEHPAVvehCar@@@Z
    ARTS_IMPORT i32 Speeding(vehCar* arg1);

    // ?Stopped@aiPoliceOfficer@@AAEHPAVvehCar@@@Z
    ARTS_IMPORT i32 Stopped(vehCar* arg1);
};

// check_size(aiPoliceOfficer, 0x9870); // size known, members are not - cannot verify
