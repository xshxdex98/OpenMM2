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

#include "ph/phboundgeometry.h"

class phMaterial;

class dgBoundGeometry : public phBoundGeometry
{
public:
    // ??0dgBoundGeometry@@QAE@XZ
    ARTS_IMPORT dgBoundGeometry();

    // ??1dgBoundGeometry@@QAE@XZ
    ARTS_IMPORT ~dgBoundGeometry();

    // ?GetMaterial@dgBoundGeometry@@UBEPBVphMaterial@@H@Z
    ARTS_IMPORT virtual const phMaterial* GetMaterial(i32 arg1) const;

    // ?GetNumMaterials@dgBoundGeometry@@UBEHXZ
    ARTS_IMPORT virtual i32 GetNumMaterials() const;

    // ?SetFriction@dgBoundGeometry@@UAEXM@Z
    ARTS_IMPORT virtual void SetFriction(f32 arg1);

    // ?SetElasticity@dgBoundGeometry@@UAEXM@Z
    ARTS_IMPORT virtual void SetElasticity(f32 arg1);
};

// ??_7dgBoundGeometry@@6B@
// vtable at 0x005B1D3C
// check_size(dgBoundGeometry, 0x80); // size known, members are not - cannot verify
