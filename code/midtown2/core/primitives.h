/*
    OpenMM2 - An Open Source Re-Implementation of Midtown Madness 2
    Copyright (C) 2026 The OpenMM2 contributors

    Adapted from Open1560 by Brick, an Open Source Re-Implementation of
    Midtown Madness 1. Copyright (C) 2020 Brick.

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

#include <cstddef>
#include <cstdint>

// The entire type vocabulary of the codebase. Game code never uses int, float, unsigned, short,
// long or bool - only these. The platform types appear only at a Win32 or DirectX call site that
// demands them.
//
// b32 is load-bearing rather than cosmetic. In a hooked build it must be i32, because the original
// machine code stores a full dword; in a standalone build it can be a real bool. Keeping the two
// spellings apart is what lets the representation change later without auditing every use.

#ifdef ARTS_STANDALONE
using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using b16 = bool;
using b32 = bool;
#else
using i8 = char;
using i16 = short;
using i32 = int;
using i64 = long long;

using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;

using b16 = i16;
using b32 = i32;
#endif

using f32 = float;
using f64 = double;

using usize = std::size_t;
using isize = std::ptrdiff_t;

using ilong = signed long;
using ulong = unsigned long;

// Literal suffixes, for the places where the type would otherwise be ambiguous.
constexpr i32 operator""_i32(unsigned long long value)
{
    return static_cast<i32>(value);
}

constexpr u32 operator""_u32(unsigned long long value)
{
    return static_cast<u32>(value);
}

constexpr usize operator""_usize(unsigned long long value)
{
    return static_cast<usize>(value);
}

constexpr f32 operator""_f32(long double value)
{
    return static_cast<f32>(value);
}
