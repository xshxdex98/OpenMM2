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

#include "arts7/asnetobject.h"
#include "vector7/matrix34.h"

#include "mmnetpath.h"

struct NET_INPUTDATA;
struct NET_POSDATA;
class Vector3;
class vehCar;

class mmNetObject : public asNetObject
{
public:
    // ??0mmNetObject@@QAE@XZ
    ARTS_IMPORT mmNetObject();

    // ??1mmNetObject@@UAE@XZ
    ARTS_IMPORT virtual ~mmNetObject();

    // ?Activate@mmNetObject@@QAEXXZ
    ARTS_IMPORT void Activate();

    // ?Clear@mmNetObject@@QAEXXZ
    ARTS_IMPORT void Clear();

    // ?Cull@mmNetObject@@UAEXXZ
    ARTS_IMPORT virtual void Cull();

    // ?Deactivate@mmNetObject@@QAEXXZ
    ARTS_IMPORT void Deactivate();

    // ?GetAngleApproach@mmNetObject@@QAEMMM@Z
    ARTS_IMPORT f32 GetAngleApproach(f32 arg1, f32 arg2);

    // ?GetPositionApproach@mmNetObject@@QAEMABVVector3@@0@Z
    ARTS_IMPORT f32 GetPositionApproach(const Vector3& arg1, const Vector3& arg2);

    // ?Init@mmNetObject@@QAEXPAVvehCar@@PADMHK1_N@Z
    ARTS_IMPORT void Init(vehCar* arg1, char* arg2, f32 arg3, i32 arg4, ulong arg5, char* arg6, bool arg7);

    // ?InputUpdate@mmNetObject@@QAEXPAUNET_INPUTDATA@@M@Z
    ARTS_IMPORT void InputUpdate(NET_INPUTDATA* arg1, f32 arg2);

    // ?PositionUpdate@mmNetObject@@QAEXPAUNET_POSDATA@@M@Z
    ARTS_IMPORT void PositionUpdate(NET_POSDATA* arg1, f32 arg2);

    // ?Predict@mmNetObject@@QAEXXZ
    ARTS_IMPORT void Predict();

    // ?ReInit@mmNetObject@@QAEXPAVvehCar@@PADMHK1_N@Z
    ARTS_IMPORT void ReInit(vehCar* arg1, char* arg2, f32 arg3, i32 arg4, ulong arg5, char* arg6, bool arg7);

    // ?ResetValues@mmNetObject@@QAEXXZ
    ARTS_IMPORT void ResetValues();

    // ?Set@mmNetObject@@QAEXH@Z
    ARTS_IMPORT void Set(i32 arg1);

    // ?SetActive@mmNetObject@@QAEXH@Z
    ARTS_IMPORT void SetActive(i32 arg1);

    // ?SetCar@mmNetObject@@QAEXPAVvehCar@@@Z
    ARTS_IMPORT void SetCar(vehCar* arg1);

    // ?SetInputData@mmNetObject@@QAEXXZ
    ARTS_IMPORT void SetInputData();

    // ?SetPositionData@mmNetObject@@QAEHK@Z
    ARTS_IMPORT i32 SetPositionData(ulong arg1);

    // ?SetScore@mmNetObject@@QAEXH@Z
    ARTS_IMPORT void SetScore(i32 arg1);

    // ?Update@mmNetObject@@UAEXXZ
    ARTS_IMPORT virtual void Update();

public:
    // Members from 0x020; everything below that belongs to asNetObject.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 field_20[4]; // 0x020
    u8 field_24; // 0x024
    u8 field_25[11]; // 0x025
    i32 field_30; // 0x030
    u8 field_34[4]; // 0x034
    mmNetPath MMNetPath; // 0x038
    u8 field_A8[60]; // 0x0A8
    u8 field_E4[4]; // 0x0E4
    u8 field_E8[4]; // 0x0E8
    u8 field_EC[4]; // 0x0EC
    u8 field_F0[4]; // 0x0F0
    u8 field_F4[4]; // 0x0F4
    u8 field_F8[28]; // 0x0F8
    vehCar* VehCar; // 0x114
    i32 field_118; // 0x118
    u8 field_11C; // 0x11C
    u8 field_11D[19]; // 0x11D
    Matrix34 Matrix34_1; // 0x130
    Matrix34 Matrix34_2; // 0x160
    u8 field_190[16]; // 0x190
    Matrix34 Matrix34_4; // 0x1A0
    u8 field_1D0[72]; // 0x1D0
    u8 field_218[4]; // 0x218
    u8 field_21C[4]; // 0x21C
    u8 field_220[4]; // 0x220
    u8 field_224[612]; // 0x224
    Matrix34 Matrix34_3; // 0x488
    u8 field_4B8[452]; // 0x4B8
};

// ??_7mmNetObject@@6B@
// vtable at 0x005B1138
check_size(mmNetObject, 0x67C);
