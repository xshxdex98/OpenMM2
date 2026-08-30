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

#include "matrix34.h"

#include "vector3.h"

#include <cmath>

define_dummy_symbol(vector7_matrix34);

// Transcribed from the Hex-Rays output in MM2_RE_KIT/MM2_PSEUDOCODE/Matrix34/Matrix34.c, with
// every name confirmed against midtown2.map by address rather than taken from the dump's own
// labels. The dump carries three extra bodies under MM2::Matrix34:: names (0x406780, 0x5A6090,
// 0x5A6460); none of those addresses is a Matrix34 symbol in the map, so they are part of the
// foreign symbol set the RE kit warns about and are ignored here.
//
// WHAT IS NOT HERE, AND WHY. Nine of the 82 methods stay ARTS_IMPORT; docs/ported_matrix34.md
// gives the reason for each. The short version: Print and Transpose(void) have no body in the dump
// at all, Transpose3x4(void) calls one of them through a junk `time()` label, MakeRotate's branch
// was reconstructed from x87 status-word bits into conditions that contradict each other, both
// RotateTo overloads pass the axis's own z component where the rotation angle belongs,
// FromQuaternion and Interpolate need a Quaternion member layout the IDB does not have, and
// SolveSVD and Transform4 are garbled past reading. A plausible-looking guess is worse than the
// original machine code, which at least works.
//
// CALLING CONVENTION. Hex-Rays guessed __fastcall on most of these and emitted `(this, int a2,
// ...)`, where a2 is the unused edx slot of a __thiscall. That reading is confirmed rather than
// assumed: Matrix34::Rotate calls `Dot3x3(this, (int)&v4, &v4.m00)` - the same local appears in
// both the dummy slot and the real argument slot, which only happens if the dummy is edx junk.
// Every function here has the dummy dropped and then matches its mangled signature exactly.
//
// FLOAT FIDELITY. The 1999 build is x87 at the CRT's default 53-bit precision control, so
// intermediates are held wider than f32 and round only when stored. Where the dump shows a value
// passing through a float stack slot it is written as f32 here; where it stays in an st(n) register
// it is written as f64. That distinction is load-bearing in this file - FromEulersXYZ computes
// m11 from the register copy of sz*sy and m21 from the float spill of the same product, and the
// two differ. Sub-expression grouping and accumulation order are reproduced verbatim for the same
// reason: (a*b)*c is not a*(b*c) here, and matrix code feeds physics.
//
// TRIG. cos/sin/atan2/asin are called with the float promoted to double, because that is what the
// original does - it pushes a dword onto the x87 stack and calls the double-precision helper.
// Writing std::cos(f32) would select cosf and change the result. Note that midtown2.map has
// __CIasin and __CIacos but no __CIcos, __CIsin, __CIatan2 or __CIsqrt: the 1999 compiler emitted
// those inline. A modern /arch:IA32 build calls the helpers, so core/crtshim.cpp has to supply
// them - it already does this for _CIsqrt.

// ?Warningf@@YAXPBDZZ - still the original, at 0x4C97B0. Declared here because nothing in the
// tree owns the arts print family yet; the mangled name follows from the signature.
ARTS_IMPORT void Warningf(const char* format, ...);

// The Euler extractors clamp the sine term before asin. 0.99998999f is the float 0x3F7FFF58, the
// same constant Midtown Madness 1 uses for its aligned-vector cutoff.
static constexpr f32 kEulerSinClamp = 0.99998999f;

// ?Add@Matrix34@@QAEXABV1@0@Z
void Matrix34::Add(const Matrix34& arg1, const Matrix34& arg2)
{
    // Rows 1 and 2 come out with the operands the other way round. Addition is commutative in
    // IEEE so it cannot change the result, but it is what the instruction stream does.
    m00 = arg1.m00 + arg2.m00;
    m01 = arg1.m01 + arg2.m01;
    m02 = arg1.m02 + arg2.m02;
    m10 = arg2.m10 + arg1.m10;
    m11 = arg2.m11 + arg1.m11;
    m12 = arg2.m12 + arg1.m12;
    m20 = arg1.m20 + arg2.m20;
    m21 = arg1.m21 + arg2.m21;
    m22 = arg1.m22 + arg2.m22;
    m30 = arg1.m30 + arg2.m30;
    m31 = arg1.m31 + arg2.m31;
    m32 = arg1.m32 + arg2.m32;
}

// ?Add@Matrix34@@QAEXABV1@@Z
void Matrix34::Add(const Matrix34& arg1)
{
    m00 = arg1.m00 + m00;
    m01 = arg1.m01 + m01;
    m02 = arg1.m02 + m02;
    m10 = m10 + arg1.m10;
    m11 = arg1.m11 + m11;
    m12 = arg1.m12 + m12;
    m20 = arg1.m20 + m20;
    m21 = arg1.m21 + m21;
    m22 = arg1.m22 + m22;
    m30 = m30 + arg1.m30;
    m31 = arg1.m31 + m31;
    m32 = arg1.m32 + m32;
}

// ?Add3x3@Matrix34@@QAEXABV1@0@Z
void Matrix34::Add3x3(const Matrix34& arg1, const Matrix34& arg2)
{
    m00 = arg1.m00 + arg2.m00;
    m01 = arg1.m01 + arg2.m01;
    m02 = arg1.m02 + arg2.m02;
    m10 = arg2.m10 + arg1.m10;
    m11 = arg2.m11 + arg1.m11;
    m12 = arg2.m12 + arg1.m12;
    m20 = arg1.m20 + arg2.m20;
    m21 = arg1.m21 + arg2.m21;
    m22 = arg1.m22 + arg2.m22;
}

// ?Add3x3@Matrix34@@QAEXABV1@@Z
void Matrix34::Add3x3(const Matrix34& arg1)
{
    m00 = arg1.m00 + m00;
    m01 = arg1.m01 + m01;
    m02 = arg1.m02 + m02;
    m10 = m10 + arg1.m10;
    m11 = arg1.m11 + m11;
    m12 = arg1.m12 + m12;
    m20 = arg1.m20 + m20;
    m21 = arg1.m21 + m21;
    m22 = arg1.m22 + m22;
}

// ?AddScaled@Matrix34@@QAEXABV1@M@Z
void Matrix34::AddScaled(const Matrix34& arg1, f32 arg2)
{
    // Despite having no 3x3 in the name, this touches only the rotation part - the translation row
    // is left alone. Add/Subtract do all twelve; AddScaled does nine.
    m00 = static_cast<f32>(f64 {arg2} * arg1.m00 + m00);
    m01 = static_cast<f32>(f64 {arg2} * arg1.m01 + m01);
    m02 = static_cast<f32>(f64 {arg2} * arg1.m02 + m02);
    m10 = static_cast<f32>(f64 {arg2} * arg1.m10 + m10);
    m11 = static_cast<f32>(f64 {arg2} * arg1.m11 + m11);
    m12 = static_cast<f32>(f64 {arg2} * arg1.m12 + m12);
    m20 = static_cast<f32>(f64 {arg2} * arg1.m20 + m20);
    m21 = static_cast<f32>(f64 {arg2} * arg1.m21 + m21);
    m22 = static_cast<f32>(f64 {arg2} * arg1.m22 + m22);
}

// ?Determinant3x3@Matrix34@@QBEMXZ
f32 Matrix34::Determinant3x3() const
{
    // m22 is loaded once and reused from the register; the other eight are re-read per term.
    f64 v1 = m22;

    return static_cast<f32>(f64 {m01} * m12 * m20 + f64 {m02} * m10 * m21 + f64 {m00} * m11 * v1 -
        f64 {m00} * m12 * m21 - f64 {m01} * m10 * v1 - f64 {m02} * m11 * m20);
}

// ?Dot@Matrix34@@QAEXABV1@0@Z
void Matrix34::Dot(const Matrix34& arg1, const Matrix34& arg2)
{
    // Full 3x4 multiply, result written straight out. Not aliasing-safe: nothing is cached, so
    // `a.Dot(a, b)` reads rows it has already overwritten. The one-argument overload below exists
    // precisely because this one cannot be used in place.
    m00 = arg1.m00 * arg2.m00 + arg1.m01 * arg2.m10 + arg2.m20 * arg1.m02;
    m01 = arg1.m01 * arg2.m11 + arg2.m21 * arg1.m02 + arg2.m01 * arg1.m00;
    m02 = arg1.m01 * arg2.m12 + arg2.m22 * arg1.m02 + arg2.m02 * arg1.m00;
    m10 = arg1.m11 * arg2.m10 + arg1.m10 * arg2.m00 + arg1.m12 * arg2.m20;
    m11 = arg1.m12 * arg2.m21 + arg1.m11 * arg2.m11 + arg1.m10 * arg2.m01;
    m12 = arg1.m12 * arg2.m22 + arg1.m11 * arg2.m12 + arg1.m10 * arg2.m02;
    m20 = arg1.m20 * arg2.m00 + arg2.m20 * arg1.m22 + arg1.m21 * arg2.m10;
    m21 = arg2.m21 * arg1.m22 + arg2.m11 * arg1.m21 + arg2.m01 * arg1.m20;
    m22 = arg2.m22 * arg1.m22 + arg2.m12 * arg1.m21 + arg2.m02 * arg1.m20;
    m30 = arg1.m31 * arg2.m10 + arg1.m30 * arg2.m00 + arg1.m32 * arg2.m20 + arg2.m30;
    m31 = arg1.m32 * arg2.m21 + arg1.m31 * arg2.m11 + arg1.m30 * arg2.m01 + arg2.m31;
    m32 = arg1.m32 * arg2.m22 + arg1.m31 * arg2.m12 + arg1.m30 * arg2.m02 + arg2.m32;
}

