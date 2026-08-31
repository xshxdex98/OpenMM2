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

// lvlLevel is the abstract level; cityLevel is the one MM2 actually instantiates. These five are
// the base's answers for a level that has no geometry of its own - it does not collide, has no
// bounding sphere, and reflects nothing.
//
// cityLevel overrides Collide and GetBoundSphere with the real SDL-backed implementations, which
// is why the base can afford to say no.

#include "lvllevel.h"

#include "vector7/vector3.h"
#include "vector7/vector4.h"

// ?ClampToWorld@lvlLevel@@UAE_NAAVVector3@@@Z - 0x004472F0
//
// False means "not clamped" - the caller keeps the position it passed in. A level with real bounds
// overrides this and may modify the vector.
bool lvlLevel::ClampToWorld(Vector3& arg1)
{
    (void) arg1;

    return false;
}

// ?SetObjectDetail@lvlLevel@@UAEXH@Z - 0x00465470
//
// The detail slider. The base level has no objects whose LOD could change.
void lvlLevel::SetObjectDetail(i32 arg1)
{
    (void) arg1;
}

// ?Collide@lvlLevel@@UAE_NHAAVlvlSegment@@PAVlvlIntersection@@@Z - 0x004655B0
//
// No geometry, so nothing to hit. cityLevel overrides this with the SDL traversal.
bool lvlLevel::Collide(i32 arg1, lvlSegment& arg2, lvlIntersection* arg3)
{
    (void) arg1;
    (void) arg2;
    (void) arg3;

    return false;
}

// ?GetBoundSphere@lvlLevel@@UAE_NAAVVector4@@H@Z - 0x004655C0
//
// False leaves the Vector4 untouched, which the caller reads as "this level has no bound".
bool lvlLevel::GetBoundSphere(Vector4& arg1, i32 arg2)
{
    (void) arg1;
    (void) arg2;

    return false;
}

// ?GetEnvMap@lvlLevel@@UAEPAVgfxTexture@@HABVVector3@@AAM@Z - 0x00465690
//
// No environment map, so reflective surfaces fall back to their flat shading. The f32& is an
// out-parameter for the blend strength and is left alone when the texture is null.
gfxTexture* lvlLevel::GetEnvMap(i32 arg1, const Vector3& arg2, f32& arg3)
{
    (void) arg1;
    (void) arg2;
    (void) arg3;

    return nullptr;
}

define_dummy_symbol(lvl_lvllevel);
