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

// aiVehicleInstance is the drawable half of a traffic car - the lvlInstance the city draws.
//
// Traffic cars are excluded from both the reflection and shadow-map passes. That is a deliberate
// cost decision, not an omission: there can be dozens of them in view, and each is drawn once for
// the main pass only.

#include "aivehicleinstance.h"

// ?DrawShadowMap@aiVehicleInstance@@UAEXXZ
void aiVehicleInstance::DrawShadowMap()
{}

// ?DrawReflected@aiVehicleInstance@@UAEXM@Z
void aiVehicleInstance::DrawReflected(f32 arg1)
{
    (void) arg1;
}

// ?SizeOf@aiVehicleInstance@@UAEIXZ
//
// 0x3C, emitted as `mov eax, 0x3c; ret` - the 1999 compiler's own sizeof, and independent evidence
// for the class's size.
u32 aiVehicleInstance::SizeOf()
{
    return 0x3C;
}

define_dummy_symbol(mmai_aivehicleinstance);
