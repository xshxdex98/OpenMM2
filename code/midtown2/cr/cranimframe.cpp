/*
    OpenMM2 - An Open Source Re-Implementation of Midtown Madness 2

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

#include "cranimframe.h"

define_dummy_symbol(cr_cranimframe);

// ?Zero@crAnimFrame@@QAEXXZ
void crAnimFrame::Zero()
{
    // crAnimFrame is two fields - a count and a pointer to that many floats - and the layout the
    // generated header emits (ValueCount at 0x0, Values at 0x4) is exactly what the disassembly
    // indexes, which is why this one is portable while most of aiPath is not.
    //
    // The original reloads ValueCount on every iteration instead of hoisting it, and indexes with
    // the pre-incremented counter, writing Values[i - 1] after ++i. Nothing inside the loop can
    // change either field, so the straightforward spelling is equivalent. Its comparisons are
    // signed (`jle` then `jl`), which differs from the unsigned count only above 2^31 entries.
    for (u32 i = 0; i < ValueCount; ++i)
        Values[i] = 0.0f;
}
