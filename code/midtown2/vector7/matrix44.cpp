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

#include "matrix44.h"

#include "data7/printer.h"
#include "matrix34.h"
#include "vector3.h"
#include "vector4.h"

#include <cmath>

define_dummy_symbol(vector7_matrix44);

// Twenty-three of Matrix44's twenty-four methods, read off the DISASSEMBLY of midtown2.exe. The
// Hex-Rays dump in MM2_RE_KIT was used only to cross-check, and for five of these - Zero,
// Transpose, FromMatrix34, Add(const Matrix44&, const Matrix44&) and AddScaled - there is nothing
// in the kit to cross-check against: they are part of Trap 3, the 286 real functions the RE kit
// has no pseudocode for. Every function's terminating `ret imm16` was compared against the
// parameter list its mangled name encodes before it was written, because vector7 does contain map
// symbols where the two disagree; all twenty-three here agree.
//
// WHAT IS NOT HERE, AND WHY.  (One function, after the second pass.)
//
//   InvertTo          1,766 B, the largest function in vector7. It is NOT Gauss-Jordan with
//                     pivoting, whatever this file said before it was disassembled. The first
//                     instruction after the prologue is `call ?Determinant@Matrix44@@QBEMXZ`,
//                     followed by `fdivr dword ptr [1.0f]`, and the remaining 1,700 bytes are
//                     sixteen straight-line cofactor blocks, six triple-products each, all scaled
//                     by that reciprocal. There is no loop, no pivot search and no row swap
//                     anywhere in it. That makes it mechanical rather than delicate - but it is
//                     288 individual member reads with no structure to check them against, which
//                     is the exact shape of the wrong-but-plausible value docs/harness.md says no
//                     byte comparison can see. It gets its own pass.
//
// Determinant, Dot(const Matrix44&) and Print landed in the second pass and are at the end of this
// file.
//
// FLOAT FIDELITY. The 1999 build is x87 at the CRT's default 53-bit precision control, so
// intermediates are held wider than f32 and round only when stored. A value that passes through a
// float stack slot is written f32 here and one that stays in an st(n) register is written f64.
// Sub-expression grouping and accumulation order are reproduced verbatim.
//
// TRIG. MakeRotX/Y/Z use the bare `fcos` and `fsin` instructions, not a CRT call. Under
// /arch:IA32 a modern MSVC lowers std::cos(double) to _CIcos, which core/crtshim.cpp implements as
// exactly that instruction plus the argument reduction the bare instruction lacks - so the value
// matches. Writing std::cos(f32) instead would select cosf and change the result.

// ?__vectorDotMatrix@@YAXAAVVector4@@ABV1@ABVMatrix44@@@Z - still the original, at 0x4C1670.
// Dot(lhs, rhs) hands its last row to this helper instead of inlining it. Declared here because
// nothing in the tree owns vector7's free functions yet.
ARTS_IMPORT void __vectorDotMatrix(Vector4& out, const Vector4& arg1, const Matrix44& arg2);

// ?Add@Matrix44@@QAEXABV1@0@Z
void Matrix44::Add(const Matrix44& arg1, const Matrix44& arg2)
{
    m00 = arg1.m00 + arg2.m00;
    m01 = arg1.m01 + arg2.m01;
    m02 = arg1.m02 + arg2.m02;
    m03 = arg1.m03 + arg2.m03;

    // Row 1 comes out with the operands the other way round. Addition is commutative in IEEE so it
    // cannot change the result, but it is what the instruction stream does.
    m10 = arg2.m10 + arg1.m10;
    m11 = arg2.m11 + arg1.m11;
    m12 = arg2.m12 + arg1.m12;
    m13 = arg2.m13 + arg1.m13;

    m20 = arg1.m20 + arg2.m20;
    m21 = arg1.m21 + arg2.m21;
    m22 = arg1.m22 + arg2.m22;
    m23 = arg1.m23 + arg2.m23;

    m30 = arg1.m30 + arg2.m30;
    m31 = arg1.m31 + arg2.m31;
    m32 = arg1.m32 + arg2.m32;
    m33 = arg1.m33 + arg2.m33;
}

