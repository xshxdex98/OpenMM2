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

#include "dg/dgunhitbangerinstance.h"

class Matrix34;
class Vector3;
class dgHitBangerInstance;

class aiSubwayInstance : public dgUnhitBangerInstance
{
public:
    // ?GetPosition@aiSubwayInstance@@UAEABVVector3@@XZ
    ARTS_IMPORT virtual const Vector3& GetPosition();

    // ?GetMatrix@aiSubwayInstance@@UAEABVMatrix34@@AAV2@@Z
    ARTS_IMPORT virtual const Matrix34& GetMatrix(Matrix34& arg1);

    // ?SetMatrix@aiSubwayInstance@@UAEXABVMatrix34@@@Z
    ARTS_IMPORT virtual void SetMatrix(const Matrix34& arg1);

    // ?GetVelocity@aiSubwayInstance@@UAEABVVector3@@XZ
    ARTS_IMPORT virtual const Vector3& GetVelocity();

    // ?Draw@aiSubwayInstance@@UAEXH@Z
    ARTS_IMPORT virtual void Draw(i32 arg1);

    // ?SizeOf@aiSubwayInstance@@UAEIXZ
    ARTS_IMPORT virtual u32 SizeOf();

    // ?ImpactCB@aiSubwayInstance@@UAEXPAVdgHitBangerInstance@@@Z
    ARTS_IMPORT virtual void ImpactCB(dgHitBangerInstance* arg1);
};

// ??_7aiSubwayInstance@@6B@
// vtable at 0x005B5798
// check_size(aiSubwayInstance, 0x28); // size known, members are not - cannot verify
