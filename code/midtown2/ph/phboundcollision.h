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
class Vector3;

class phBoundCollision
{
public:
    // ?GetDisp@phBoundCollision@@SAXPBVMatrix34@@0ABVVector3@@AAV3@@Z
    ARTS_IMPORT static void GetDisp(const Matrix34* arg1, const Matrix34* arg2, const Vector3& arg3, Vector3& arg4);

    // ?SegSegDistNorm@phBoundCollision@@SAXABVVector3@@00000AAV2@11AAMAAH@Z
    ARTS_IMPORT static void SegSegDistNorm(const Vector3& arg1, const Vector3& arg2, const Vector3& arg3, const Vector3& arg4, const Vector3& arg5, const Vector3& arg6, Vector3& arg7, Vector3& arg8, Vector3& arg9, f32& arg10, i32& arg11);

    // ?SetPenetration@phBoundCollision@@SAXM@Z
    ARTS_IMPORT static void SetPenetration(f32 arg1);

private:
    // ?testNoOverlap@phBoundCollision@@CAHQBM0@Z
    ARTS_IMPORT static i32 testNoOverlap(const f32*const arg1, const f32*const arg2);
};

// check_size(phBoundCollision, 0x0); // TODO: no layout in the IDB type library