// ?Add@Matrix44@@QAEXABV1@@Z
void Matrix44::Add(const Matrix44& arg1)
{
    m00 = arg1.m00 + m00;
    m01 = arg1.m01 + m01;
    m02 = arg1.m02 + m02;
    m03 = arg1.m03 + m03;

    // m10 and m30 load the destination first and the operand second; the other fourteen do not.
    m10 = m10 + arg1.m10;
    m11 = arg1.m11 + m11;
    m12 = arg1.m12 + m12;
    m13 = arg1.m13 + m13;

    m20 = arg1.m20 + m20;
    m21 = arg1.m21 + m21;
    m22 = arg1.m22 + m22;
    m23 = arg1.m23 + m23;

    m30 = m30 + arg1.m30;
    m31 = arg1.m31 + m31;
    m32 = arg1.m32 + m32;
    m33 = arg1.m33 + m33;
}

// ?AddScaled@Matrix44@@QAEXABV1@M@Z
void Matrix44::AddScaled(const Matrix44& arg1, f32 arg2)
{
    // TWELVE elements, not sixteen: the fourth column - m03, m13, m23, m33 - is not touched. This
    // is a Matrix34-shaped operation running on a Matrix44, and it is not an oversight in the
    // transcription; there is no instruction in the function that reads or writes offset 0x0C,
    // 0x1C, 0x2C or 0x3C.
    m00 = static_cast<f32>(f64 {arg2} * arg1.m00 + m00);
    m01 = static_cast<f32>(f64 {arg2} * arg1.m01 + m01);
    m02 = static_cast<f32>(f64 {arg2} * arg1.m02 + m02);

    m10 = static_cast<f32>(f64 {arg2} * arg1.m10 + m10);
    m11 = static_cast<f32>(f64 {arg2} * arg1.m11 + m11);
    m12 = static_cast<f32>(f64 {arg2} * arg1.m12 + m12);

    m20 = static_cast<f32>(f64 {arg2} * arg1.m20 + m20);
    m21 = static_cast<f32>(f64 {arg2} * arg1.m21 + m21);
    m22 = static_cast<f32>(f64 {arg2} * arg1.m22 + m22);

    m30 = static_cast<f32>(f64 {arg2} * arg1.m30 + m30);
    m31 = static_cast<f32>(f64 {arg2} * arg1.m31 + m31);
    m32 = static_cast<f32>(f64 {arg2} * arg1.m32 + m32);
}

// ?Bezier@Matrix44@@QAEXXZ
void Matrix44::Bezier()
{
    // The cubic Bezier basis, negated relative to the textbook form. Every entry is stored as an
    // immediate dword, so these are exact bit patterns, not computed values.
    m00 = 1.0f;
    m01 = -3.0f;
    m02 = 3.0f;
    m03 = -1.0f;

    m10 = -3.0f;
    m11 = 6.0f;
    m12 = -3.0f;
    m13 = 0.0f;

    m20 = 3.0f;
    m21 = -3.0f;
    m22 = 0.0f;
    m23 = 0.0f;

    m30 = -1.0f;
    m31 = 0.0f;
    m32 = 0.0f;
    m33 = 0.0f;
}

// ?CatmullRom@Matrix44@@QAEXXZ
void Matrix44::CatmullRom()
{
    // The Catmull-Rom basis without its 1/2 - the caller is expected to supply the halving.
    m00 = -1.0f;
    m01 = 3.0f;
    m02 = -3.0f;
    m03 = 1.0f;

    m10 = 2.0f;
    m11 = -5.0f;
    m12 = 4.0f;
    m13 = -1.0f;

    m20 = -1.0f;
    m21 = 0.0f;
    m22 = 1.0f;
    m23 = 0.0f;

    m30 = 0.0f;
    m31 = 2.0f;
    m32 = 0.0f;
    m33 = 0.0f;
}

