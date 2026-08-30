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

class crSkeleton;
class pedAnimation;

class pedAnimationInstance
{
public:
    // ?Draw@pedAnimationInstance@@QAEX_N@Z
    ARTS_IMPORT void Draw(bool arg1);

    // ?DrawShadow@pedAnimationInstance@@QAEXXZ
    ARTS_IMPORT void DrawShadow();

    // ?Init@pedAnimationInstance@@QAEXPAD@Z
    ARTS_IMPORT void Init(char* arg1);

    // ?PreUpdate@pedAnimationInstance@@QAEXM@Z
    ARTS_IMPORT void PreUpdate(f32 arg1);

    // ?Reset@pedAnimationInstance@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?Start@pedAnimationInstance@@QAEXH@Z
    ARTS_IMPORT void Start(i32 arg1);

    // ?Update@pedAnimationInstance@@QAEXXZ
    ARTS_IMPORT void Update();

    // ?VerifySeq@pedAnimationInstance@@QAEXH@Z
    ARTS_IMPORT void VerifySeq(i32 arg1);

private:
    // ?Load@pedAnimationInstance@@AAEPAVpedAnimation@@PBD@Z
    ARTS_IMPORT pedAnimation* Load(const char* arg1);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u32 field_0; // 0x000
    pedAnimation* pAnimation; // 0x004
    u32 ShaderIndex; // 0x008
    u16 CurrentFrame; // 0x00C
    u8 CurrentAnimation; // 0x00E
    i8 field_F[41]; // 0x00F
    u32 field_38; // 0x038
    u32 ShaderSet; // 0x03C
    crSkeleton* crSkeleton; // 0x040
    u32 crSkeletonData; // 0x044
    u32 field_48; // 0x048
    void* field_4C; // 0x04C
    void* field_50; // 0x050
    void* field_54; // 0x054
    void* field_58; // 0x058
    u32 field_5C; // 0x05C
    u32 field_60; // 0x060
    u32 field_64; // 0x064
    u32 field_68; // 0x068
    i16 field_6C; // 0x06C
};

check_size(pedAnimationInstance, 0x70);
