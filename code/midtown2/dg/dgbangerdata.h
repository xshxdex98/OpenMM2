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

#include "arts7/asbirthrule.h"
#include "arts7/asnode.h"
#include "vector7/vector3.h"

class dgBangerData : public asNode
{
public:
    // ??0dgBangerData@@QAE@XZ
    ARTS_IMPORT dgBangerData();

    // ??1dgBangerData@@UAE@XZ
    ARTS_IMPORT virtual ~dgBangerData();

    // ?Save@dgBangerData@@UAE_NXZ
    ARTS_IMPORT virtual bool Save();

    // ?Load@dgBangerData@@UAE_NXZ
    ARTS_IMPORT virtual bool Load();

    // ?GetClassName@dgBangerData@@UAEPADXZ
    ARTS_IMPORT virtual char* GetClassName();

    // ?GetDirName@dgBangerData@@UAEPBDXZ
    ARTS_IMPORT virtual const char* GetDirName();

    // ?AdjustBound@dgBangerData@@QAEXXZ
    ARTS_IMPORT void AdjustBound();

    // ?AdjustPrim@dgBangerData@@QAEXXZ
    ARTS_IMPORT void AdjustPrim();

    // ?InitBound@dgBangerData@@QAEXXZ
    ARTS_IMPORT void InitBound();

    // ?LoadEntry@dgBangerData@@QAEHPAD@Z
    ARTS_IMPORT i32 LoadEntry(char* arg1);

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u32 Index; // 0x018
    Vector3 Position; // 0x01C
    Vector3 vector328; // 0x028
    u32 field_34; // 0x034
    u32 field_38; // 0x038
    u32 field_3C; // 0x03C
    Vector3* Positions; // 0x040
    i16 PositionCount; // 0x044
    Vector3 field_48; // 0x048
    u32 field_54; // 0x054
    u32 field_58; // 0x058
    u32 field_5C; // 0x05C
    asBirthRule BirthRule; // 0x060
    u32 field_114; // 0x114
    u32 Flags2; // 0x118
    i16 field_11C; // 0x11C
    u8 field_11E; // 0x11E
    u8 field_11F; // 0x11F
    u8 Flags; // 0x120
    u8 field_121; // 0x121
    u32 field_124; // 0x124
    i16 GeomId; // 0x128
    u32 field_12C; // 0x12C
    u32 field_130; // 0x130
    u32 field_134; // 0x134
    i8 pad138[28]; // 0x138
};

// ??_7dgBangerData@@6B@
// vtable at 0x005B1460
check_size(dgBangerData, 0x154);
