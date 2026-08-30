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

#include "vector3.h"

class Quaternion;
class Vector4;

class Matrix34
{
public:
    // ?Add@Matrix34@@QAEXABV1@0@Z
    ARTS_EXPORT void Add(const Matrix34& arg1, const Matrix34& arg2);

    // ?Add@Matrix34@@QAEXABV1@@Z
    ARTS_EXPORT void Add(const Matrix34& arg1);

    // ?Add3x3@Matrix34@@QAEXABV1@0@Z
    ARTS_EXPORT void Add3x3(const Matrix34& arg1, const Matrix34& arg2);

    // ?Add3x3@Matrix34@@QAEXABV1@@Z
    ARTS_EXPORT void Add3x3(const Matrix34& arg1);

    // ?AddScaled@Matrix34@@QAEXABV1@M@Z
    ARTS_EXPORT void AddScaled(const Matrix34& arg1, f32 arg2);

    // ?Determinant3x3@Matrix34@@QBEMXZ
    ARTS_EXPORT f32 Determinant3x3() const;

    // ?Dot@Matrix34@@QAEXABV1@0@Z
    ARTS_EXPORT void Dot(const Matrix34& lhs, const Matrix34& rhs);

    // ?Dot@Matrix34@@QAEXABV1@@Z
    ARTS_EXPORT void Dot(const Matrix34& arg1);

    // ?Dot3x3@Matrix34@@QAEXABV1@0@Z
    ARTS_EXPORT void Dot3x3(const Matrix34& arg1, const Matrix34& arg2);

    // ?Dot3x3@Matrix34@@QAEXABV1@@Z
    ARTS_EXPORT void Dot3x3(const Matrix34& arg1);

    // ?Dot3x3CrossProdMtx@Matrix34@@QAEXABVVector3@@@Z
    ARTS_EXPORT void Dot3x3CrossProdMtx(const Vector3& arg1);

    // ?Dot3x3CrossProdTranspose@Matrix34@@QAEXABVVector3@@@Z
    ARTS_EXPORT void Dot3x3CrossProdTranspose(const Vector3& arg1);

    // ?Dot3x3Transpose@Matrix34@@QAEXABV1@0@Z
    ARTS_EXPORT void Dot3x3Transpose(const Matrix34& arg1, const Matrix34& arg2);

    // ?Dot3x3Transpose@Matrix34@@QAEXABV1@@Z
    ARTS_EXPORT void Dot3x3Transpose(const Matrix34& arg1);

    // ?DotTranspose@Matrix34@@QAEXABV1@0@Z
    ARTS_EXPORT void DotTranspose(const Matrix34& arg1, const Matrix34& arg2);

    // ?DotTranspose@Matrix34@@QAEXABV1@@Z
    ARTS_EXPORT void DotTranspose(const Matrix34& arg1);

    // ?FastInverse@Matrix34@@QAEXABV1@@Z
    ARTS_EXPORT void FastInverse(const Matrix34& arg1);

    // ?FastInverse@Matrix34@@QAEXXZ
    ARTS_EXPORT void FastInverse();

    // ?FromEulers@Matrix34@@QAEXABVVector3@@PAD@Z
    ARTS_EXPORT void FromEulers(const Vector3& arg1, char* arg2);

    // ?FromEulersXYZ@Matrix34@@QAEXABVVector3@@@Z
    ARTS_EXPORT void FromEulersXYZ(const Vector3& arg1);

    // ?FromEulersXZY@Matrix34@@QAEXABVVector3@@@Z
    ARTS_EXPORT void FromEulersXZY(const Vector3& arg1);

    // ?FromEulersYXZ@Matrix34@@QAEXABVVector3@@@Z
    ARTS_EXPORT void FromEulersYXZ(const Vector3& arg1);

    // ?FromEulersYZX@Matrix34@@QAEXABVVector3@@@Z
    ARTS_EXPORT void FromEulersYZX(const Vector3& arg1);

    // ?FromEulersZXY@Matrix34@@QAEXABVVector3@@@Z
    ARTS_EXPORT void FromEulersZXY(const Vector3& arg1);

    // ?FromEulersZYX@Matrix34@@QAEXABVVector3@@@Z
    ARTS_EXPORT void FromEulersZYX(const Vector3& arg1);

    // ?FromQuaternion@Matrix34@@QAEXABVQuaternion@@@Z
    ARTS_IMPORT void FromQuaternion(const Quaternion& arg1);

