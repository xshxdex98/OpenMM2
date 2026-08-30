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

class Vector3;

class dgImpact
{
public:
    // ?CalcCollision@dgImpact@@QAEHABVVector3@@MPAV2@11111_N2HHM@Z
    ARTS_IMPORT i32 CalcCollision(const Vector3& arg1, f32 arg2, Vector3* arg3, Vector3* arg4, Vector3* arg5, Vector3* arg6, Vector3* arg7, Vector3* arg8, bool arg9, bool arg10, i32 arg11, i32 arg12, f32 arg13);

    // ?CalcCollision@dgImpact@@QAEHABVVector3@@MPAV2@111M@Z
    ARTS_IMPORT i32 CalcCollision(const Vector3& arg1, f32 arg2, Vector3* arg3, Vector3* arg4, Vector3* arg5, Vector3* arg6, f32 arg7);

    // ?CalcImpact@dgImpact@@SAHPAV1@MM@Z
    ARTS_IMPORT static i32 CalcImpact(dgImpact* arg1, f32 arg2, f32 arg8);
};

// check_size(dgImpact, 0x0); // TODO: no layout in the IDB type library
