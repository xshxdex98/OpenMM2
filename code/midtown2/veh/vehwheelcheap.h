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

class Vector3;
class aiVehicleData;
class phInertialCS;

class vehWheelCheap : public asNode
{
public:
    // ??0vehWheelCheap@@QAE@XZ
    ARTS_IMPORT vehWheelCheap();

    // ??1vehWheelCheap@@UAE@XZ
    ARTS_IMPORT virtual ~vehWheelCheap();

    // ?Update@vehWheelCheap@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@vehWheelCheap@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Init@vehWheelCheap@@QAEXPAVVector3@@PAVaiVehicleData@@PAVphInertialCS@@@Z
    ARTS_IMPORT void Init(Vector3* arg1, aiVehicleData* arg2, phInertialCS* arg3);
};

// ??_7vehWheelCheap@@6B@
// vtable at 0x005B8790
// check_size(vehWheelCheap, 0x18); // size known, members are not - cannot verify