// ?Dot@Matrix44@@QAEAAV1@ABV1@0@Z
Matrix44& Matrix44::Dot(const Matrix44& arg1, const Matrix44& arg2)
{
    // this[i][j] = sum over k of arg2[i][k] * arg1[k][j] - so the SECOND argument supplies the
    // rows. That is the row-vector convention, and it is the reverse of what the parameter order
    // suggests; it was read off the offsets, not assumed.
    //
    // The four terms of each element are accumulated in a different order in almost every one of
    // the twelve, which is why no two lines below have the same shape. Rewriting them uniformly
    // would be wrong: x87 addition is not associative.
    //
    // The destination is written progressively while the operands are still being read, so a call
    // where `this` aliases arg1 or arg2 depends on the interleaving. It is preserved.
    m00 = static_cast<f32>(f64 {arg1.m00} * arg2.m00 + f64 {arg1.m20} * arg2.m02 + f64 {arg1.m10} * arg2.m01 +
        f64 {arg2.m03} * arg1.m30);
    m01 = static_cast<f32>(f64 {arg1.m11} * arg2.m01 + f64 {arg1.m21} * arg2.m02 + f64 {arg2.m00} * arg1.m01 +
        f64 {arg1.m31} * arg2.m03);
    m02 = static_cast<f32>(f64 {arg1.m22} * arg2.m02 + f64 {arg1.m02} * arg2.m00 + f64 {arg1.m32} * arg2.m03 +
        f64 {arg1.m12} * arg2.m01);
    m03 = static_cast<f32>(f64 {arg2.m03} * arg1.m33 + f64 {arg1.m13} * arg2.m01 + f64 {arg1.m23} * arg2.m02 +
        f64 {arg1.m03} * arg2.m00);

    m10 = static_cast<f32>(f64 {arg2.m13} * arg1.m30 + f64 {arg1.m10} * arg2.m11 + f64 {arg1.m00} * arg2.m10 +
        f64 {arg2.m12} * arg1.m20);
    m11 = static_cast<f32>(f64 {arg2.m13} * arg1.m31 + f64 {arg2.m12} * arg1.m21 + f64 {arg1.m11} * arg2.m11 +
        f64 {arg1.m01} * arg2.m10);
    m12 = static_cast<f32>(f64 {arg2.m13} * arg1.m32 + f64 {arg2.m12} * arg1.m22 + f64 {arg1.m02} * arg2.m10 +
        f64 {arg2.m11} * arg1.m12);
    m13 = static_cast<f32>(f64 {arg2.m13} * arg1.m33 + f64 {arg2.m12} * arg1.m23 + f64 {arg1.m03} * arg2.m10 +
        f64 {arg1.m13} * arg2.m11);

    m20 = static_cast<f32>(f64 {arg1.m00} * arg2.m20 + f64 {arg2.m23} * arg1.m30 + f64 {arg1.m10} * arg2.m21 +
        f64 {arg2.m22} * arg1.m20);
    m21 = static_cast<f32>(f64 {arg2.m23} * arg1.m31 + f64 {arg2.m22} * arg1.m21 + f64 {arg1.m11} * arg2.m21 +
        f64 {arg2.m20} * arg1.m01);
    m22 = static_cast<f32>(f64 {arg1.m02} * arg2.m20 + f64 {arg2.m23} * arg1.m32 + f64 {arg2.m22} * arg1.m22 +
        f64 {arg2.m21} * arg1.m12);
    m23 = static_cast<f32>(f64 {arg2.m23} * arg1.m33 + f64 {arg2.m22} * arg1.m23 + f64 {arg1.m13} * arg2.m21 +
        f64 {arg1.m03} * arg2.m20);

    // The last row is a plain row-vector-times-matrix, and the original calls the shared helper
    // for it rather than emitting a fourth copy.
    __vectorDotMatrix(
        reinterpret_cast<Vector4&>(m30), reinterpret_cast<const Vector4&>(arg2.m30), arg1);

    return *this;
}