// ?Dot@Matrix34@@QAEXABV1@@Z
void Matrix34::Dot(const Matrix34& arg1)
{
    // In-place multiply: every element is computed into a temporary before any of them is stored,
    // which is what makes this safe when `this` is also read. Ten of the eleven temporaries are
    // float stack slots; only the m00 pair stays in registers, so m00 is the one element that
    // accumulates without an intermediate round.
    f64 v3 = f64 {arg1.m11} * m01 + f64 {m02} * arg1.m21 + f64 {arg1.m01} * m00;
    f32 a2a = static_cast<f32>(f64 {arg1.m12} * m01 + f64 {arg1.m22} * m02 + f64 {arg1.m02} * m00);
    f32 v14 = static_cast<f32>(f64 {arg1.m10} * m11 + f64 {m12} * arg1.m20 + f64 {m10} * arg1.m00);
    f32 v13 = static_cast<f32>(f64 {arg1.m11} * m11 + f64 {m12} * arg1.m21 + f64 {arg1.m01} * m10);
    f32 v12 = static_cast<f32>(f64 {arg1.m12} * m11 + f64 {arg1.m02} * m10 + f64 {arg1.m22} * m12);
    f32 v11 = static_cast<f32>(f64 {arg1.m10} * m21 + f64 {m20} * arg1.m00 + f64 {arg1.m20} * m22);
    f32 v10 = static_cast<f32>(f64 {arg1.m11} * m21 + f64 {arg1.m01} * m20 + f64 {arg1.m21} * m22);
    f32 v9 = static_cast<f32>(f64 {arg1.m22} * m22 + f64 {arg1.m12} * m21 + f64 {arg1.m02} * m20);
    f32 v8 = static_cast<f32>(
        f64 {arg1.m10} * m31 + f64 {m30} * arg1.m00 + f64 {m32} * arg1.m20 + arg1.m30);
    f32 v7 = static_cast<f32>(
        f64 {arg1.m11} * m31 + f64 {arg1.m01} * m30 + f64 {m32} * arg1.m21 + arg1.m31);
    f32 v6 = static_cast<f32>(
        f64 {arg1.m22} * m32 + f64 {arg1.m12} * m31 + f64 {arg1.m02} * m30 + arg1.m32);
    f64 v4 = f64 {arg1.m10} * m01 + f64 {arg1.m00} * m00;
    f64 v5 = f64 {m02} * arg1.m20;

    m02 = a2a;
    m00 = static_cast<f32>(v4 + v5);
    m01 = static_cast<f32>(v3);
    m10 = v14;
    m11 = v13;
    m12 = v12;
    m20 = v11;
    m21 = v10;
    m22 = v9;
    m30 = v8;
    m31 = v7;
    m32 = v6;
}

// ?Dot3x3@Matrix34@@QAEXABV1@0@Z
void Matrix34::Dot3x3(const Matrix34& arg1, const Matrix34& arg2)
{
    m00 = arg1.m00 * arg2.m00 + arg1.m01 * arg2.m10 + arg2.m20 * arg1.m02;
    m01 = arg1.m01 * arg2.m11 + arg2.m21 * arg1.m02 + arg2.m01 * arg1.m00;
    m02 = arg1.m01 * arg2.m12 + arg2.m22 * arg1.m02 + arg2.m02 * arg1.m00;
    m10 = arg1.m11 * arg2.m10 + arg1.m10 * arg2.m00 + arg1.m12 * arg2.m20;
    m11 = arg1.m12 * arg2.m21 + arg1.m11 * arg2.m11 + arg1.m10 * arg2.m01;
    m12 = arg1.m12 * arg2.m22 + arg1.m11 * arg2.m12 + arg1.m10 * arg2.m02;
    m20 = arg1.m20 * arg2.m00 + arg2.m20 * arg1.m22 + arg1.m21 * arg2.m10;
    m21 = arg2.m21 * arg1.m22 + arg2.m11 * arg1.m21 + arg2.m01 * arg1.m20;
    m22 = arg2.m22 * arg1.m22 + arg2.m12 * arg1.m21 + arg2.m02 * arg1.m20;
}

// ?Dot3x3@Matrix34@@QAEXABV1@@Z
void Matrix34::Dot3x3(const Matrix34& arg1)
{
    // As Dot(const Matrix34&), minus the translation row - and here m00 is the element computed
    // last and stored first, rather than the one held in registers.
    f64 v4 = f64 {arg1.m11} * m01 + f64 {m02} * arg1.m21 + f64 {arg1.m01} * m00;
    f32 v12 = static_cast<f32>(f64 {arg1.m12} * m01 + f64 {arg1.m22} * m02 + f64 {arg1.m02} * m00);
    f32 v11 = static_cast<f32>(f64 {arg1.m10} * m11 + f64 {m12} * arg1.m20 + f64 {m10} * arg1.m00);
    f32 v10 = static_cast<f32>(f64 {arg1.m11} * m11 + f64 {m12} * arg1.m21 + f64 {arg1.m01} * m10);
    f32 v9 = static_cast<f32>(f64 {arg1.m12} * m11 + f64 {arg1.m02} * m10 + f64 {arg1.m22} * m12);
    f32 v8 = static_cast<f32>(f64 {arg1.m10} * m21 + f64 {m20} * arg1.m00 + f64 {arg1.m20} * m22);
    f32 v7 = static_cast<f32>(f64 {arg1.m11} * m21 + f64 {arg1.m01} * m20 + f64 {arg1.m21} * m22);
    f32 v6 = static_cast<f32>(f64 {arg1.m22} * m22 + f64 {arg1.m12} * m21 + f64 {arg1.m02} * m20);

    m00 = static_cast<f32>(f64 {arg1.m10} * m01 + f64 {arg1.m00} * m00 + f64 {m02} * arg1.m20);
    m01 = static_cast<f32>(v4);
    m02 = v12;
    m10 = v11;
    m11 = v10;
    m12 = v9;
    m20 = v8;
    m21 = v7;
    m22 = v6;
}

// ?Dot3x3CrossProdMtx@Matrix34@@QAEXABVVector3@@@Z
void Matrix34::Dot3x3CrossProdMtx(const Vector3& arg1)
{
    // Each row of the 3x3 is replaced by its cross product with arg1. Row by row, with the y term
    // rounded through a float slot and x/z kept in registers - the same shape as Vector3::Cross,
    // and the same store order (y, z, x) for the same aliasing reason.
    f64 v4 = f64 {m01} * arg1.z - f64 {arg1.y} * m02;
    f32 v14 = static_cast<f32>(f64 {arg1.x} * m02 - f64 {m00} * arg1.z);
    f64 v5 = f64 {arg1.y} * m00;
    f64 v6 = f64 {m01} * arg1.x;

    m01 = v14;
    m02 = static_cast<f32>(v5 - v6);
    m00 = static_cast<f32>(v4);

    f64 v7 = f64 {m11} * arg1.z - f64 {arg1.y} * m12;
    f32 v15 = static_cast<f32>(f64 {m12} * arg1.x - f64 {m10} * arg1.z);
    f64 v8 = f64 {arg1.y} * m10;
    f64 v9 = f64 {m11} * arg1.x;

    m11 = v15;
    m12 = static_cast<f32>(v8 - v9);
    m10 = static_cast<f32>(v7);

    f64 v10 = f64 {m21} * arg1.z - f64 {arg1.y} * m22;
    f32 v16 = static_cast<f32>(f64 {arg1.x} * m22 - f64 {m20} * arg1.z);
    f64 v11 = f64 {arg1.y} * m20;
    f64 v12 = f64 {arg1.x} * m21;

    m21 = v16;
    m22 = static_cast<f32>(v11 - v12);
    m20 = static_cast<f32>(v10);
}

// ?Dot3x3CrossProdTranspose@Matrix34@@QAEXABVVector3@@@Z
void Matrix34::Dot3x3CrossProdTranspose(const Vector3& arg1)
{
    f64 v4 = f64 {m02} * arg1.y - f64 {arg1.z} * m01;
    f32 v14 = static_cast<f32>(f64 {arg1.z} * m00 - f64 {m02} * arg1.x);
    f64 v5 = f64 {arg1.x} * m01;
    f64 v6 = f64 {m00} * arg1.y;

    m01 = v14;
    m02 = static_cast<f32>(v5 - v6);
    m00 = static_cast<f32>(v4);

    f64 v7 = f64 {m12} * arg1.y - f64 {arg1.z} * m11;
    f32 v15 = static_cast<f32>(f64 {arg1.z} * m10 - f64 {m12} * arg1.x);
    f64 v8 = f64 {m11} * arg1.x;
    f64 v9 = f64 {m10} * arg1.y;

    m11 = v15;
    m12 = static_cast<f32>(v8 - v9);
    m10 = static_cast<f32>(v7);

    f64 v10 = f64 {m22} * arg1.y - f64 {arg1.z} * m21;
    f32 v16 = static_cast<f32>(f64 {arg1.z} * m20 - f64 {m22} * arg1.x);
    f64 v11 = f64 {m21} * arg1.x;
    f64 v12 = f64 {m20} * arg1.y;

    m21 = v16;
    m22 = static_cast<f32>(v11 - v12);
    m20 = static_cast<f32>(v10);
}

// ?Dot3x3Transpose@Matrix34@@QAEXABV1@0@Z
void Matrix34::Dot3x3Transpose(const Matrix34& arg1, const Matrix34& arg2)
{
    m00 = arg1.m00 * arg2.m00 + arg1.m01 * arg2.m01 + arg2.m02 * arg1.m02;
    m01 = arg1.m01 * arg2.m11 + arg2.m12 * arg1.m02 + arg2.m10 * arg1.m00;
    m02 = arg1.m01 * arg2.m21 + arg2.m22 * arg1.m02 + arg2.m20 * arg1.m00;
    m10 = arg1.m11 * arg2.m01 + arg1.m10 * arg2.m00 + arg1.m12 * arg2.m02;
    m11 = arg1.m12 * arg2.m12 + arg1.m11 * arg2.m11 + arg1.m10 * arg2.m10;
    m12 = arg1.m12 * arg2.m22 + arg1.m11 * arg2.m21 + arg1.m10 * arg2.m20;
    m20 = arg1.m20 * arg2.m00 + arg2.m02 * arg1.m22 + arg1.m21 * arg2.m01;
    m21 = arg2.m12 * arg1.m22 + arg2.m11 * arg1.m21 + arg2.m10 * arg1.m20;
    m22 = arg2.m22 * arg1.m22 + arg2.m21 * arg1.m21 + arg2.m20 * arg1.m20;
}

