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

#include "lvl/lvlinstance.h"
#include "vector7/matrix34.h"

class Vector3;

class gizInstance : public lvlInstance
{
public:
    // ??0gizInstance@@QAE@XZ
    ARTS_IMPORT gizInstance();

    // ??1gizInstance@@QAE@XZ
    ARTS_IMPORT ~gizInstance();

    // ?Draw@gizInstance@@UAEXH@Z
    ARTS_IMPORT virtual void Draw(i32 arg1);

    // ?GetMatrix@gizInstance@@UAEABVMatrix34@@AAV2@@Z
    ARTS_IMPORT virtual const Matrix34& GetMatrix(Matrix34& arg1);

    // ?GetPosition@gizInstance@@UAEABVVector3@@XZ
    ARTS_IMPORT virtual const Vector3& GetPosition();

    // ?Init@gizInstance@@QAEXPADH@Z
    ARTS_IMPORT void Init(char* arg1, i32 arg2);

    // ?SetMatrix@gizInstance@@UAEXABVMatrix34@@@Z
    ARTS_IMPORT virtual void SetMatrix(const Matrix34& arg1);

public:
    // Members from 0x014; everything below that belongs to lvlInstance.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    Matrix34 matrix3414; // 0x014
    f32 field_44; // 0x044
    u8 field_48; // 0x048
};

// ??_7gizInstance@@6B@
// vtable at 0x005B6278
check_size(gizInstance, 0x4C);
