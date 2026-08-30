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

#include "primitives.h"

// THE DE-OSSIFICATION MARKERS. These two decide, per function, whether the binary runs the
// original 1999 machine code or our C++.
//
// ARTS_IMPORT - still the original. The declaration exists so C++ can call it, but the definition
//               lives in game.asm and the linker resolves it there. Expands to nothing.
//
// ARTS_EXPORT - reimplemented here. __declspec(dllexport) is not about exporting: it forces the
//               compiler to EMIT the function even when nothing in this translation unit calls
//               it, so the linker has a definition to resolve against once tools/asm.py strips
//               the matching PROC out of game.asm.
//
// Flipping a declaration from IMPORT to EXPORT and running tools/asm.py is the entire porting
// operation. Getting it half-done is the dangerous state: if the PROC survives in the assembly,
// the assembly wins silently and the C++ is never called - which looks exactly like a correct
// reimplementation. tools/asm.py exists to catch that.

#define ARTS_IMPORT

#ifdef ARTS_STANDALONE
#    define ARTS_EXPORT
#else
#    define ARTS_EXPORT __declspec(dllexport)
#endif

#if defined(_MSC_VER)
#    define ARTS_FORCEINLINE __forceinline
#    define ARTS_NOINLINE __declspec(noinline)
#    define ARTS_RESTRICT __restrict
#elif defined(__GNUC__) || defined(__clang__)
#    define ARTS_FORCEINLINE inline __attribute__((always_inline))
#    define ARTS_NOINLINE __attribute__((noinline))
#    define ARTS_RESTRICT __restrict__
#else
#    define ARTS_FORCEINLINE inline
#    define ARTS_NOINLINE
#    define ARTS_RESTRICT
#endif

#define ARTS_CONCAT_(a, b) a##b
#define ARTS_CONCAT(a, b) ARTS_CONCAT_(a, b)

#define ARTS_STRINGIFY_(x) #x
#define ARTS_STRINGIFY(x) ARTS_STRINGIFY_(x)

// Copying is wrong for most engine objects: they are owned by the scene graph, referenced by the
// remaining assembly, or both.
#define ARTS_NON_COPYABLE(TYPE)      \
    TYPE(const TYPE&) = delete;      \
    TYPE& operator=(const TYPE&) = delete

// A NOTE ON WHERE REIMPLEMENTED CODE LANDS.
//
// game.asm reproduces .text byte for byte and tools/asm.py pads over each ported function, so the
// retail code keeps its addresses. The C++ replacing those functions still has to live somewhere,
// and it grows .text past its original 0x1AF000 - which pushes .rdata off 0x1B0000.
//
// #pragma code_seg does NOT help. Putting the reimplementations in ".mm2cpp" cost a whole extra
// page, because MSVC link places a custom code section next to .text either way, and renaming it
// ".zzmm2" changed nothing - the linker groups by section attributes, not by name.
//
// The displacement is therefore inherent to this linker, and the fix is not layout at all: it is
// to symbolise the remaining ~3,520 data pointers so nothing depends on the layout. Tracked in
// docs/harness.md.
