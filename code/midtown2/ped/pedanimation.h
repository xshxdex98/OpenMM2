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

#include "cr/crskeletondata.h"
#include "vector7/matrix34.h"

class AnimationSequence;
class Stream;
class Vector2;
class crSkeleton;
class modModel;
class modShader;

class pedAnimation
{
public:
    // ?DrawSkeleton@pedAnimation@@QAEXHPAVcrSkeleton@@@Z
    ARTS_IMPORT void DrawSkeleton(i32 arg1, crSkeleton* arg2);

    // ?Init@pedAnimation@@SAXXZ
    ARTS_IMPORT static void Init();

    // ?Load@pedAnimation@@QAEXPADPAVStream@@@Z
    ARTS_IMPORT void Load(char* arg1, Stream* arg2);

    // ?LookupSequence@pedAnimation@@QAEHPAD@Z
    ARTS_IMPORT i32 LookupSequence(char* arg1);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i32 AnimationCount; // 0x000
    AnimationSequence* Animations; // 0x004
    Matrix34 matrix348; // 0x008
    modModel* pModel; // 0x038
    modShader** ppShaders; // 0x03C
    crSkeleton* pSkeleton; // 0x040
    crSkeletonData SkeletonData; // 0x044
    f32* field_4C; // 0x04C
    Vector2* field_50; // 0x050
    u8* field_54; // 0x054
    u8* field_58; // 0x058
    u8** field_5C; // 0x05C
    u32* pColors; // 0x060
    u32 SphereIndex; // 0x064
    i32 nColorCount; // 0x068
    u16 HashTableIndex; // 0x06C
};

check_size(pedAnimation, 0x70);
