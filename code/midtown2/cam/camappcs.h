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

#include "cambasecs.h"

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
    i32 field_90; // 0x090
    i32 TrackTo; // 0x094
    f32 field_98; // 0x098
    i32 field_9C; // 0x09C
    i32 ApproachOn; // 0x0A0
    i32 AppAppOn; // 0x0A4
    i32 AppRot; // 0x0A8
    i32 AppXRot; // 0x0AC
    i32 AppYPos; // 0x0B0
    i32 AppXZPos; // 0x0B4
    f32 AppApp; // 0x0B8
    i32 AppRotMin; // 0x0BC
    i32 AppPosMin; // 0x0C0
    i32 LookAbove; // 0x0C4
    i32 field_C8; // 0x0C8
    i32 MaxDist; // 0x0CC
    i32 MinDist; // 0x0D0
    f32 LookAt; // 0x0D4
    i32 field_D8; // 0x0D8
    i32 field_DC; // 0x0DC
    i32 field_E0; // 0x0E0
    u8 field_E4[12]; // 0x0E4
    i32 field_F0; // 0x0F0
    i32 field_F4; // 0x0F4
    i32 field_F8; // 0x0F8
    i32 field_FC; // 0x0FC
    i32 field_100; // 0x100
    i32 field_104; // 0x104
};

// ??_7camAppCS@@6B@
// vtable at 0x005B5174
check_size(camAppCS, 0x108);
