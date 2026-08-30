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

#include "matrix33.h"

#include "vector3.h"

define_dummy_symbol(vector7_matrix33);

// SOURCE. Every one of the seventeen bodies below was disassembled out of midtown2.exe at the RVA
// midtown2.map gives for the symbol, not transcribed from MM2_RE_KIT. The kit has a Matrix33
// folder, but the class has no IDB type, so every member access in the pseudocode is spelled
// `*(float *)(a1 + 20)` and the offsets have to be read anyway - at which point the machine code
// is the shorter and more honest source. The whole class occupies one contiguous run,
// 0x496230-0x4969FC, and tools/verify_extents.py reports no data word pointing inside any of the
// seventeen extents.
//
// FLOAT FIDELITY. Same rule as matrix34.cpp: the 1999 build is x87 at the CRT default 53-bit
// precision control, so a value that reaches memory through `fstp dword ptr` is written as f32
// here and a value that stays in an st(n) register is written as f64. Sub-expression grouping and
// accumulation order are reproduced exactly - Dot builds each element as ((a + b) + c) in a
// specific order that is not the textbook one, and (a+b)+c is not a+(b+c) in float.
//
// RELATIONSHIP TO Matrix34. Matrix33::Dot is instruction-for-instruction Matrix34::Dot3x3, and
// Matrix33::DotCrossProdMtx is Matrix34::Dot3x3CrossProdMtx; Matrix33::Inverse is the 3x3 half of
// Matrix34::Inverse, down to sharing the "Matrix34::Inverse()" text in its warning. The two files
// were cross-checked against each other, which is a real second opinion because matrix34.cpp was
// derived from a different source.

// ?Warningf@@YAXPBDZZ - still the original, at 0x4C97B0. Declared here for the same reason
// matrix34.cpp declares it: nothing in the tree owns the arts print family yet.
ARTS_IMPORT void Warningf(const char* format, ...);

// ?Set@Matrix33@@QAEXABV1@@Z
void Matrix33::Set(const Matrix33& arg1)
{
    // Nine dword moves through edx, not float loads - a bit copy.
    m00 = arg1.m00;
    m01 = arg1.m01;
    m02 = arg1.m02;
    m10 = arg1.m10;
    m11 = arg1.m11;
    m12 = arg1.m12;
    m20 = arg1.m20;
    m21 = arg1.m21;
    m22 = arg1.m22;
}

