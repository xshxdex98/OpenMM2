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
class Vector4;

class Matrix44
{
public:
    // ?Add@Matrix44@@QAEXABV1@0@Z
    ARTS_EXPORT void Add(const Matrix44& arg1, const Matrix44& arg2);

    // ?Add@Matrix44@@QAEXABV1@@Z
    ARTS_EXPORT void Add(const Matrix44& arg1);

    // ?AddScaled@Matrix44@@QAEXABV1@M@Z
    ARTS_EXPORT void AddScaled(const Matrix44& arg1, f32 arg2);

    // ?Bezier@Matrix44@@QAEXXZ
    ARTS_EXPORT void Bezier();

    // ?CatmullRom@Matrix44@@QAEXXZ
    ARTS_EXPORT void CatmullRom();

    // ?Determinant@Matrix44@@QBEMXZ
    ARTS_EXPORT f32 Determinant() const;

    // ?Dot@Matrix44@@QAEAAV1@ABV1@0@Z
    ARTS_EXPORT Matrix44& Dot(const Matrix44& arg1, const Matrix44& arg2);

    // ?Dot@Matrix44@@QAEAAV1@ABV1@@Z
    ARTS_EXPORT Matrix44& Dot(const Matrix44& arg1);

    // ?FastInverse@Matrix44@@QAEXABV1@@Z
    ARTS_EXPORT void FastInverse(const Matrix44& arg1);

    // ?FromMatrix34@Matrix44@@QAEXABVMatrix34@@@Z
    ARTS_EXPORT void FromMatrix34(const Matrix34& arg1);

    // ?Hermite@Matrix44@@QAEXXZ
    ARTS_EXPORT void Hermite();

    // ?Identity@Matrix44@@QAEXXZ
    ARTS_EXPORT void Identity();

    // ?InvertTo@Matrix44@@QBEXAAV1@@Z
    ARTS_IMPORT void InvertTo(Matrix44& arg1) const;

    // ?MakeRotX@Matrix44@@QAEAAV1@M@Z
    ARTS_EXPORT Matrix44& MakeRotX(f32 arg1);

    // ?MakeRotY@Matrix44@@QAEAAV1@M@Z
    ARTS_EXPORT Matrix44& MakeRotY(f32 arg1);

    // ?MakeRotZ@Matrix44@@QAEAAV1@M@Z
    ARTS_EXPORT Matrix44& MakeRotZ(f32 arg1);

    // ?Print@Matrix44@@QBEXPBD@Z
    ARTS_EXPORT void Print(const char* arg1) const;

    // ?Set@Matrix44@@QAEXABV1@@Z
    ARTS_EXPORT void Set(const Matrix44& arg1);

    // ?Subtract@Matrix44@@QAEXABV1@0@Z
    ARTS_EXPORT void Subtract(const Matrix44& arg1, const Matrix44& arg2);

    // ?Subtract@Matrix44@@QAEXABV1@@Z
    ARTS_EXPORT void Subtract(const Matrix44& arg1);

    // ?ToMatrix34@Matrix44@@QBEXAAVMatrix34@@@Z
    ARTS_EXPORT void ToMatrix34(Matrix34& arg1) const;

    // ?Transform4@Matrix44@@QBEXPBVVector3@@PAVVector4@@H@Z
    ARTS_EXPORT void Transform4(const Vector3* arg1, Vector4* arg2, i32 arg3) const;

    // ?Transpose@Matrix44@@QAEXXZ
    ARTS_EXPORT void Transpose();

    // ?Zero@Matrix44@@QAEXXZ
    ARTS_EXPORT void Zero();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    f32 m00; // 0x000
    f32 m01; // 0x004
    f32 m02; // 0x008
    f32 m03; // 0x00C
    f32 m10; // 0x010
    f32 m11; // 0x014
    f32 m12; // 0x018
    f32 m13; // 0x01C
    f32 m20; // 0x020
    f32 m21; // 0x024
    f32 m22; // 0x028
    f32 m23; // 0x02C
    f32 m30; // 0x030
    f32 m31; // 0x034
    f32 m32; // 0x038
    f32 m33; // 0x03C
};

check_size(Matrix44, 0x40);
