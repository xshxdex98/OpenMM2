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
#include "data7/datcallback.h"

class Matrix34;
class Vector3;
class camCarCS;
class datParser;
class vehCar;

class camViewCS : public asNode
{
public:
    // ??0camViewCS@@QAE@XZ
    ARTS_IMPORT camViewCS();

    // ??1camViewCS@@UAE@XZ
    ARTS_IMPORT virtual ~camViewCS();

    // ?Update@camViewCS@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@camViewCS@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?FileIO@camViewCS@@UAEXAAVdatParser@@@Z
    ARTS_IMPORT virtual void FileIO(datParser& arg1);

    // ?ForceMatrixDelta@camViewCS@@QAEXABVMatrix34@@@Z
    ARTS_IMPORT void ForceMatrixDelta(const Matrix34& arg1);

    // ?ForceMatrixDelta@camViewCS@@QAEXABVVector3@@@Z
    ARTS_IMPORT void ForceMatrixDelta(const Vector3& arg1);

    // ?Init@camViewCS@@QAEXPAVvehCar@@@Z
    ARTS_IMPORT void Init(vehCar* arg1);

    // ?Instance@camViewCS@@SAPAV1@AAVvehCar@@@Z
    ARTS_IMPORT static camViewCS* Instance(vehCar& arg1);

    // ?NewCam@camViewCS@@QAEHPAVcamCarCS@@HMVdatCallback@@@Z
    ARTS_IMPORT i32 NewCam(camCarCS* arg1, i32 arg2, f32 arg3, datCallback arg4);

    // ?OneShot@camViewCS@@QAEXXZ
    ARTS_IMPORT void OneShot();

    // ?SetCam@camViewCS@@QAEXPAVcamCarCS@@@Z
    ARTS_IMPORT void SetCam(camCarCS* arg1);

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i32 field_18; // 0x018
    u8 field_1C[8]; // 0x01C
    datCallback field_24; // 0x024
    i32 field_30; // 0x030
    i32 field_34; // 0x034
    i32 field_38; // 0x038
    u8 field_3C; // 0x03C
    u8 field_3D[3]; // 0x03D
    f32 field_40; // 0x040
    f32 field_44; // 0x044
    u8 field_48; // 0x048
    u8 field_49[3]; // 0x049
};

// ??_7camViewCS@@6B@
// vtable at 0x005B4F18
check_size(camViewCS, 0x4C);
