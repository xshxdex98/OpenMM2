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

class Matrix34;
class Vector2;
class Vector3;
class gfxTexture;

class ptxGlass
{
public:
    // ?AddShards@ptxGlass@@QAEXABVVector3@@00ABVVector2@@11000@Z
    ARTS_IMPORT void AddShards(const Vector3& arg1, const Vector3& arg2, const Vector3& arg3, const Vector2& arg4, const Vector2& arg5, const Vector2& arg6, const Vector3& arg7, const Vector3& arg8, const Vector3& arg9);

    // ?CreateShards@ptxGlass@@QAEXMMQAVVector2@@AAVMatrix34@@ABVVector3@@2@Z
    ARTS_IMPORT void CreateShards(f32 arg1, f32 arg2, Vector2*const arg3, Matrix34& arg4, const Vector3& arg5, const Vector3& arg6);

    // ?DrawShards@ptxGlass@@QAEXH@Z
    ARTS_IMPORT void DrawShards(i32 arg1);

    // ?Init@ptxGlass@@QAEXHHPAVgfxTexture@@@Z
    ARTS_IMPORT void Init(i32 arg1, i32 arg2, gfxTexture* arg3);

    // ?SetCentroidFromVerts@ptxShard@ptxGlass@@QAEXXZ
    ARTS_IMPORT void SetCentroidFromVerts();

    // ?Update@ptxGlass@@QAEXM@Z
    ARTS_IMPORT void Update(f32 arg1);
};

// check_size(ptxGlass, 0x0); // TODO: no layout in the IDB type library