// ?Dot3x3Transpose@Matrix34@@QAEXABV1@@Z
void Matrix34::Dot3x3Transpose(const Matrix34& arg1)
{
    f64 v4 = f64 {arg1.m01} * m01 + f64 {arg1.m00} * m00 + f64 {m02} * arg1.m02;
    f32 v14 = static_cast<f32>(f64 {arg1.m12} * m02 + f64 {arg1.m10} * m00 + f64 {arg1.m11} * m01);
    f64 v5 = f64 {arg1.m21} * m01 + f64 {arg1.m22} * m02;
    f64 v6 = f64 {m00} * arg1.m20;

    m01 = v14;
    m02 = static_cast<f32>(v5 + v6);
    m00 = static_cast<f32>(v4);

    f64 v7 = f64 {arg1.m01} * m11 + f64 {m12} * arg1.m02 + f64 {m10} * arg1.m00;
    f32 v15 = static_cast<f32>(f64 {arg1.m10} * m10 + f64 {m12} * arg1.m12 + f64 {m11} * arg1.m11);
    f64 v8 = f64 {arg1.m22} * m12 + f64 {arg1.m21} * m11;
    f64 v9 = f64 {m10} * arg1.m20;

    m11 = v15;
    m12 = static_cast<f32>(v8 + v9);
    m10 = static_cast<f32>(v7);

    f64 v10 = f64 {m20} * arg1.m00 + f64 {arg1.m01} * m21 + f64 {m22} * arg1.m02;
    f32 v16 = static_cast<f32>(f64 {arg1.m10} * m20 + f64 {m22} * arg1.m12 + f64 {m21} * arg1.m11);
    f64 v11 = f64 {arg1.m22} * m22 + f64 {arg1.m21} * m21;
    f64 v12 = f64 {m20} * arg1.m20;

    m21 = v16;
    m22 = static_cast<f32>(v11 + v12);
    m20 = static_cast<f32>(v10);
}

// ?DotTranspose@Matrix34@@QAEXABV1@0@Z
void Matrix34::DotTranspose(const Matrix34& arg1, const Matrix34& arg2)
{
    m00 = arg1.m00 * arg2.m00 + arg1.m01 * arg2.m01 + arg2.m02 * arg1.m02;
    m01 = arg1.m01 * arg2.m11 + arg2.m12 * arg1.m02 + arg2.m10 * arg1.m00;
    m02 = arg1.m01 * arg2.m21 + arg2.m22 * arg1.m02 + arg2.m20 * arg1.m00;
    m10 = arg1.m11 * arg2.m01 + arg1.m10 * arg2.m00 + arg1.m12 * arg2.m02;
    m11 = arg1.m12 * arg2.m12 + arg1.m11 * arg2.m11 + arg1.m10 * arg2.m10;
    m12 = arg1.m12 * arg2.m22 + arg1.m11 * arg2.m21 + arg1.m10 * arg2.m20;
    m20 = arg1.m20 * arg2.m00 + arg2.m02 * arg1.m22 + arg1.m21 * arg2.m01;
    m21 = arg2.m12 * arg1.m22 + arg2.m11 * arg1.m21 + arg2.m10 * arg1.m20;
    m22 = arg2.m22 * arg1.m22 + arg2.m21 * arg1.m21 + arg2.m20 * arg1.m20;

    // The translation is the difference of the two, pushed through arg2's transpose. The three
    // differences stay in registers here; the in-place overload writes them to memory first.
    f64 v5 = f64 {arg1.m30} - arg2.m30;
    f64 v6 = f64 {arg1.m31} - arg2.m31;
    f64 v7 = f64 {arg1.m32} - arg2.m32;

    m30 = static_cast<f32>(v7 * arg2.m02 + v5 * arg2.m00 + v6 * arg2.m01);
    m31 = static_cast<f32>(v5 * arg2.m10 + v7 * arg2.m12 + v6 * arg2.m11);
    m32 = static_cast<f32>(v5 * arg2.m20 + v7 * arg2.m22 + v6 * arg2.m21);
}

// ?DotTranspose@Matrix34@@QAEXABV1@@Z
void Matrix34::DotTranspose(const Matrix34& arg1)
{
    f64 v4 = f64 {arg1.m01} * m01 + f64 {arg1.m00} * m00 + f64 {m02} * arg1.m02;
    f32 v17 = static_cast<f32>(f64 {arg1.m12} * m02 + f64 {arg1.m10} * m00 + f64 {arg1.m11} * m01);
    f64 v5 = f64 {arg1.m21} * m01 + f64 {arg1.m22} * m02;
    f64 v6 = f64 {m00} * arg1.m20;

    m01 = v17;
    m02 = static_cast<f32>(v5 + v6);
    m00 = static_cast<f32>(v4);

    f64 v7 = f64 {arg1.m01} * m11 + f64 {m12} * arg1.m02 + f64 {m10} * arg1.m00;
    f32 v18 = static_cast<f32>(f64 {arg1.m10} * m10 + f64 {m12} * arg1.m12 + f64 {m11} * arg1.m11);
    f64 v8 = f64 {arg1.m22} * m12 + f64 {arg1.m21} * m11;
    f64 v9 = f64 {m10} * arg1.m20;

    m11 = v18;
    m12 = static_cast<f32>(v8 + v9);
    m10 = static_cast<f32>(v7);

    f64 v10 = f64 {m20} * arg1.m00 + f64 {arg1.m01} * m21 + f64 {m22} * arg1.m02;
    f32 v19 = static_cast<f32>(f64 {arg1.m10} * m20 + f64 {m22} * arg1.m12 + f64 {m21} * arg1.m11);
    f64 v11 = f64 {arg1.m22} * m22 + f64 {arg1.m21} * m21;
    f64 v12 = f64 {m20} * arg1.m20;

    m21 = v19;
    m22 = static_cast<f32>(v11 + v12);
    m20 = static_cast<f32>(v10);

    // The subtraction is done in place, into the member, before the transform reads it back - so
    // the difference is rounded to f32 here and is not in the two-argument overload.
    m30 = m30 - arg1.m30;
    m31 = m31 - arg1.m31;
    m32 = m32 - arg1.m32;

    f64 v13 = f64 {arg1.m01} * m31 + f64 {m32} * arg1.m02 + f64 {m30} * arg1.m00;
    f32 v20 = static_cast<f32>(f64 {m30} * arg1.m10 + f64 {m32} * arg1.m12 + f64 {m31} * arg1.m11);
    f64 v14 = f64 {arg1.m22} * m32 + f64 {arg1.m21} * m31;
    f64 v15 = f64 {m30} * arg1.m20;

    m31 = v20;
    m32 = static_cast<f32>(v14 + v15);
    m30 = static_cast<f32>(v13);
}

// ?FastInverse@Matrix34@@QAEXABV1@@Z
void Matrix34::FastInverse(const Matrix34& arg1)
{
    // Inverse of a rigid transform: transpose the 3x3, and negate the translation expressed in the
    // transposed frame. Written element by element with no caching, so unlike Midtown Madness 1's
    // version this is NOT safe when arg1 aliases *this - m10 is overwritten before arg1.m10 is
    // read. The void overload below is what makes the in-place case work, by copying first.
    f64 m32_ = arg1.m32;
    f64 m00_ = arg1.m00;
    f32 m30_ = arg1.m30;
    f32 m31_ = arg1.m31;

    f32 v11 = static_cast<f32>(m00_ * m30_);
    m00 = static_cast<f32>(m00_);
    f32 v12 = static_cast<f32>(f64 {arg1.m01} * m31_ + v11);
    m10 = arg1.m01;
    f64 m02_ = arg1.m02;
    m20 = static_cast<f32>(m02_);
    m30 = static_cast<f32>(-(m02_ * m32_ + v12));

    f32 v13 = static_cast<f32>(f64 {arg1.m10} * m30_);
    m01 = arg1.m10;
    f32 v14 = static_cast<f32>(f64 {arg1.m11} * m31_ + v13);
    m11 = arg1.m11;
    f64 m12_ = arg1.m12;
    m21 = static_cast<f32>(m12_);
    m31 = static_cast<f32>(-(m12_ * m32_ + v14));

    f32 v15 = static_cast<f32>(f64 {arg1.m20} * m30_);
    m02 = arg1.m20;
    f32 v16 = static_cast<f32>(f64 {arg1.m21} * m31_ + v15);
    m12 = arg1.m21;
    f64 m22_ = arg1.m22;
    m22 = static_cast<f32>(m22_);
    m32 = static_cast<f32>(-(m22_ * m32_ + v16));
}

// ?FastInverse@Matrix34@@QAEXXZ
void Matrix34::FastInverse()
{
    Matrix34 v3 = *this;

    FastInverse(v3);
}

// ?FromEulers@Matrix34@@QAEXABVVector3@@PAD@Z
void Matrix34::FromEulers(const Vector3& arg1, char* arg2)
{
    char v3 = arg2[0];

    if (v3 == 'x')
    {
        char v4 = arg2[1];

        if (v4 == 'y' && arg2[2] == 'z')
            FromEulersXYZ(arg1);
        else if (v4 == 'z' && arg2[2] == 'y')
            FromEulersXZY(arg1);
        else
            Warningf("Matrix34::FromEulers()- Bad string '%s'", arg2);
    }
    else if (v3 == 'y')
    {
        char v5 = arg2[1];

        if (v5 == 'x' && arg2[2] == 'z')
            FromEulersYXZ(arg1);
        else if (v5 == 'z' && arg2[2] == 'x')
            FromEulersYZX(arg1);
        else
            Warningf("Matrix34::FromEulers()- Bad string '%s'", arg2);
    }
    else if (v3 == 'z')
    {
        char v6 = arg2[1];

        if (v6 == 'x' && arg2[2] == 'y')
            FromEulersZXY(arg1);
        else if (v6 == 'y' && arg2[2] == 'x')
            FromEulersZYX(arg1);
        else
            Warningf("Matrix34::FromEulers()- Bad string '%s'", arg2);
    }
    else
    {
        Warningf("Matrix34::FromEulers()- Bad string '%s'", arg2);
    }
}