    // ?GetEulers@Matrix34@@QBE?AVVector3@@PBD@Z
    ARTS_EXPORT Vector3 GetEulers(const char* arg1) const;

    // ?GetEulers@Matrix34@@QBE?AVVector3@@XZ
    ARTS_EXPORT Vector3 GetEulers() const;

    // ?GetLookAt@Matrix34@@QBEXPAVVector3@@0M@Z
    ARTS_EXPORT void GetLookAt(Vector3* arg1, Vector3* arg2, f32 arg3) const;

    // ?GetPolar@Matrix34@@QBEXPAVVector4@@PAVVector3@@M@Z
    ARTS_EXPORT void GetPolar(Vector4* arg1, Vector3* arg2, f32 arg3) const;

    // ?Identity@Matrix34@@QAEXXZ
    ARTS_EXPORT void Identity();

    // ?Identity3x3@Matrix34@@QAEXXZ
    ARTS_EXPORT void Identity3x3();

    // ?Interpolate@Matrix34@@QAEXABV1@0M@Z
    ARTS_IMPORT void Interpolate(const Matrix34& arg1, const Matrix34& arg2, f32 arg3);

    // ?Inverse@Matrix34@@QAEXABV1@@Z
    ARTS_EXPORT void Inverse(const Matrix34& arg1);

    // ?Inverse@Matrix34@@QAEXXZ
    ARTS_EXPORT void Inverse();

    // ?LookAt@Matrix34@@QAEXABVVector3@@0@Z
    ARTS_EXPORT void LookAt(const Vector3& id, const Vector3& values);

    // ?MakeRotate@Matrix34@@QAEXABVVector3@@M@Z
    ARTS_IMPORT void MakeRotate(const Vector3& arg1, f32 rotation);

    // ?MakeRotateUnitAxis@Matrix34@@QAEXABVVector3@@M@Z
    ARTS_EXPORT void MakeRotateUnitAxis(const Vector3& arg1, f32 arg2);

    // ?MakeRotateX@Matrix34@@QAEXM@Z
    ARTS_EXPORT void MakeRotateX(f32 arg1);

    // ?MakeRotateY@Matrix34@@QAEXM@Z
    ARTS_EXPORT void MakeRotateY(f32 arg1);

    // ?MakeRotateZ@Matrix34@@QAEXM@Z
    ARTS_EXPORT void MakeRotateZ(f32 arg1);

    // ?MakeScale@Matrix34@@QAEXM@Z
    ARTS_EXPORT void MakeScale(f32 arg1);

    // ?MakeScale@Matrix34@@QAEXMMM@Z
    ARTS_EXPORT void MakeScale(f32 arg1, f32 arg2, f32 arg3);

    // ?Normalize@Matrix34@@QAEXXZ
    ARTS_EXPORT void Normalize();

    // ?PolarView@Matrix34@@QAEXMMMM@Z
    ARTS_EXPORT void PolarView(f32 arg1, f32 arg2, f32 arg3, f32 arg4);

    // ?Print@Matrix34@@QBEXPBD@Z
    ARTS_IMPORT void Print(const char* arg1) const;

    // ?Rotate@Matrix34@@QAEXABVVector3@@M@Z
    ARTS_EXPORT void Rotate(const Vector3& arg1, f32 arg2);

    // ?RotateFull@Matrix34@@QAEXABVVector3@@M@Z
    ARTS_EXPORT void RotateFull(const Vector3& arg1, f32 arg2);

    // ?RotateFullUnitAxis@Matrix34@@QAEXABVVector3@@M@Z
    ARTS_EXPORT void RotateFullUnitAxis(const Vector3& arg1, f32 arg2);

    // ?RotateFullX@Matrix34@@QAEXM@Z
    ARTS_EXPORT void RotateFullX(f32 arg1);

    // ?RotateFullY@Matrix34@@QAEXM@Z
    ARTS_EXPORT void RotateFullY(f32 arg1);

    // ?RotateFullZ@Matrix34@@QAEXM@Z
    ARTS_EXPORT void RotateFullZ(f32 arg1);

    // ?RotateTo@Matrix34@@QAEXABVVector3@@0@Z
    ARTS_IMPORT void RotateTo(const Vector3& arg1, const Vector3& arg2);

    // ?RotateTo@Matrix34@@QAEXABVVector3@@0M@Z
    ARTS_IMPORT void RotateTo(const Vector3& arg1, const Vector3& arg2, f32 arg3);

