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

#include "dgunhitbangerinstance.h"

class Vector3;

class dgUnhitMtxBangerInstance : public dgUnhitBangerInstance
{
public:
    // ?GetPosition@dgUnhitMtxBangerInstance@@UAEABVVector3@@XZ
    ARTS_IMPORT virtual const Vector3& GetPosition();

    // ?GetMatrix@dgUnhitMtxBangerInstance@@UAEABVMatrix34@@AAV2@@Z
    ARTS_IMPORT virtual const Matrix34& GetMatrix(Matrix34& arg1);

    // ?SetMatrix@dgUnhitMtxBangerInstance@@UAEXABVMatrix34@@@Z
    ARTS_IMPORT virtual void SetMatrix(const Matrix34& arg1);

    // ?SizeOf@dgUnhitMtxBangerInstance@@UAEIXZ
    ARTS_IMPORT virtual u32 SizeOf();

public:
    // Members from 0x018; everything below that belongs to dgUnhitBangerInstance.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 gap[16]; // 0x018
    Matrix34 Matrix; // 0x028
};

// ??_7dgUnhitMtxBangerInstance@@6B@
// vtable at 0x005B1494
check_size(dgUnhitMtxBangerInstance, 0x58);
