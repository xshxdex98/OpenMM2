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

#include "vector3.h"

#include "data7/printer.h"
#include "matrix34.h"
#include "vector2.h"
#include "vector4.h"

#include <cmath>

define_dummy_symbol(vector7_vector3);

// Transcribed from the Hex-Rays output in MM2_RE_KIT/MM2_PSEUDOCODE/Vector3/Vector3.c, with every
// name confirmed against midtown2.map by address rather than taken from the dump's own labels.
//
// Only the methods whose decompiled arity matches the mangled signature are here. Hex-Rays guessed
// __fastcall on several of these and produced bodies with one parameter too many - Subtract and
// Scale(const Vector3&, float) both came out wrong that way, and Dist decompiled to `return pos`,
// which is simply the x87 return value being missed. A plausible-looking guess is worse than the
// original machine code.
//
// SIX OF THOSE ARE NOT A DECOMPILER ARTEFACT, AND THEY CAN NEVER BE PORTED UNDER THESE NAMES.
//
// The extra parameter is real. Read off the disassembly - and byte-verified against midtown2.exe
// at the map's own RVA, so there is no question of the wrong function having been looked at - six
// midtown2.map symbols name code whose ABI their mangling cannot describe:
//
//   0x45CE00  ?Add@Vector3@@QAEXABV1@@Z          `void Add(const Vector3&)` is 1 argument and
//                                                `ret 4`. The code is `ret 8`: it takes a hidden
//                                                return slot plus one reference, writes the sum
//                                                into the slot and returns it in eax. That is a
//                                                by-value `operator+`, not Add.
//   0x45CE40  ?Scale@Vector3@@QAEXABV1@M@Z       same shape - hidden slot plus a float. A by-value
//                                                `operator*(float)`.
//   0x45CD30  ?Subtract@Vector3@@QAEXABV1@@Z     1 argument by the name, `ret 8` in the code, and
//                                                the body is `this = a - b`.
//   0x474000  ?SubtractScaled@Vector3@@QAEXABV1@M@Z   the body ADDS: `this += a * s`.
//   0x484420  ?AddScaled@Vector3@@QAEXABV1@0M@Z  3 arguments by the name, `ret 8` in the code, and
//                                                the body SUBTRACTS: `this -= a * s`.
//   0x43DD50  ??XVector3@@QAEXABV0@@Z            `operator*=(const Vector3&)` by the name; the
//                                                second argument is loaded as a float and the body
//                                                is Scale(const Vector3&, float).
//
// The call sites settle it. Disassembling a caller of 0x45CE00 (0x44B481, inside psdl_draw) shows
// two dwords pushed and left for it to clean, with `mov ecx, eax` supplying `this` from the
// previous call's return - exactly the by-value convention and nothing like `void Add(const
// Vector3&)`.
//
// This matters because the mangled name is the only thing the linker matches on. Writing
// `void Vector3::Add(const Vector3&)` produces a __thiscall that pops 4 bytes, the assembly's
// callers push 8, and the stack is off by four on every call - a corruption that appears nowhere
// near vector7. There is no way to declare the real signature and still get this symbol. So the
// six stay ARTS_IMPORT permanently, and the generated declarations for them in vector3.h are
// wrong: do not port them, and do not "fix" the header to match the code either, because that
// changes the symbol.
//
// Everything else in Vector3 was re-checked the same way - `ret imm16` against the parameter list
// the mangled name encodes - and agrees.
//
// FLOAT FIDELITY. The 1999 build is x87, so intermediates are held at higher precision than f32
// and only round when they are stored. Where the dump shows a value passing through a float
// temporary it is written as f32 here, and where it stays in a register it is written as f64 -
// the difference is real, and physics amplifies it. Build with /arch:IA32 so the compiler uses
// x87 too; see tools/build.py.

// ??0Vector3@@QAE@XZ
Vector3::Vector3()
{
    // `mov eax, ecx; ret`. The constructor does not touch x, y or z, so a Vector3 is born holding
    // whatever the allocator last left in that memory - memMemoryAllocator::Allocate does not zero.
    // This is the mechanical reason uninitialised members are the number-one bug class in this
    // engine, and it must not be "fixed": the assembly relies on the constructor being free.
}

