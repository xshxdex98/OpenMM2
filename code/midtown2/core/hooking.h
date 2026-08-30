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

#include "defines.h"

// PIN THE LAYOUT.
//
// A reimplemented class shares its memory with the original machine code that has not been ported
// yet: the assembly reads members at fixed byte offsets, and a vtable it builds is indexed by
// fixed slot. So the C++ struct must have exactly the size the 1999 compiler produced. Get it
// wrong and there is no error - the assembly simply reads the wrong bytes, and the failure
// surfaces somewhere else entirely, usually as a NaN in physics or a pointer that passes a null
// check and then crashes.
//
// check_size makes that a compile error instead. Sizes come from data/layouts.json (the IDA type
// library) and data/mm2types.json (MM2Hook), via tools/genheaders.py.
//
// Unlike Open1560's version, this is a plain static_assert: OpenMM2 links a standalone executable
// rather than injecting into a running one, so there is no hooking framework to defer to and the
// check can be made unconditionally at compile time.
#define check_size(TYPE, SIZE)                             \
    static_assert(sizeof(TYPE) == (SIZE),                  \
        "sizeof(" #TYPE ") must be " ARTS_STRINGIFY(SIZE)  \
        " to match midtown2.exe - see docs/harness.md")

// Force a translation unit to be linked in. A .cpp that only defines functions the assembly calls
// has nothing referencing it, so the linker would drop the object entirely and silently fall back
// to the original code.
#define define_dummy_symbol(NAME) \
    extern "C" int ARTS_CONCAT(dummy_symbol_, NAME) = 0

#define include_dummy_symbol(NAME)                                  \
    extern "C" int ARTS_CONCAT(dummy_symbol_, NAME);                \
    static void* const ARTS_CONCAT(dummy_symbol_ref_, NAME) =       \
        &ARTS_CONCAT(dummy_symbol_, NAME)

// The original APIs take char* for strings they never write. _xconst gives a char* in a hooked
// build and a const char* standalone, so the call sites do not need const_cast.
#ifdef ARTS_STANDALONE
#    define xconst(VALUE) (VALUE)
#    define aconst const

ARTS_FORCEINLINE const char* operator""_xconst(const char* str, std::size_t)
{
    return str;
}
#else
#    define xconst(VALUE) const_cast<char*>(VALUE)
#    define aconst

ARTS_FORCEINLINE char* operator""_xconst(const char* str, std::size_t)
{
    return const_cast<char*>(str);
}
#endif
