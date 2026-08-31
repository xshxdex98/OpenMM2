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

#include "vector7/matrix34.h"

#include "dgbangerinstance.h"

class Vector3;
class lvlInstance;
class lvlInstance_vtbl;

class dgHitBangerInstance : public dgBangerInstance
{
public:
    // ??0dgHitBangerInstance@@QAE@XZ
    ARTS_IMPORT dgHitBangerInstance();

    // ??1dgHitBangerInstance@@QAE@XZ
    ARTS_IMPORT ~dgHitBangerInstance();

    // ?GetPosition@dgHitBangerInstance@@UAEABVVector3@@XZ
    ARTS_IMPORT virtual const Vector3& GetPosition();

    // ?GetMatrix@dgHitBangerInstance@@UAEABVMatrix34@@AAV2@@Z
    ARTS_IMPORT virtual const Matrix34& GetMatrix(Matrix34& arg1);

    // ?SetMatrix@dgHitBangerInstance@@UAEXABVMatrix34@@@Z
    ARTS_IMPORT virtual void SetMatrix(const Matrix34& new_capacity);

    // ?Detach@dgHitBangerInstance@@UAEXXZ
    ARTS_IMPORT virtual void Detach();

    // ?SizeOf@dgHitBangerInstance@@UAEIXZ
    ARTS_IMPORT virtual u32 SizeOf();

public:
    // Members from 0x028; everything below that belongs to dgBangerInstance.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    Matrix34 Matrix; // 0x028
};

// ??_7dgHitBangerInstance@@6B@
// vtable at 0x005B15B8
check_size(dgHitBangerInstance, 0x58);
