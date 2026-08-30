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

class Matrix44;
class Vector3;

class Vector4
{
public:
    // ?ComputePlane@Vector4@@QAEXABVVector3@@00@Z
    ARTS_EXPORT void ComputePlane(const Vector3& arg1, const Vector3& arg2, const Vector3& arg3);

    // ?ComputePlane@Vector4@@QAEXABVVector3@@0@Z
    ARTS_EXPORT void ComputePlane(const Vector3& arg1, const Vector3& arg2);

    // ?Cross@Vector4@@QAEXABV1@0@Z
    ARTS_EXPORT void Cross(const Vector4& arg1, const Vector4& arg2);

    // ?Dot@Vector4@@QAEAAV1@ABV1@ABVMatrix44@@@Z
    ARTS_EXPORT Vector4& Dot(const Vector4& arg1, const Matrix44& arg2);

    // ?Dot@Vector4@@QBEMABV1@@Z
    ARTS_EXPORT f32 Dot(const Vector4& arg1) const;

    // ?Dot3@Vector4@@QBEMABV1@@Z
    ARTS_EXPORT f32 Dot3(const Vector4& arg1) const;

    // ?Dot3x3@Vector4@@QAEAAV1@ABV1@ABVMatrix44@@@Z
    ARTS_EXPORT Vector4& Dot3x3(const Vector4& arg1, const Matrix44& arg2);

    // ?Max@Vector4@@QAEXABV1@0@Z
    ARTS_EXPORT void Max(const Vector4& arg1, const Vector4& arg2);

    // ?Min@Vector4@@QAEXABV1@0@Z
    ARTS_EXPORT void Min(const Vector4& arg1, const Vector4& arg2);

    // ?Outcode@Vector4@@QBEHXZ
    ARTS_EXPORT i32 Outcode() const;

    // ?Print@Vector4@@QBEXPBD@Z
    ARTS_EXPORT void Print(const char* arg1) const;

    // ?Print@Vector4@@QBEXXZ
    ARTS_EXPORT void Print() const;

    // ?Set@Vector4@@QAEXMMMM@Z
    ARTS_EXPORT void Set(f32 arg1, f32 arg2, f32 arg3, f32 arg4);

    // ?Subtract@Vector4@@QAEXABVVector3@@0@Z
    ARTS_EXPORT void Subtract(const Vector3& arg1, const Vector3& arg2);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    f32 x; // 0x000
    f32 y; // 0x004
    f32 z; // 0x008
    f32 w; // 0x00C
};

check_size(Vector4, 0x10);