// ?FromEulersXYZ@Matrix34@@QAEXABVVector3@@@Z
void Matrix34::FromEulersXYZ(const Vector3& arg1)
{
    // All six FromEulers* share this preamble: an exactly-zero angle short-circuits to cos=1,
    // sin=0 rather than calling the library. In every one of them the cosines stay in x87
    // registers and the sines go through float stack slots, so the sines are the rounded ones.
    f64 v4;
    f32 v14;

    if (arg1.x == 0.0f)
    {
        v4 = 1.0;
        v14 = 0.0f;
    }
    else
    {
        v4 = std::cos(f64 {arg1.x});
        v14 = static_cast<f32>(std::sin(f64 {arg1.x}));
    }

    f64 v5;
    f32 v10;

    if (arg1.y == 0.0f)
    {
        v5 = 1.0;
        v10 = 0.0f;
    }
    else
    {
        v5 = std::cos(f64 {arg1.y});
        v10 = static_cast<f32>(std::sin(f64 {arg1.y}));
    }

    f64 v6;
    f32 v13;

    if (arg1.z == 0.0f)
    {
        v6 = 1.0;
        v13 = 0.0f;
    }
    else
    {
        v6 = std::cos(f64 {arg1.z});
        v13 = static_cast<f32>(std::sin(f64 {arg1.z}));
    }

    m00 = static_cast<f32>(v6 * v5);
    m01 = static_cast<f32>(f64 {v13} * v5);
    m02 = -v10;

    f64 v8 = v6 * v10;

    m10 = static_cast<f32>(v8 * v14 - f64 {v13} * v4);

    // sz*sy is computed once, kept in a register for m11 and spilled to a float slot for m21.
    // The two therefore see different values, and that is not a transcription error.
    f64 v9 = f64 {v13} * v10;
    f32 v12 = static_cast<f32>(v9);

    m11 = static_cast<f32>(v9 * v14 + v6 * v4);
    m12 = static_cast<f32>(v5 * v14);
    m20 = static_cast<f32>(v8 * v4 + f64 {v13} * v14);
    m21 = static_cast<f32>(f64 {v12} * v4 - v6 * v14);
    m22 = static_cast<f32>(v5 * v4);
}

// ?FromEulersXZY@Matrix34@@QAEXABVVector3@@@Z
void Matrix34::FromEulersXZY(const Vector3& arg1)
{
    f64 v4;
    f32 v13;

    if (arg1.x == 0.0f)
    {
        v4 = 1.0;
        v13 = 0.0f;
    }
    else
    {
        v4 = std::cos(f64 {arg1.x});
        v13 = static_cast<f32>(std::sin(f64 {arg1.x}));
    }

    f64 v5;
    f32 v11;

    if (arg1.y == 0.0f)
    {
        v5 = 1.0;
        v11 = 0.0f;
    }
    else
    {
        v5 = std::cos(f64 {arg1.y});
        v11 = static_cast<f32>(std::sin(f64 {arg1.y}));
    }

    // Unlike the other five, sin(z) stays in a register here rather than being spilled.
    f64 v6;
    f64 v7;

    if (arg1.z == 0.0f)
    {
        v6 = 1.0;
        v7 = 0.0;
    }
    else
    {
        v6 = std::cos(f64 {arg1.z});
        v7 = std::sin(f64 {arg1.z});
    }

    m00 = static_cast<f32>(v6 * v5);
    m01 = static_cast<f32>(v7);
    m02 = static_cast<f32>(-(v6 * v11));
    m10 = static_cast<f32>(f64 {v11} * v13 - v7 * v5 * v4);
    m11 = static_cast<f32>(v6 * v4);

    f64 v8 = v7 * v11;
    f32 v10 = static_cast<f32>(v8);

    m12 = static_cast<f32>(v8 * v4 + v5 * v13);
    m20 = static_cast<f32>(v7 * v5 * v13 + f64 {v11} * v4);
    m21 = static_cast<f32>(-(v6 * v13));
    m22 = static_cast<f32>(v5 * v4 - f64 {v10} * v13);
}

// ?FromEulersYXZ@Matrix34@@QAEXABVVector3@@@Z
void Matrix34::FromEulersYXZ(const Vector3& arg1)
{
    f64 v4;
    f32 v16;

    if (arg1.x == 0.0f)
    {
        v4 = 1.0;
        v16 = 0.0f;
    }
    else
    {
        v4 = std::cos(f64 {arg1.x});
        v16 = static_cast<f32>(std::sin(f64 {arg1.x}));
    }

    f64 v5;
    f32 v15;

    if (arg1.y == 0.0f)
    {
        v5 = 1.0;
        v15 = 0.0f;
    }
    else
    {
        v5 = std::cos(f64 {arg1.y});
        v15 = static_cast<f32>(std::sin(f64 {arg1.y}));
    }

    f64 v6;
    f32 v14;

    if (arg1.z == 0.0f)
    {
        v6 = 1.0;
        v14 = 0.0f;
    }
    else
    {
        v6 = std::cos(f64 {arg1.z});
        v14 = static_cast<f32>(std::sin(f64 {arg1.z}));
    }

    f32 v10 = static_cast<f32>(v6 * v5);
    f64 v8 = f64 {v14} * v15;
    f32 v9 = static_cast<f32>(v8);

    m00 = static_cast<f32>(f64 {v10} - v8 * v16);

    f32 v11 = static_cast<f32>(v6 * v15);
    f32 v12 = static_cast<f32>(f64 {v14} * v5);

    m01 = static_cast<f32>(f64 {v11} * v16 + v12);
    m02 = static_cast<f32>(-(f64 {v15} * v4));
    m10 = static_cast<f32>(-(f64 {v14} * v4));
    m11 = static_cast<f32>(v6 * v4);
    m12 = v16;
    m20 = static_cast<f32>(f64 {v12} * v16 + v11);
    m21 = static_cast<f32>(f64 {v9} - f64 {v10} * v16);
    m22 = static_cast<f32>(v5 * v4);
}

// ?FromEulersYZX@Matrix34@@QAEXABVVector3@@@Z
void Matrix34::FromEulersYZX(const Vector3& arg1)
{
    f64 v4;
    f32 v13;

    if (arg1.x == 0.0f)
    {
        v4 = 1.0;
        v13 = 0.0f;
    }
    else
    {
        v4 = std::cos(f64 {arg1.x});
        v13 = static_cast<f32>(std::sin(f64 {arg1.x}));
    }

    // sin(y) stays in a register in this one.
    f64 v5;
    f64 v6;

    if (arg1.y == 0.0f)
    {
        v5 = 1.0;
        v6 = 0.0;
    }
    else
    {
        v5 = std::cos(f64 {arg1.y});
        v6 = std::sin(f64 {arg1.y});
    }

    f64 v7;
    f32 v12;

    if (arg1.z == 0.0f)
    {
        v7 = 1.0;
        v12 = 0.0f;
    }
    else
    {
        v7 = std::cos(f64 {arg1.z});
        v12 = static_cast<f32>(std::sin(f64 {arg1.z}));
    }

    m00 = static_cast<f32>(v7 * v5);

    f64 v9 = f64 {v12} * v5;

    m01 = static_cast<f32>(v9 * v4 + v6 * v13);
    m02 = static_cast<f32>(v9 * v13 - v6 * v4);
    m10 = -v12;
    m11 = static_cast<f32>(v7 * v4);
    m12 = static_cast<f32>(v7 * v13);
    m20 = static_cast<f32>(v7 * v6);

    f64 v10 = v6 * v12;

    m21 = static_cast<f32>(v10 * v4 - v5 * v13);
    m22 = static_cast<f32>(v10 * v13 + v5 * v4);
}

// ?FromEulersZXY@Matrix34@@QAEXABVVector3@@@Z
void Matrix34::FromEulersZXY(const Vector3& arg1)
{
    f64 v4;
    f32 v15;

    if (arg1.x == 0.0f)
    {
        v4 = 1.0;
        v15 = 0.0f;
    }
    else
    {
        v4 = std::cos(f64 {arg1.x});
        v15 = static_cast<f32>(std::sin(f64 {arg1.x}));
    }

    f64 v5;
    f32 v13;

    if (arg1.y == 0.0f)
    {
        v5 = 1.0;
        v13 = 0.0f;
    }
    else
    {
        v5 = std::cos(f64 {arg1.y});
        v13 = static_cast<f32>(std::sin(f64 {arg1.y}));
    }

    f64 v6;
    f64 v7;

    if (arg1.z == 0.0f)
    {
        v6 = 1.0;
        v7 = 0.0;
    }
    else
    {
        v6 = std::cos(f64 {arg1.z});
        v7 = std::sin(f64 {arg1.z});
    }

    f32 v10 = static_cast<f32>(v7 * v13);
    f32 v12 = static_cast<f32>(v6 * v5);

    m00 = static_cast<f32>(f64 {v10} * v15 + v12);
    m01 = static_cast<f32>(v7 * v4);

    f64 v8 = v7 * v5;
    f64 v9 = v6 * v13;

    m02 = static_cast<f32>(v8 * v15 - v9);
    m10 = static_cast<f32>(v9 * v15 - v8);
    m11 = static_cast<f32>(v6 * v4);
    m12 = static_cast<f32>(f64 {v12} * v15 + v10);
    m20 = static_cast<f32>(f64 {v13} * v4);
    m21 = -v15;
    m22 = static_cast<f32>(v5 * v4);
}

// ?FromEulersZYX@Matrix34@@QAEXABVVector3@@@Z
void Matrix34::FromEulersZYX(const Vector3& arg1)
{
    f64 v4;
    f32 v13;

    if (arg1.x == 0.0f)
    {
        v4 = 1.0;
        v13 = 0.0f;
    }
    else
    {
        v4 = std::cos(f64 {arg1.x});
        v13 = static_cast<f32>(std::sin(f64 {arg1.x}));
    }

    f64 v5;
    f32 v11;

    if (arg1.y == 0.0f)
    {
        v5 = 1.0;
        v11 = 0.0f;
    }
    else
    {
        v5 = std::cos(f64 {arg1.y});
        v11 = static_cast<f32>(std::sin(f64 {arg1.y}));
    }

    f64 v6;
    f64 v7;

    if (arg1.z == 0.0f)
    {
        v6 = 1.0;
        v7 = 0.0;
    }
    else
    {
        v6 = std::cos(f64 {arg1.z});
        v7 = std::sin(f64 {arg1.z});
    }

    m00 = static_cast<f32>(v6 * v5);

    f64 v8 = v6 * v11;

    m01 = static_cast<f32>(v8 * v13 + v7 * v4);
    m02 = static_cast<f32>(v7 * v13 - v8 * v4);
    m10 = static_cast<f32>(-(v7 * v5));

    f64 v9 = v7 * v11;

    m11 = static_cast<f32>(v6 * v4 - v9 * v13);
    m12 = static_cast<f32>(v9 * v4 + v6 * v13);
    m20 = v11;
    m21 = static_cast<f32>(-(v5 * v13));
    m22 = static_cast<f32>(v5 * v4);
}

