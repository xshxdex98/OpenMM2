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

#include "freefuncs.h"

#include "vector7/vector3.h"

#include <cmath>

define_dummy_symbol(misc_mathfuncs);

// The floating-point free functions. Every one of these was transcribed from the DISASSEMBLY of
// midtown2.exe; the Hex-Rays output in MM2_RE_KIT was used only to confirm the reading, never as
// the source. That matters here more than anywhere else in the port, for two reasons this file
// runs into constantly.
//
// FLOAT FIDELITY. The 1999 build is x87. Intermediates live in 80-bit registers and round to f32
// only when they are stored to memory, so `f64` below means "the original kept this in a register"
// and `f32` means "the original stored it". Sub-expression grouping and accumulation order are
// reproduced exactly as the fld/fmul/faddp sequence has them, because neither is associative at
// finite precision and physics amplifies the difference. Build with /arch:IA32 so the compiler
// uses x87 as well.
//
// COMPARISON POLARITY. `fcom`/`fcomp` set three flag bits and the original tests a MASK of them,
// which is not the same thing as a C++ relational operator:
//
//     test ah, 0x01   ->  C0        ->  a < b,  OR a and b are unordered
//     test ah, 0x40   ->  C3        ->  a == b, OR unordered
//     test ah, 0x41   ->  C3 | C0   ->  a <= b, OR unordered
//
// An unordered compare (either operand NaN) sets C3, C2 and C0 together, so every one of those
// masks comes out TRUE on a NaN, where the plain C++ operator comes out false. To keep the NaN
// paths identical - and NaNs do reach this code; that is what IsNearZero and EnableNanSignal are
// for - a branch taken on the mask being SET is written here as the negation of the strict
// opposite, `!(a > b)` rather than `a <= b`. Where the original branches on the mask being CLEAR
// the plain operator is already exact and is used as-is.

// ?Lerp@@YAMMMM@Z
f32 Lerp(f32 arg1, f32 arg2, f32 arg3)
{
    // fld [arg3]; fsub [arg2]; fmul [arg1]; fadd [arg2] - one register chain with no intermediate
    // store, so the whole thing is f64 and rounds once, on the way out.
    return static_cast<f32>((f64 {arg3} - arg2) * arg1 + arg2);
}

// ?Max@@YAMMM@Z
f32 Max(f32 arg1, f32 arg2)
{
    // test ah,0x41 - takes arg2 when arg1 <= arg2, or when the two are unordered.
    return !(arg1 > arg2) ? arg2 : arg1;
}

// ?wpmax@@YAMMM@Z
f32 wpmax(f32 arg1, f32 arg2)
{
    // Byte-for-byte the same function as Max, compiled a second time into the waypoint code.
    return !(arg1 > arg2) ? arg2 : arg1;
}

// ?max@@YAMMM@Z
f32 max(f32 arg1, f32 arg2)
{
    // test ah,0x01 - C0 alone. Differs from Max only on equal arguments, where this returns the
    // FIRST and Max returns the second. Indistinguishable for f32 unless the zeros are signed.
    return !(arg1 >= arg2) ? arg2 : arg1;
}

// ?min@@YAMMM@Z
f32 min(f32 arg1, f32 arg2)
{
    // Branch is on the mask being clear, so the plain operator is already exact: a NaN leaves
    // C3|C0 set and falls through to arg1.
    return (arg1 > arg2) ? arg2 : arg1;
}

// ?wpmin@@YAMMM@Z
f32 wpmin(f32 arg1, f32 arg2)
{
    return (arg1 >= arg2) ? arg2 : arg1;
}

// ?IsNearZero@@YAHMM@Z
i32 IsNearZero(f32 arg1, f32 arg2)
{
    // Not a predicate despite the name - a three-way sign test of arg1 against the tolerance arg2,
    // and the map confirms the return type is int rather than bool.
    if (arg1 > arg2)
        return 1;

    if (-arg2 > arg1)
        return -1;

    return 0;
}

// ?inRange@@YAHMMM@Z
i32 inRange(f32 arg1, f32 arg2, f32 arg3)
{
    // arg2 and arg3 are the bounds in either order. The original picks them with two SEPARATE
    // compares of the same pair, using two different masks, so on a NaN bound both picks fall to
    // arg2 as the low end and arg3 as the high end.
    f32 lo = !(arg2 >= arg3) ? arg2 : arg3;
    f32 hi = (arg2 > arg3) ? arg2 : arg3;

    // hi goes through a dword store (it is written over the arg2 slot) while lo stays in ST(0).
    // Both are f32 copies of an f32 argument either way, so nothing rounds.
    if (!(arg1 >= lo))
        return 0;

    if (arg1 > hi)
        return 0;

    return 1;
}

