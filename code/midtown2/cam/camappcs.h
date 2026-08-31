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

#include "vector7/matrix34.h"
#include "vector7/vector3.h"

#include "cambasecs.h"

class Base_vtbl;
class asNode;
class camViewCS;
class datParser;

class camAppCS : public camBaseCS
{
public:
    // ??0camAppCS@@QAE@XZ
    ARTS_IMPORT camAppCS();

    // ??1camAppCS@@UAE@XZ
    ARTS_IMPORT virtual ~camAppCS();

    // ?FileIO@camAppCS@@UAEXAAVdatParser@@@Z
    ARTS_IMPORT virtual void FileIO(datParser& renderer);

protected:
    // ?ApproachIt@camAppCS@@IAEXXZ
    ARTS_IMPORT void ApproachIt();

private:
    // ?DApproach@camAppCS@@AAEHAAMMMM0M@Z
    ARTS_IMPORT i32 DApproach(f32& arg1, f32 arg2, f32 arg3, f32 arg4, f32& arg5, f32 arg6);

    // ?UpdateApproach@camAppCS@@AAEXXZ
    ARTS_IMPORT void UpdateApproach();

    // ?UpdateMaxDist@camAppCS@@AAEXXZ
    ARTS_IMPORT void UpdateMaxDist();

public:
    // Members from 0x090; everything below that belongs to camBaseCS.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    Matrix34* unk_90; // 0x090
    Vector3 TrackTo; // 0x094
    i32 ApproachOn; // 0x0A0
    i32 AppAppOn; // 0x0A4
    f32 AppRot; // 0x0A8
    f32 AppXRot; // 0x0AC
    f32 AppYPos; // 0x0B0
    f32 AppXZPos; // 0x0B4
    f32 AppApp; // 0x0B8
    f32 AppRotMin; // 0x0BC
    f32 AppPosMin; // 0x0C0
    f32 LookAbove; // 0x0C4
    void* unk_C8; // 0x0C8
    f32 MaxDist; // 0x0CC
    f32 MinDist; // 0x0D0
    f32 LookAt; // 0x0D4
    Matrix34 unk_D8; // 0x0D8
};

// ??_7camAppCS@@6B@
// vtable at 0x005B5174
check_size(camAppCS, 0x108);
