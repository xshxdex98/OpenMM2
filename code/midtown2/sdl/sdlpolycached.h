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
#include "vector7/vector4.h"

#include "sdlpoly.h"

class sdlPolyCached : public sdlPoly
{
public:
    // ?InitFromPoly@sdlPolyCached@@QAEXPBVsdlPoly@@@Z
    ARTS_IMPORT void InitFromPoly(const sdlPoly* arg1);

public:
    // Members from 0x060; everything below that belongs to sdlPoly.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    Vector3 Verts[4]; // 0x060
};

check_size(sdlPolyCached, 0x90);