// ?Set@Vector3@@QAEXMMM@Z
void Vector3::Set(f32 arg1, f32 arg2, f32 arg3)
{
    x = arg1;
    y = arg2;
    z = arg3;
}

// ?Set@Vector3@@QAEXABV1@@Z
void Vector3::Set(const Vector3& arg1)
{
    *this = arg1;
}

// ?Scale@Vector3@@QAEXM@Z
void Vector3::Scale(f32 arg1)
{
    x = arg1 * x;
    y = arg1 * y;
    z = arg1 * z;
}

// ?Mag@Vector3@@QBEMXZ
f32 Vector3::Mag() const
{
    // The sum is built in f64 deliberately. Written as `std::sqrt(x * x + ...)` with f32 operands
    // the overload set picks sqrt(float) and the whole computation collapses to single precision,
    // which the x87 original does not do - it accumulates in 80-bit registers and rounds once, at
    // the store. dumpbin showed the float overload being selected, which is how this was caught.
    f64 sq = f64 {x} * x + f64 {y} * y + f64 {z} * z;

    return static_cast<f32>(std::sqrt(sq));
}

// ?InvMag@Vector3@@QBEMXZ
f32 Vector3::InvMag() const
{
    f64 sq = f64 {x} * x + f64 {y} * y + f64 {z} * z;

    // The original tests the sum against zero rather than guarding the sqrt, so a zero-length
    // vector yields 0 instead of an infinity. Keep the comparison exactly where it is.
    if (sq == 0.0)
        return 0.0f;

    return static_cast<f32>(1.0 / std::sqrt(sq));
}

// ?Normalize@Vector3@@QAEXXZ
void Vector3::Normalize()
{
    f64 sq = f64 {x} * x + f64 {y} * y + f64 {z} * z;
    f64 inv;

    if (sq == 0.0)
        inv = 0.0;
    else
        inv = 1.0 / std::sqrt(sq);

    x = static_cast<f32>(inv * x);
    y = static_cast<f32>(inv * y);
    z = static_cast<f32>(inv * z);
}

// ?Dot@Vector3@@QBEMABV1@@Z
f32 Vector3::Dot(const Vector3& arg1) const
{
    // Summed z, y, x - the order the original accumulates in, which is not the order it would be
    // written by hand and is not associative in floating point.
    return static_cast<f32>(f64 {arg1.z} * z + f64 {arg1.y} * y + f64 {arg1.x} * x);
}

// ?Cross@Vector3@@QAEXABV1@0@Z
void Vector3::Cross(const Vector3& arg1, const Vector3& arg2)
{
    // Y is computed through an f32 temporary while X and Z stay in x87 registers, and the results
    // are stored y, z, x. Both details matter: the temporary rounds, and the store order decides
    // what happens when the destination aliases an operand.
    f64 nx = f64 {arg2.z} * arg1.y - f64 {arg1.z} * arg2.y;
    f32 ny = static_cast<f32>(f64 {arg1.z} * arg2.x - f64 {arg1.x} * arg2.z);
    f64 a = f64 {arg1.x} * arg2.y;
    f64 b = f64 {arg2.x} * arg1.y;

    y = ny;
    z = static_cast<f32>(a - b);
    x = static_cast<f32>(nx);
}

// ?Cross@Vector3@@QAEXABV1@@Z
void Vector3::Cross(const Vector3& arg1)
{
    // In-place form. Same shape as the two-argument version, same store order, and here the
    // aliasing is guaranteed rather than incidental.
    f64 nx = f64 {y} * arg1.z - f64 {arg1.y} * z;
    f32 ny = static_cast<f32>(f64 {arg1.x} * z - f64 {x} * arg1.z);
    f64 a = f64 {arg1.y} * x;
    f64 b = f64 {y} * arg1.x;

    y = ny;
    z = static_cast<f32>(a - b);
    x = static_cast<f32>(nx);
}

// ?Negate@Vector3@@QAEXABV1@@Z
void Vector3::Negate(const Vector3& arg1)
{
    x = -arg1.x;
    y = -arg1.y;
    z = -arg1.z;
}

// ?Lerp@Vector3@@QAEXMABV1@0@Z
void Vector3::Lerp(f32 arg1, const Vector3& arg2, const Vector3& arg3)
{
    x = static_cast<f32>((f64 {arg3.x} - arg2.x) * arg1 + arg2.x);
    y = static_cast<f32>((f64 {arg3.y} - arg2.y) * arg1 + arg2.y);
    z = static_cast<f32>((f64 {arg3.z} - arg2.z) * arg1 + arg2.z);
}

