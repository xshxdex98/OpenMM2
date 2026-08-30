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

class Matrix33
{
public:
    // ?Add@Matrix33@@QAEXABV1@0@Z
    ARTS_EXPORT void Add(const Matrix33& arg1, const Matrix33& arg2);

    // ?Add@Matrix33@@QAEXABV1@@Z
    ARTS_EXPORT void Add(const Matrix33& arg1);

    // ?CrossProduct@Matrix33@@QAEXABVVector3@@@Z
    ARTS_EXPORT void CrossProduct(const Vector3& arg1);

    // ?Diagonal@Matrix33@@QAEXABVVector3@@@Z
    ARTS_EXPORT void Diagonal(const Vector3& arg1);

    // ?Diagonal@Matrix33@@QAEXM@Z
    ARTS_EXPORT void Diagonal(f32 arg1);

    // ?Dot@Matrix33@@QAEXABV1@0@Z
    ARTS_EXPORT void Dot(const Matrix33& arg1, const Matrix33& arg2);

    // ?Dot@Matrix33@@QAEXABV1@@Z
    ARTS_EXPORT void Dot(const Matrix33& arg1);

    // ?DotCrossProdMtx@Matrix33@@QAEXABVVector3@@@Z
    ARTS_EXPORT void DotCrossProdMtx(const Vector3& arg1);

    // ?Inverse@Matrix33@@QAEXABV1@@Z
    ARTS_EXPORT void Inverse(const Matrix33& arg1);

    // ?Inverse@Matrix33@@QAEXXZ
    ARTS_EXPORT void Inverse();

    // ?IsZero@Matrix33@@QBE_NXZ
    ARTS_EXPORT bool IsZero() const;

    // ?Negate@Matrix33@@QAEXXZ
    ARTS_EXPORT void Negate();

    // ?Set@Matrix33@@QAEXABV1@@Z
    ARTS_EXPORT void Set(const Matrix33& arg1);

    // ?Subtract@Matrix33@@QAEXABV1@0@Z
    ARTS_EXPORT void Subtract(const Matrix33& arg1, const Matrix33& arg2);

    // ?Subtract@Matrix33@@QAEXABV1@@Z
    ARTS_EXPORT void Subtract(const Matrix33& arg1);

    // ?Transpose@Matrix33@@QAEXABV1@@Z
    ARTS_EXPORT void Transpose(const Matrix33& arg1);

    // ?Transpose@Matrix33@@QAEXXZ
    ARTS_EXPORT void Transpose();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    f32 m00; // 0x000
    f32 m01; // 0x004
    f32 m02; // 0x008
    f32 m10; // 0x00C
    f32 m11; // 0x010
    f32 m12; // 0x014
    f32 m20; // 0x018
    f32 m21; // 0x01C
    f32 m22; // 0x020
};

check_size(Matrix33, 0x24);