// ?GetEulers@Matrix34@@QBE?AVVector3@@PBD@Z
Vector3 Matrix34::GetEulers(const char* arg1) const
{
    // The switch is on arg1[2], and the remaining two characters are then checked in reverse - so
    // the case labels below read backwards relative to the order string they accept. The comment
    // on each branch gives the string, not the label.
    Vector3 result;

    switch (arg1[2])
    {
        case 'x': {
            char v6 = arg1[1];

            if (v6 == 'y' && arg1[0] == 'z') // "zyx"
            {
                f32 v16 = static_cast<f32>(std::atan2(-f64 {m21}, f64 {m22}));
                f32 v22 = static_cast<f32>(std::asin(f64 {m20}));
                f64 v7 = std::atan2(-f64 {m10}, f64 {m00});

                result.x = v16;
                result.y = v22;
                result.z = static_cast<f32>(v7);

                return result;
            }

            if (v6 == 'z' && arg1[0] == 'y') // "yzx"
            {
                f32 v17 = static_cast<f32>(std::atan2(f64 {m12}, f64 {m11}));
                f32 v23 = static_cast<f32>(std::atan2(f64 {m20}, f64 {m00}));
                f64 v9 = std::asin(-f64 {m10});

                result.x = v17;
                result.y = v23;
                result.z = static_cast<f32>(v9);

                return result;
            }

            break;
        }

        case 'y': {
            char v10 = arg1[1];

            if (v10 == 'x' && arg1[0] == 'z') // "zxy"
            {
                f32 v18 = static_cast<f32>(std::asin(-f64 {m21}));
                f32 v24 = static_cast<f32>(std::atan2(f64 {m20}, f64 {m22}));
                f64 v11 = std::atan2(f64 {m01}, f64 {m11});

                result.x = v18;
                result.y = v24;
                result.z = static_cast<f32>(v11);

                return result;
            }

            if (v10 == 'z' && arg1[0] == 'x') // "xzy"
            {
                f32 v19 = static_cast<f32>(std::atan2(-f64 {m21}, f64 {m11}));
                f32 v25 = static_cast<f32>(std::atan2(-f64 {m02}, f64 {m00}));
                f64 v12 = std::asin(f64 {m01});

                result.x = v19;
                result.y = v25;
                result.z = static_cast<f32>(v12);

                return result;
            }

            break;
        }

        case 'z': {
            char v13 = arg1[1];

            if (v13 == 'x' && arg1[0] == 'y') // "yxz"
            {
                f32 v20 = static_cast<f32>(std::asin(f64 {m12}));
                f32 v26 = static_cast<f32>(std::atan2(-f64 {m02}, f64 {m22}));
                f64 v14 = std::atan2(-f64 {m10}, f64 {m11});

                result.x = v20;
                result.y = v26;
                result.z = static_cast<f32>(v14);

                return result;
            }

            if (v13 == 'y' && arg1[0] == 'x') // "xyz"
            {
                f32 v21 = static_cast<f32>(std::atan2(f64 {m12}, f64 {m22}));
                f32 v27 = static_cast<f32>(std::asin(-f64 {m02}));
                f64 v15 = std::atan2(f64 {m01}, f64 {m00});

                result.x = v21;
                result.y = v27;
                result.z = static_cast<f32>(v15);

                return result;
            }

            break;
        }
    }

    Warningf("Matrix34::GetEulers() - order variable is invalid");

    result.x = 0.0f;
    result.y = 0.0f;
    result.z = 0.0f;

    return result;
}

// ?GetEulers@Matrix34@@QBE?AVVector3@@XZ
Vector3 Matrix34::GetEulers() const
{
    // The "xyz" branch of the overload above, without the dispatch or the degenerate-input guards.
    f32 v5 = static_cast<f32>(std::atan2(f64 {m12}, f64 {m22}));
    f32 v6 = static_cast<f32>(std::asin(-f64 {m02}));
    f64 v3 = std::atan2(f64 {m01}, f64 {m00});

    Vector3 result;

    result.x = v5;
    result.y = v6;
    result.z = static_cast<f32>(v3);

    return result;
}

// ?GetLookAt@Matrix34@@QBEXPAVVector3@@0M@Z
void Matrix34::GetLookAt(Vector3* arg1, Vector3* arg2, f32 arg3) const
{
    // The eye is copied as three dwords rather than through the FPU - which matters only in that
    // a signalling NaN in the translation row would survive intact.
    arg1->x = m30;
    arg1->y = m31;
    arg1->z = m32;

    arg2->x = static_cast<f32>(f64 {m30} - f64 {arg3} * m20);
    arg2->y = static_cast<f32>(f64 {m31} - f64 {arg3} * m21);
    arg2->z = static_cast<f32>(f64 {m32} - f64 {arg3} * m22);
}

// ?GetPolar@Matrix34@@QBEXPAVVector4@@PAVVector3@@M@Z
void Matrix34::GetPolar(Vector4* arg1, Vector3* arg2, f32 arg3) const
{
    Vector3 v4;
    Vector3 v5;

    GetLookAt(&v4, &v5, arg3);
    v4.GetPolar(v5, arg1, arg2);
}

// ?Identity@Matrix34@@QAEXXZ
void Matrix34::Identity()
{
    m00 = 1.0f;
    m01 = 0.0f;
    m02 = 0.0f;
    m10 = 0.0f;
    m11 = 1.0f;
    m12 = 0.0f;
    m20 = 0.0f;
    m21 = 0.0f;
    m22 = 1.0f;
    m30 = 0.0f;
    m31 = 0.0f;
    m32 = 0.0f;
}

// ?Identity3x3@Matrix34@@QAEXXZ
void Matrix34::Identity3x3()
{
    m00 = 1.0f;
    m01 = 0.0f;
    m02 = 0.0f;
    m10 = 0.0f;
    m11 = 1.0f;
    m12 = 0.0f;
    m20 = 0.0f;
    m21 = 0.0f;
    m22 = 1.0f;
}

// ?Inverse@Matrix34@@QAEXABV1@@Z
void Matrix34::Inverse(const Matrix34& arg1)
{
    *this = arg1;

    Inverse();
}

// ?Inverse@Matrix34@@QAEXXZ
void Matrix34::Inverse()
{
    // General affine inverse through the adjugate. Two of the three first-column cofactors are
    // spilled to float slots and the third stays in a register, so the determinant is not built
    // from three equally-rounded terms.
    f64 v1 = f64 {m11} * m22 - f64 {m21} * m12;
    f32 v6 = static_cast<f32>(f64 {m10} * m22 - f64 {m20} * m12);
    f32 v5 = static_cast<f32>(f64 {m21} * m10 - f64 {m20} * m11);
    f64 v2 = v1 * m00 - f64 {v6} * m01 + f64 {v5} * m02;

    if (v2 == 0.0)
    {
        // Singular: warn and leave the matrix exactly as it was.
        Warningf("Matrix34::Inverse()- Matrix not invertable");
    }
    else
    {
        f64 v3 = 1.0 / v2;
        Matrix34 v4;

        v4.m00 = static_cast<f32>(v3 * v1);
        v4.m10 = static_cast<f32>(-(v3 * v6));
        v4.m20 = static_cast<f32>(v3 * v5);
        v4.m30 = static_cast<f32>(-(f64 {v4.m00} * m30 + f64 {v4.m20} * m32 + f64 {v4.m10} * m31));
        v4.m01 = static_cast<f32>(-((f64 {m01} * m22 - f64 {m21} * m02) * v3));
        v4.m11 = static_cast<f32>((f64 {m00} * m22 - f64 {m20} * m02) * v3);
        v4.m21 = static_cast<f32>(-((f64 {m21} * m00 - f64 {m20} * m01) * v3));
        v4.m31 = static_cast<f32>(-(f64 {v4.m01} * m30 + f64 {v4.m21} * m32 + f64 {v4.m11} * m31));
        v4.m02 = static_cast<f32>((f64 {m01} * m12 - f64 {m02} * m11) * v3);
        v4.m12 = static_cast<f32>(-((f64 {m00} * m12 - f64 {m10} * m02) * v3));
        v4.m22 = static_cast<f32>((f64 {m11} * m00 - f64 {m10} * m01) * v3);
        v4.m32 = static_cast<f32>(-(f64 {v4.m02} * m30 + f64 {v4.m22} * m32 + f64 {v4.m12} * m31));

        *this = v4;
    }
}

// ?LookAt@Matrix34@@QAEXABVVector3@@0@Z
void Matrix34::LookAt(const Vector3& arg1, const Vector3& arg2)
{
    // m2 is the look axis (eye minus target, normalized), m0 the right axis (cross of world up
    // with m2, normalized), m1 the up axis - and unlike Midtown Madness 1 this one does NOT
    // renormalize m1, which is redundant once m0 and m2 are orthonormal.
    m20 = arg1.x - arg2.x;
    m21 = arg1.y - arg2.y;
    m22 = arg1.z - arg2.z;

    f64 v4 = f64 {m20} * m20 + f64 {m21} * m21 + f64 {m22} * m22;
    f64 v5;

    if (v4 == 0.0)
        v5 = 0.0;
    else
        v5 = 1.0 / std::sqrt(v4);

    m20 = static_cast<f32>(v5 * m20);
    m21 = static_cast<f32>(v5 * m21);
    m22 = static_cast<f32>(v5 * m22);

    // cross((0,1,0), m2) is (m22, 0, -m20). The first three stores are dead - m00 and m02 are
    // both written again below - but they are in the instruction stream, so they are here too.
    f32 ida = m22;
    f64 v6 = -f64 {m20};

    m02 = static_cast<f32>(v6);
    m01 = 0.0f;
    m00 = ida;

    f64 v7 = v6 * v6 + f64 {ida} * ida;
    f64 v8;

    if (v7 == 0.0)
        v8 = 0.0;
    else
        v8 = 1.0 / std::sqrt(v7);

    f32 valuesa = static_cast<f32>(f64 {ida} * v8);

    m00 = valuesa;
    m01 = 0.0f;

    f64 v10 = v6 * v8;

    m02 = static_cast<f32>(v10);

    // cross(m2, m0) written out in full, including the two terms multiplied by m0's zero y. The
    // multiplications are real instructions, so they stay - folding them away would change what
    // happens when the other operand is an infinity or a NaN.
    f64 v11 = f64 {valuesa} * m22 - v10 * m20;
    f64 v12 = v10 * m21 - f64 {0.0f} * m22;
    f32 v16 = static_cast<f32>(v11);
    f64 v13 = f64 {0.0f} * m20;
    f64 v14 = f64 {valuesa} * m21;

    m11 = v16;
    m12 = static_cast<f32>(v13 - v14);
    m10 = static_cast<f32>(v12);

    m30 = arg1.x;
    m31 = arg1.y;
    m32 = arg1.z;
}

