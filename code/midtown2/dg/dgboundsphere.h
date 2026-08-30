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

#include "ph/phboundsphere.h"

class phMaterial;

class dgBoundSphere : public phBoundSphere
{
public:
    // ??0dgBoundSphere@@QAE@XZ
    ARTS_IMPORT dgBoundSphere();

    // ??1dgBoundSphere@@QAE@XZ
    ARTS_IMPORT ~dgBoundSphere();

    // ?GetMaterial@dgBoundSphere@@UBEPBVphMaterial@@H@Z
    ARTS_IMPORT virtual const phMaterial* GetMaterial(i32 arg1) const;

    // ?GetNumMaterials@dgBoundSphere@@UBEHXZ
    ARTS_IMPORT virtual i32 GetNumMaterials() const;

    // ?SetElasticity@dgBoundSphere@@UAEXM@Z
    ARTS_IMPORT virtual void SetElasticity(f32 arg1);

    // ?SetFriction@dgBoundSphere@@UAEXM@Z
    ARTS_IMPORT virtual void SetFriction(f32 arg1);
};

// ??_7dgBoundSphere@@6B@
// vtable at 0x005B1E38
// check_size(dgBoundSphere, 0x84); // size known, members are not - cannot verify
