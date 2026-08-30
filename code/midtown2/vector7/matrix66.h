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

#include "matrix33.h"

class Matrix66
{
public:
    // ?Add@Matrix66@@QAEXABV1@0@Z
    ARTS_EXPORT void Add(const Matrix66& arg1, const Matrix66& arg2);

    // ?Add@Matrix66@@QAEXABV1@@Z
    ARTS_EXPORT void Add(const Matrix66& arg1);

    // ?Dot@Matrix66@@QAEXABV1@@Z
    ARTS_EXPORT void Dot(const Matrix66& arg1);

    // ?Inverse@Matrix66@@QAEXABV1@@Z
    ARTS_EXPORT void Inverse(const Matrix66& arg1);

    // ?Inverse@Matrix66@@QAEXXZ
    ARTS_EXPORT void Inverse();

    // ?Set@Matrix66@@QAEXABV1@@Z
    ARTS_EXPORT void Set(const Matrix66& arg1);

    // ?Set@Matrix66@@QAEXABVMatrix33@@000@Z
    ARTS_EXPORT void Set(const Matrix33& arg1, const Matrix33& arg2, const Matrix33& arg3, const Matrix33& arg4);

    // ?Subtract@Matrix66@@QAEXABV1@0@Z
    ARTS_EXPORT void Subtract(const Matrix66& arg1, const Matrix66& arg2);

    // ?Subtract@Matrix66@@QAEXABV1@@Z
    ARTS_EXPORT void Subtract(const Matrix66& arg1);

    // ?Transpose@Matrix66@@QAEXABV1@@Z
    ARTS_EXPORT void Transpose(const Matrix66& arg1);

    // ?Transpose@Matrix66@@QAEXXZ
    ARTS_EXPORT void Transpose();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    Matrix33 m00; // 0x000
    Matrix33 m01; // 0x024
    Matrix33 m10; // 0x048
    Matrix33 m11; // 0x06C
};

check_size(Matrix66, 0x90);