// ?MakeRotateUnitAxis@Matrix34@@QAEXABVVector3@@M@Z
void Matrix34::MakeRotateUnitAxis(const Vector3& arg1, f32 arg2)
{
    // Rodrigues' formula for an axis the caller guarantees is unit length. sin and (1 - cos) are
    // both float slots; cos stays in a register, so it is the only unrounded one.
    f64 v4 = std::cos(f64 {arg2});
    f32 v6 = static_cast<f32>(std::sin(f64 {arg2}));
    f32 v7 = static_cast<f32>(1.0 - v4);

    m00 = static_cast<f32>(f64 {arg1.x} * arg1.x * v7 + v4);
    m11 = static_cast<f32>(f64 {arg1.y} * arg1.y * v7 + v4);
    m22 = static_cast<f32>(f64 {arg1.z} * arg1.z * v7 + v4);
    m01 = static_cast<f32>(f64 {arg1.y} * arg1.x * v7 + f64 {v6} * arg1.z);
    m10 = static_cast<f32>(f64 {arg1.y} * arg1.x * v7 - f64 {v6} * arg1.z);
    m02 = static_cast<f32>(f64 {arg1.z} * arg1.x * v7 - f64 {v6} * arg1.y);
    m20 = static_cast<f32>(f64 {arg1.z} * arg1.x * v7 + f64 {v6} * arg1.y);
    m12 = static_cast<f32>(f64 {arg1.z} * arg1.y * v7 + f64 {v6} * arg1.x);
    m21 = static_cast<f32>(f64 {arg1.z} * arg1.y * v7 - f64 {v6} * arg1.x);
}

// ?MakeRotateX@Matrix34@@QAEXM@Z
void Matrix34::MakeRotateX(f32 arg1)
{
    f64 v2 = std::cos(f64 {arg1});

    m00 = 1.0f;
    m01 = 0.0f;
    m02 = 0.0f;
    m10 = 0.0f;

    f64 v4 = std::sin(f64 {arg1});

    m11 = static_cast<f32>(v2);
    m12 = static_cast<f32>(v4);
    m21 = static_cast<f32>(-v4);
    m20 = 0.0f;
    m22 = static_cast<f32>(v2);
}

// ?MakeRotateY@Matrix34@@QAEXM@Z
void Matrix34::MakeRotateY(f32 arg1)
{
    f64 v3 = std::cos(f64 {arg1});

    m01 = 0.0f;

    f64 v5 = std::sin(f64 {arg1});

    m00 = static_cast<f32>(v3);
    m02 = static_cast<f32>(-v5);
    m10 = 0.0f;
    m11 = 1.0f;
    m12 = 0.0f;
    m21 = 0.0f;
    m20 = static_cast<f32>(v5);
    m22 = static_cast<f32>(v3);
}

// ?MakeRotateZ@Matrix34@@QAEXM@Z
void Matrix34::MakeRotateZ(f32 arg1)
{
    f64 v3 = std::cos(f64 {arg1});

    m02 = 0.0f;

    f64 v5 = std::sin(f64 {arg1});

    m00 = static_cast<f32>(v3);
    m01 = static_cast<f32>(v5);
    m10 = static_cast<f32>(-v5);
    m12 = 0.0f;
    m11 = static_cast<f32>(v3);
    m20 = 0.0f;
    m21 = 0.0f;
    m22 = 1.0f;
}

// ?MakeScale@Matrix34@@QAEXM@Z
void Matrix34::MakeScale(f32 arg1)
{
    // Copied as dwords, not through the FPU. The translation row is not touched.
    m00 = arg1;
    m01 = 0.0f;
    m02 = 0.0f;
    m10 = 0.0f;
    m11 = arg1;
    m12 = 0.0f;
    m20 = 0.0f;
    m21 = 0.0f;
    m22 = arg1;
}

// ?MakeScale@Matrix34@@QAEXMMM@Z
void Matrix34::MakeScale(f32 arg1, f32 arg2, f32 arg3)
{
    m00 = arg1;
    m01 = 0.0f;
    m02 = 0.0f;
    m10 = 0.0f;
    m11 = arg2;
    m12 = 0.0f;
    m20 = 0.0f;
    m21 = 0.0f;
    m22 = arg3;
}

// ?Normalize@Matrix34@@QAEXXZ
void Matrix34::Normalize()
{
    // Re-orthonormalize, keeping m2's direction: rebuild m0 as cross(m1, m2), then m1 as
    // cross(m2, m0), then scale all three to unit length. Each cross uses the y/z-through-a-float
    // pattern and the y, z, x store order.
    f32 v16 = static_cast<f32>(f64 {m22} * m11 - f64 {m21} * m12);
    f32 v14 = static_cast<f32>(f64 {m20} * m12 - f64 {m22} * m10);
    f64 v1 = f64 {m21} * m10;
    f64 v2 = f64 {m11} * m20;

    m00 = v16;
    m01 = v14;
    m02 = static_cast<f32>(v1 - v2);

    // m02 is read back out of memory while the other two terms come from the registers that were
    // just stored - so the sum is not symmetric in how its three terms were rounded.
    f64 v3 = f64 {m02} * m02 + f64 {v14} * v14 + f64 {v16} * v16;
    f64 v4;

    if (v3 == 0.0)
        v4 = 0.0;
    else
        v4 = 1.0 / std::sqrt(v3);

    f32 v17 = static_cast<f32>(v4 * v16);

    m00 = v17;

    f32 v15 = static_cast<f32>(v4 * v14);

    m01 = v15;

    f64 v5 = v4 * m02;

    m02 = static_cast<f32>(v5);

    f64 v6 = v5 * m21 - f64 {v15} * m22;
    f32 v13 = static_cast<f32>(f64 {v17} * m22 - v5 * m20);
    f64 v7 = f64 {v15} * m20;
    f64 v8 = f64 {v17} * m21;

    m11 = v13;
    m12 = static_cast<f32>(v7 - v8);
    m10 = static_cast<f32>(v6);

    f64 v9 = f64 {m10} * m10 + f64 {m11} * m11 + f64 {m12} * m12;
    f64 v10;

    if (v9 == 0.0)
        v10 = 0.0;
    else
        v10 = 1.0 / std::sqrt(v9);

    m10 = static_cast<f32>(v10 * m10);
    m11 = static_cast<f32>(v10 * m11);
    m12 = static_cast<f32>(v10 * m12);

    f64 v11 = f64 {m20} * m20 + f64 {m21} * m21 + f64 {m22} * m22;
    f64 v12;

    if (v11 == 0.0)
        v12 = 0.0;
    else
        v12 = 1.0 / std::sqrt(v11);

    m20 = static_cast<f32>(v12 * m20);
    m21 = static_cast<f32>(v12 * m21);
    m22 = static_cast<f32>(v12 * m22);
}

// ?PolarView@Matrix34@@QAEXMMMM@Z
void Matrix34::PolarView(f32 arg1, f32 arg2, f32 arg3, f32 arg4)
{
    // (offz, roty, rotx, rotz), matching Midtown Madness 1's parameter names. Midtown Madness 1
    // builds this from Identity plus three RotateFull calls; Midtown Madness 2 builds the Euler
    // vector directly and pushes the offset out along the look axis instead.
    Vector3 idx;

    idx.x = -arg3;
    idx.z = arg4;
    idx.y = arg2;

    FromEulersZXY(idx);

    m30 = arg1 * m20;
    m31 = arg1 * m21;
    m32 = arg1 * m22;
}

// ?Rotate@Matrix34@@QAEXABVVector3@@M@Z
void Matrix34::Rotate(const Vector3& arg1, f32 arg2)
{
    Matrix34 v4;

    v4.MakeRotate(arg1, arg2);
    Dot3x3(v4);
}

// ?RotateFull@Matrix34@@QAEXABVVector3@@M@Z
void Matrix34::RotateFull(const Vector3& arg1, f32 arg2)
{
    // MakeRotate leaves the translation row untouched, so the RotateFull family has to zero it
    // before the full 3x4 multiply. The original does that with a 12-byte memset, which is three
    // zero floats.
    Matrix34 a3a;

    a3a.MakeRotate(arg1, arg2);

    a3a.m30 = 0.0f;
    a3a.m31 = 0.0f;
    a3a.m32 = 0.0f;

    Dot(a3a);
}

// ?RotateFullUnitAxis@Matrix34@@QAEXABVVector3@@M@Z
void Matrix34::RotateFullUnitAxis(const Vector3& arg1, f32 arg2)
{
    Matrix34 a3;

    a3.MakeRotateUnitAxis(arg1, arg2);

    a3.m30 = 0.0f;
    a3.m31 = 0.0f;
    a3.m32 = 0.0f;

    Dot(a3);
}

// ?RotateFullX@Matrix34@@QAEXM@Z
void Matrix34::RotateFullX(f32 arg1)
{
    Matrix34 a3;

    a3.MakeRotateX(arg1);

    a3.m30 = 0.0f;
    a3.m31 = 0.0f;
    a3.m32 = 0.0f;

    Dot(a3);
}

