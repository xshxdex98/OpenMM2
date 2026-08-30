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
class Vector4;

class Vector3
{
public:
    // ??0Vector3@@QAE@XZ
    ARTS_EXPORT Vector3();

    // ?Add@Vector3@@QAEXABV1@@Z
    ARTS_IMPORT void Add(const Vector3& arg1);

    // ?AddScaled@Vector3@@QAEXABV1@0M@Z
    ARTS_IMPORT void AddScaled(const Vector3& arg1, const Vector3& arg2, f32 arg3);

    // ?Angle@Vector3@@QBEMABV1@@Z
    ARTS_EXPORT f32 Angle(const Vector3& arg1) const;

    // ?Approach@Vector3@@QAE_NABV1@MM@Z
    ARTS_EXPORT bool Approach(const Vector3& arg1, f32 arg2, f32 arg3);

    // ?Cross@Vector3@@QAEXABV1@0@Z
    ARTS_EXPORT void Cross(const Vector3& arg1, const Vector3& arg2);

    // ?Cross@Vector3@@QAEXABV1@@Z
    ARTS_EXPORT void Cross(const Vector3& arg1);

    // ?Dist@Vector3@@QBEMABV1@@Z
    ARTS_EXPORT f32 Dist(const Vector3& arg1) const;

    // ?Dot@Vector3@@QAEXABV1@ABVMatrix34@@@Z
    ARTS_EXPORT void Dot(const Vector3& arg1, const Matrix34& arg2);

    // ?Dot@Vector3@@QBEMABV1@@Z
    ARTS_EXPORT f32 Dot(const Vector3& arg1) const;

    // ?Dot3x3@Vector3@@QAEXABVMatrix34@@@Z
    ARTS_EXPORT void Dot3x3(const Matrix34& arg1);

    // ?Dot3x3Transpose@Vector3@@QAEXABVMatrix34@@@Z
    ARTS_IMPORT void Dot3x3Transpose(const Matrix34& arg1);

    // ?Extend@Vector3@@QAEXABV1@M@Z
    ARTS_IMPORT void Extend(const Vector3& arg1, f32 arg2);

    // ?Extend@Vector3@@QAEXM@Z
    ARTS_EXPORT void Extend(f32 arg1);

    // ?FastAngle@Vector3@@QBEMABV1@@Z
    ARTS_EXPORT f32 FastAngle(const Vector3& arg1) const;

    // ?FlatDist@Vector3@@QBEMABV1@@Z
    ARTS_EXPORT f32 FlatDist(const Vector3& arg1) const;

    // ?GetPolar@Vector3@@QBEXABV1@PAVVector4@@PAV1@@Z
    ARTS_EXPORT void GetPolar(const Vector3& arg1, Vector4* arg2, Vector3* arg3) const;

    // ?GetVector2@Vector3@@QBEXHAAVVector2@@@Z
    ARTS_EXPORT void GetVector2(i32 arg1, Vector2& arg2) const;

    // ?HSVtoRGB@Vector3@@QAEXXZ
    ARTS_EXPORT void HSVtoRGB();

    // ?InvMag@Vector3@@QBEMXZ
    ARTS_EXPORT f32 InvMag() const;

    // ?InvScale@Vector3@@QAEXM@Z
    ARTS_IMPORT void InvScale(f32 arg1);

    // ?IsEqual@Vector3@@QBE_NABV1@@Z
    ARTS_EXPORT bool IsEqual(const Vector3& arg1) const;

    // ?Lerp@Vector3@@QAEXMABV1@0@Z
    ARTS_EXPORT void Lerp(f32 arg1, const Vector3& arg2, const Vector3& arg3);

    // ?Mag@Vector3@@QBEMXZ
    ARTS_EXPORT f32 Mag() const;

    // ?Mag2@Vector3@@QBEMXZ
    ARTS_EXPORT f32 Mag2() const;

    // ?Negate@Vector3@@QAEXABV1@@Z
    ARTS_EXPORT void Negate(const Vector3& arg1);

    // ?Normalize@Vector3@@QAEXXZ
    ARTS_EXPORT void Normalize();

    // ??LVector3@@QBE?AV0@ABV0@@Z
    ARTS_EXPORT Vector3 operator%(const Vector3& arg1) const;

    // ??XVector3@@QAEXABV0@@Z
    ARTS_IMPORT void operator*=(const Vector3& arg1);

    // ??YVector3@@QAEXAAV0@@Z
    ARTS_EXPORT void operator+=(Vector3& arg1);

    // ??YVector3@@QAEXABV0@@Z
    ARTS_EXPORT void operator+=(const Vector3& arg1);

    // ??GVector3@@QBE?AV0@ABV0@@Z
    ARTS_EXPORT Vector3 operator-(const Vector3& arg1) const;

    // ??ZVector3@@QAEXAAV0@@Z
    ARTS_EXPORT void operator-=(Vector3& arg1);

    // ??ZVector3@@QAEXABV0@@Z
    ARTS_EXPORT void operator-=(const Vector3& arg1);

    // ??KVector3@@QBE?AV0@M@Z
    ARTS_EXPORT Vector3 operator/(f32 arg1) const;

    // ?Print@Vector3@@QBEXPBD@Z
    ARTS_EXPORT void Print(const char* arg1) const;

    // ?Print@Vector3@@QBEXXZ
    ARTS_EXPORT void Print() const;

    // ?RGBtoHSV@Vector3@@QAEXXZ
    ARTS_EXPORT void RGBtoHSV();

    // ?RotateAboutAxis@Vector3@@QAEXMH@Z
    ARTS_EXPORT void RotateAboutAxis(f32 arg1, i32 arg2);

    // ?RotateX@Vector3@@QAEXM@Z
    ARTS_EXPORT void RotateX(f32 arg1);

    // ?RotateY@Vector3@@QAEXM@Z
    ARTS_EXPORT void RotateY(f32 arg1);

    // ?RotateZ@Vector3@@QAEXM@Z
    ARTS_EXPORT void RotateZ(f32 arg1);

    // ?Scale@Vector3@@QAEXABV1@M@Z
    ARTS_EXPORT void Scale(const Vector3& arg1, f32 arg2);

    // ?Scale@Vector3@@QAEXM@Z
    ARTS_EXPORT void Scale(f32 arg1);

    // ?Set@Vector3@@QAEXABV1@@Z
    ARTS_EXPORT void Set(const Vector3& arg1);

    // ?Set@Vector3@@QAEXMMM@Z
    ARTS_EXPORT void Set(f32 arg1, f32 arg2, f32 arg3);

    // ?Subtract@Vector3@@QAEXABV1@@Z
    ARTS_IMPORT void Subtract(const Vector3& arg1);

    // ?SubtractScaled@Vector3@@QAEXABV1@M@Z
    ARTS_EXPORT void SubtractScaled(const Vector3& arg1, f32 arg2);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    f32 x; // 0x000
    f32 y; // 0x004
    f32 z; // 0x008
};

check_size(Vector3, 0xC);
