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

#include "lvlinstance.h"

class Matrix34;
class Vector3;
class dgPhysEntity;
struct lvlMultiRoomState;
class phBound;

class lvlMultiRoomInstance : public lvlInstance
{
public:
    // ?AttachEntity@lvlMultiRoomInstance@@UAEPAVdgPhysEntity@@XZ
    ARTS_IMPORT virtual dgPhysEntity* AttachEntity();

    // ?Create@lvlMultiRoomInstance@@SAXPAVlvlInstance@@H@Z
    ARTS_IMPORT static void Create(lvlInstance* arg1, i32 arg2);

    // ?Draw@lvlMultiRoomInstance@@UAEXH@Z
    ARTS_IMPORT virtual void Draw(i32 arg1);

    // ?GetBound@lvlMultiRoomInstance@@UAEPBVphBound@@H@Z
    ARTS_IMPORT virtual const phBound* GetBound(i32 arg1);

    // ?GetEntity@lvlMultiRoomInstance@@UAEPAVdgPhysEntity@@XZ
    ARTS_IMPORT virtual dgPhysEntity* GetEntity();

    // ?GetMatrix@lvlMultiRoomInstance@@UAEABVMatrix34@@AAV2@@Z
    ARTS_IMPORT virtual const Matrix34& GetMatrix(Matrix34& arg1);

    // ?GetPosition@lvlMultiRoomInstance@@UAEABVVector3@@XZ
    ARTS_IMPORT virtual const Vector3& GetPosition();

    // ?GetVelocity@lvlMultiRoomInstance@@UAEABVVector3@@XZ
    ARTS_IMPORT virtual const Vector3& GetVelocity();

    // ?IsCollidable@lvlMultiRoomInstance@@UAE_NXZ
    ARTS_IMPORT virtual bool IsCollidable();

    // ?IsLandmark@lvlMultiRoomInstance@@UAE_NXZ
    ARTS_IMPORT virtual bool IsLandmark();

    // ?IsTerrainCollidable@lvlMultiRoomInstance@@UAE_NXZ
    ARTS_IMPORT virtual bool IsTerrainCollidable();

    // ?SetMatrix@lvlMultiRoomInstance@@UAEXABVMatrix34@@@Z
    ARTS_IMPORT virtual void SetMatrix(const Matrix34& arg1);

    // ?SizeOf@lvlMultiRoomInstance@@UAEIXZ
    ARTS_IMPORT virtual u32 SizeOf();

private:
    // ??0lvlMultiRoomInstance@@AAE@PAVlvlInstance@@PAUlvlMultiRoomState@@H@Z
    ARTS_IMPORT lvlMultiRoomInstance(lvlInstance* arg1, lvlMultiRoomState* arg2, i32 arg3);
};

// ??_7lvlMultiRoomInstance@@6B@
// vtable at 0x005B1C48
// check_size(lvlMultiRoomInstance, 0x0); // TODO: no layout in the IDB type library
