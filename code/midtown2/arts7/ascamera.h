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

#include "asnode.h"

class Vector3;
class datCallback;
class gfxBitmap;

class asCamera : public asNode
{
public:
    enum kNearClip : i32;

    // ??0asCamera@@QAE@XZ
    ARTS_IMPORT asCamera();

    // ??1asCamera@@UAE@XZ
    ARTS_IMPORT virtual ~asCamera();

    // ?DrawBegin@asCamera@@QAEXXZ
    ARTS_IMPORT void DrawBegin();

    // ?DrawEnd@asCamera@@QAEXXZ
    ARTS_IMPORT void DrawEnd();

    // ?FadeIn@asCamera@@QAEXMH@Z
    ARTS_IMPORT void FadeIn(f32 arg1, i32 arg2);

    // ?FadeOut@asCamera@@QAEXMH@Z
    ARTS_IMPORT void FadeOut(f32 arg1, i32 arg2);

    // ?GetNearClip@asCamera@@QAEXPAVVector3@@AAVMatrix34@@W4kNearClip@1@@Z
    ARTS_IMPORT void GetNearClip(Vector3* arg1, Matrix34& arg2, asCamera::kNearClip arg3);

    // ?GetViewportHeight@asCamera@@QAEHXZ
    ARTS_IMPORT i32 GetViewportHeight();

    // ?GetViewportWidth@asCamera@@QAEHXZ
    ARTS_IMPORT i32 GetViewportWidth();

    // ?SetAmbient@asCamera@@QAEXABVVector3@@@Z
    ARTS_IMPORT void SetAmbient(const Vector3& arg1);

    // ?SetClipArea@asCamera@@QAEXMMMM@Z
    ARTS_IMPORT void SetClipArea(f32 arg1, f32 arg2, f32 arg3, f32 arg4);

    // ?SetFog@asCamera@@QAEXMMMM@Z
    ARTS_IMPORT void SetFog(f32 arg1, f32 arg2, f32 arg3, f32 arg4);

    // ?SetLighting@asCamera@@QAEXH@Z
    ARTS_IMPORT void SetLighting(i32 arg1);

    // ?SetMonochrome@asCamera@@QAEXH@Z
    ARTS_IMPORT void SetMonochrome(i32 arg1);

    // ?SetUnderlay@asCamera@@QAEXPAD@Z
    ARTS_IMPORT void SetUnderlay(char* arg1);

    // ?SetUnderlayCB@asCamera@@QAEXPAVgfxBitmap@@PAVdatCallback@@@Z
    ARTS_IMPORT void SetUnderlayCB(gfxBitmap* arg1, datCallback* arg2);

    // ?SetView@asCamera@@QAEXMMMM@Z
    ARTS_IMPORT void SetView(f32 arg1, f32 arg2, f32 arg3, f32 arg4);

    // ?SetViewport@asCamera@@QAEXMMMMH@Z
    ARTS_IMPORT void SetViewport(f32 arg1, f32 arg2, f32 arg3, f32 arg4, i32 arg5);

    // ?SetWorld@asCamera@@QAEXAAVMatrix34@@@Z
    ARTS_IMPORT void SetWorld(Matrix34& arg1);

    // ?SphereVisible@asCamera@@QAEMABVVector3@@MPAM@Z
    ARTS_IMPORT f32 SphereVisible(const Vector3& arg1, f32 arg2, f32* arg3);

    // ?Update@asCamera@@UAEXXZ
    ARTS_IMPORT virtual void Update();

private:
    // ?Regen@asCamera@@AAEXXZ
    ARTS_IMPORT void Regen();

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i32 Cullable2D; // 0x018
    i32 field_1C; // 0x01C
    u32 field_20; // 0x020
    f32 field_24; // 0x024
    f32 field_28; // 0x028
    f32 field_2C; // 0x02C
    f32 field_30; // 0x030
    u32 field_34; // 0x034
    u32 field_38; // 0x038
    u32 field_3C; // 0x03C
    u32 field_40; // 0x040
    u32 field_44; // 0x044
    u32 field_48; // 0x048
    f32 field_4C; // 0x04C
    u32 field_50; // 0x050
    f32 field_54; // 0x054
    f32 field_58; // 0x058
    f32 field_5C; // 0x05C
    f32 field_60; // 0x060
    f32 field_64; // 0x064
    f32 field_68; // 0x068
    f32 field_6C; // 0x06C
    f32 field_70; // 0x070
    f32 field_74; // 0x074
    f32 field_78; // 0x078
    f32 field_7C; // 0x07C
    f32 field_80; // 0x080
    f32 field_84; // 0x084
    f32 field_88; // 0x088
    f32 field_8C; // 0x08C
    f32 field_90; // 0x090
    f32 field_94; // 0x094
    f32 field_98; // 0x098
    f32 field_9C; // 0x09C
    u32 field_A0; // 0x0A0
    u32 field_A4; // 0x0A4
    u32 field_A8; // 0x0A8
    f32 field_AC; // 0x0AC
    u32 field_B0; // 0x0B0
    u32 field_B4; // 0x0B4
    f32 field_B8; // 0x0B8
    f32 field_BC; // 0x0BC
    f32 field_C0; // 0x0C0
    f32 field_C4; // 0x0C4
    f32 field_C8; // 0x0C8
    f32 field_CC; // 0x0CC
    f32 field_D0; // 0x0D0
    Matrix34 matrix34D4; // 0x0D4
    Matrix34 matrix34104; // 0x104
    u32 field_134; // 0x134
    u32 field_138; // 0x138
    u32 field_13C; // 0x13C
    u32 field_140; // 0x140
    u32 field_144; // 0x144
    u32 field_148; // 0x148
    f32 field_14C; // 0x14C
    f32 field_150; // 0x150
    f32 field_154; // 0x154
    u32 field_158; // 0x158
    u32 field_15C; // 0x15C
    u32 field_160; // 0x160
    f32 field_164; // 0x164
    u32 field_168; // 0x168
    u32 field_16C; // 0x16C
};

// ??_7asCamera@@6B@
// vtable at 0x005B2978
check_size(asCamera, 0x170);