// ?RotateFullY@Matrix34@@QAEXM@Z
void Matrix34::RotateFullY(f32 arg1)
{
    Matrix34 a3;

    a3.MakeRotateY(arg1);

    a3.m30 = 0.0f;
    a3.m31 = 0.0f;
    a3.m32 = 0.0f;

    Dot(a3);
}

// ?RotateFullZ@Matrix34@@QAEXM@Z
void Matrix34::RotateFullZ(f32 arg1)
{
    Matrix34 a3;

    a3.MakeRotateZ(arg1);

    a3.m30 = 0.0f;
    a3.m31 = 0.0f;
    a3.m32 = 0.0f;

    Dot(a3);
}

// ?RotateUnitAxis@Matrix34@@QAEXABVVector3@@M@Z
void Matrix34::RotateUnitAxis(const Vector3& arg1, f32 arg2)
{
    Matrix34 v5;

    v5.MakeRotateUnitAxis(arg1, arg2);
    Dot3x3(v5);
}

// ?RotateX@Matrix34@@QAEXM@Z
void Matrix34::RotateX(f32 arg1)
{
    Matrix34 v4;

    v4.MakeRotateX(arg1);
    Dot3x3(v4);
}

// ?RotateY@Matrix34@@QAEXM@Z
void Matrix34::RotateY(f32 arg1)
{
    Matrix34 v5;

    v5.MakeRotateY(arg1);
    Dot3x3(v5);
}

// ?RotateZ@Matrix34@@QAEXM@Z
void Matrix34::RotateZ(f32 arg1)
{
    Matrix34 v5;

    v5.MakeRotateZ(arg1);
    Dot3x3(v5);
}

// ?Scale@Matrix34@@QAEXM@Z
void Matrix34::Scale(f32 arg1)
{
    // Row order here, column order in the three-argument overload - they are not the same loop
    // with a different multiplier, and neither is a call to the other.
    m00 = arg1 * m00;
    m01 = arg1 * m01;
    m02 = arg1 * m02;
    m10 = arg1 * m10;
    m11 = arg1 * m11;
    m12 = arg1 * m12;
    m20 = arg1 * m20;
    m21 = arg1 * m21;
    m22 = arg1 * m22;
}

// ?Scale@Matrix34@@QAEXMMM@Z
void Matrix34::Scale(f32 arg1, f32 arg2, f32 arg3)
{
    m00 = arg1 * m00;
    m10 = arg1 * m10;
    m20 = arg1 * m20;
    m01 = arg2 * m01;
    m11 = arg2 * m11;
    m21 = arg2 * m21;
    m02 = arg3 * m02;
    m12 = arg3 * m12;
    m22 = arg3 * m22;
}

// ?ScaleFull@Matrix34@@QAEXM@Z
void Matrix34::ScaleFull(f32 arg1)
{
    m00 = arg1 * m00;
    m01 = arg1 * m01;
    m02 = arg1 * m02;
    m10 = arg1 * m10;
    m11 = arg1 * m11;
    m12 = arg1 * m12;
    m20 = arg1 * m20;
    m21 = arg1 * m21;
    m22 = arg1 * m22;
    m30 = arg1 * m30;
    m31 = arg1 * m31;
    m32 = arg1 * m32;
}

// ?ScaleFull@Matrix34@@QAEXMMM@Z
void Matrix34::ScaleFull(f32 arg1, f32 arg2, f32 arg3)
{
    m00 = arg1 * m00;
    m10 = arg1 * m10;
    m20 = arg1 * m20;
    m30 = arg1 * m30;
    m01 = arg2 * m01;
    m11 = arg2 * m11;
    m21 = arg2 * m21;
    m31 = arg2 * m31;
    m02 = arg3 * m02;
    m12 = arg3 * m12;
    m22 = arg3 * m22;
    m32 = arg3 * m32;
}

// ?Set@Matrix34@@QAEXABV1@@Z
void Matrix34::Set(const Matrix34& arg1)
{
    *this = arg1;
}

// ?Subtract@Matrix34@@QAEXABV1@0@Z
void Matrix34::Subtract(const Matrix34& arg1, const Matrix34& arg2)
{
    m00 = arg1.m00 - arg2.m00;
    m01 = arg1.m01 - arg2.m01;
    m02 = arg1.m02 - arg2.m02;
    m10 = arg1.m10 - arg2.m10;
    m11 = arg1.m11 - arg2.m11;
    m12 = arg1.m12 - arg2.m12;
    m20 = arg1.m20 - arg2.m20;
    m21 = arg1.m21 - arg2.m21;
    m22 = arg1.m22 - arg2.m22;
    m30 = arg1.m30 - arg2.m30;
    m31 = arg1.m31 - arg2.m31;
    m32 = arg1.m32 - arg2.m32;
}

// ?Subtract@Matrix34@@QAEXABV1@@Z
void Matrix34::Subtract(const Matrix34& arg1)
{
    m00 = m00 - arg1.m00;
    m01 = m01 - arg1.m01;
    m02 = m02 - arg1.m02;
    m10 = m10 - arg1.m10;
    m11 = m11 - arg1.m11;
    m12 = m12 - arg1.m12;
    m20 = m20 - arg1.m20;
    m21 = m21 - arg1.m21;
    m22 = m22 - arg1.m22;
    m30 = m30 - arg1.m30;
    m31 = m31 - arg1.m31;
    m32 = m32 - arg1.m32;
}

// ?Subtract3x3@Matrix34@@QAEXABV1@0@Z
void Matrix34::Subtract3x3(const Matrix34& arg1, const Matrix34& arg2)
{
    m00 = arg1.m00 - arg2.m00;
    m01 = arg1.m01 - arg2.m01;
    m02 = arg1.m02 - arg2.m02;
    m10 = arg1.m10 - arg2.m10;
    m11 = arg1.m11 - arg2.m11;
    m12 = arg1.m12 - arg2.m12;
    m20 = arg1.m20 - arg2.m20;
    m21 = arg1.m21 - arg2.m21;
    m22 = arg1.m22 - arg2.m22;
}

// ?Subtract3x3@Matrix34@@QAEXABV1@@Z
void Matrix34::Subtract3x3(const Matrix34& arg1)
{
    m00 = m00 - arg1.m00;
    m01 = m01 - arg1.m01;
    m02 = m02 - arg1.m02;
    m10 = m10 - arg1.m10;
    m11 = m11 - arg1.m11;
    m12 = m12 - arg1.m12;
    m20 = m20 - arg1.m20;
    m21 = m21 - arg1.m21;
    m22 = m22 - arg1.m22;
}

// ?ToEulers@Matrix34@@QBEXAAVVector3@@PAD@Z
void Matrix34::ToEulers(Vector3& arg1, char* arg2) const
{
    char v3 = arg2[0];

    if (v3 == 'x')
    {
        char v4 = arg2[1];

        if (v4 == 'y' && arg2[2] == 'z')
            ToEulersXYZ(arg1);
        else if (v4 == 'z' && arg2[2] == 'y')
            ToEulersXZY(arg1);
        else
            Warningf("Matrix34::ToEulers()- Bad string '%s'", arg2);
    }
    else if (v3 == 'y')
    {
        char v5 = arg2[1];

        if (v5 == 'x' && arg2[2] == 'z')
            ToEulersYXZ(arg1);
        else if (v5 == 'z' && arg2[2] == 'x')
            ToEulersYZX(arg1);
        else
            Warningf("Matrix34::ToEulers()- Bad string '%s'", arg2);
    }
    else if (v3 == 'z')
    {
        char v6 = arg2[1];

        if (v6 == 'x' && arg2[2] == 'y')
            ToEulersZXY(arg1);
        else if (v6 == 'y' && arg2[2] == 'x')
            ToEulersZYX(arg1);
        else
            Warningf("Matrix34::ToEulers()- Bad string '%s'", arg2);
    }
    else
    {
        Warningf("Matrix34::ToEulers()- Bad string '%s'", arg2);
    }
}

// ?ToEulersXYZ@Matrix34@@QBEXAAVVector3@@@Z
void Matrix34::ToEulersXYZ(Vector3& arg1) const
{
    // All six ToEulers* guard atan2 against both arguments being exactly zero, and clamp the asin
    // argument to +/-0.99998999 rather than +/-1 - so a perfectly axis-aligned matrix comes back
    // with a pitch of 89.9744 degrees, not 90. That is the original's behaviour, not a slip.
    f32 v9;

    if (m00 == 0.0f && m01 == 0.0f)
        v9 = 0.0f;
    else
        v9 = static_cast<f32>(std::atan2(f64 {m01}, f64 {m00}));

    f64 v3 = -f64 {m02};
    f32 v10;

    if (v3 >= -kEulerSinClamp)
    {
        if (v3 <= kEulerSinClamp)
            v10 = static_cast<f32>(v3);
        else
            v10 = kEulerSinClamp;
    }
    else
    {
        v10 = -kEulerSinClamp;
    }

    f64 v4 = m12;
    f64 v5 = m22;
    f64 v6;

    if (v5 == 0.0 && v4 == 0.0)
        v6 = 0.0;
    else
        v6 = std::atan2(v4, v5);

    arg1.x = static_cast<f32>(v6);

    f64 v7 = std::asin(f64 {v10});

    arg1.y = static_cast<f32>(v7);
    arg1.z = v9;
}

// ?ToEulersXZY@Matrix34@@QBEXAAVVector3@@@Z
void Matrix34::ToEulersXZY(Vector3& arg1) const
{
    f64 v3 = m01;
    f32 v10;

    if (v3 >= -kEulerSinClamp)
    {
        if (v3 <= kEulerSinClamp)
            v10 = m01;
        else
            v10 = kEulerSinClamp;
    }
    else
    {
        v10 = -kEulerSinClamp;
    }

    f64 v4 = -f64 {m02};
    f64 v5 = m00;
    f64 v6;

    if (v5 == 0.0 && v4 == 0.0)
        v6 = 0.0;
    else
        v6 = std::atan2(v4, v5);

    f64 v7 = -f64 {m21};
    f64 v8 = m11;
    f64 v9;

    if (v8 == 0.0 && v7 == 0.0)
        v9 = 0.0;
    else
        v9 = std::atan2(v7, v8);

    arg1.x = static_cast<f32>(v9);
    arg1.y = static_cast<f32>(v6);
    arg1.z = static_cast<f32>(std::asin(f64 {v10}));
}

