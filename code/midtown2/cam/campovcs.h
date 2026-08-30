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

#include "camcarcs.h"

class datParser;

class camPovCS : public camCarCS
{
public:
    // ??0camPovCS@@QAE@XZ
    ARTS_IMPORT camPovCS();

    // ??1camPovCS@@UAE@XZ
    ARTS_IMPORT virtual ~camPovCS();

    // ?AfterLoad@camPovCS@@UAEXXZ
    ARTS_IMPORT virtual void AfterLoad();

    // ?FileIO@camPovCS@@UAEXAAVdatParser@@@Z
    ARTS_IMPORT virtual void FileIO(datParser& arg1);

    // ?GetClassName@camPovCS@@UAEPADXZ
    ARTS_IMPORT virtual char* GetClassName();

    // ?MakeActive@camPovCS@@UAEXXZ
    ARTS_IMPORT virtual void MakeActive();

    // ?Reset@camPovCS@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Update@camPovCS@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?UpdateInput@camPovCS@@UAEXXZ
    ARTS_IMPORT virtual void UpdateInput();

private:
    // ?UpdatePOV@camPovCS@@AAEXXZ
    ARTS_IMPORT void UpdatePOV();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 field_4[20]; // 0x004
    u16 field_18; // 0x018
    u8 field_1A[102]; // 0x01A
    f32 field_80; // 0x080
    f32 field_84; // 0x084
    f32 field_88; // 0x088
    f32 field_8C; // 0x08C
    u8 field_90[16]; // 0x090
    i32 field_A0; // 0x0A0
    i32 field_A4; // 0x0A4
    i32 field_A8; // 0x0A8
    u8 field_AC[4]; // 0x0AC
    i32 field_B0; // 0x0B0
    i32 field_B4; // 0x0B4
    i32 field_B8; // 0x0B8
    i32 field_BC; // 0x0BC
    i32 field_C0; // 0x0C0
    u8 field_C4[4]; // 0x0C4
    i32 field_C8; // 0x0C8
    f32 field_CC; // 0x0CC
    f32 field_D0; // 0x0D0
    i32 field_D4; // 0x0D4
    u8 field_D8[48]; // 0x0D8
    i32 field_108; // 0x108
    u8 field_10C[4]; // 0x10C
    i32 field_110; // 0x110
    i32 Offset; // 0x114
    f32 field_118; // 0x118
    i32 field_11C; // 0x11C
    i32 ReverseOffset; // 0x120
    f32 field_124; // 0x124
    f32 field_128; // 0x128
    i32 POVJitterAmp; // 0x12C
    i32 field_130; // 0x130
    i32 field_134; // 0x134
    i32 field_138; // 0x138
    i32 field_13C; // 0x13C
    i32 Pitch; // 0x140
    i32 field_144; // 0x144
};

// ??_7camPovCS@@6B@
// vtable at 0x005B4E80
check_size(camPovCS, 0x148);
