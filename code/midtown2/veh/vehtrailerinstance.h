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
#include "vector7/vector3.h"

class Matrix34;
class dgPhysEntity;
class lvlInstance_vtbl;
class vehTrailer;

class vehTrailerInstance : public lvlInstance
{
public:
    // ?GetPosition@vehTrailerInstance@@UAEABVVector3@@XZ
    ARTS_IMPORT virtual const Vector3& GetPosition();

    // ?GetMatrix@vehTrailerInstance@@UAEABVMatrix34@@AAV2@@Z
    ARTS_IMPORT virtual const Matrix34& GetMatrix(Matrix34& arg1);

    // ?SetMatrix@vehTrailerInstance@@UAEXABVMatrix34@@@Z
    ARTS_IMPORT virtual void SetMatrix(const Matrix34& arg1);

    // ?GetEntity@vehTrailerInstance@@UAEPAVdgPhysEntity@@XZ
    ARTS_IMPORT virtual dgPhysEntity* GetEntity();

    // ?AttachEntity@vehTrailerInstance@@UAEPAVdgPhysEntity@@XZ
    ARTS_IMPORT virtual dgPhysEntity* AttachEntity();

    // ?GetVelocity@vehTrailerInstance@@UAEABVVector3@@XZ
    ARTS_IMPORT virtual const Vector3& GetVelocity();

    // ?Draw@vehTrailerInstance@@UAEXH@Z
    ARTS_IMPORT virtual void Draw(i32 label_id);

    // ?DrawShadow@vehTrailerInstance@@UAEXXZ
    ARTS_IMPORT virtual void DrawShadow();

    // ?DrawShadowMap@vehTrailerInstance@@UAEXXZ
    ARTS_IMPORT virtual void DrawShadowMap();

    // ?SizeOf@vehTrailerInstance@@UAEIXZ
    ARTS_IMPORT virtual u32 SizeOf();

    // ?GetTrailerHitch@vehTrailerInstance@@QAE_NPAVVector3@@@Z
    ARTS_IMPORT bool GetTrailerHitch(Vector3* arg1);

    // ?Init@vehTrailerInstance@@QAEXPBDABVVector3@@H@Z
    ARTS_IMPORT void Init(const char* name, const Vector3& arg2, i32 arg3);

public:
    // Members from 0x014; everything below that belongs to lvlInstance.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    vehTrailer* Trailer; // 0x014
    i32 Variant; // 0x018
    Vector3 TrailerHitchPosition; // 0x01C
    u8[8] pad_20; // 0x020
};

// ??_7vehTrailerInstance@@6B@
// vtable at 0x005B2F84
check_size(vehTrailerInstance, 0x28);
