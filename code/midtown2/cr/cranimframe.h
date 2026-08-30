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

class Stream;
class crSkeleton;
class crSkeletonData;
class datTokenizer;

class crAnimFrame
{
public:
    // ??0crAnimFrame@@QAE@ABV0@@Z
    ARTS_IMPORT crAnimFrame(const crAnimFrame& arg1);

    // ??0crAnimFrame@@QAE@_N@Z
    ARTS_IMPORT crAnimFrame(bool arg1);

    // ??1crAnimFrame@@QAE@XZ
    ARTS_IMPORT ~crAnimFrame();

    // ?Add@crAnimFrame@@QAEXABV1@HH@Z
    ARTS_IMPORT void Add(const crAnimFrame& arg1, i32 arg2, i32 arg3);

    // ?AddScaled@crAnimFrame@@QAEXABV1@MHH@Z
    ARTS_IMPORT void AddScaled(const crAnimFrame& arg1, f32 arg2, i32 arg3, i32 arg4);

    // ?AllocateBuffers@crAnimFrame@@SAXHH@Z
    ARTS_IMPORT static void AllocateBuffers(i32 arg1, i32 arg2);

    // ?Blend@crAnimFrame@@QAEXMABV1@0HH@Z
    ARTS_IMPORT void Blend(f32 arg1, const crAnimFrame& arg2, const crAnimFrame& arg3, i32 arg4, i32 arg5);

    // ?DeallocateBuffers@crAnimFrame@@SAXXZ
    ARTS_IMPORT static void DeallocateBuffers();

    // ?Flip@crAnimFrame@@QAEXXZ
    ARTS_IMPORT void Flip();

    // ?Init@crAnimFrame@@QAEXH@Z
    ARTS_IMPORT void Init(i32 arg1);

    // ?LoadAscii@crAnimFrame@@QAEXAAVdatTokenizer@@H@Z
    ARTS_IMPORT void LoadAscii(datTokenizer& arg1, i32 arg2);

    // ?LoadBin@crAnimFrame@@QAEXPAVStream@@H@Z
    ARTS_IMPORT void LoadBin(Stream* arg1, i32 arg2);

    // ?Merge@crAnimFrame@@QAEXABV1@M@Z
    ARTS_IMPORT void Merge(const crAnimFrame& arg1, f32 arg2);

    // ?Mirror@crAnimFrame@@QAEXPBVcrSkeletonData@@@Z
    ARTS_IMPORT void Mirror(const crSkeletonData* arg1);

    // ??4crAnimFrame@@QAEABV0@ABV0@@Z
    ARTS_IMPORT const crAnimFrame& operator=(const crAnimFrame& arg1);

    // ?Pose@crAnimFrame@@QAEXAAVcrSkeleton@@_N@Z
    ARTS_IMPORT void Pose(crSkeleton& arg1, bool arg2);

    // ?Print@crAnimFrame@@QAEXXZ
    ARTS_IMPORT void Print();

    // ?SaveBin@crAnimFrame@@QAEXPAVStream@@@Z
    ARTS_IMPORT void SaveBin(Stream* arg1);

    // ?Zero@crAnimFrame@@QAEXXZ
    ARTS_EXPORT void Zero();

private:
    // ?Copy@crAnimFrame@@AAEXABV1@@Z
    ARTS_IMPORT void Copy(const crAnimFrame& arg1);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u32 ValueCount; // 0x000
    f32* Values; // 0x004
};

check_size(crAnimFrame, 0x8);