// ?IsEqual@Vector3@@QBE_NABV1@@Z
bool Vector3::IsEqual(const Vector3& arg1) const
{
    return x == arg1.x && y == arg1.y && z == arg1.z;
}

// ?Mag2@Vector3@@QBEMXZ
f32 Vector3::Mag2() const
{
    // Mag without the fsqrt, and accumulated in the same order.
    return static_cast<f32>(f64 {x} * x + f64 {y} * y + f64 {z} * z);
}

// ?Dist@Vector3@@QBEMABV1@@Z
f32 Vector3::Dist(const Vector3& arg1) const
{
    // The three differences are computed onto the x87 stack and never stored, so they stay wider
    // than f32. Squaring starts from z and works back to x - the reverse of Mag - which is why
    // Dist(a) and (this - a).Mag() are not bit-identical.
    f64 dx = f64 {x} - arg1.x;
    f64 dy = f64 {y} - arg1.y;
    f64 dz = f64 {z} - arg1.z;

    return static_cast<f32>(std::sqrt(dz * dz + dy * dy + dx * dx));
}

// ?FlatDist@Vector3@@QBEMABV1@@Z
f32 Vector3::FlatDist(const Vector3& arg1) const
{
    // Distance in the ground plane. Y is never loaded at all.
    f64 dx = f64 {x} - arg1.x;
    f64 dz = f64 {z} - arg1.z;

    return static_cast<f32>(std::sqrt(dz * dz + dx * dx));
}

// ??YVector3@@QAEXABV0@@Z
void Vector3::operator+=(const Vector3& arg1)
{
    // The operand is loaded first and the destination second, so this is arg1 + *this. Addition is
    // commutative in IEEE, but the instruction order is what it is.
    x = arg1.x + x;
    y = arg1.y + y;
    z = arg1.z + z;
}

// ??YVector3@@QAEXAAV0@@Z
void Vector3::operator+=(Vector3& arg1)
{
    // A second, byte-identical copy compiled against a non-const reference. Both symbols are real
    // and both are called, so both have to exist.
    x = arg1.x + x;
    y = arg1.y + y;
    z = arg1.z + z;
}

// ??ZVector3@@QAEXABV0@@Z
void Vector3::operator-=(const Vector3& arg1)
{
    x = x - arg1.x;
    y = y - arg1.y;
    z = z - arg1.z;
}

// ??ZVector3@@QAEXAAV0@@Z
void Vector3::operator-=(Vector3& arg1)
{
    x = x - arg1.x;
    y = y - arg1.y;
    z = z - arg1.z;
}

// ---------------------------------------------------------------------------------------------
// Second pass. Everything below was read off the DISASSEMBLY at the RVA midtown2.map gives for the
// symbol - disassembled with capstone from the original instruction bytes in game.asm.pristine and
// cross-checked against midtown2.exe. The Hex-Rays dump was not used: it is the source of the
// wrong arities Trap 14 documents, and for several of these it produces a body that pops a
// different number of bytes than the machine code does.
//
// `py tools/verify_arity.py` was run first and every symbol here is among the 6,315 that agree.
// The six that do not - Add, AddScaled, Subtract, SubtractScaled, Scale(const Vector3&, f32) and
// operator*= - are still ARTS_IMPORT and are still untouchable, for the reason above.
//
// ONE MORE OF THE SAME FAMILY WAS FOUND HERE, AND IT IS NOT IN THAT LIST OF SIX.
//
//   0x595B70  ?InvScale@Vector3@@QAEXM@Z    The name says `void InvScale(float)`, the code does
//                                           `ret 4`, and verify_arity.py is satisfied - the stack
//                                           balances. It is still unportable. `[ebp+8]` is loaded
//                                           with `mov eax, dword ptr [ebp+8]` and then dereferenced
//                                           at +0, +4 and +8: the argument is a POINTER, not a
//                                           float. The body is
//                                               a.z*z + a.y*y + a.x*x
//                                           left in st(0) - instruction for instruction the same
//                                           code as ?Dot@Vector3@@QBEMABV1@@Z at 0x466470, which
//                                           returns f32. So the symbol names an
//                                           `f32 Dot(const Vector3&) const`, not a
//                                           `void InvScale(f32)`, and writing the declaration the
//                                           name demands hands it a float where it wants an
//                                           address. It stays ARTS_IMPORT.
//
// That is Trap 14's own footnote: arity is necessary, not sufficient. Read the body.
//
// ?Dot3x3Transpose@Vector3@@QAEXABVMatrix34@@@Z is skipped for a different reason. The map gives it
// 2,320 bytes against 96 for its sibling Dot3x3, which is Trap 7 - the size is distance to the next
// symbol and several unrelated functions are inside that PROC. Its real extent has to be settled
// before it can be read.

