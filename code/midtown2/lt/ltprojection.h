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

class Vector3;
class gfxTexture;

class ltProjection
{
public:
    // ??0ltProjection@@QAE@H@Z
    ARTS_IMPORT ltProjection(i32 size);

    // ??1ltProjection@@QAE@XZ
    ARTS_IMPORT ~ltProjection();

    // ?DrawAmbient@ltProjection@@QAEXABVVector3@@@Z
    ARTS_IMPORT void DrawAmbient(const Vector3& arg1);

    // ?DrawBegin@ltProjection@@QAEXABVMatrix34@@@Z
    ARTS_IMPORT void DrawBegin(const Matrix34& arg1);

    // ?DrawDebug@ltProjection@@QAEXXZ
    ARTS_IMPORT void DrawDebug();

    // ?DrawEnd@ltProjection@@QAEXXZ
    ARTS_IMPORT void DrawEnd();

    // ?DrawMultiStageBegin@ltProjection@@QAEXHABVMatrix34@@@Z
    ARTS_IMPORT void DrawMultiStageBegin(i32 arg1, const Matrix34& arg2);

    // ?DrawMultiStageEnd@ltProjection@@QAEXH@Z
    ARTS_IMPORT void DrawMultiStageEnd(i32 p);

    // ?MakeOrthoMatrix@ltProjection@@QAEXABVVector3@@00M@Z
    ARTS_IMPORT void MakeOrthoMatrix(const Vector3& arg1, const Vector3& arg2, const Vector3& arg3, f32 arg4);

    // ?OffScreenBegin@ltProjection@@QAEX_N@Z
    ARTS_IMPORT void OffScreenBegin(bool arg1);

    // ?OffScreenEnd@ltProjection@@QAEXXZ
    ARTS_IMPORT void OffScreenEnd();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    gfxTexture* Dest; // 0x000
    Matrix34 Transform; // 0x004
    u8 SetSomeTexEnv; // 0x034
    u8 SetSomeTransform; // 0x035
    u8 UseDefaultRenderTarget; // 0x036
    u8 field_37; // 0x037
    i32 field_38; // 0x038
};

check_size(ltProjection, 0x3C);