// ?AddDeadZone@@YAMMM@Z
f32 AddDeadZone(f32 arg1, f32 arg2)
{
    // Remaps a control axis so |arg1| <= arg2 reads as zero and the rest of the range stretches
    // back out to reach 1. Note the divisor is (1 - arg2) in both branches, so the negative side
    // is scaled by the same factor as the positive one rather than by (1 + arg2).
    if (arg1 > arg2)
        return static_cast<f32>((f64 {arg1} - arg2) / (1.0 - f64 {arg2}));

    if (-arg2 > arg1)
        return static_cast<f32>((f64 {arg1} + arg2) / (1.0 - f64 {arg2}));

    return 0.0f;
}

// ?IsPointBehindPlane@@YA_NABVVector3@@00M@Z
bool IsPointBehindPlane(const Vector3& arg1, const Vector3& arg2, const Vector3& arg3, f32 arg4)
{
    // dot(arg1 - arg2, arg3) tested against the plane constant arg4. All three differences are
    // taken before any multiply - they are exact, being two f32 values subtracted at 80-bit
    // precision, so writing them inline is the same value the original held in ST(n). The products
    // are summed z + y first and then x; that grouping is the fxch/faddp order and is not the
    // order anyone would write by hand.
    f64 d = (f64 {arg1.z} - arg2.z) * arg3.z + (f64 {arg1.y} - arg2.y) * arg3.y;

    d = d + (f64 {arg1.x} - arg2.x) * arg3.x;

    // test ah,0x01 - true on C0, which is set for "less than" AND for unordered.
    return !(d >= arg4);
}

// ?IsPointNearPlane@@YA_NABVVector3@@00M@Z
bool IsPointNearPlane(const Vector3& arg1, const Vector3& arg2, const Vector3& arg3, f32 arg4)
{
    // The same dot product as IsPointBehindPlane, but the multiplies are issued y before z. The
    // sum is still (y + z) + x, so the result is bit-identical; only the instruction order differs
    // and it is kept because that is what the machine code does.
    f64 d = (f64 {arg1.y} - arg2.y) * arg3.y + (f64 {arg1.z} - arg2.z) * arg3.z;

    d = d + (f64 {arg1.x} - arg2.x) * arg3.x;

    return !(std::fabs(d) > arg4);
}

// ?IsPointInBox@@YA_NABVVector3@@MMM@Z
bool IsPointInBox(const Vector3& arg1, f32 arg2, f32 arg3, f32 arg4)
{
    // Six compares in x, -x, y, -y, z, -z order, each bailing out immediately. The box is centred
    // on the origin with half-extents arg2 / arg3 / arg4.
    if (arg1.x > arg2)
        return false;

    if (-arg2 > arg1.x)
        return false;

    if (arg1.y > arg3)
        return false;

    if (-arg3 > arg1.y)
        return false;

    if (arg1.z > arg4)
        return false;

    if (-arg4 > arg1.z)
        return false;

    return true;
}

// ?IsPointInRegion@@YA_NMMPBVVector3@@H@Z
bool IsPointInRegion(f32 arg1, f32 arg2, const Vector3* arg3, i32 arg4)
{
    // Crossing-number point-in-polygon in the XZ plane; Y is never read. The wrap index is
    // computed with a real signed idiv every iteration - `(i + 1) % arg4` - rather than the usual
    // "reset at the end" trick, so the division cost is per-edge. Kept as-is.
    i32 count = 0;

    for (i32 i = 0; i < arg4; ++i)
    {
        const Vector3& cur = arg3[i];
        const Vector3& next = arg3[(i + 1) % arg4];

        // Both endpoints strictly on the same side in x, or both below the point in z: no
        // crossing is possible, so the edge is skipped without any arithmetic.
        if (cur.x < arg1 && next.x < arg1)
            continue;

        if (cur.x > arg1 && next.x > arg1)
            continue;

        if (cur.z < arg2 && next.z < arg2)
            continue;

        // test ah,0x40 - C3 alone, set for equal AND for unordered, so a vertical edge or a NaN x
        // is skipped. Written as the negation of both strict compares to keep the NaN case.
        if (!(cur.x < next.x || cur.x > next.x))
            continue;

        // Both endpoints above the point in z: the edge crosses, and there is nothing to
        // interpolate.
        if (cur.z > arg2 && next.z > arg2)
        {
            ++count;
            continue;
        }

        // Otherwise find where the edge crosses the point's x and test that z. The order is
        // ((dz * dx) / ddx) + cur.z, exactly as fmulp / fdivp / fadd have it.
        f64 t = (f64 {next.z} - cur.z) * (f64 {arg1} - cur.x) / (f64 {next.x} - cur.x) + cur.z;

        if (t > arg2)
            ++count;
    }

    // `and eax,1` - odd crossing count means inside.
    return (count & 1) != 0;
}

