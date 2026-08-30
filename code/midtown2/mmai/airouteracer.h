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

#include "aivehiclephysics.h"

class aiRouteRacer : public aiVehiclePhysics
{
public:
    // ??0aiRouteRacer@@QAE@XZ
    ARTS_IMPORT aiRouteRacer();

    // ??1aiRouteRacer@@QAE@XZ
    ARTS_IMPORT ~aiRouteRacer();

    // ?DrawRouteThroughTraffic@aiRouteRacer@@QAEXXZ
    ARTS_IMPORT void DrawRouteThroughTraffic();

    // ?Finished@aiRouteRacer@@QAEHXZ
    ARTS_IMPORT i32 Finished();

    // ?Init@aiRouteRacer@@QAEXHPAD@Z
    ARTS_IMPORT void Init(i32 arg1, char* arg4);

    // ?Reset@aiRouteRacer@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?Update@aiRouteRacer@@QAEXXZ
    ARTS_IMPORT void Update();

private:
    // ?Disabled@aiRouteRacer@@AAEXXZ
    ARTS_IMPORT void Disabled();

    // ?DriveRoute@aiRouteRacer@@AAEXXZ
    ARTS_IMPORT void DriveRoute();
};

// check_size(aiRouteRacer, 0x9794); // size known, members are not - cannot verify