// ?FastInverse@Matrix44@@QAEXABV1@@Z
void Matrix44::FastInverse(const Matrix44& rhs)
{
    // The inverse of a rigid transform: transpose the 3x3 and negate the translation projected
    // onto each of its rows. Only valid when the 3x3 is orthonormal, which is why it is "fast".
    //
    // The running dot product is spilled to a float stack slot after every term, so it rounds to
    // f32 twice before the third term is added. That is visible here and must stay.
    //
    // Each source element is read ONCE, into a local, because the original holds it in st(0)
    // across the store that overwrites the same offset - m22 in particular is stored to m22 and
    // then still used. Re-reading the member instead would change the result whenever `this`
    // aliases rhs.
    f32 tx = rhs.m30;
    f32 ty = rhs.m31;
    f32 tz = rhs.m32;

    f32 acc;
    f32 e;

    e = rhs.m00;
    acc = static_cast<f32>(f64 {e} * tx);
    m00 = e;

    e = rhs.m01;
    acc = static_cast<f32>(f64 {e} * ty + acc);
    m10 = e;

    e = rhs.m02;
    m20 = e;
    m30 = static_cast<f32>(-(f64 {e} * tz + acc));

    e = rhs.m10;
    acc = static_cast<f32>(f64 {e} * tx);
    m01 = e;

    e = rhs.m11;
    acc = static_cast<f32>(f64 {e} * ty + acc);
    m11 = e;

    e = rhs.m12;
    m21 = e;
    m31 = static_cast<f32>(-(f64 {e} * tz + acc));

    e = rhs.m20;
    acc = static_cast<f32>(f64 {e} * tx);
    m02 = e;

    e = rhs.m21;
    acc = static_cast<f32>(f64 {e} * ty + acc);
    m12 = e;

    e = rhs.m22;
    m33 = 1.0f;
    m22 = e;
    m23 = 0.0f;
    m13 = 0.0f;
    m03 = 0.0f;
    m32 = static_cast<f32>(-(f64 {e} * tz + acc));
}

// ?FromMatrix34@Matrix44@@QAEXABVMatrix34@@@Z
void Matrix44::FromMatrix34(const Matrix34& arg1)
{
    m00 = arg1.m00;
    m01 = arg1.m01;
    m02 = arg1.m02;
    m03 = 0.0f;

    m10 = arg1.m10;
    m11 = arg1.m11;
    m12 = arg1.m12;
    m13 = 0.0f;

    m20 = arg1.m20;
    m21 = arg1.m21;
    m22 = arg1.m22;
    m23 = 0.0f;

    m30 = arg1.m30;
    m31 = arg1.m31;
    m32 = arg1.m32;
    m33 = 1.0f;
}

// ?Hermite@Matrix44@@QAEXXZ
void Matrix44::Hermite()
{
    // The standard cubic Hermite basis.
    m00 = 2.0f;
    m01 = -2.0f;
    m02 = 1.0f;
    m03 = 1.0f;

    m10 = -3.0f;
    m11 = 3.0f;
    m12 = -2.0f;
    m13 = -1.0f;

    m20 = 0.0f;
    m21 = 0.0f;
    m22 = 1.0f;
    m23 = 0.0f;

    m30 = 1.0f;
    m31 = 0.0f;
    m32 = 0.0f;
    m33 = 0.0f;
}

// ?Identity@Matrix44@@QAEXXZ
void Matrix44::Identity()
{
    m00 = 1.0f;
    m01 = 0.0f;
    m02 = 0.0f;
    m03 = 0.0f;

    m10 = 0.0f;
    m11 = 1.0f;
    m12 = 0.0f;
    m13 = 0.0f;

    m20 = 0.0f;
    m21 = 0.0f;
    m22 = 1.0f;
    m23 = 0.0f;

    m30 = 0.0f;
    m31 = 0.0f;
    m32 = 0.0f;
    m33 = 1.0f;
}

