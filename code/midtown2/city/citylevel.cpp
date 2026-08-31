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

// cityLevel is the level MM2 actually instantiates - the city, its rooms and its SDL geometry.
// These six are the entry points that answer with a constant.
//
// GetBound is the one that is not a stub: it returns the class's own static lvlSDL reinterpreted
// as an lvlLevelBound. The city's bound IS its SDL, so there is no separate bound object to build
// and the two views share an address.
//
// GetWaterLevel is NOT ported. It returns the global at 0x0062998C, and porting it before that
// address is symbolised would bake a hard-coded pointer into C++.

#include "citylevel.h"

#include "vector7/vector3.h"

// ?PostDraw@cityLevel@@UAEXXZ - 0x00445300
//
// The hook after the city has drawn. Nothing runs there in retail.
void cityLevel::PostDraw()
{}

// ?Collide@cityLevel@@UAE_NHAAVlvlSegment@@PAVlvlIntersection@@@Z - 0x00447020
//
// False, and deliberately so despite the city having geometry: segment collision against the city
// runs through the SDL traversal in lvlSDL, not through this entry point.
bool cityLevel::Collide(i32 arg1, lvlSegment& arg2, lvlIntersection* arg3)
{
    (void) arg1;
    (void) arg2;
    (void) arg3;

    return false;
}

// ?GetBound@cityLevel@@UAEPBVlvlLevelBound@@XZ - 0x00443930
//
// The city's bound is its SDL. Returning the address of the static as an lvlLevelBound* is what
// the binary does - the two are different views of the same object, not a conversion.
const lvlLevelBound* cityLevel::GetBound()
{
    return reinterpret_cast<const lvlLevelBound*>(&SDL);
}

// ?GetLightingIntensity@cityLevel@@UBEMABVVector3@@@Z - 0x00445290
//
// Full brightness everywhere. The city has no per-position light falloff; time of day is applied
// through the shaders instead.
f32 cityLevel::GetLightingIntensity(const Vector3& arg1) const
{
    (void) arg1;

    return 1.0f;
}

// ?SetPtxHeight@cityLevel@@UAEXAAVasParticles@@@Z - 0x004452A0
//
// Would have clamped a particle system to the ground height beneath it.
void cityLevel::SetPtxHeight(asParticles& arg1)
{
    (void) arg1;
}

// ?GetLastStartRoom@cityLevel@@QAEHXZ - 0x00447010
//
// The room the PVS traversal started from last frame, kept as a class-wide static rather than per
// instance because only one city is ever loaded.
i32 cityLevel::GetLastStartRoom()
{
    return sm_LastPvsRoom;
}

define_dummy_symbol(city_citylevel);
