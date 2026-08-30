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

#include "matrix66.h"

define_dummy_symbol(vector7_matrix66);

// SOURCE. Disassembled out of midtown2.exe at 0x496A00-0x496ECB, the run immediately after
// Matrix33. Nothing here touches a float: every one of the eleven bodies is a fixed sequence of
// `lea ecx, [reg + block]` followed by a call into Matrix33, so the only thing that had to be read
// correctly was which block each offset is - and Dot pins that, see the note in matrix66.h. The
// float behaviour is therefore entirely Matrix33s, and this file cannot change it.
//
// SAFE TO PORT WITHOUT THE FULL CLASS. Matrix66 has no vtable, no statics, and no function whose
// extent contains another symbol, so these eleven can land as a group with no constructor or
// destructor involved - there are none in the binary to port.

// ?Set@Matrix66@@QAEXABVMatrix33@@000@Z
void Matrix66::Set(const Matrix33& arg1, const Matrix33& arg2, const Matrix33& arg3, const Matrix33& arg4)
{
    m00.Set(arg1);
    m01.Set(arg2);
    m10.Set(arg3);
    m11.Set(arg4);
}

// ?Set@Matrix66@@QAEXABV1@@Z
void Matrix66::Set(const Matrix66& arg1)
{
    // A call into the four-block overload, with the four block addresses computed by `lea` off
    // the one argument and `this` passed through in ecx untouched.
    Set(arg1.m00, arg1.m01, arg1.m10, arg1.m11);
}

// ?Add@Matrix66@@QAEXABV1@@Z
void Matrix66::Add(const Matrix66& arg1)
{
    m00.Add(arg1.m00);
    m01.Add(arg1.m01);
    m10.Add(arg1.m10);
    m11.Add(arg1.m11);
}

// ?Add@Matrix66@@QAEXABV1@0@Z
void Matrix66::Add(const Matrix66& arg1, const Matrix66& arg2)
{
    m00.Add(arg1.m00, arg2.m00);
    m01.Add(arg1.m01, arg2.m01);
    m10.Add(arg1.m10, arg2.m10);
    m11.Add(arg1.m11, arg2.m11);
}

// ?Subtract@Matrix66@@QAEXABV1@@Z
void Matrix66::Subtract(const Matrix66& arg1)
{
    m00.Subtract(arg1.m00);
    m01.Subtract(arg1.m01);
    m10.Subtract(arg1.m10);
    m11.Subtract(arg1.m11);
}

// ?Subtract@Matrix66@@QAEXABV1@0@Z
void Matrix66::Subtract(const Matrix66& arg1, const Matrix66& arg2)
{
    m00.Subtract(arg1.m00, arg2.m00);
    m01.Subtract(arg1.m01, arg2.m01);
    m10.Subtract(arg1.m10, arg2.m10);
    m11.Subtract(arg1.m11, arg2.m11);
}

// ?Dot@Matrix66@@QAEXABV1@@Z
void Matrix66::Dot(const Matrix66& arg1)
{
    // The 2x2 block product, accumulated into four Matrix33 locals and only then written back
    // through Set - which is what makes `this == &arg1` safe here and nowhere else in the class.
    // The stack frame is 0xB8: four 0x24-byte results plus one 0x24-byte scratch.
    Matrix33 v00;
    Matrix33 v01;
    Matrix33 v10;
    Matrix33 v11;
    Matrix33 t;

    v00.Dot(m00, arg1.m00);
    t.Dot(m01, arg1.m10);
    v00.Add(t);

    v01.Dot(m00, arg1.m01);
    t.Dot(m01, arg1.m11);
    v01.Add(t);

    v10.Dot(m10, arg1.m00);
    t.Dot(m11, arg1.m10);
    v10.Add(t);

    v11.Dot(m10, arg1.m01);
    t.Dot(m11, arg1.m11);
    v11.Add(t);

    Set(v00, v01, v10, v11);
}

// ?Transpose@Matrix66@@QAEXXZ
void Matrix66::Transpose()
{
    // The diagonal blocks transpose in place; the off-diagonal pair swaps, and the swap needs one
    // 0x24-byte copy because Matrix33::Transpose(const Matrix33&) is being used to move as well as
    // to transpose.
    Matrix33 v1;

    m00.Transpose();

    v1.Set(m01);
    m01.Transpose(m10);
    m10.Transpose(v1);

    m11.Transpose();
}

// ?Transpose@Matrix66@@QAEXABV1@@Z
void Matrix66::Transpose(const Matrix66& arg1)
{
    m00.Transpose(arg1.m00);
    m01.Transpose(arg1.m10);
    m10.Transpose(arg1.m01);
    m11.Transpose(arg1.m11);
}

// ?Inverse@Matrix66@@QAEXABV1@@Z
void Matrix66::Inverse(const Matrix66& arg1)
{
    // The block inverse. Both off-diagonal results are built from the Schur complement of the
    // block that is being inverted, and each has two spellings - one that is valid in general and
    // one that is only reachable when the corresponding block is exactly zero. The choice is made
    // by Matrix33::IsZero, an exact comparison against 0.0f with no tolerance, so a block that is
    // merely tiny takes the general path and the inverses inside it may warn.
    //
    // The 0xD8 frame holds six Matrix33 locals; the names below are the stack slots.
    Matrix33 invA00; // ebp-0x6C
    Matrix33 invA10; // ebp-0xB4
    Matrix33 t;      // ebp-0xD8
    Matrix33 v;      // ebp-0x24, the reused scratch
    Matrix33 w;      // ebp-0x48
    Matrix33 u;      // ebp-0x90

    invA00.Inverse(arg1.m00);
    invA10.Inverse(arg1.m10);
    t.Dot(invA10, arg1.m11);

    v.Set(arg1.m00);
    v.Dot(t);

    // m10 is the inverted Schur complement of arg1.m10, and it is computed into the destination
    // rather than into a local - so this overload requires that *this and arg1 do not alias, which
    // is exactly why Inverse(void) goes through a full 0x90-byte copy.
    m10.Set(arg1.m01);
    m10.Subtract(v);
    m10.Inverse();

    if (arg1.m10.IsZero())
    {
        w.Dot(arg1.m01, m10);
        v.Diagonal(1.0f);
        v.Subtract(w);
        m00.Dot(invA00, v);
    }
    else
    {
        m00.Set(t);
        m00.Dot(m10);
        m00.Negate();
    }

    u.Dot(invA00, arg1.m01);

    v.Set(arg1.m10);
    v.Dot(u);

    m11.Set(arg1.m11);
    m11.Subtract(v);
    m11.Inverse();

    if (arg1.m00.IsZero())
    {
        w.Dot(arg1.m11, m11);
        v.Diagonal(1.0f);
        v.Subtract(w);
        m01.Dot(invA10, v);
    }
    else
    {
        m01.Set(u);
        m01.Dot(m11);
        m01.Negate();
    }
}

// ?Inverse@Matrix66@@QAEXXZ
void Matrix66::Inverse()
{
    // `sub esp, 0x90` - one whole Matrix66 on the stack, which is the second independent witness
    // for the 0x90 size in matrix66.h.
    Matrix66 v1;

    v1.Inverse(*this);

    Set(v1);
}
