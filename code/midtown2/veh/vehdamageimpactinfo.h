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

#include "vector7/vector3.h"

class phCollider;

struct vehDamageImpactInfo
{
public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    phCollider* pCollider; // 0x000
    Vector3 MaybeLastImpactPos; // 0x004
    Vector3 field_10; // 0x010
    Vector3 field_1C; // 0x01C
    Vector3 field_28; // 0x028
    f32 field_34; // 0x034
    f32 field_38; // 0x038
    f32 SomeTimer; // 0x03C
};

check_size(vehDamageImpactInfo, 0x40);
