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

define_dummy_symbol(misc_miscfuncs);

// The integer, pointer and string free functions.
//
// Deliberately separate from mathfuncs.cpp, and NOT because of what they do. This translation unit
// must not include <cmath>, because MSVC's <cmath> drags in <cstdlib> and <cstring>, and those
// declare the CRT entry points with _ACRTIMP - which becomes __declspec(dllimport) depending on
// the CRT model and turns a call to `toupper` into a call to `__imp__toupper`. The link runs with
// -NODEFAULTLIB against the game's own statically linked 1999 CRT (see tools/link.py), so the only
// name that resolves is the undecorated `_toupper` that is already in game.asm. Declaring it here
// by hand is what keeps that true.
//
// The same reasoning is why some perfectly readable functions are still ARTS_IMPORT: `_memset`,
// `_strlen` and `_strcpy` do not exist as symbols anywhere in midtown2.exe - the 1999 compiler
// inlined all three as `rep stos` / `repne scas` - so any function whose C++ form would lower to
// one of them cannot be linked. See docs/ported_freefuncs.md.

// midtown2.exe's own toupper, at 0x5A8CF3, published in the map as `_toupper`. `__toupper` at
// 0x5A8CEB is a different function - the C `_toupper` macro helper - and is not what UpperCase
// calls. Declared here rather than included, for the reason above.
extern "C" i32 __cdecl toupper(i32 c);

// ?irand@@YAHXZ
i32 irand()
{
    // The Microsoft LCG, with the seed as a game global rather than CRT state so that a replay can
    // be made deterministic. LogRandomCalls is a null-checked hook that exists to find the call
    // that desyncs a replay; it is null in a shipping run.
    if (LogRandomCalls)
        LogRandomCalls();

    // The multiply is written through u32 because signed overflow is undefined in C++ and this
    // multiply overflows constantly. The original is `lea`/`shl`/`sub` on a signed int, which
    // wraps; u32 arithmetic then cast back reproduces the same bits with defined behaviour.
    gRandSeed = static_cast<i32>(214013u * static_cast<u32>(gRandSeed) + 2531011u);

    // `sar eax,10h` - an ARITHMETIC shift, so gRandSeed must stay signed here.
    return (gRandSeed >> 16) & 0x7FFF;
}

// ?irand@@YAHH@Z
i32 irand(i32 arg1)
{
    // Same LCG, but stateless: arg1 is the seed and nothing is written back. This is the one used
    // to derive a repeatable value from an object id, not to advance a stream.
    return (static_cast<i32>(214013u * static_cast<u32>(arg1) + 2531011u) >> 16) & 0x7FFF;
}

// ?ilog2@@YAHH@Z
i32 ilog2(i32 arg1)
{
    // Floor of log2, by shifting down until nothing is left. `sar` and not `shr`, so a NEGATIVE
    // argument never terminates: the shift converges on -1 and stays there. That hang is in the
    // original and is reproduced; the callers only ever pass a texture dimension.
    i32 result = 0;

    for (i32 i = arg1 >> 1; i != 0; i >>= 1)
        ++result;

    return result;
}

// ?IsOnStack@@YA_NPAX@Z
bool IsOnStack(void* arg1)
{
    // Takes the address of its own parameter slot and asks whether arg1 is above it. Anything the
    // caller allocated in a frame further out is; anything on the heap, which lives below the
    // stack in this process layout, is not.
    //
    // `sbb eax,eax` after `cmp` makes this an UNSIGNED comparison, so the cast through usize is
    // not just there to avoid comparing unrelated pointers - it is the correct reading.
    return reinterpret_cast<usize>(&arg1) < reinterpret_cast<usize>(arg1);
}

// ?cmpPtx@@YAHPBX0@Z
i32 cmpPtx(const void* arg1, const void* arg2)
{
    // A qsort comparator over structures whose first field is an i32. Note the operands are the
    // other way round from the usual convention, so this sorts DESCENDING.
    return *static_cast<const i32*>(arg2) - *static_cast<const i32*>(arg1);
}

// ?CleanName@@YAPBDPBD@Z
const char* CleanName(const char* arg1)
{
    // Strips the directory part of a path by returning a pointer just past the LAST backslash.
    // Forward slashes are not treated as separators. The whole string is walked even after a match
    // - there is no scan from the end - and a string that ends in a backslash returns the empty
    // tail rather than the last component.
    const char* result = arg1;
    const char* cur = arg1;
    char ch = *cur;

    if (ch != '\0')
    {
        do
        {
            if (ch == '\\')
                result = cur + 1;

            // Reads cur[1] before the increment, so it never looks past the terminator.
            ch = cur[1];
            ++cur;
        } while (ch != '\0');
    }

    return result;
}

// ?UpperCase@@YAXPAD@Z
void UpperCase(char* arg1)
{
    // Upper-cases in place - and recomputes the length on EVERY iteration, which makes it O(n^2).
    // That is what the machine code does (two separate `repne scasb` runs per character), and it
    // is reproduced rather than hoisted, because a caller could be relying on the loop noticing a
    // terminator that toupper itself moved.
    //
    // The length is spelled out as a loop instead of calling strlen for the linkage reason in the
    // file header: there is no `_strlen` symbol in midtown2.exe to call.
    i32 len = 0;

    while (arg1[len] != '\0')
        ++len;

    if (len <= 0)
        return;

    i32 i = 0;

    do
    {
        // `movsx eax, byte ptr` - the character is SIGN-extended into toupper's int argument, so
        // any byte over 0x7F arrives negative. MSVC's plain `char` is signed, so passing arg1[i]
        // straight through reproduces that; do not add an unsigned cast to "fix" it.
        arg1[i] = static_cast<char>(toupper(arg1[i]));
        ++i;

        len = 0;

        while (arg1[len] != '\0')
            ++len;
    } while (i < len);
}

// ?gfxFVFOffset@@YAIII@Z
u32 gfxFVFOffset(u32 arg1, u32 arg2)
{
    // Byte offset of one component (arg2) inside a vertex whose flexible vertex format is arg1.
    // Only the low byte of arg1 is ever tested - `mov cl, byte ptr [ebp+8]` - so the flags above
    // 0xFF, which gfxFVFSize does account for, are invisible here.
    if (arg2 == 2)
        return 0;

    u32 result = 12;

    // The 0x10 case returns before any of the format flags are looked at, so a vertex with no
    // 0x10 component still reports its offset as 12.
    if (arg2 == 0x10)
        return result;

    if (arg1 & 0x10)
        result = 0x18;

    if (arg1 & 0x20)
        result += 4;

    if (arg2 == 0x40)
        return result;

    if (arg1 & 0x40)
        result += 4;

    if (arg1 & 0x80)
        result += 4;

    if (arg2 == 0x100)
        return result;

    return result + 8;
}

// ?jdiv_round_up@@YAJJJ@Z
ilong jdiv_round_up(ilong arg1, ilong arg2)
{
    // From the libjpeg the game statically links. Signed idiv, so this is only correct for
    // positive arguments - which is the contract libjpeg gives it.
    return (arg1 + arg2 - 1) / arg2;
}

// ?jround_up@@YAJJJ@Z
ilong jround_up(ilong arg1, ilong arg2)
{
    // Rounds arg1 up to a multiple of arg2, via remainder rather than a second multiply. The
    // original computes (arg1 + arg2 - 1) once into ecx and uses it twice.
    ilong t = arg1 + arg2 - 1;

    return t - (t % arg2);
}
