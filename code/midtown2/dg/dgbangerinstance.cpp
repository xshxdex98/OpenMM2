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

// dgBangerInstance is a breakable prop placed in the city - a bin, a hydrant, a lamp post.
//
// Like the traffic cars, bangers are left out of the reflection and shadow passes. SetupGfxLights
// returning zero says the same thing from the lighting side: a banger contributes no light source,
// so the renderer does not reserve a light slot for it.
//
// ComputeLod is NOT ported. It writes the global at 0x00627674 before returning, and porting it
// before that address is symbolised would bake a hard-coded pointer into C++.

#include "dgbangerinstance.h"

#include "vector7/matrix34.h"

// ?DrawShadow@dgBangerInstance@@UAEXXZ
void dgBangerInstance::DrawShadow()
{}

// ?DrawReflected@dgBangerInstance@@UAEXM@Z
void dgBangerInstance::DrawReflected(f32 arg1)
{
    (void) arg1;
}

// ?SetupGfxLights@dgBangerInstance@@UBEHABVMatrix34@@@Z
//
// Const (`UBE`). No light sources on a banger.
i32 dgBangerInstance::SetupGfxLights(const Matrix34& arg1) const
{
    (void) arg1;

    return 0;
}

define_dummy_symbol(dg_dgbangerinstance);