// ?MakeRotX@Matrix44@@QAEAAV1@M@Z
Matrix44& Matrix44::MakeRotX(f32 arg1)
{
    f64 c = std::cos(f64 {arg1});
    f64 s = std::sin(f64 {arg1});

    m00 = 1.0f;
    m01 = 0.0f;
    m02 = 0.0f;
    m03 = 0.0f;

    m10 = 0.0f;
    m11 = static_cast<f32>(c);
    m12 = static_cast<f32>(s);
    m13 = 0.0f;

    m20 = 0.0f;
    // The original negates the register copy of sin rather than reloading m12, but negation is
    // exact and commutes with rounding, so m21 is -m12 either way.
    m21 = static_cast<f32>(-s);
    m22 = static_cast<f32>(c);
    m23 = 0.0f;

    m30 = 0.0f;
    m31 = 0.0f;
    m32 = 0.0f;
    m33 = 1.0f;

    return *this;
}

// ?MakeRotY@Matrix44@@QAEAAV1@M@Z
Matrix44& Matrix44::MakeRotY(f32 arg1)
{
    f64 c = std::cos(f64 {arg1});
    f64 s = std::sin(f64 {arg1});

    m00 = static_cast<f32>(c);
    m01 = 0.0f;
    m02 = static_cast<f32>(-s);
    m03 = 0.0f;

    m10 = 0.0f;
    m11 = 1.0f;
    m12 = 0.0f;
    m13 = 0.0f;

    m20 = static_cast<f32>(s);
    m21 = 0.0f;
    m22 = static_cast<f32>(c);
    m23 = 0.0f;

    m30 = 0.0f;
    m31 = 0.0f;
    m32 = 0.0f;
    m33 = 1.0f;

    return *this;
}

// ?MakeRotZ@Matrix44@@QAEAAV1@M@Z
Matrix44& Matrix44::MakeRotZ(f32 arg1)
{
    f64 c = std::cos(f64 {arg1});
    f64 s = std::sin(f64 {arg1});

    m00 = static_cast<f32>(c);
    m01 = static_cast<f32>(s);
    m02 = 0.0f;
    m03 = 0.0f;

    m10 = static_cast<f32>(-s);
    m11 = static_cast<f32>(c);
    m12 = 0.0f;
    m13 = 0.0f;

    m20 = 0.0f;
    m21 = 0.0f;
    m22 = 1.0f;
    m23 = 0.0f;

    m30 = 0.0f;
    m31 = 0.0f;
    m32 = 0.0f;
    m33 = 1.0f;

    return *this;
}

// ?Set@Matrix44@@QAEXABV1@@Z
void Matrix44::Set(const Matrix44& x)
{
    // Sixteen integer moves. Not memcpy and not an FPU copy - the bit patterns come across
    // untouched, which matters when the matrix is carrying a NaN or a denormal.
    m00 = x.m00;
    m01 = x.m01;
    m02 = x.m02;
    m03 = x.m03;

    m10 = x.m10;
    m11 = x.m11;
    m12 = x.m12;
    m13 = x.m13;

    m20 = x.m20;
    m21 = x.m21;
    m22 = x.m22;
    m23 = x.m23;

    m30 = x.m30;
    m31 = x.m31;
    m32 = x.m32;
    m33 = x.m33;
}

// ?Subtract@Matrix44@@QAEXABV1@0@Z
void Matrix44::Subtract(const Matrix44& arg1, const Matrix44& arg2)
{
    m00 = arg1.m00 - arg2.m00;
    m01 = arg1.m01 - arg2.m01;
    m02 = arg1.m02 - arg2.m02;
    m03 = arg1.m03 - arg2.m03;

    m10 = arg1.m10 - arg2.m10;
    m11 = arg1.m11 - arg2.m11;
    m12 = arg1.m12 - arg2.m12;
    m13 = arg1.m13 - arg2.m13;

    m20 = arg1.m20 - arg2.m20;
    m21 = arg1.m21 - arg2.m21;
    m22 = arg1.m22 - arg2.m22;
    m23 = arg1.m23 - arg2.m23;

    m30 = arg1.m30 - arg2.m30;
    m31 = arg1.m31 - arg2.m31;
    m32 = arg1.m32 - arg2.m32;
    m33 = arg1.m33 - arg2.m33;
}

