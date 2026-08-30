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

#include "vector4.h"

#include "data7/printer.h"
#include "matrix44.h"
#include "vector3.h"

#include <cmath>

define_dummy_symbol(vector7_vector4);

// All thirteen Vector4 methods, read off the DISASSEMBLY of midtown2.exe rather than the Hex-Rays
// dump. The dump was used only to cross-check, and it could not have been the source: its
// Vector4::Set is typed against ImGuiStorage, its Max takes an ImPlot::GetterFuncPtr, and its two
// Matrix44 overloads are not in the kit at all - Trap 3, the 286 real functions with no
// pseudocode. That is the foreign-symbol contamination README.md warns about, and none of it
// survived into this file.
//
// ARITY WAS CHECKED, NOT ASSUMED. Every function's terminating `ret imm16` was compared against
// the parameter list its mangled name encodes, because vector7 does contain map symbols where the
// two disagree - see the warning at the top of vector3.cpp. All thirteen here agree.
//
// FLOAT FIDELITY. The 1999 build is x87 at the CRT's default 53-bit precision control. A value
// that stays in an st(n) register is written f64 here and one that passes through a float stack
// slot is written f32, because the second rounds and the first does not. Accumulation order is
// reproduced verbatim: floating-point addition is not associative, and this feeds the clipper.

// ?ComputePlane@Vector4@@QAEXABVVector3@@00@Z
void Vector4::ComputePlane(const Vector3& arg1, const Vector3& arg2, const Vector3& arg3)
{
    // The plane through three points, anchored at arg1, with edge vectors arg2 - arg1 and
    // arg3 - arg1.
    //
    // Three of the six edge components are spilled to float stack slots and three are kept in
    // registers, and the split is not cosmetic: the cross product below mixes spilled and register
    // values, so making them all f32 - or all f64 - moves the normal in the last bit.
    f64 e1x = f64 {arg2.x} - arg1.x;
    f64 e1y = f64 {arg2.y} - arg1.y;
    f32 e1z = static_cast<f32>(f64 {arg2.z} - arg1.z);
    f32 e2x = static_cast<f32>(f64 {arg3.x} - arg1.x);
    f64 e2y = f64 {arg3.y} - arg1.y;
    f32 e2z = static_cast<f32>(f64 {arg3.z} - arg1.z);

    f32 nz = static_cast<f32>(e2y * e1x - e1y * e2x);
    f32 nx = static_cast<f32>(f64 {e2z} * e1y - e2y * e1z);
    f64 ny = f64 {e1z} * e2x - f64 {e2z} * e1x;

    f64 sq = f64 {nx} * nx + ny * ny + f64 {nz} * nz;
    f64 inv;

    // fcom against 0.0f, then `test ah, 0x40` - C3, which is set when the two are equal and also
    // when the compare is unordered. Written as the negation of the strict opposite so a NaN sum
    // takes the branch the original takes; plain `sq == 0.0` is false on a NaN and would not.
    if (!(sq != 0.0))
        inv = 0.0;
    else
        inv = 1.0 / std::sqrt(sq);

    f32 ox = static_cast<f32>(inv * nx);
    f32 oy = static_cast<f32>(inv * ny);
    f64 oz = inv * nz;

    // The plane constant is the anchor point projected onto the normal, accumulated z, x, y.
    f64 d = oz * arg1.z + f64 {ox} * arg1.x + f64 {oy} * arg1.y;

    x = ox;
    y = oy;
    z = static_cast<f32>(oz);
    w = static_cast<f32>(d);
}

// ?ComputePlane@Vector4@@QAEXABVVector3@@0@Z
void Vector4::ComputePlane(const Vector3& arg1, const Vector3& arg2)
{
    // Plane from a point (arg1) and an already-normalised normal (arg2). The normal is copied as
    // three raw dwords, not through the FPU.
    x = arg2.x;
    y = arg2.y;
    z = arg2.z;

    // The original stores a zero here and overwrites it on the next instruction. Dead, but it is
    // what the instruction stream does and keeping it costs nothing.
    w = 0.0f;

    w = static_cast<f32>(f64 {arg1.z} * z + f64 {arg1.y} * y + f64 {arg1.x} * x);
}

// ?Cross@Vector4@@QAEXABV1@0@Z
void Vector4::Cross(const Vector4& arg1, const Vector4& arg2)
{
    // The 3D cross product of the xyz parts, with w forced to 1. Y goes through a float stack slot
    // while X and Z stay in registers - the same split Vector3::Cross has - and the store order is
    // w, y, z, x, which decides the outcome when the destination aliases an operand.
    f64 nx = f64 {arg2.z} * arg1.y - f64 {arg1.z} * arg2.y;
    f32 ny = static_cast<f32>(f64 {arg1.z} * arg2.x - f64 {arg1.x} * arg2.z);
    f64 a = f64 {arg1.x} * arg2.y;
    f64 b = f64 {arg2.x} * arg1.y;

    w = 1.0f;
    y = ny;
    z = static_cast<f32>(a - b);
    x = static_cast<f32>(nx);
}

// ?Dot@Vector4@@QAEAAV1@ABV1@ABVMatrix44@@@Z
Vector4& Vector4::Dot(const Vector4& arg1, const Matrix44& arg2)
{
    // Row vector times matrix, all four components. The four columns are accumulated in different
    // orders - the w term comes first for x and second for y, z and w - so these four lines are
    // deliberately not written to the same shape.
    x = static_cast<f32>(
        f64 {arg2.m30} * arg1.w + f64 {arg2.m10} * arg1.y + f64 {arg2.m20} * arg1.z + f64 {arg1.x} * arg2.m00);
    y = static_cast<f32>(
        f64 {arg2.m01} * arg1.x + f64 {arg2.m31} * arg1.w + f64 {arg2.m11} * arg1.y + f64 {arg2.m21} * arg1.z);
    z = static_cast<f32>(
        f64 {arg2.m02} * arg1.x + f64 {arg2.m32} * arg1.w + f64 {arg2.m12} * arg1.y + f64 {arg2.m22} * arg1.z);
    w = static_cast<f32>(
        f64 {arg2.m03} * arg1.x + f64 {arg2.m33} * arg1.w + f64 {arg2.m13} * arg1.y + f64 {arg2.m23} * arg1.z);

    return *this;
}

