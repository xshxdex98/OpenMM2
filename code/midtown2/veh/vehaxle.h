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

#include "arts7/asnode.h"
#include "vector7/matrix34.h"

class Base_vtbl;
class datParser;
class vehCarSim;
class vehWheel;

class vehAxle : public asNode
{
public:
    // ??0vehAxle@@QAE@XZ
    ARTS_IMPORT vehAxle();

    // ??1vehAxle@@UAE@XZ
    ARTS_IMPORT virtual ~vehAxle();

    // ?Update@vehAxle@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?FileIO@vehAxle@@UAEXAAVdatParser@@@Z
    ARTS_IMPORT virtual void FileIO(datParser& arg1);

    // ?GetClassName@vehAxle@@UAEPADXZ
    ARTS_IMPORT virtual char* GetClassName();

    // ?ComputeConstants@vehAxle@@QAEXXZ
    ARTS_IMPORT void ComputeConstants();

    // ?Init@vehAxle@@QAEXPAVvehCarSim@@PBD1PAVvehWheel@@2@Z
    ARTS_IMPORT void Init(vehCarSim* arg1, const char* arg2, const char* arg3, vehWheel* arg4, vehWheel* arg5);

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    vehCarSim* CarSim; // 0x018
    Matrix34 AxlePivot; // 0x01C
    Matrix34 AxleMatrix; // 0x04C
    vehWheel* LeftWheel; // 0x07C
    vehWheel* RightWheel; // 0x080
    f32 unknown132; // 0x084
    f32 unknown136; // 0x088
    f32 ScaledTorqueCoef; // 0x08C
    f32 ScaledDampCoef; // 0x090
    f32 TorqueCoef; // 0x094
    f32 DampCoef; // 0x098
};

// ??_7vehAxle@@6B@
// vtable at 0x005B309C
check_size(vehAxle, 0x9C);