// ?Subtract@Matrix44@@QAEXABV1@@Z
void Matrix44::Subtract(const Matrix44& arg1)
{
    m00 = m00 - arg1.m00;
    m01 = m01 - arg1.m01;
    m02 = m02 - arg1.m02;
    m03 = m03 - arg1.m03;

    m10 = m10 - arg1.m10;
    m11 = m11 - arg1.m11;
    m12 = m12 - arg1.m12;
    m13 = m13 - arg1.m13;

    m20 = m20 - arg1.m20;
    m21 = m21 - arg1.m21;
    m22 = m22 - arg1.m22;
    m23 = m23 - arg1.m23;

    m30 = m30 - arg1.m30;
    m31 = m31 - arg1.m31;
    m32 = m32 - arg1.m32;
    m33 = m33 - arg1.m33;
}

// ?ToMatrix34@Matrix44@@QBEXAAVMatrix34@@@Z
void Matrix44::ToMatrix34(Matrix34& arg1) const
{
    // The fourth column is dropped without being looked at.
    arg1.m00 = m00;
    arg1.m01 = m01;
    arg1.m02 = m02;

    arg1.m10 = m10;
    arg1.m11 = m11;
    arg1.m12 = m12;

    arg1.m20 = m20;
    arg1.m21 = m21;
    arg1.m22 = m22;

    arg1.m30 = m30;
    arg1.m31 = m31;
    arg1.m32 = m32;
}

// ?Transform4@Matrix44@@QBEXPBVVector3@@PAVVector4@@H@Z
void Matrix44::Transform4(const Vector3* arg1, Vector4* arg2, i32 arg3) const
{
    // Transforms arg3 points, BACKWARDS - from arg3 - 1 down to 0 - and does nothing at all when
    // the count is zero or negative. The direction is not incidental: it is what makes an in-place
    // call over overlapping buffers behave the way it does.
    //
    // Each output component is accumulated in registers with a single store at the end, but the
    // three components do not accumulate in the same order: x starts from the z term and y and z
    // start from the x term.
    for (i32 i = arg3 - 1; i >= 0; --i)
    {
        const Vector3& v = arg1[i];
        Vector4& out = arg2[i];

        out.x = static_cast<f32>(f64 {m20} * v.z + f64 {m00} * v.x + f64 {v.y} * m10 + m30);
        out.y = static_cast<f32>(f64 {m01} * v.x + f64 {v.z} * m21 + f64 {v.y} * m11 + m31);
        out.z = static_cast<f32>(f64 {m02} * v.x + f64 {v.z} * m22 + f64 {v.y} * m12 + m32);

        // Stored as an immediate zero dword, so this is +0.0f exactly.
        out.w = 0.0f;
    }
}

// ?Transpose@Matrix44@@QAEXXZ
void Matrix44::Transpose()
{
    // In place, as six swaps. Each swap reads one side into the FPU and the other into a general
    // register, so neither value is clobbered before it is written.
    f32 t;

    t = m10;
    m10 = m01;
    m01 = t;

    t = m20;
    m20 = m02;
    m02 = t;

    t = m21;
    m21 = m12;
    m12 = t;

    t = m30;
    m30 = m03;
    m03 = t;

    t = m31;
    m31 = m13;
    m13 = t;

    t = m32;
    m32 = m23;
    m23 = t;
}

// ?Zero@Matrix44@@QAEXXZ
void Matrix44::Zero()
{
    // Sixteen stores of an integer zero, so every element is +0.0f rather than a computed zero.
    m00 = 0.0f;
    m01 = 0.0f;
    m02 = 0.0f;
    m03 = 0.0f;

    m10 = 0.0f;
    m11 = 0.0f;
    m12 = 0.0f;
    m13 = 0.0f;

    m20 = 0.0f;
    m21 = 0.0f;
    m22 = 0.0f;
    m23 = 0.0f;

    m30 = 0.0f;
    m31 = 0.0f;
    m32 = 0.0f;
    m33 = 0.0f;
}


