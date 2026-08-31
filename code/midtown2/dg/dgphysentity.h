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

#pragma once

#include "core/arts.h"

#include "misc/base.h"

class Base_vtbl;
class phCollider;

class dgPhysEntity : public Base
{
public:
    // ??1dgPhysEntity@@UAE@XZ
    ARTS_IMPORT virtual ~dgPhysEntity();

    // ?PreUpdate@dgPhysEntity@@UAEXXZ
    ARTS_EXPORT virtual void PreUpdate();

    // ?Update@dgPhysEntity@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?GetCollider@dgPhysEntity@@UAEPAVphCollider@@XZ
    ARTS_IMPORT virtual phCollider* GetCollider();

    // ?DetachMe@dgPhysEntity@@UAEXXZ
    ARTS_EXPORT virtual void DetachMe();

    // ?RequiresTerrainCollision@dgPhysEntity@@UAE_NXZ
    ARTS_EXPORT virtual bool RequiresTerrainCollision();

    // ?FirstImpactCallback@dgPhysEntity@@UAEXXZ
    ARTS_EXPORT virtual void FirstImpactCallback();

public:
    // Members from 0x004; everything below that belongs to Base.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 buffer[176]; // 0x004
};

// ??_7dgPhysEntity@@6B@
// vtable at 0x005B0BD8
check_size(dgPhysEntity, 0xB4);
