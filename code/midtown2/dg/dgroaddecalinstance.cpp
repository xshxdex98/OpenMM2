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

// The road decal - the painted markings laid flat on the city's roads. It is an lvlInstance that
// draws nothing through the normal path and carries no transform of its own.
//
// GetMatrix is NOT ported. Its signature returns `const Matrix34&` and the binary returns null in
// eax, which is a null reference: legal in machine code, undefined behaviour to write in C++, and
// the optimiser is entitled to assume it never happens. Leaving it in assembly keeps the exact
// retail behaviour without asking the compiler to reason about a reference that is not one.

#include "dgroaddecalinstance.h"

#include "vector7/matrix34.h"

// ?SetMatrix@dgRoadDecalInstance@@UAEXABVMatrix34@@@Z - 0x00443E40
//
// A decal is baked into the road at load time and never moves, so its transform is fixed and the
// setter is ignored.
void dgRoadDecalInstance::SetMatrix(const Matrix34& arg1)
{
    (void) arg1;
}

// ?Draw@dgRoadDecalInstance@@UAEXH@Z - 0x00443E00
//
// Decals are drawn by the road geometry that owns them, not as instances in their own right.
void dgRoadDecalInstance::Draw(i32 arg1)
{
    (void) arg1;
}

// ?SizeOf@dgRoadDecalInstance@@UAEIXZ - 0x00443E20
//
// 0x24. The value is the compiler's own sizeof, emitted as `mov eax, 0x24; ret`, which makes it
// independent evidence for the class's size rather than a number anyone chose.
u32 dgRoadDecalInstance::SizeOf()
{
    return 0x24;
}

define_dummy_symbol(dg_dgroaddecalinstance);
