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
class vehCarSim;
class vehWheel;

class vehSuspension : public asNode
{
public:
    // ??0vehSuspension@@QAE@XZ
    ARTS_IMPORT vehSuspension();

    // ??1vehSuspension@@UAE@XZ
    ARTS_IMPORT virtual ~vehSuspension();

    // ?Update@vehSuspension@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?GetClassName@vehSuspension@@UAEPADXZ
    ARTS_IMPORT virtual char* GetClassName();

    // ?Copy@vehSuspension@@QAEXABV1@@Z
    ARTS_IMPORT void Copy(const vehSuspension& arg1);

    // ?Init@vehSuspension@@QAEXPAVvehCarSim@@PBD1PAVvehWheel@@@Z
    ARTS_IMPORT void Init(vehCarSim* arg1, const char* arg2, const char* arg3, vehWheel* arg4);

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    Matrix34 SuspensionPivot; // 0x018
    Matrix34 SuspensionMatrix; // 0x048
    vehCarSim* CarSim; // 0x078
    vehWheel* Wheel; // 0x07C
    f32 unknown128; // 0x080
    i32 Mode; // 0x084
};

// ??_7vehSuspension@@6B@
// vtable at 0x005B3068
check_size(vehSuspension, 0x88);
