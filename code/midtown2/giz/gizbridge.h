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

#include "dg/dgbangerinstance.h"
#include "dg/dgunhitmtxbangerinstance.h"
#include "vector7/matrix34.h"

class gizBridge : public dgUnhitMtxBangerInstance
{
public:
    // ??0gizBridge@@QAE@XZ
    ARTS_IMPORT gizBridge();

    // ??1gizBridge@@QAE@XZ
    ARTS_IMPORT ~gizBridge();

    // ?Cull@gizBridge@@QAEXH@Z
    ARTS_IMPORT void Cull(i32 arg1);

    // ?Draw@gizBridge@@UAEXH@Z
    ARTS_IMPORT virtual void Draw(i32 arg1);

    // ?Init@gizBridge@@QAEXPADABVMatrix34@@@Z
    ARTS_IMPORT void Init(char* arg1, const Matrix34& arg2);

    // ?Reset@gizBridge@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?SizeOf@gizBridge@@UAEIXZ
    ARTS_IMPORT virtual u32 SizeOf();

    // ?Trigger@gizBridge@@QAE_NXZ
    ARTS_IMPORT bool Trigger();

    // ?Update@gizBridge@@QAEXXZ
    ARTS_IMPORT void Update();

private:
    // ?Reposition@gizBridge@@AAEXXZ
    ARTS_IMPORT void Reposition();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    dgBangerInstance Instance; // 0x000
    i32 field_18; // 0x018
    i32 field_1C; // 0x01C
    i32 field_20; // 0x020
    i32 field_24; // 0x024
    i32 field_28; // 0x028
    i32 field_2C; // 0x02C
    i32 field_30; // 0x030
    i32 field_34; // 0x034
    i32 field_38; // 0x038
    i32 field_3C; // 0x03C
    i32 field_40; // 0x040
    i32 field_44; // 0x044
    i32 field_48; // 0x048
    i32 field_4C; // 0x04C
    i32 field_50; // 0x050
    i32 field_54; // 0x054
    i32 field_58; // 0x058
    i32 field_5C; // 0x05C
    f32 field_60; // 0x060
    Matrix34 Matrix34; // 0x064
    f32 Rotation; // 0x094
    gizBridge* NextBridgePart; // 0x098
    gizBridge::mmBridgeAudio* mmBridgeAudio; // 0x09C
    i32 field_A0; // 0x0A0
    i32 field_A4; // 0x0A4
    i32 field_A8; // 0x0A8
    i32 field_AC; // 0x0AC
    i32 field_B0; // 0x0B0
    i32 field_B4; // 0x0B4
    i32 field_B8; // 0x0B8
    i32 field_BC; // 0x0BC
    i32 field_C0; // 0x0C0
    i32 field_C4; // 0x0C4
    i32 field_C8; // 0x0C8
    i32 field_CC; // 0x0CC
    i32 field_D0; // 0x0D0
    i32 field_D4; // 0x0D4
    i32 field_D8; // 0x0D8
    i32 field_DC; // 0x0DC
    i32 field_E0; // 0x0E0
    i32 field_E4; // 0x0E4
    i32 field_E8; // 0x0E8
    i32 field_EC; // 0x0EC
    i32 field_F0; // 0x0F0
    i32 field_F4; // 0x0F4
    i32 field_F8; // 0x0F8
    i32 field_FC; // 0x0FC
    i32 field_100; // 0x100
    i32 field_104; // 0x104
    i32 field_108; // 0x108
    i32 field_10C; // 0x10C
    i32 field_110; // 0x110
    i32 field_114; // 0x114
    i32 field_118; // 0x118
};

// ??_7gizBridge@@6B@
// vtable at 0x005B5F8C
check_size(gizBridge, 0x11C);
