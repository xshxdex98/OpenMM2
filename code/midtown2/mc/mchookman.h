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

#include "mmai/aivehiclephysics.h"

class mcHookman : public aiVehiclePhysics
{
public:
    // ??0mcHookman@@QAE@XZ
    ARTS_IMPORT mcHookman();

    // ??1mcHookman@@QAE@XZ
    ARTS_IMPORT ~mcHookman();

    // ?DrawRouteThroughTraffic@mcHookman@@QAEXXZ
    ARTS_IMPORT void DrawRouteThroughTraffic();

    // ?Init@mcHookman@@QAEXHPAD@Z
    ARTS_IMPORT void Init(i32 DrawList, char* arg4);

    // ?Reset@mcHookman@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?Update@mcHookman@@QAEXXZ
    ARTS_IMPORT void Update();

private:
    // ?DriveCircuit@mcHookman@@AAEXXZ
    ARTS_IMPORT void DriveCircuit();

    // ?DriveToHideout@mcHookman@@AAEXXZ
    ARTS_IMPORT void DriveToHideout();

    // ?ReturnToCircuit@mcHookman@@AAEXXZ
    ARTS_IMPORT void ReturnToCircuit();
};

// check_size(mcHookman, 0x9870); // size known, members are not - cannot verify
