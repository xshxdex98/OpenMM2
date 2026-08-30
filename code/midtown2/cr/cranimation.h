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

#include "data7/datrefcount.h"

class crAnimFrame;

class crAnimation : public datRefCount
{
public:
    // ??0crAnimation@@QAE@HH@Z
    ARTS_IMPORT crAnimation(i32 arg1, i32 arg2);

    // ??1crAnimation@@UAE@XZ
    ARTS_IMPORT virtual ~crAnimation();

    // ?AnimExists@crAnimation@@SA_NPBD@Z
    ARTS_IMPORT static bool AnimExists(const char* arg1);

    // ?CopyAnim@crAnimation@@QAEXPAV1@HH@Z
    ARTS_IMPORT void CopyAnim(crAnimation* arg1, i32 arg2, i32 arg3);

    // ?Flip@crAnimation@@QAEXXZ
    ARTS_IMPORT void Flip();

    // ?GetAnimation@crAnimation@@SAPAV1@PBD_N1PAVcrAnimFrame@@1@Z
    ARTS_IMPORT static crAnimation* GetAnimation(const char* arg1, bool arg2, bool arg3, crAnimFrame* arg4, bool arg5);

    // ?GetBlendFrame@crAnimation@@QBEXAAVcrAnimFrame@@M@Z
    ARTS_IMPORT void GetBlendFrame(crAnimFrame& arg1, f32 arg2) const;

    // ?GetChanAnimation@crAnimation@@SAPAV1@PBD_N@Z
    ARTS_IMPORT static crAnimation* GetChanAnimation(const char* arg1, bool arg2);

    // ?InitHashTables@crAnimation@@SAXXZ
    ARTS_IMPORT static void InitHashTables();

    // ?Normalize@crAnimation@@QAEX_N@Z
    ARTS_IMPORT void Normalize(bool arg1);

    // ?NormalizeX@crAnimation@@QAEXXZ
    ARTS_IMPORT void NormalizeX();

    // ?NormalizeY@crAnimation@@QAEXXZ
    ARTS_IMPORT void NormalizeY();

    // ?OutputAnimationList@crAnimation@@SAXXZ
    ARTS_IMPORT static void OutputAnimationList();

    // ?SaveAnim@crAnimation@@QAE_NPBD@Z
    ARTS_IMPORT bool SaveAnim(const char* arg1);

    // ?Subtract@crAnimation@@QAEXAAVcrAnimFrame@@@Z
    ARTS_IMPORT void Subtract(crAnimFrame& arg1);

    // ?ZeroX@crAnimation@@QAEXXZ
    ARTS_IMPORT void ZeroX();

    // ?ZeroY@crAnimation@@QAEXXZ
    ARTS_IMPORT void ZeroY();

    // ?ZeroYSeg@crAnimation@@QAEXHH@Z
    ARTS_IMPORT void ZeroYSeg(i32 arg1, i32 arg2);

private:
    // ??0crAnimation@@AAE@XZ
    ARTS_IMPORT crAnimation();

    // ?DeleteAnimTable@crAnimation@@CAXXZ
    ARTS_IMPORT static void DeleteAnimTable();

    // ?DeleteChanTable@crAnimation@@CAXXZ
    ARTS_IMPORT static void DeleteChanTable();

    // ?LoadAnim@crAnimation@@AAE_NPBD_N@Z
    ARTS_IMPORT bool LoadAnim(const char* arg1, bool arg2);

    // ?LoadChanAnim@crAnimation@@AAE_NPBD@Z
    ARTS_IMPORT bool LoadChanAnim(const char* arg1);

public:
    // Members from 0x008; everything below that belongs to datRefCount.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u32 field_8; // 0x008
    u32 field_C; // 0x00C
    u32 field_10; // 0x010
    crAnimFrame* AnimationFrames; // 0x014
    f32 field_18; // 0x018
    u8 field_1C; // 0x01C
    u8 field_1D; // 0x01D
};

// ??_7crAnimation@@6B@
// vtable at 0x005B63D0
check_size(crAnimation, 0x20);