// 0x3F7FFFFE, one ulp below 1.0f. Angle compares the cosine against this rather than 1.0f, so an
// exactly-parallel pair returns 0 without going near acos.
static constexpr f32 kAngleCosClamp = 0.99999988f;

// 0x40490FDB, float pi. Angle returns it when the cosine falls below -1.0f.
static constexpr f32 kPi = 3.1415927f;

// 0x3E2AAAAB, float 1/6. RGBtoHSV builds its hue in sixths and scales at the end.
static constexpr f32 kSixthTurn = 0.16666667f;

// ?Angle@Vector3@@QBEMABV1@@Z
f32 Vector3::Angle(const Vector3& arg1) const
{
    // Both squared magnitudes are accumulated in x87 registers, multiplied together, rooted once,
    // and only then narrowed - `fstp dword ptr [ebp+8]` reuses the argument slot as the temporary.
    // The reciprocal below therefore divides the f32 value, not the wide one.
    f64 mag_arg = f64 {arg1.x} * arg1.x + f64 {arg1.y} * arg1.y + f64 {arg1.z} * arg1.z;
    f64 mag_this = f64 {x} * x + f64 {y} * y + f64 {z} * z;

    f32 mag = static_cast<f32>(std::sqrt(mag_arg * mag_this));
    f64 inv = 1.0 / mag;

    // The guard is on the reciprocal, not on the magnitude, and it must stay on this side of the
    // division - moving it changes which inputs return 0.
    if (inv == 0.0)
        return 0.0f;

    // The dot is accumulated x, z, y - not the order Dot() itself uses, and not associative.
    f32 cosine = static_cast<f32>((f64 {arg1.x} * x + f64 {arg1.z} * z + f64 {arg1.y} * y) * inv);

    if (cosine > kAngleCosClamp)
        return 0.0f;

    if (cosine < -1.0f)
        return kPi;

    return static_cast<f32>(std::acos(f64 {cosine}));
}

// ?Approach@Vector3@@QAE_NABV1@MM@Z
bool Vector3::Approach(const Vector3& arg1, f32 arg2, f32 arg3)
{
    // Each component steps toward the target by arg2*arg3 and clamps if it would overshoot. The
    // step is recomputed in every branch, as the original does.
    //
    // The detail that matters: `fst` writes the narrowed component and leaves the WIDE value in
    // st(0), and the overshoot test compares that wide value. Testing the stored f32 instead would
    // clamp on a different set of inputs.
    //
    // The result is true only when all three have arrived, and the equality test after each one is
    // what short-circuits the rest.
    if (arg1.x > x)
    {
        f64 stepped = f64 {arg2} * arg3 + x;
        x = static_cast<f32>(stepped);

        if (stepped > arg1.x)
            x = arg1.x;
    }
    else if (arg1.x < x)
    {
        f64 stepped = x - f64 {arg2} * arg3;
        x = static_cast<f32>(stepped);

        if (stepped < arg1.x)
            x = arg1.x;
    }

    if (arg1.x != x)
        return false;

    if (arg1.y > y)
    {
        f64 stepped = f64 {arg2} * arg3 + y;
        y = static_cast<f32>(stepped);

        if (stepped > arg1.y)
            y = arg1.y;
    }
    else if (arg1.y < y)
    {
        f64 stepped = y - f64 {arg2} * arg3;
        y = static_cast<f32>(stepped);

        if (stepped < arg1.y)
            y = arg1.y;
    }

    if (arg1.y != y)
        return false;

    if (arg1.z > z)
    {
        f64 stepped = f64 {arg2} * arg3 + z;
        z = static_cast<f32>(stepped);

        if (stepped > arg1.z)
            z = arg1.z;
    }
    else if (arg1.z < z)
    {
        f64 stepped = z - f64 {arg2} * arg3;
        z = static_cast<f32>(stepped);

        if (stepped < arg1.z)
            z = arg1.z;
    }

    return arg1.z == z;
}

