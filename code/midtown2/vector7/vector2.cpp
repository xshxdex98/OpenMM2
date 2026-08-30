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

#include "vector2.h"

#include <cmath>

define_dummy_symbol(vector7_vector2);

// Read off the disassembly of midtown2.exe at 0x45CC50-0x45CC94. The whole class is three
// functions and 45 bytes of code; the Hex-Rays dump in MM2_RE_KIT agrees with all three, which is
// worth stating because it does not agree everywhere else in vector7.
//
// FLOAT FIDELITY. Both magnitudes accumulate entirely in x87 registers - there is not a single
// store between the first fld and the final result - so the sum is f64 here and rounds once, on
// the way out. Writing it with f32 operands would select the float overload of sqrt and collapse
// the whole computation to single precision, which is the mistake Vector3::Mag documents.

// ??0Vector2@@QAE@XZ
Vector2::Vector2()
{
    // `mov eax, ecx; ret` - the constructor does not touch either member. Objects are born on
    // whatever the allocator last had in that memory, which is the mechanical reason MM1's
    // number-one bug class is uninitialised members. Do not "fix" this.
}

// ?Mag@Vector2@@QBEMXZ
f32 Vector2::Mag() const
{
    f64 sq = f64 {x} * x + f64 {y} * y;

    return static_cast<f32>(std::sqrt(sq));
}

// ?Mag2@Vector2@@QBEMXZ
f32 Vector2::Mag2() const
{
    // Byte for byte Mag without the fsqrt.
    return static_cast<f32>(f64 {x} * x + f64 {y} * y);
}
