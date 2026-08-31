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

// dgPhysEntity is the base of everything the physics manager moves - cars, trailers, bangers,
// pedestrians. These four are its defaults.
//
// RequiresTerrainCollision returning TRUE is the one that matters: every physics entity is tested
// against the terrain unless it opts out, so the base errs toward colliding. The rest are hooks a
// derived entity overrides if it needs them.

#include "dgphysentity.h"

// ?PreUpdate@dgPhysEntity@@UAEXXZ - 0x0042CBE0
//
// Called before the integration step. Nothing in the base needs to run first.
void dgPhysEntity::PreUpdate()
{}

// ?DetachMe@dgPhysEntity@@UAEXXZ - 0x0042CC00
void dgPhysEntity::DetachMe()
{}

// ?RequiresTerrainCollision@dgPhysEntity@@UAE_NXZ - 0x0042CC10
//
// True by default - the safe answer, since an entity that skips terrain collision falls through
// the world. vehCar and the bangers keep it; the ones that are already constrained override it.
bool dgPhysEntity::RequiresTerrainCollision()
{
    return true;
}

// ?FirstImpactCallback@dgPhysEntity@@UAEXXZ - 0x0042CC20
//
// Fired on the first contact of a collision pair, for entities that want to react once rather than
// per contact point. The base does not react at all.
void dgPhysEntity::FirstImpactCallback()
{}

define_dummy_symbol(dg_dgphysentity);
