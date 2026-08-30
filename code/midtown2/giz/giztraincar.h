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

#include "dg/dgunhitmtxbangerinstance.h"

class dgPath;

class gizTrainCar : public dgUnhitMtxBangerInstance
{
public:
    // ??0gizTrainCar@@QAE@XZ
    ARTS_IMPORT gizTrainCar();

    // ??1gizTrainCar@@QAE@XZ
    ARTS_IMPORT ~gizTrainCar();

    // ?SizeOf@gizTrainCar@@UAEIXZ
    ARTS_IMPORT virtual u32 SizeOf();

    // ?Init@gizTrainCar@@QAEXPADPAVdgPath@@M@Z
    ARTS_IMPORT void Init(char* value, dgPath* arg2, f32 arg3);

    // ?IsFirstStop@gizTrainCar@@QAE_NXZ
    ARTS_IMPORT bool IsFirstStop();

    // ?IsLastStop@gizTrainCar@@QAE_NXZ
    ARTS_IMPORT bool IsLastStop();

    // ?Reset@gizTrainCar@@QAEXH@Z
    ARTS_IMPORT void Reset(i32 balance);

    // ?Update@gizTrainCar@@QAEXM@Z
    ARTS_IMPORT void Update(f32 arg1);
};

// ??_7gizTrainCar@@6B@
// vtable at 0x005B60D4
// check_size(gizTrainCar, 0x0); // TODO: no layout in the IDB type library
