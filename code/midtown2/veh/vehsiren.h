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

class Matrix34;
class Vector3;

class vehSiren
{
public:
    // ??0vehSiren@@QAE@XZ
    ARTS_IMPORT vehSiren();

    // ??1vehSiren@@QAE@XZ
    ARTS_IMPORT ~vehSiren();

    // ?AddLight@vehSiren@@QAE_NABVVector3@@0@Z
    ARTS_IMPORT bool AddLight(const Vector3& arg1, const Vector3& arg2);

    // ?Draw@vehSiren@@QAEXABVMatrix34@@@Z
    ARTS_IMPORT void Draw(const Matrix34& arg1);

    // ?Init@vehSiren@@QAE_NXZ
    ARTS_IMPORT bool Init();

    // ?Reset@vehSiren@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?Update@vehSiren@@QAEXXZ
    ARTS_IMPORT void Update();
};

// check_size(vehSiren, 0x164); // size known, members are not - cannot verify