// ?Dot@Vector3@@QAEXABV1@ABVMatrix34@@@Z
void Vector3::Dot(const Vector3& arg1, const Matrix34& arg2)
{
    // The full affine transform in the row-vector convention: this = arg1 * arg2, translation row
    // included.
    //
    // x is stored before y and z are computed, and y before z, so a call where `this` aliases arg1
    // reads components it has already overwritten. The statements are in the original's order so
    // the compiler cannot hoist a load past a store it must assume may alias.
    //
    // Row 0 accumulates m10*y first and picks up arg1.x third; rows 1 and 2 go x, y, z. Only row 0
    // is out of step, and it is out of step in the machine code.
    x = static_cast<f32>(
        f64 {arg2.m10} * arg1.y + f64 {arg2.m20} * arg1.z + f64 {arg1.x} * arg2.m00 + arg2.m30);
    y = static_cast<f32>(
        f64 {arg2.m01} * arg1.x + f64 {arg2.m11} * arg1.y + f64 {arg2.m21} * arg1.z + arg2.m31);
    z = static_cast<f32>(
        f64 {arg2.m02} * arg1.x + f64 {arg2.m12} * arg1.y + f64 {arg2.m22} * arg1.z + arg2.m32);
}

// ?Dot3x3@Vector3@@QAEXABVMatrix34@@@Z
void Vector3::Dot3x3(const Matrix34& arg1)
{
    // The rotation part only - no translation row - and in place. Every component is read before
    // any is written: the machine code loads m12*y for the z term at 0x4795A2 and stores the new y
    // at 0x4795A8, one instruction later. Computing all three first reproduces that.
    //
    // The new y is the one that goes through an f32 stack slot; x and z stay in registers.
    f64 nx = f64 {arg1.m20} * z + f64 {arg1.m10} * y + f64 {arg1.m00} * x;
    f32 ny = static_cast<f32>(f64 {arg1.m21} * z + f64 {arg1.m01} * x + f64 {arg1.m11} * y);
    f64 nz = f64 {arg1.m22} * z + f64 {arg1.m02} * x + f64 {arg1.m12} * y;

    y = ny;
    z = static_cast<f32>(nz);
    x = static_cast<f32>(nx);
}

// ?Extend@Vector3@@QAEXABV1@M@Z
//
// NOT PORTED, and it must stay that way until the symbol map is corrected.
//
// The map records this function as 272 bytes at 0x004C0620, but a static initialiser begins at
// 0x004C0670 - fifty bytes in - and the CRT's initialiser table points straight at it. Porting the
// function strips its whole recorded extent, which deletes that initialiser, so _initterm called
// into the middle of the reimplementation and the game died before printing its second line.
//
// Every byte-level gate passed while that was true: bytes inside a ported region are *expected* to
// differ, and the deleted function was inside one. tools/verify_extents.py now refuses this class
// outright, which is what makes it safe to leave the transcription here for whoever settles the
// real extent. The body below was verified against the disassembly and is believed correct; it is
// the extent that is wrong, not the code.
//
// void Vector3::Extend(const Vector3& arg1, f32 arg2)
// {
//     f64 mag = std::sqrt(f64 {arg1.x} * arg1.x + f64 {arg1.y} * arg1.y + f64 {arg1.z} * arg1.z);
//     f64 scale = (mag + arg2) / mag;
//
//     x = static_cast<f32>(scale * arg1.x);
//     y = static_cast<f32>(scale * arg1.y);
//     z = static_cast<f32>(scale * arg1.z);
// }

// ?Extend@Vector3@@QAEXM@Z
void Vector3::Extend(f32 arg1)
{
    // The in-place form of the above, and instruction for instruction the same shape.
    f64 mag = std::sqrt(f64 {x} * x + f64 {y} * y + f64 {z} * z);
    f64 scale = (mag + arg1) / mag;

    x = static_cast<f32>(scale * x);
    y = static_cast<f32>(scale * y);
    z = static_cast<f32>(scale * z);
}