    // ?RotateUnitAxis@Matrix34@@QAEXABVVector3@@M@Z
    ARTS_EXPORT void RotateUnitAxis(const Vector3& arg1, f32 arg2);

    // ?RotateX@Matrix34@@QAEXM@Z
    ARTS_EXPORT void RotateX(f32 arg1);

    // ?RotateY@Matrix34@@QAEXM@Z
    ARTS_EXPORT void RotateY(f32 arg1);

    // ?RotateZ@Matrix34@@QAEXM@Z
    ARTS_EXPORT void RotateZ(f32 arg1);

    // ?Scale@Matrix34@@QAEXM@Z
    ARTS_EXPORT void Scale(f32 arg1);

    // ?Scale@Matrix34@@QAEXMMM@Z
    ARTS_EXPORT void Scale(f32 arg1, f32 arg2, f32 arg3);

    // ?ScaleFull@Matrix34@@QAEXM@Z
    ARTS_EXPORT void ScaleFull(f32 arg1);

    // ?ScaleFull@Matrix34@@QAEXMMM@Z
    ARTS_EXPORT void ScaleFull(f32 arg1, f32 arg2, f32 arg3);

    // ?Set@Matrix34@@QAEXABV1@@Z
    ARTS_EXPORT void Set(const Matrix34& arg1);

    // ?SolveSVD@Matrix34@@QBE?AVVector3@@ABV2@@Z
    ARTS_IMPORT Vector3 SolveSVD(const Vector3& arg1) const;

    // ?Subtract@Matrix34@@QAEXABV1@0@Z
    ARTS_EXPORT void Subtract(const Matrix34& arg1, const Matrix34& arg2);

    // ?Subtract@Matrix34@@QAEXABV1@@Z
    ARTS_EXPORT void Subtract(const Matrix34& arg1);

    // ?Subtract3x3@Matrix34@@QAEXABV1@0@Z
    ARTS_EXPORT void Subtract3x3(const Matrix34& arg1, const Matrix34& arg2);

    // ?Subtract3x3@Matrix34@@QAEXABV1@@Z
    ARTS_EXPORT void Subtract3x3(const Matrix34& arg1);

    // ?ToEulers@Matrix34@@QBEXAAVVector3@@PAD@Z
    ARTS_EXPORT void ToEulers(Vector3& arg1, char* arg2) const;

    // ?ToEulersXYZ@Matrix34@@QBEXAAVVector3@@@Z
    ARTS_EXPORT void ToEulersXYZ(Vector3& arg1) const;

    // ?ToEulersXZY@Matrix34@@QBEXAAVVector3@@@Z
    ARTS_EXPORT void ToEulersXZY(Vector3& arg1) const;

    // ?ToEulersYXZ@Matrix34@@QBEXAAVVector3@@@Z
    ARTS_EXPORT void ToEulersYXZ(Vector3& arg1) const;

    // ?ToEulersYZX@Matrix34@@QBEXAAVVector3@@@Z
    ARTS_EXPORT void ToEulersYZX(Vector3& arg1) const;

    // ?ToEulersZXY@Matrix34@@QBEXAAVVector3@@@Z
    ARTS_EXPORT void ToEulersZXY(Vector3& arg1) const;

    // ?ToEulersZYX@Matrix34@@QBEXAAVVector3@@@Z
    ARTS_EXPORT void ToEulersZYX(Vector3& arg1) const;

    // ?Transform@Matrix34@@QBEXABVVector3@@AAV2@@Z
    ARTS_IMPORT void Transform(const Vector3& arg1, Vector3& arg2) const;

    // ?Transform4@Matrix34@@QBEXPBVVector3@@PAVVector4@@H@Z
    ARTS_IMPORT void Transform4(const Vector3* arg1, Vector4* arg2, i32 arg3) const;

    // ?Transpose@Matrix34@@QAEXABV1@@Z
    ARTS_EXPORT void Transpose(const Matrix34& arg1);

    // ?Transpose@Matrix34@@QAEXXZ
    ARTS_EXPORT void Transpose();

    // ?Transpose3x4@Matrix34@@QAEXABV1@@Z
    ARTS_EXPORT void Transpose3x4(const Matrix34& arg1);

    // ?Transpose3x4@Matrix34@@QAEXXZ
    ARTS_EXPORT void Transpose3x4();

    // ?Zero@Matrix34@@QAEXXZ
    ARTS_EXPORT void Zero();

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
    f32 m30; // 0x024
    f32 m31; // 0x028
    f32 m32; // 0x02C
};

check_size(Matrix34, 0x30);