// ?Dot3@Vector4@@QBEMABV1@@Z
f32 Vector4::Dot3(const Vector4& arg1) const
{
    return static_cast<f32>(f64 {arg1.z} * z + f64 {arg1.y} * y + f64 {arg1.x} * x);
}

// ?Dot3x3@Vector4@@QAEAAV1@ABV1@ABVMatrix44@@@Z
Vector4& Vector4::Dot3x3(const Vector4& arg1, const Matrix44& arg2)
{
    // The rotation-only form of Dot: no translation row, and w is left exactly as it was.
    x = static_cast<f32>(f64 {arg2.m20} * arg1.z + f64 {arg2.m10} * arg1.y + f64 {arg1.x} * arg2.m00);
    y = static_cast<f32>(f64 {arg2.m01} * arg1.x + f64 {arg2.m21} * arg1.z + f64 {arg2.m11} * arg1.y);
    z = static_cast<f32>(f64 {arg2.m02} * arg1.x + f64 {arg2.m22} * arg1.z + f64 {arg2.m12} * arg1.y);

    return *this;
}

// ?Dot@Vector4@@QBEMABV1@@Z
f32 Vector4::Dot(const Vector4& arg1) const
{
    // Summed w, z, y, x - the order the original accumulates in.
    return static_cast<f32>(f64 {arg1.w} * w + f64 {arg1.z} * z + f64 {arg1.y} * y + f64 {arg1.x} * x);
}

// ?Max@Vector4@@QAEXABV1@0@Z
void Vector4::Max(const Vector4& arg1, const Vector4& arg2)
{
    // test ah, 0x41 - C3|C0, set when arg1 <= arg2 and also when the compare is unordered, so a
    // NaN anywhere yields arg2. `!(a > b)` reproduces that; `a <= b` would not.
    x = !(arg1.x > arg2.x) ? arg2.x : arg1.x;
    y = !(arg1.y > arg2.y) ? arg2.y : arg1.y;
    z = !(arg1.z > arg2.z) ? arg2.z : arg1.z;

    // w is selected by the same compare but copied as a raw dword rather than through the FPU. The
    // difference is only visible for a signalling NaN.
    w = !(arg1.w > arg2.w) ? arg2.w : arg1.w;
}

// ?Min@Vector4@@QAEXABV1@0@Z
void Vector4::Min(const Vector4& arg1, const Vector4& arg2)
{
    // test ah, 0x01 - C0 alone, set when arg1 < arg2 and when unordered.
    x = !(arg1.x >= arg2.x) ? arg1.x : arg2.x;
    y = !(arg1.y >= arg2.y) ? arg1.y : arg2.y;
    z = !(arg1.z >= arg2.z) ? arg1.z : arg2.z;
    w = !(arg1.w >= arg2.w) ? arg1.w : arg2.w;
}

// ?Outcode@Vector4@@QBEHXZ
i32 Vector4::Outcode() const
{
    // The homogeneous clip outcode, and it never touches the FPU - the whole thing is integer
    // arithmetic on the raw float bit patterns.
    //
    // For each of x, y and z it asks whether |w| < |component| by clearing the sign bits and
    // subtracting: both operands are then non-negative, so no wrap is possible and the difference's
    // sign bit is the answer. That one bit is shifted into a slot chosen by the component's own
    // sign, so x contributes bit 0 or 1, y bit 2 or 3, z bit 4 or 5, and a point inside the frustum
    // comes out zero.
    const u32* bits = reinterpret_cast<const u32*>(this);

    u32 bx = bits[0];
    u32 by = bits[1];
    u32 bz = bits[2];
    u32 aw = bits[3] & 0x7FFFFFFF;

    u32 out = ((aw - (by & 0x7FFFFFFF)) >> 31) << ((by >> 31) + 2);

    out |= ((aw - (bz & 0x7FFFFFFF)) >> 31) << ((bz >> 31) + 4);
    out |= ((aw - (bx & 0x7FFFFFFF)) >> 31) << (bx >> 31);

    return static_cast<i32>(out);
}

// ?Print@Vector4@@QBEXPBD@Z
void Vector4::Print(const char* arg1) const
{
    Printf("%s: %f,%f,%f,%f\n", arg1, x, y, z, w);
}

// ?Print@Vector4@@QBEXXZ
void Vector4::Print() const
{
    Printf("%f,%f,%f,%f\n", x, y, z, w);
}

// ?Set@Vector4@@QAEXMMMM@Z
void Vector4::Set(f32 arg1, f32 arg2, f32 arg3, f32 arg4)
{
    // Four integer moves, not four FPU stores - the bit patterns are copied verbatim.
    x = arg1;
    y = arg2;
    z = arg3;
    w = arg4;
}

// ?Subtract@Vector4@@QAEXABVVector3@@0@Z
void Vector4::Subtract(const Vector3& arg1, const Vector3& arg2)
{
    // Three components only. w is deliberately left alone, so this builds a direction inside an
    // existing plane equation without disturbing its constant.
    x = arg1.x - arg2.x;
    y = arg1.y - arg2.y;
    z = arg1.z - arg2.z;
}
