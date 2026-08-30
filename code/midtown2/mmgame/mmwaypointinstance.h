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
#include "vector7/vector3.h"

class lvlInstance_vtbl;

class mmWaypointInstance : public lvlInstance
{
public:
    // ??0mmWaypointInstance@@QAE@XZ
    ARTS_IMPORT mmWaypointInstance();

    // ??1mmWaypointInstance@@QAE@XZ
    ARTS_IMPORT ~mmWaypointInstance();

    // ?GetPosition@mmWaypointInstance@@UAEABVVector3@@XZ
    ARTS_IMPORT virtual const Vector3& GetPosition();

    // ?GetMatrix@mmWaypointInstance@@UAEABVMatrix34@@AAV2@@Z
    ARTS_IMPORT virtual const Matrix34& GetMatrix(Matrix34& arg1);

    // ?SetMatrix@mmWaypointInstance@@UAEXABVMatrix34@@@Z
    ARTS_IMPORT virtual void SetMatrix(const Matrix34& arg1);

    // ?SetVariant@mmWaypointInstance@@UAEXH@Z
    ARTS_IMPORT virtual void SetVariant(i32 arg1);

    // ?MakeInvisible@mmWaypointInstance@@QAEXXZ
    ARTS_IMPORT void MakeInvisible();

    // ?MakeVisible@mmWaypointInstance@@QAEXXZ
    ARTS_IMPORT void MakeVisible();

public:
    // Members from 0x014; everything below that belongs to lvlInstance.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    Vector3 Scale; // 0x014
    Matrix34 Matrix; // 0x020
    i32 HasBeenDrawn; // 0x050
    bool Visible; // 0x054
    u8 pad_55[3]; // 0x055
    i32 ShaderSet; // 0x058
};

// ??_7mmWaypointInstance@@6B@
// vtable at 0x005B11E8
check_size(mmWaypointInstance, 0x5C);