// ?FastAngle@Vector3@@QBEMABV1@@Z
f32 Vector3::FastAngle(const Vector3& arg1) const
{
    // Angle without the normalisation - it assumes both vectors are already unit length - and
    // without the clamp either, so an input a hair outside [-1, 1] reaches acos as-is. The dot goes
    // straight into __CIacos and is never narrowed.
    return static_cast<f32>(std::acos(f64 {arg1.z} * z + f64 {arg1.y} * y + f64 {arg1.x} * x));
}

// ?GetPolar@Vector3@@QBEXABV1@PAVVector4@@PAV1@@Z
void Vector3::GetPolar(const Vector3& arg1, Vector4* arg2, Vector3* arg3) const
{
    // Polar coordinates of this point about arg1: arg2 receives (radius, azimuth, elevation, 0) and
    // arg3 receives a copy of the origin.
    //
    // dx and dz are written to float slots and read back; dy is not. The magnitude therefore
    // squares the NARROWED dz against the wide dx and dy, which is not what writing all three
    // uniformly would produce.
    f64 wide_dx = f64 {x} - arg1.x;
    f32 dx = static_cast<f32>(wide_dx);

    f64 wide_dz = f64 {z} - arg1.z;
    f32 dz = static_cast<f32>(wide_dz);

    f64 dy = f64 {y} - arg1.y;

    f64 mag = std::sqrt(f64 {dz} * dz + dy * dy + wide_dx * wide_dx);

    // fabs, not a sign test. A negative magnitude is impossible, but the instruction is there.
    f32 radius = static_cast<f32>(std::fabs(mag));

    arg2->x = radius;
    arg2->y = static_cast<f32>(std::atan2(f64 {dx}, f64 {dz}));

    // The flat distance is built from the narrowed dx and dz and divided by the narrowed radius.
    // The original also stores it to a slot nothing reads; that dead store is the one instruction
    // here with no counterpart below.
    f64 flat = std::sqrt(f64 {dz} * dz + f64 {dx} * dx);

    arg2->z = static_cast<f32>(std::acos(flat / radius));
    arg2->w = 0.0f;

    arg3->x = arg1.x;
    arg3->y = arg1.y;
    arg3->z = arg1.z;
}

// ?GetVector2@Vector3@@QBEXHAAVVector2@@@Z
void Vector3::GetVector2(i32 arg1, Vector2& arg2) const
{
    // A six-way jump table selecting an ordered pair of axes. The components are copied as dwords,
    // never loaded onto the FPU stack, so nothing rounds. Anything outside 0..5 leaves arg2
    // untouched - the default case is the epilogue.
    switch (arg1)
    {
        case 0:
            arg2.x = y;
            arg2.y = z;
            break;

        case 1:
            arg2.x = z;
            arg2.y = y;
            break;

        case 2:
            arg2.x = z;
            arg2.y = x;
            break;

        case 3:
            arg2.x = x;
            arg2.y = z;
            break;

        case 4:
            arg2.x = x;
            arg2.y = y;
            break;

        case 5:
            arg2.x = y;
            arg2.y = x;
            break;
    }
}

// ?HSVtoRGB@Vector3@@QAEXXZ
void Vector3::HSVtoRGB()
{
    // In place: x is hue, y is saturation and z is value going in, and they come out r, g, b.
    if (y == 0.0f)
    {
        y = z;
        x = z;

        return;
    }

    // fmod against a DOUBLE 1.0 - the only qword float constant in the whole of Vector3 - then
    // scaled into sixths. The truncation is __ftol, so it rounds toward zero.
    f64 sector = std::fmod(f64 {x}, 1.0) * 6.0f;
    i32 index = static_cast<i32>(sector);
    f32 fraction = static_cast<f32>(sector - index);

    // p stays in a register, q and t go through float slots. Each is its own sub-expression in the
    // original, not a shared (1 - something) factor.
    f64 p = (1.0 - f64 {y}) * z;
    f32 q = static_cast<f32>((1.0 - f64 {fraction} * y) * z);
    f32 t = static_cast<f32>((1.0 - (1.0 - f64 {fraction}) * y) * z);

    // The value is read out of z before p overwrites it in the cases that write z, which is why
    // each case captures it first. The dispatch is `cmp eax, 5; ja default`, so a negative index
    // takes the default and the vector is left alone.
    f32 value = z;

    switch (index)
    {
        case 0:
            z = static_cast<f32>(p);
            x = value;
            y = t;
            break;

        case 1:
            z = static_cast<f32>(p);
            x = q;
            y = value;
            break;

        case 2:
            x = static_cast<f32>(p);
            y = value;
            z = t;
            break;

        case 3:
            x = static_cast<f32>(p);
            y = q;
            break;

        case 4:
            y = static_cast<f32>(p);
            x = t;
            break;

        case 5:
            y = static_cast<f32>(p);
            x = value;
            z = q;
            break;
    }
}

