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
#include "ph/phboundbox.h"
#include "ph/phpolygon.h"
#include "vector7/vector3.h"

class lvlMaterial;
class phBound_vtbl;
class phHotEdge;
class phMaterial;

class dgBoundBox : public phBoundBox
{
public:
    // ??0dgBoundBox@@QAE@XZ
    ARTS_IMPORT dgBoundBox();

    // ??1dgBoundBox@@QAE@XZ
    ARTS_IMPORT ~dgBoundBox();

    // ?GetMaterial@dgBoundBox@@UBEPBVphMaterial@@H@Z
    ARTS_IMPORT virtual const phMaterial* GetMaterial(i32 arg1) const;

    // ?GetNumMaterials@dgBoundBox@@UBEHXZ
    ARTS_IMPORT virtual i32 GetNumMaterials() const;

    // ?SetFriction@dgBoundBox@@UAEXM@Z
    ARTS_IMPORT virtual void SetFriction(f32 arg1);

    // ?SetElasticity@dgBoundBox@@UAEXM@Z
    ARTS_IMPORT virtual void SetElasticity(f32 arg1);

public:
    // Members from 0x314; everything below that belongs to phBoundBox.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    lvlMaterial* Material; // 0x314
};

// ??_7dgBoundBox@@6B@
// vtable at 0x005B1D98
check_size(dgBoundBox, 0x318);
