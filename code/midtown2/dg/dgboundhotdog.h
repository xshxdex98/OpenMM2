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

#include "ph/phbound.h"
#include "ph/phboundhotdog.h"
#include "ph/phmaterial.h"
#include "vector7/vector3.h"

class lvlMaterial;
class phBound_vtbl;

class dgBoundHotdog : public phBoundHotdog
{
public:
    // ??0dgBoundHotdog@@QAE@XZ
    ARTS_IMPORT dgBoundHotdog();

    // ??1dgBoundHotdog@@QAE@XZ
    ARTS_IMPORT ~dgBoundHotdog();

    // ?GetMaterial@dgBoundHotdog@@UBEPBVphMaterial@@H@Z
    ARTS_IMPORT virtual const phMaterial* GetMaterial(i32 arg1) const;

    // ?GetNumMaterials@dgBoundHotdog@@UBEHXZ
    ARTS_IMPORT virtual i32 GetNumMaterials() const;

    // ?SetFriction@dgBoundHotdog@@UAEXM@Z
    ARTS_IMPORT virtual void SetFriction(f32 arg1);

    // ?SetElasticity@dgBoundHotdog@@UAEXM@Z
    ARTS_IMPORT virtual void SetElasticity(f32 arg1);

public:
    // Members from 0x084; everything below that belongs to phBoundHotdog.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    lvlMaterial* Material; // 0x084
};

// ??_7dgBoundHotdog@@6B@
// vtable at 0x005B1DF4
check_size(dgBoundHotdog, 0x88);
