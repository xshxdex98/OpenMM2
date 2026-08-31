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

// The base of everything placed in a city - buildings, props, bangers, cars, the player. Almost
// every virtual here is a DEFAULT rather than an implementation: the base answers "no" or "nothing"
// and the derived classes that actually have geometry, an entity or a shadow override it.
//
// That is why so many of these are one instruction in the binary. They are not stubs left
// unfinished in 1999 - they are the base case of a dispatch that runs for every instance in the
// city every frame, and their addresses sit in ??_7lvlInstance@@6B@ for the classes that do not
// override them.
//
// Only the trivial ones are ported here. Init and Reset's siblings that touch the room list stay
// in assembly. GetRadius indexes a 0x20-stride table at 0x006316D0 by GeomSet and waits on that
// table being symbolised; GetVelocity no longer waits, because the zero vector it returns now has
// a name - see data/globals.json.

#include "lvlinstance.h"

#include "misc/freefuncs.h"
#include "vector7/matrix34.h"
#include "vector7/vector3.h"

// ?Detach@lvlInstance@@UAEXXZ - 0x0043FC30
//
// A bare `ret`. Detaching is meaningful only for an instance with a physics entity, and the base
// has none - dgPhysEntity-backed classes override this to unhook themselves from the manager.
void lvlInstance::Detach()
{}

// ?IsCollidable@lvlInstance@@UAE_NXZ - 0x0043FC40
//
// Bit 4 of Flags. The original reads the LOW byte of the u16 and shifts:
//     return (bool)((byte)this[8] >> 4 & 1);
bool lvlInstance::IsCollidable()
{
    return (Flags >> 4) & 1;
}

// ?IsTerrainCollidable@lvlInstance@@UAE_NXZ - 0x0043FC50
//
// Bit 8, NOT bit 0. The original reads the HIGH byte of the same u16 -
//     return (bool)((byte)this[9] & 1);
// - so this is one flag word tested at two different byte offsets, which is easy to misread as two
// separate fields.
bool lvlInstance::IsTerrainCollidable()
{
    return (Flags >> 8) & 1;
}

// ?IsLandmark@lvlInstance@@UAE_NXZ - 0x00463180
//
// Constant false. Only lvlLandmarkInstance answers otherwise.
bool lvlInstance::IsLandmark()
{
    return false;
}

// ?Reset@lvlInstance@@UAEXXZ - 0x00463280
//
// A bare `ret`. The base holds no per-frame state to clear; the bangers and the physics entities
// override it.
void lvlInstance::Reset()
{}

// ?GetNumLightSources@lvlInstance@@UAEHXZ - 0x004632B0
//
// No light sources by default, so SetupGfxLights below is never asked for any.
i32 lvlInstance::GetNumLightSources()
{
    return 0;
}

// ?GetEntity@lvlInstance@@UAEPAVdgPhysEntity@@XZ - 0x004643B0
//
// The base instance has no physics entity. dgBangerInstance and the vehicles return theirs.
dgPhysEntity* lvlInstance::GetEntity()
{
    return nullptr;
}

// ?AttachEntity@lvlInstance@@UAEPAVdgPhysEntity@@XZ - 0x004643C0
//
// Returns null rather than creating one: an instance that can gain an entity overrides this, and
// the caller treats null as "this instance never moves".
dgPhysEntity* lvlInstance::AttachEntity()
{
    return nullptr;
}

// ?GetVelocity@lvlInstance@@UAEABVVector3@@XZ - 0x004643A0
//
// A static instance does not move, so it reports the zero vector - and specifically THE zero
// vector at 0x006A3B08, not a fresh one. Six other classes return the same address, and assembly
// that has not been ported compares the pointer rather than the value.
const Vector3& lvlInstance::GetVelocity()
{
    return ARTS_ZERO_VECTOR3;
}

// ?SetVariant@lvlInstance@@UAEXH@Z - 0x004643D0
//
// Ignored by the base. dgBangerInstance packs the variant into the top nibble of the u16 at 0x14.
void lvlInstance::SetVariant(i32 arg1)
{
    (void) arg1;
}

// ?DrawShadow@lvlInstance@@UAEXXZ - 0x004643F0
void lvlInstance::DrawShadow()
{}

// ?DrawShadowMap@lvlInstance@@UAEXXZ - 0x00464400
void lvlInstance::DrawShadowMap()
{}

// ?DrawGlow@lvlInstance@@UAEXXZ - 0x00464410
void lvlInstance::DrawGlow()
{}

// ?DrawReflected@lvlInstance@@UAEXM@Z - 0x00464420
//
// The float is the reflection plane height. Nothing in the base reflects, so it is dropped.
void lvlInstance::DrawReflected(f32 arg1)
{
    (void) arg1;
}

// ?SetupGfxLights@lvlInstance@@UAEHABVMatrix34@@@Z - 0x00464670
//
// Returns zero lights set up, consistent with GetNumLightSources above.
i32 lvlInstance::SetupGfxLights(const Matrix34& arg1)
{
    (void) arg1;

    return 0;
}

// ?DrawReflectedParts@lvlInstance@@UAEXH@Z - 0x004648B0
void lvlInstance::DrawReflectedParts(i32 arg1)
{
    (void) arg1;
}

define_dummy_symbol(lvl_lvlinstance);