// ---------------------------------------------------------------------------------------------
// Second pass: the three functions the first pass held back. Read the same way - capstone over the
// original instruction bytes, arity checked with tools/verify_arity.py first. All three are among
// the 6,315 symbols whose `ret imm16` agrees with their mangled parameter list.

// ?Determinant@Matrix44@@QBEMXZ
f32 Matrix44::Determinant() const
{
    // Cofactor expansion along row 0. The original spills the twelve elements of rows 1-3 into
    // stack copies first and reads the copies - they are exact dword copies, so the arithmetic is
    // unaffected and the locals are not reproduced.
    //
    // What IS reproduced is the accumulation order. Each 3x3 minor is built as
    // (((((a + b) + c) - d) - e) - f) with the three positive terms first, and the four minors are
    // then combined strictly left to right. Nothing here passes through an f32 slot: the whole
    // expansion stays on the x87 stack until the single store at the end.
    f64 minor0 = f64 {m12} * m23 * m31 + f64 {m13} * m21 * m32 + f64 {m11} * m22 * m33 -
        f64 {m11} * m23 * m32 - f64 {m12} * m21 * m33 - f64 {m13} * m22 * m31;

    f64 minor1 = f64 {m12} * m23 * m30 + f64 {m13} * m20 * m32 + f64 {m10} * m22 * m33 -
        f64 {m10} * m23 * m32 - f64 {m12} * m20 * m33 - f64 {m13} * m22 * m30;

    f64 minor2 = f64 {m11} * m23 * m30 + f64 {m13} * m20 * m31 + f64 {m10} * m21 * m33 -
        f64 {m10} * m23 * m31 - f64 {m11} * m20 * m33 - f64 {m13} * m21 * m30;

    f64 minor3 = f64 {m11} * m22 * m30 + f64 {m12} * m20 * m31 + f64 {m10} * m21 * m32 -
        f64 {m10} * m22 * m31 - f64 {m11} * m20 * m32 - f64 {m12} * m21 * m30;

    return static_cast<f32>(minor0 * m00 - minor1 * m01 + minor2 * m02 - minor3 * m03);
}

