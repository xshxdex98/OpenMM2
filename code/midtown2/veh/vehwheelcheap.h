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
#include "lvl/lvlintersection.h"
#include "lvl/lvlsegment.h"
#include "lvl/lvlsegmentinfo.h"
#include "vector7/matrix34.h"
#include "vector7/vector3.h"

class Base_vtbl;
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

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    phInertialCS* InertialCS; // 0x018
    f32 Spring; // 0x01C
    f32 Damping; // 0x020
    lvlSegment Segment; // 0x024
    lvlIntersection Intersection; // 0x04C
    lvlSegmentInfo SegmentInfo; // 0x0E8
    i32 Grounded; // 0x0F4
    Vector3 Position; // 0x0F8
    f32 Radius; // 0x104
    f32 Gap; // 0x108
    f32 field_10C; // 0x10C
    f32 field_110; // 0x110
    f32 field_114; // 0x114
    f32 RubberSpring; // 0x118
    f32 RubberDamp; // 0x11C
    f32 Limit; // 0x120
    f32 field_124; // 0x124
    Matrix34 Matrix; // 0x128
    Matrix34 Matrix2; // 0x158
};

// ??_7vehWheelCheap@@6B@
// vtable at 0x005B8790
check_size(vehWheelCheap, 0x188);
