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

class Matrix34;
struct OppIconInfo;

class mmIcons : public asNode
{
public:
    // ??0mmIcons@@QAE@XZ
    ARTS_IMPORT mmIcons();

    // ??1mmIcons@@UAE@XZ
    ARTS_IMPORT virtual ~mmIcons();

    // ?Cull@mmIcons@@UAEXXZ
    ARTS_IMPORT virtual void Cull();

    // ?Update@mmIcons@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Init@mmIcons@@QAEXPAVMatrix34@@MMH@Z
    ARTS_IMPORT void Init(Matrix34* arg1, f32 arg2, f32 arg3, i32 arg4);

    // ?RegisterOpponents@mmIcons@@QAEXPAUOppIconInfo@@HPAX@Z
    ARTS_IMPORT void RegisterOpponents(OppIconInfo* arg1, i32 arg2, void* arg3);

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i32 field_18; // 0x018
    i32 field_1C; // 0x01C
    i32 field_20; // 0x020
    i32 field_24; // 0x024
    f32 field_28; // 0x028
    i32 field_2C; // 0x02C
    i32 field_30; // 0x030
    f32 field_34; // 0x034
    i32 field_38; // 0x038
    i32 field_3C; // 0x03C
    i32 field_40; // 0x040
    i32 field_44; // 0x044
    i32 field_48; // 0x048
    i32 field_4C; // 0x04C
    i32 field_50; // 0x050
    i32 field_54; // 0x054
    i32 field_58; // 0x058
    f32 field_5C; // 0x05C
    i32 field_60; // 0x060
    f32 field_64; // 0x064
    i32 field_68; // 0x068
    i32 field_6C; // 0x06C
    f32 field_70; // 0x070
    f32 field_74; // 0x074
    i32 field_78; // 0x078
    i32 field_7C; // 0x07C
    i32 field_80; // 0x080
    i32 field_84; // 0x084
    i32 field_88; // 0x088
    i32 field_8C; // 0x08C
    i32 field_90; // 0x090
    i32 field_94; // 0x094
    i32 field_98; // 0x098
    i32 field_9C; // 0x09C
    u8 field_A0[8]; // 0x0A0
    i32 field_A8; // 0x0A8
    u8 field_AC[4]; // 0x0AC
    f32 field_B0; // 0x0B0
    u8 field_B4[4]; // 0x0B4
    i32 field_B8; // 0x0B8
};

// ??_7mmIcons@@6B@
// vtable at 0x005B0E1C
check_size(mmIcons, 0xBC);
