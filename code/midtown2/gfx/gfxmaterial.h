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

#include "vector7/vector4.h"

struct gfxMaterial
{
public:
    // ?Reset@gfxMaterial@@QAEXXZ
    ARTS_IMPORT void Reset();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    Vector4 Diffuse; // 0x000
    Vector4 Ambient; // 0x010
    Vector4 Specular; // 0x020
    Vector4 Emissive; // 0x030
    f32 Shininess; // 0x040
    u32 Color; // 0x044
};

check_size(gfxMaterial, 0x48);
