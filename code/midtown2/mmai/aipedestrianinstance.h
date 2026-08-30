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

class Matrix34;
class Vector3;
class aiPedestrian;
class dgPhysEntity;
class phBound;

class aiPedestrianInstance : public lvlInstance
{
public:
    // ??0aiPedestrianInstance@@QAE@PAVaiPedestrian@@@Z
    ARTS_IMPORT aiPedestrianInstance(aiPedestrian* arg1);

    // ?GetPosition@aiPedestrianInstance@@UAEABVVector3@@XZ
    ARTS_IMPORT virtual const Vector3& GetPosition();

    // ?GetMatrix@aiPedestrianInstance@@UAEABVMatrix34@@AAV2@@Z
    ARTS_IMPORT virtual const Matrix34& GetMatrix(Matrix34& arg1);

    // ?SetMatrix@aiPedestrianInstance@@UAEXABVMatrix34@@@Z
    ARTS_IMPORT virtual void SetMatrix(const Matrix34& arg1);

    // ?GetEntity@aiPedestrianInstance@@UAEPAVdgPhysEntity@@XZ
    ARTS_IMPORT virtual dgPhysEntity* GetEntity();

    // ?AttachEntity@aiPedestrianInstance@@UAEPAVdgPhysEntity@@XZ
    ARTS_IMPORT virtual dgPhysEntity* AttachEntity();

    // ?GetVelocity@aiPedestrianInstance@@UAEABVVector3@@XZ
    ARTS_IMPORT virtual const Vector3& GetVelocity();

    // ?Detach@aiPedestrianInstance@@UAEXXZ
    ARTS_IMPORT virtual void Detach();

    // ?Draw@aiPedestrianInstance@@UAEXH@Z
    ARTS_IMPORT virtual void Draw(i32 obj);

    // ?DrawShadow@aiPedestrianInstance@@UAEXXZ
    ARTS_IMPORT virtual void DrawShadow();

    // ?DrawShadowMap@aiPedestrianInstance@@UAEXXZ
    ARTS_IMPORT virtual void DrawShadowMap();

    // ?SizeOf@aiPedestrianInstance@@UAEIXZ
    ARTS_IMPORT virtual u32 SizeOf();

    // ?IsCollidable@aiPedestrianInstance@@UAE_NXZ
    ARTS_IMPORT virtual bool IsCollidable();

    // ?GetBound@aiPedestrianInstance@@UAEPBVphBound@@H@Z
    ARTS_IMPORT virtual const phBound* GetBound(i32 arg1);

    // ?ComputeLod@aiPedestrianInstance@@UAEHMM@Z
    ARTS_IMPORT virtual i32 ComputeLod(f32 arg1, f32 arg2);
};

// ??_7aiPedestrianInstance@@6B@
// vtable at 0x005B62F0
// check_size(aiPedestrianInstance, 0x2C); // size known, members are not - cannot verify
