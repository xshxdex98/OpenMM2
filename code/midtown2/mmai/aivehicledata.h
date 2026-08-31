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
#include "vector7/vector3.h"

class Base_vtbl;
class datParser;
class dgBoundBox;

class aiVehicleData : public asNode
{
public:
    // ??0aiVehicleData@@QAE@XZ
    ARTS_IMPORT aiVehicleData();

    // ??1aiVehicleData@@UAE@XZ
    ARTS_IMPORT virtual ~aiVehicleData();

    // ?FileIO@aiVehicleData@@UAEXAAVdatParser@@@Z
    ARTS_IMPORT virtual void FileIO(datParser& child);

    // ?GetClassName@aiVehicleData@@UAEPADXZ
    ARTS_IMPORT virtual char* GetClassName();

    // ?GetDirName@aiVehicleData@@UAEPBDXZ
    ARTS_IMPORT virtual const char* GetDirName();

    // ?SetFricElas@aiVehicleData@@QAEXXZ
    ARTS_IMPORT void SetFricElas();

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    Vector3 Size; // 0x018
    Vector3 MaxAng; // 0x024
    Vector3 CG; // 0x030
    Vector3 WheelPositions[6]; // 0x03C
    f32 Mass; // 0x084
    f32 Friction; // 0x088
    f32 Elasticity; // 0x08C
    f32 MaxDamage; // 0x090
    f32 PtxThresh; // 0x094
    f32 Spring; // 0x098
    f32 Damping; // 0x09C
    f32 RubberSpring; // 0x0A0
    f32 RubberDamp; // 0x0A4
    f32 Limit; // 0x0A8
    f32 WheelRadius; // 0x0AC
    i32 unknown176; // 0x0B0
    i32 DataId; // 0x0B4
    dgBoundBox* BoundingBox; // 0x0B8
};

// ??_7aiVehicleData@@6B@
// vtable at 0x005B5C20
check_size(aiVehicleData, 0xBC);