// ?FindTValueSegToOrigin@@YAMABVVector3@@0@Z
f32 FindTValueSegToOrigin(const Vector3& arg1, const Vector3& arg2)
{
    // Parameter along the segment from -arg1 in direction arg2 that comes closest to the origin,
    // clamped to [0, 1] by returning the endpoints outright.
    //
    // The dot product accumulates z, then y, then x, and is then negated in place.
    f64 t = f64 {arg1.z} * arg2.z + f64 {arg1.y} * arg2.y;

    t = -(t + f64 {arg1.x} * arg2.x);

    // `fst dword ptr [ebp+0Ch]` - the negated dot is stored back over the arg2 POINTER slot on the
    // stack, which rounds it to f32, and every use after this point reads that f32 back. ST(0)
    // still holds the unrounded value, and it is the UNROUNDED one the zero test below compares.
    // Two precisions of the same quantity three instructions apart; both are reproduced.
    f32 ts = static_cast<f32>(t);

    if (!(t > 0.0))
        return 0.0f;

    // |arg2|^2, accumulated (x + y) + z - a different order from the dot product above.
    f64 d = (f64 {arg2.x} * arg2.x + f64 {arg2.y} * arg2.y) + f64 {arg2.z} * arg2.z;

    d = d - ts;

    if (!(d > 0.0))
        return 1.0f;

    // The denominator is rebuilt as (d + ts) rather than reusing the squared magnitude, and after
    // d has been rounded through the subtraction the two are not the same number. Keep the add.
    return static_cast<f32>(ts / (d + ts));
}

// ?RealQuadratic@@YAHMMPAM0M@Z
i32 RealQuadratic(f32 arg1, f32 arg2, f32* arg3, f32* arg4, f32 arg5)
{
    // Roots of x^2 + arg1*x + arg2, with arg5 a relative tolerance on the discriminant. Returns
    // how many roots were written: 2, 1 or 0.
    //
    // The tolerance is scaled by arg2 and stored back over the arg5 slot, which rounds it to f32.
    f32 ce = static_cast<f32>(f64 {arg2} * arg5);

    // (b*b) * 0.25 - c, held at register precision throughout.
    f64 disc = f64 {arg1} * arg1 * 0.25 - arg2;

    if (disc > ce)
    {
        f64 s = std::sqrt(disc);
        f64 h = f64 {arg1} * -0.5;

        // Store order is arg3 then arg4, which matters if a caller passes the same pointer twice.
        *arg3 = static_cast<f32>(h - s);
        *arg4 = static_cast<f32>(h + s);

        return 2;
    }

    // fcompp against -(ce): a discriminant inside the tolerance band counts as a double root and
    // writes only arg3, leaving whatever arg4 pointed at untouched.
    if (!(disc >= -ce))
        return 0;

    *arg3 = static_cast<f32>(f64 {arg1} * -0.5);

    return 1;
}

// ?OrderIntersections@@YAXPAM000PAH1@Z
void OrderIntersections(f32* arg1, f32* arg2, f32* arg3, f32* arg4, i32* arg5, i32* arg6)
{
    // Sorts three parallel pairs by the first pair, so that *arg1 <= *arg2 afterwards. The branch
    // is on the mask being clear, so the plain `>` is exact and a NaN leaves everything alone.
    if (*arg1 > *arg2)
    {
        // The original swaps each float pair half through an integer move and half through the FPU
        // (`mov eax,[edx]` one direction, `fld` / `fstp` the other). That is only distinguishable
        // for a signalling NaN, which the FPU leg would quiet.
        f32 t1 = *arg1;
        *arg1 = *arg2;
        *arg2 = t1;

        f32 t2 = *arg3;
        *arg3 = *arg4;
        *arg4 = t2;

        i32 t3 = *arg5;
        *arg5 = *arg6;
        *arg6 = t3;
    }
}

// ?frand@@YAMXZ
f32 frand()
{
    // The multiplier is stored as an f32 in the binary and is 2^-15 exactly, so it maps irand()'s
    // [0, 0x7FFF] onto [0, 1) and the product is the same whichever precision it happens at.
    return static_cast<f32>(irand() * 3.0517578125e-05);
}

// ?frand@@YAMH@Z
f32 frand(i32 arg1)
{
    return static_cast<f32>(irand(arg1) * 3.0517578125e-05);
}
