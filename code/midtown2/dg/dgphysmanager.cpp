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

// dgPhysManager inherits phPhysicsManager's interface for promoting, demoting and pooling physics
// instances, and implements none of it. Every one of these returns null or does nothing.
//
// That is not an oversight. MM2 does not manage physics instances through this interface at all -
// dgPhysManager keeps a fixed pool of dgBangerActives and drives collision through
// CollideInstances, so the phInstance-based API it inherits has no MM2 meaning. The overrides
// exist to give the vtable slots a body; the engine never calls them.
//
// Draw is the same story from the other direction: a debug-draw hook with nothing behind it in the
// retail build. It is the only non-virtual of the eight.
//
// The class is not polymorphic through phPhysicsManager in the C++ sense here - its base is a
// vptr-only class that declares no virtual method, so genheaders flattens it and dgPhysManager
// carries the vptr itself. See tools/genheaders.py's vptr_only_base().

#include "dgphysmanager.h"

// ?Draw@dgPhysManager@@QAEXXZ - 0x00469EC0
//
// A bare `ret`. The debug draw was compiled out of the retail build.
void dgPhysManager::Draw()
{}

// ?PromoteInstance@dgPhysManager@@UAEPAVphColliderBase@@PAVphInstance@@@Z - 0x00469F20
//
// Promotion means giving a static instance an active collider. MM2 never does it through this
// path, so there is nothing to hand back.
phColliderBase* dgPhysManager::PromoteInstance(phInstance* arg1)
{
    (void) arg1;

    return nullptr;
}

// ?DemoteInstance@dgPhysManager@@UAEXPAVphInstance@@@Z - 0x00469F30
void dgPhysManager::DemoteInstance(phInstance* arg1)
{
    (void) arg1;
}

// ?KillInstance@dgPhysManager@@UAEXPAVphInstance@@@Z - 0x00469F40
void dgPhysManager::KillInstance(phInstance* arg1)
{
    (void) arg1;
}

// ?DisableInstance@dgPhysManager@@UAEXPAVphInstance@@@Z - 0x00469F50
void dgPhysManager::DisableInstance(phInstance* arg1)
{
    (void) arg1;
}

// ?EnableInstance@dgPhysManager@@UAEXPAVphInstance@@H@Z - 0x00469F60
void dgPhysManager::EnableInstance(phInstance* arg1, i32 arg2)
{
    (void) arg1;
    (void) arg2;
}

// ?GetCollider@dgPhysManager@@UBEPAVphColliderBase@@PBVphInstance@@@Z - 0x00469F70
//
// `UBE` in the mangled name, not `UAE` - this one is const, and the const has to be declared or
// the symbol does not match.
phColliderBase* dgPhysManager::GetCollider(const phInstance* arg1) const
{
    (void) arg1;

    return nullptr;
}

// ?CreateInstance@dgPhysManager@@UAEPAVphInstance@@XZ - 0x00469F80
//
// The pool is fixed and built in the constructor, so nothing is ever created on demand.
phInstance* dgPhysManager::CreateInstance()
{
    return nullptr;
}

define_dummy_symbol(dg_dgphysmanager);