// ?Print@Vector3@@QBEXXZ
void Vector3::Print() const
{
    Printf("%f,%f,%f", x, y, z);
}

// ?Print@Vector3@@QBEXPBD@Z
void Vector3::Print(const char* arg1) const
{
    Printf("%s: %f,%f,%f", arg1, x, y, z);
}

// ?RGBtoHSV@Vector3@@QAEXXZ
void Vector3::RGBtoHSV()
{
    // In place, the inverse of HSVtoRGB. z becomes the value and is used as the running maximum
    // while the hue is being built, which is why the expressions below read z rather than a local.
    f32 r = x;
    f32 g = y;
    f32 b = z;

    z = r;

    if (r < g)
        z = g;

    if (z < b)
        z = b;

    f32 low = r;

    if (r > g)
        low = g;

    if (low > b)
        low = b;

    f64 delta = f64 {z} - low;

    if (delta == 0.0f)
    {
        x = 0.0f;
        y = 0.0f;

        return;
    }

    y = static_cast<f32>(delta / z);

    // Each of the two terms is divided by delta separately. Factoring the division out is one
    // rounding fewer and a different answer.
    f64 hue;

    if (r == z)
        hue = (f64 {z} - b) / delta - (f64 {z} - g) / delta;
    else if (g == z)
        hue = (f64 {z} - r) / delta - (f64 {z} - b) / delta + 2.0f;
    else
        hue = (f64 {z} - g) / delta - (f64 {z} - r) / delta + 4.0f;

    if (hue < 0.0f)
        hue += 6.0f;

    x = static_cast<f32>(hue * kSixthTurn);
}

// ?RotateAboutAxis@Vector3@@QAEXMH@Z
void Vector3::RotateAboutAxis(f32 arg1, i32 arg2)
{
    // arg2 is a character - 'z', 'x', 'y' - tested in that order. Anything else does nothing.
    //
    // Both the sine and the cosine are stored to float slots here and read back, unlike
    // RotateX/Y/Z where the first use of the cosine is the wide one. So this is not the same
    // rotation as RotateZ to the last bit, and the difference is in the original.
    f32 sine = static_cast<f32>(std::sin(f64 {arg1}));
    f32 cosine = static_cast<f32>(std::cos(f64 {arg1}));

    if (arg2 == 'z')
    {
        f64 nx = f64 {cosine} * x - f64 {sine} * y;
        f64 ny = f64 {cosine} * y + f64 {sine} * x;

        y = static_cast<f32>(ny);
        x = static_cast<f32>(nx);
    }
    else if (arg2 == 'x')
    {
        f64 ny = f64 {cosine} * y - f64 {sine} * z;
        f64 nz = f64 {sine} * y + f64 {cosine} * z;

        z = static_cast<f32>(nz);
        y = static_cast<f32>(ny);
    }
    else if (arg2 == 'y')
    {
        f64 nz = f64 {cosine} * z - f64 {sine} * x;
        f64 nx = f64 {cosine} * x + f64 {sine} * z;

        x = static_cast<f32>(nx);
        z = static_cast<f32>(nz);
    }
}

// ?RotateX@Vector3@@QAEXM@Z
void Vector3::RotateX(f32 arg1)
{
    // `fst dword ptr [ebp+8]` narrows the cosine into the argument slot and leaves the wide value
    // in st(0), so the FIRST cosine product uses the wide cosine and the second uses the f32 one.
    // Driving both from one variable is the plausible-looking wrong answer.
    f64 sine = std::sin(f64 {arg1});
    f64 cosine = std::cos(f64 {arg1});
    f32 narrow_cosine = static_cast<f32>(cosine);

    f64 ny = cosine * y - sine * z;
    f64 nz = f64 {narrow_cosine} * z + sine * y;

    z = static_cast<f32>(nz);
    y = static_cast<f32>(ny);
}