// ?ToEulersYXZ@Matrix34@@QBEXAAVVector3@@@Z
void Matrix34::ToEulersYXZ(Vector3& arg1) const
{
    f64 v2 = -f64 {m10};
    f32 v7;

    if (m11 == 0.0f && v2 == 0.0)
        v7 = 0.0f;
    else
        v7 = static_cast<f32>(std::atan2(v2, f64 {m11}));

    f64 v3 = -f64 {m02};
    f64 v4 = m22;
    f32 v8;

    if (v4 == 0.0 && v3 == 0.0)
        v8 = 0.0f;
    else
        v8 = static_cast<f32>(std::atan2(v3, v4));

    f64 v5 = m12;
    f32 v9;

    if (v5 >= -kEulerSinClamp)
    {
        if (v5 <= kEulerSinClamp)
            v9 = m12;
        else
            v9 = kEulerSinClamp;
    }
    else
    {
        v9 = -kEulerSinClamp;
    }

    arg1.x = static_cast<f32>(std::asin(f64 {v9}));
    arg1.y = v8;
    arg1.z = v7;
}

// ?ToEulersYZX@Matrix34@@QBEXAAVVector3@@@Z
void Matrix34::ToEulersYZX(Vector3& arg1) const
{
    f64 v3 = -f64 {m10};
    f32 v10;

    if (v3 >= -kEulerSinClamp)
    {
        if (v3 <= kEulerSinClamp)
            v10 = static_cast<f32>(v3);
        else
            v10 = kEulerSinClamp;
    }
    else
    {
        v10 = -kEulerSinClamp;
    }

    f64 v4 = m20;
    f64 v5 = m00;
    f64 v6;

    if (v5 == 0.0 && v4 == 0.0)
        v6 = 0.0;
    else
        v6 = std::atan2(v4, v5);

    f64 v7 = m12;
    f64 v8 = m11;
    f64 v9;

    if (v8 == 0.0 && v7 == 0.0)
        v9 = 0.0;
    else
        v9 = std::atan2(v7, v8);

    arg1.x = static_cast<f32>(v9);
    arg1.y = static_cast<f32>(v6);
    arg1.z = static_cast<f32>(std::asin(f64 {v10}));
}

// ?ToEulersZXY@Matrix34@@QBEXAAVVector3@@@Z
void Matrix34::ToEulersZXY(Vector3& arg1) const
{
    f32 v6;

    if (m11 == 0.0f && m01 == 0.0f)
        v6 = 0.0f;
    else
        v6 = static_cast<f32>(std::atan2(f64 {m01}, f64 {m11}));

    f64 v2 = m20;
    f64 v3 = m22;
    f32 v7;

    if (v3 == 0.0 && v2 == 0.0)
        v7 = 0.0f;
    else
        v7 = static_cast<f32>(std::atan2(v2, v3));

    f64 v4 = -f64 {m21};
    f32 v8;

    if (v4 >= -kEulerSinClamp)
    {
        if (v4 <= kEulerSinClamp)
            v8 = static_cast<f32>(v4);
        else
            v8 = kEulerSinClamp;
    }
    else
    {
        v8 = -kEulerSinClamp;
    }

    arg1.x = static_cast<f32>(std::asin(f64 {v8}));
    arg1.y = v7;
    arg1.z = v6;
}

// ?ToEulersZYX@Matrix34@@QBEXAAVVector3@@@Z
void Matrix34::ToEulersZYX(Vector3& arg1) const
{
    f64 v3 = -f64 {m10};
    f32 v10;

    if (m00 == 0.0f && v3 == 0.0)
        v10 = 0.0f;
    else
        v10 = static_cast<f32>(std::atan2(v3, f64 {m00}));

    f64 v4 = m20;
    f32 v11;

    if (v4 >= -kEulerSinClamp)
    {
        if (v4 <= kEulerSinClamp)
            v11 = m20;
        else
            v11 = kEulerSinClamp;
    }
    else
    {
        v11 = -kEulerSinClamp;
    }

    f64 v5 = -f64 {m21};
    f64 v6 = m22;
    f64 v7;

    if (v6 == 0.0 && v5 == 0.0)
        v7 = 0.0;
    else
        v7 = std::atan2(v5, v6);

    arg1.x = static_cast<f32>(v7);

    f64 v8 = std::asin(f64 {v11});

    arg1.y = static_cast<f32>(v8);
    arg1.z = v10;
}

// ?Transform@Matrix34@@QBEXABVVector3@@AAV2@@Z
//
// NOT PORTED, and it must stay that way until switch tables are handled.
//
// Four dwords in .data point INSIDE this function (the first at +0x60) - they are a switch
// jump table. Those interior addresses cannot be relocated to a reimplementation, because the
// C++ has no corresponding interior. With this function ported they resolved to whatever
// landed nearby, which was Transpose3x4, and the game called it with a null argument during
// startup: a read of 0x18 at RVA 0x1B3887.
//
// The implementation below was correct; the harness is what cannot carry it yet.
// void Matrix34::Transform(const Vector3& arg1, Vector3& arg2) const
// {
//     arg2.x = static_cast<f32>(f64 {m10} * arg1.y + f64 {m20} * arg1.z + f64 {arg1.x} * m00 + m30);
//     arg2.y = static_cast<f32>(f64 {m01} * arg1.x + f64 {m11} * arg1.y + f64 {m21} * arg1.z + m31);
//     arg2.z = static_cast<f32>(f64 {m02} * arg1.x + f64 {m12} * arg1.y + f64 {m22} * arg1.z + m32);
// }

// ?Transpose@Matrix34@@QAEXABV1@@Z
void Matrix34::Transpose(const Matrix34& arg1)
{
    // 3x3 only. Midtown Madness 1's version copies the translation row through as well; this one
    // leaves the destination's translation exactly as it found it.
    f64 v4 = arg1.m20;
    f64 v5 = arg1.m10;

    m00 = arg1.m00;
    m01 = static_cast<f32>(v5);
    m02 = static_cast<f32>(v4);

    f64 v6 = arg1.m21;
    f64 v7 = arg1.m11;

    m10 = arg1.m01;
    m11 = static_cast<f32>(v7);
    m12 = static_cast<f32>(v6);

    f64 v8 = arg1.m22;
    f64 v9 = arg1.m12;

    m20 = arg1.m02;
    m21 = static_cast<f32>(v9);
    m22 = static_cast<f32>(v8);
}

// ?Transpose3x4@Matrix34@@QAEXABV1@@Z
void Matrix34::Transpose3x4(const Matrix34& arg1)
{
    f64 v4 = arg1.m20;
    f64 v5 = arg1.m10;

    m00 = arg1.m00;
    m01 = static_cast<f32>(v5);
    m02 = static_cast<f32>(v4);

    f64 v6 = arg1.m21;
    f64 v7 = arg1.m11;

    m10 = arg1.m01;
    m11 = static_cast<f32>(v7);
    m12 = static_cast<f32>(v6);

    f64 v8 = arg1.m22;
    f64 v9 = arg1.m12;

    m20 = arg1.m02;
    m21 = static_cast<f32>(v9);
    m22 = static_cast<f32>(v8);

    // The translation is arg1's, pushed through the transpose that was just written - so this
    // reads *this, not arg1, for the 3x3 part. Then negated in a second pass over memory, which
    // is why the negation is three separate statements rather than folded into the expressions.
    m30 = static_cast<f32>(f64 {arg1.m31} * m10 + f64 {m00} * arg1.m30 + f64 {arg1.m32} * m20);
    m31 = static_cast<f32>(f64 {m21} * arg1.m32 + f64 {m11} * arg1.m31 + f64 {m01} * arg1.m30);
    m32 = static_cast<f32>(f64 {m22} * arg1.m32 + f64 {m12} * arg1.m31 + f64 {m02} * arg1.m30);

    m30 = -m30;
    m31 = -m31;
    m32 = -m32;
}

// ?Zero@Matrix34@@QAEXXZ
void Matrix34::Zero()
{
    // Written as twelve dword stores, in the order the original emits them.
    m02 = 0.0f;
    m01 = 0.0f;
    m00 = 0.0f;
    m12 = 0.0f;
    m11 = 0.0f;
    m10 = 0.0f;
    m22 = 0.0f;
    m21 = 0.0f;
    m20 = 0.0f;
    m32 = 0.0f;
    m31 = 0.0f;
    m30 = 0.0f;
}

// ?Transpose@Matrix34@@QAEXXZ
void Matrix34::Transpose()
{
    // The 3x3 part only - the translation row is untouched. Three swaps, which the original does
    // as plain dword moves through eax/edx with the x87 loads interleaved. No arithmetic, so this
    // is bit-exact by construction rather than by choice of intermediate type.
    f32 t01 = m01;
    f32 t02 = m02;
    f32 t12 = m12;

    m01 = m10;
    m10 = t01;

    m02 = m20;
    m20 = t02;

    m12 = m21;
    m21 = t12;
}

// ?Transpose3x4@Matrix34@@QAEXXZ
void Matrix34::Transpose3x4()
{
    // Transposes the 3x3 in place and then replaces the translation with -(R^T * t): the inverse
    // of a rigid transform. Unlike Transpose3x4(const Matrix34&), which reads its source, this
    // form calls Transpose() first and then reads the ALREADY-transposed rows back out of itself,
    // which is why the products below are spelled with m10/m20/m21 rather than their
    // pre-transpose counterparts.
    Transpose();

    // Every read of the translation happens before any of the three stores. The temporaries are
    // not an optimisation - they are what makes that true.
    f32 v0 = static_cast<f32>(f64 {m20} * m32 + f64 {m10} * m31 + f64 {m30} * m00);
    f32 v1 = static_cast<f32>(f64 {m21} * m32 + f64 {m01} * m30 + f64 {m11} * m31);
    f32 v2 = static_cast<f32>(f64 {m22} * m32 + f64 {m02} * m30 + f64 {m12} * m31);

    m31 = v1;
    m32 = v2;
    m30 = v0;

    // A second pass over memory, in the order the original emits it.
    m30 = -m30;
    m31 = -m31;
    m32 = -m32;
}