// ?Dot@Matrix44@@QAEAAV1@ABV1@@Z
Matrix44& Matrix44::Dot(const Matrix44& arg1)
{
    // this = this * arg1, with `this` supplying the rows - the opposite way round from
    // Dot(arg1, arg2) above, which computes arg2 * arg1. Both were read off the offsets; the
    // inconsistency is the original's.
    //
    // All sixteen elements are computed before any is written, into f32 stack slots (m00 and m01
    // stay on the FPU stack and are stored last). That is what makes a call where `this` aliases
    // arg1 well defined, and it is why the locals below cannot be collapsed into direct
    // assignments.
    //
    // The four terms of each element are accumulated in a different order in almost every one of
    // the sixteen, and rows 0-1 do not use the same order as rows 2-3. x87 addition is not
    // associative, so none of this can be tidied.
    f32 r00 = static_cast<f32>(
        f64 {m02} * arg1.m20 + f64 {m01} * arg1.m10 + f64 {m00} * arg1.m00 + f64 {m03} * arg1.m30);
    f32 r01 = static_cast<f32>(
        f64 {m02} * arg1.m21 + f64 {m01} * arg1.m11 + f64 {m03} * arg1.m31 + f64 {m00} * arg1.m01);
    f32 r02 = static_cast<f32>(
        f64 {m01} * arg1.m12 + f64 {m02} * arg1.m22 + f64 {m00} * arg1.m02 + f64 {m03} * arg1.m32);
    f32 r03 = static_cast<f32>(
        f64 {m03} * arg1.m33 + f64 {m01} * arg1.m13 + f64 {m02} * arg1.m23 + f64 {m00} * arg1.m03);

    f32 r10 = static_cast<f32>(
        f64 {m12} * arg1.m20 + f64 {m11} * arg1.m10 + f64 {m13} * arg1.m30 + f64 {m10} * arg1.m00);
    f32 r11 = static_cast<f32>(
        f64 {m12} * arg1.m21 + f64 {m11} * arg1.m11 + f64 {m13} * arg1.m31 + f64 {m10} * arg1.m01);
    f32 r12 = static_cast<f32>(
        f64 {m11} * arg1.m12 + f64 {m10} * arg1.m02 + f64 {m12} * arg1.m22 + f64 {m13} * arg1.m32);
    f32 r13 = static_cast<f32>(
        f64 {m13} * arg1.m33 + f64 {m11} * arg1.m13 + f64 {m12} * arg1.m23 + f64 {m10} * arg1.m03);

    f32 r20 = static_cast<f32>(
        f64 {m23} * arg1.m30 + f64 {m21} * arg1.m10 + f64 {m20} * arg1.m00 + f64 {m22} * arg1.m20);
    f32 r21 = static_cast<f32>(
        f64 {m23} * arg1.m31 + f64 {m21} * arg1.m11 + f64 {m22} * arg1.m21 + f64 {m20} * arg1.m01);
    f32 r22 = static_cast<f32>(
        f64 {m23} * arg1.m32 + f64 {m22} * arg1.m22 + f64 {m21} * arg1.m12 + f64 {m20} * arg1.m02);
    f32 r23 = static_cast<f32>(
        f64 {m21} * arg1.m13 + f64 {m23} * arg1.m33 + f64 {m22} * arg1.m23 + f64 {m20} * arg1.m03);

    f32 r30 = static_cast<f32>(
        f64 {m33} * arg1.m30 + f64 {m31} * arg1.m10 + f64 {m30} * arg1.m00 + f64 {m32} * arg1.m20);
    f32 r31 = static_cast<f32>(
        f64 {m33} * arg1.m31 + f64 {m31} * arg1.m11 + f64 {m32} * arg1.m21 + f64 {m30} * arg1.m01);
    f32 r32 = static_cast<f32>(
        f64 {m33} * arg1.m32 + f64 {m32} * arg1.m22 + f64 {m31} * arg1.m12 + f64 {m30} * arg1.m02);
    f32 r33 = static_cast<f32>(
        f64 {m31} * arg1.m13 + f64 {m33} * arg1.m33 + f64 {m32} * arg1.m23 + f64 {m30} * arg1.m03);

    m00 = r00;
    m01 = r01;
    m02 = r02;
    m03 = r03;

    m10 = r10;
    m11 = r11;
    m12 = r12;
    m13 = r13;

    m20 = r20;
    m21 = r21;
    m22 = r22;
    m23 = r23;

    m30 = r30;
    m31 = r31;
    m32 = r32;
    m33 = r33;

    return *this;
}

// ?Print@Matrix44@@QBEXPBD@Z
void Matrix44::Print(const char* arg1) const
{
    // A null name prints "matrix {" instead of "<name> {". Nothing verifies the output, so the two
    // format strings were read out of .rdata at the addresses the two pushes carry rather than
    // guessed: 0x5CE6B8 is "%s {" and 0x5CE6C0 is "matrix {".
    if (arg1)
        Printf("%s {", arg1);
    else
        Printf("matrix {");

    // The rows are handed to Vector4::Print by address - `lea ecx, [esi+0x10]` and so on. Matrix44
    // has no Vector4 members to take the address of, so the cast is the transcription, not a
    // shortcut: the layouts are identical, four contiguous f32 at a 0x10 stride.
    const Vector4* rows = reinterpret_cast<const Vector4*>(this);

    rows[0].Print(" a");
    rows[1].Print(" b");
    rows[2].Print(" c");
    rows[3].Print(" d");

    Printf("}\n");
}
