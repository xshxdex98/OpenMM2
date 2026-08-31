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

#include "vector7/vector2.h"
#include "vector7/vector3.h"

#include "lvlfixedany.h"

class Matrix34;
class gfxViewport;
class lvlInstance;
class lvlInstance_vtbl;

class lvlFixedRotY : public lvlFixedAny
{
public:
    // ??0lvlFixedRotY@@QAE@XZ
    ARTS_IMPORT lvlFixedRotY();

    // ?GetPosition@lvlFixedRotY@@UAEABVVector3@@XZ
    ARTS_IMPORT virtual const Vector3& GetPosition();

    // ?IsVisible@lvlFixedRotY@@UAEHABVgfxViewport@@@Z
    ARTS_IMPORT virtual i32 IsVisible(const gfxViewport& viewport);

    // ?GetMatrix@lvlFixedRotY@@UAEABVMatrix34@@AAV2@@Z
    ARTS_IMPORT virtual const Matrix34& GetMatrix(Matrix34& arg1);

    // ?SetMatrix@lvlFixedRotY@@UAEXABVMatrix34@@@Z
    ARTS_IMPORT virtual void SetMatrix(const Matrix34& arg1);

    // ?SizeOf@lvlFixedRotY@@UAEIXZ
    ARTS_IMPORT virtual u32 SizeOf();

public:
    // Members from 0x014; everything below that belongs to lvlFixedAny.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    Vector3 Position; // 0x014
    Vector2 XAxis; // 0x020
};

// ??_7lvlFixedRotY@@6B@
// vtable at 0x005B1B78
check_size(lvlFixedRotY, 0x28);