// ?Add@Matrix33@@QAEXABV1@@Z
void Matrix33::Add(const Matrix33& arg1)
{
    // m10 comes out with the operands the other way round. Addition is commutative in IEEE so it
    // cannot change the result, but it is what the instruction stream does - and the same quirk
    // is in Matrix34::Add, which is where this code came from.
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

// ?Add@Matrix33@@QAEXABV1@0@Z
void Matrix33::Add(const Matrix33& arg1, const Matrix33& arg2)
{
    // Row 1 is the flipped one here.
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

// ?Subtract@Matrix33@@QAEXABV1@@Z
void Matrix33::Subtract(const Matrix33& arg1)
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

// ?Subtract@Matrix33@@QAEXABV1@0@Z
void Matrix33::Subtract(const Matrix33& arg1, const Matrix33& arg2)
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

// ?IsZero@Matrix33@@QBE_NXZ
bool Matrix33::IsZero() const
{
    // Nine `fcomp` against the 0.0f at 0x5B040C, short-circuiting to `xor al, al` on the first
    // element that fails. An exact comparison, no epsilon - and -0.0f compares equal, which
    // Matrix66::Inverse relies on when it picks its degenerate branch.
    //
    // The test after each compare is `test ah, 0x40` - C3 alone - and `je` to the false exit. C3
    // is set when the two are equal AND when the compare is unordered, so the original treats a
    // NaN element as a zero and keeps scanning. `m00 != 0.0f` is true on a NaN and would return
    // false instead, so each element is spelled as the pair of strict compares that is false on a
    // NaN. It matters: Matrix66::Inverse picks its branch on this bool.
    if (m00 < 0.0f || m00 > 0.0f)
        return false;

    if (m01 < 0.0f || m01 > 0.0f)
        return false;

    if (m02 < 0.0f || m02 > 0.0f)
        return false;

    if (m10 < 0.0f || m10 > 0.0f)
        return false;

    if (m11 < 0.0f || m11 > 0.0f)
        return false;

    if (m12 < 0.0f || m12 > 0.0f)
        return false;

    if (m20 < 0.0f || m20 > 0.0f)
        return false;

    if (m21 < 0.0f || m21 > 0.0f)
        return false;

    if (m22 < 0.0f || m22 > 0.0f)
        return false;

    return true;
}

// ?Dot@Matrix33@@QAEXABV1@@Z
void Matrix33::Dot(const Matrix33& arg1)
{
    // *this = *this * arg1, row-major. Seven of the nine elements are spilled to float stack
    // slots and two - m01 and m00 - are kept in x87 registers to the end, so they round once
    // fewer on the way. Instruction for instruction Matrix34::Dot3x3(const Matrix34&).
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

// ?Dot@Matrix33@@QAEXABV1@0@Z
void Matrix33::Dot(const Matrix33& arg1, const Matrix33& arg2)
{
    // *this = arg1 * arg2. No temporaries at all - each element is computed and stored before the
    // next begins, so an argument that aliases *this is read back after it has been overwritten
    // and the result is wrong from m01 onwards. The original has the same hazard, element for
    // element, and the store order below reproduces it rather than repairing it.
    m00 = static_cast<f32>(f64 {arg1.m00} * arg2.m00 + f64 {arg1.m01} * arg2.m10 + f64 {arg2.m20} * arg1.m02);
    m01 = static_cast<f32>(f64 {arg1.m01} * arg2.m11 + f64 {arg2.m21} * arg1.m02 + f64 {arg2.m01} * arg1.m00);
    m02 = static_cast<f32>(f64 {arg1.m01} * arg2.m12 + f64 {arg2.m22} * arg1.m02 + f64 {arg2.m02} * arg1.m00);
    m10 = static_cast<f32>(f64 {arg1.m11} * arg2.m10 + f64 {arg1.m10} * arg2.m00 + f64 {arg1.m12} * arg2.m20);
    m11 = static_cast<f32>(f64 {arg1.m12} * arg2.m21 + f64 {arg1.m11} * arg2.m11 + f64 {arg1.m10} * arg2.m01);
    m12 = static_cast<f32>(f64 {arg1.m12} * arg2.m22 + f64 {arg1.m11} * arg2.m12 + f64 {arg1.m10} * arg2.m02);
    m20 = static_cast<f32>(f64 {arg1.m20} * arg2.m00 + f64 {arg2.m20} * arg1.m22 + f64 {arg1.m21} * arg2.m10);
    m21 = static_cast<f32>(f64 {arg2.m21} * arg1.m22 + f64 {arg2.m11} * arg1.m21 + f64 {arg2.m01} * arg1.m20);
    m22 = static_cast<f32>(f64 {arg2.m22} * arg1.m22 + f64 {arg2.m12} * arg1.m21 + f64 {arg2.m02} * arg1.m20);
}

// ?DotCrossProdMtx@Matrix33@@QAEXABVVector3@@@Z
void Matrix33::DotCrossProdMtx(const Vector3& arg1)
{
    // *this = *this * CrossProduct(arg1), row by row, without materialising the skew matrix. Per
    // row the y term is rounded through a float slot while x and z stay in registers, and the
    // stores go y, z, x so every original in the row is read before any of them is overwritten.
    // Instruction for instruction Matrix34::Dot3x3CrossProdMtx.
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

// ?CrossProduct@Matrix33@@QAEXABVVector3@@@Z
void Matrix33::CrossProduct(const Vector3& arg1)
{
    // The skew-symmetric matrix K with K * v == arg1 % v. The diagonal is stored as an integer
    // zero out of a zeroed edx, not as a float, so it is exactly +0.0f.
    m01 = -arg1.z;
    m00 = 0.0f;
    m02 = arg1.y;

    m10 = arg1.z;
    m11 = 0.0f;
    m12 = -arg1.x;

    m20 = -arg1.y;
    m22 = 0.0f;
    m21 = arg1.x;
}

// ?Diagonal@Matrix33@@QAEXM@Z
void Matrix33::Diagonal(f32 arg1)
{
    // The argument never reaches the x87 stack: it is moved dword-for-dword out of [ebp+8] into
    // the three diagonal slots, and the off-diagonal ones are cleared from a zeroed register.
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

// ?Diagonal@Matrix33@@QAEXABVVector3@@@Z
void Matrix33::Diagonal(const Vector3& arg1)
{
    m00 = arg1.x;
    m01 = 0.0f;
    m02 = 0.0f;
    m10 = 0.0f;
    m11 = arg1.y;
    m12 = 0.0f;
    m20 = 0.0f;
    m21 = 0.0f;
    m22 = arg1.z;
}

// ?Negate@Matrix33@@QAEXXZ
void Matrix33::Negate()
{
    // Nine `fchs`, so a zero becomes a negative zero and a NaN keeps its payload. Not a multiply
    // by -1.0f, which would differ on both counts.
    m00 = -m00;
    m01 = -m01;
    m02 = -m02;
    m10 = -m10;
    m11 = -m11;
    m12 = -m12;
    m20 = -m20;
    m21 = -m21;
    m22 = -m22;
}

// ?Transpose@Matrix33@@QAEXXZ
void Matrix33::Transpose()
{
    // Three swaps. The upper triangle goes onto the x87 stack and the lower into integer
    // registers, which is how the original gets away with no stack temporary.
    f32 v1 = m01;
    f32 v2 = m02;
    f32 v3 = m12;

    m01 = m10;
    m02 = m20;
    m12 = m21;

    m10 = v1;
    m20 = v2;
    m21 = v3;
}

// ?Transpose@Matrix33@@QAEXABV1@@Z
void Matrix33::Transpose(const Matrix33& arg1)
{
    // A column of arg1 is pushed onto the x87 stack whole and popped into a row of *this, three
    // at a time. That is only an intra-column reordering: each column is still loaded after the
    // previous column has been stored, so nine straight assignments produce byte-identical
    // results, including when arg1 aliases *this - where both spellings are equally wrong,
    // because the second column reads a +0x04 the first column has already overwritten.
    m00 = arg1.m00;
    m01 = arg1.m10;
    m02 = arg1.m20;

    m10 = arg1.m01;
    m11 = arg1.m11;
    m12 = arg1.m21;

    m20 = arg1.m02;
    m21 = arg1.m12;
    m22 = arg1.m22;
}

// ?Inverse@Matrix33@@QAEXXZ
void Matrix33::Inverse()
{
    // Inverse through the adjugate - the same code as the 3x3 part of Matrix34::Inverse, including
    // the warning text, which still says "Matrix34" because the two share one pooled literal at
    // 0x5C76F8. Two of the three first-column cofactors are spilled to float slots and the third
    // stays in a register, so the determinant is not built from three equally-rounded terms.
    f64 v1 = f64 {m11} * m22 - f64 {m21} * m12;
    f32 v6 = static_cast<f32>(f64 {m10} * m22 - f64 {m20} * m12);
    f32 v5 = static_cast<f32>(f64 {m21} * m10 - f64 {m20} * m11);
    f64 v2 = v1 * m00 - f64 {v6} * m01 + f64 {v5} * m02;

    // `fcom` against the 0.0f at 0x5B040C, then `test ah, 0x40; je` to the inverting path - so
    // the warn path is taken when C3 is set, which is both "the determinant is exactly zero" and
    // "the compare is unordered". A NaN determinant therefore warns and leaves the matrix alone
    // in the original; `v2 == 0.0` is false on a NaN and would invert. (Matrix34::Inverse has the
    // same instruction and the plain `v2 == 0.0`; it diverges from the original on that input.)
    if (!(v2 < 0.0 || v2 > 0.0))
    {
        // Singular: warn and leave the matrix exactly as it was.
        Warningf("Matrix34::Inverse()- Matrix not invertable");
    }
    else
    {
        f64 v3 = 1.0 / v2;
        Matrix33 v4;

        v4.m00 = static_cast<f32>(v3 * v1);
        v4.m10 = static_cast<f32>(-(v3 * v6));
        v4.m20 = static_cast<f32>(v3 * v5);
        v4.m01 = static_cast<f32>(-((f64 {m01} * m22 - f64 {m21} * m02) * v3));
        v4.m11 = static_cast<f32>((f64 {m00} * m22 - f64 {m20} * m02) * v3);
        v4.m21 = static_cast<f32>(-((f64 {m21} * m00 - f64 {m20} * m01) * v3));
        v4.m02 = static_cast<f32>((f64 {m01} * m12 - f64 {m02} * m11) * v3);
        v4.m12 = static_cast<f32>(-((f64 {m00} * m12 - f64 {m10} * m02) * v3));
        v4.m22 = static_cast<f32>((f64 {m11} * m00 - f64 {m10} * m01) * v3);

        *this = v4;
    }
}

// ?Inverse@Matrix33@@QAEXABV1@@Z
void Matrix33::Inverse(const Matrix33& arg1)
{
    // Copy to a 0x24-byte stack local, invert in place, copy home. Note the consequence: when
    // arg1 is singular the warning fires and *this is overwritten with an unmodified copy of
    // arg1, which is not the same thing as being left alone.
    Matrix33 v1;

    v1.Set(arg1);
    v1.Inverse();

    Set(v1);
}