// ?RotateY@Vector3@@QAEXM@Z
void Vector3::RotateY(f32 arg1)
{
    f64 sine = std::sin(f64 {arg1});
    f64 cosine = std::cos(f64 {arg1});
    f32 narrow_cosine = static_cast<f32>(cosine);

    f64 nz = cosine * z - sine * x;
    f64 nx = sine * z + f64 {narrow_cosine} * x;

    x = static_cast<f32>(nx);
    z = static_cast<f32>(nz);
}

// ?RotateZ@Vector3@@QAEXM@Z
void Vector3::RotateZ(f32 arg1)
{
    f64 sine = std::sin(f64 {arg1});
    f64 cosine = std::cos(f64 {arg1});
    f32 narrow_cosine = static_cast<f32>(cosine);

    f64 nx = cosine * x - sine * y;
    f64 ny = f64 {narrow_cosine} * y + sine * x;

    y = static_cast<f32>(ny);
    x = static_cast<f32>(nx);
}

// ??GVector3@@QBE?AV0@ABV0@@Z
Vector3 Vector3::operator-(const Vector3& arg1) const
{
    // By value, with the hidden return slot MSVC's `?AV0@` mangling accounts for - `ret 8` for one
    // declared argument, which is correct here and is exactly the shape Trap 14's six only LOOK
    // like. verify_arity.py agrees with the name on this one.
    Vector3 result;

    result.x = x - arg1.x;
    result.y = y - arg1.y;
    result.z = z - arg1.z;

    return result;
}

// ??KVector3@@QBE?AV0@M@Z
Vector3 Vector3::operator/(f32 arg1) const
{
    // One reciprocal and three multiplies, not three divides.
    f64 inv = 1.0 / arg1;

    Vector3 result;

    result.x = static_cast<f32>(inv * x);
    result.y = static_cast<f32>(inv * y);
    result.z = static_cast<f32>(inv * z);

    return result;
}

// ??LVector3@@QBE?AV0@ABV0@@Z
Vector3 Vector3::operator%(const Vector3& arg1) const
{
    // Cross product. The same operand ordering as Cross(), including z taking arg1.y * x first.
    Vector3 result;

    result.x = static_cast<f32>(f64 {y} * arg1.z - f64 {arg1.y} * z);
    result.y = static_cast<f32>(f64 {z} * arg1.x - f64 {x} * arg1.z);
    result.z = static_cast<f32>(f64 {arg1.y} * x - f64 {y} * arg1.x);

    return result;
}

// ?Scale@Vector3@@QAEXABV1@M@Z
void Vector3::Scale(const Vector3& arg1, f32 arg2)
{
    // WRITES THROUGH arg1, which the mangled name spells `const Vector3&`. The machine code reads
    // *this and stores into the parameter, so the reference is an out-parameter that the 1999
    // declaration marked const; the const_cast is what preserves that, not a workaround.
    //
    // All three components land in single-precision temporaries before ANY of them is stored, so
    // a caller passing *this as arg1 gets the same answer the original gives. That is the whole
    // reason the original spends three stack slots on a function this small.
    f32 sx = static_cast<f32>(f64 {arg2} * x);
    f32 sy = static_cast<f32>(f64 {arg2} * y);
    f32 sz = static_cast<f32>(f64 {arg2} * z);

    Vector3& out = const_cast<Vector3&>(arg1);

    out.x = sx;
    out.y = sy;
    out.z = sz;
}

// ?SubtractScaled@Vector3@@QAEXABV1@M@Z
void Vector3::SubtractScaled(const Vector3& arg1, f32 arg2)
{
    // The name says subtract; every one of the three instructions is `fadd`. This ACCUMULATES
    // arg1 * arg2 into *this. Kept as the code has it rather than as the name reads - the symbol
    // is the contract, and every caller in the image was compiled against this behaviour.
    //
    // (Vector3::AddScaled, the mirror of this, is the one that subtracts. It cannot be ported at
    // all: its name encodes twelve bytes of arguments and its body pops eight. See Trap 14.)
    x = static_cast<f32>(f64 {arg2} * arg1.x + x);
    y = static_cast<f32>(f64 {arg2} * arg1.y + y);
    z = static_cast<f32>(f64 {arg2} * arg1.z + z);
}
