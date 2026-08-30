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
class dgBangerData;
class dgPhysEntity;
class phBound;

class dgBangerInstance : public lvlInstance
{
public:
    // ?AttachEntity@dgBangerInstance@@UAEPAVdgPhysEntity@@XZ
    ARTS_IMPORT virtual dgPhysEntity* AttachEntity();

    // ?ComputeLod@dgBangerInstance@@UAEHMM@Z
    ARTS_IMPORT virtual i32 ComputeLod(f32 arg1, f32 arg2);

    // ?Draw@dgBangerInstance@@UAEXH@Z
    ARTS_IMPORT virtual void Draw(i32 arg1);

    // ?DrawGlow@dgBangerInstance@@UAEXXZ
    ARTS_IMPORT virtual void DrawGlow();

    // ?DrawReflected@dgBangerInstance@@UAEXM@Z
    ARTS_IMPORT virtual void DrawReflected(f32 arg1);

    // ?DrawShadow@dgBangerInstance@@UAEXXZ
    ARTS_IMPORT virtual void DrawShadow();

    // ?DrawShadowMap@dgBangerInstance@@UAEXXZ
    ARTS_IMPORT virtual void DrawShadowMap();

    // ?DrawTree@dgBangerInstance@@QAEXH@Z
    ARTS_IMPORT void DrawTree(i32 arg1);

    // ?GetBound@dgBangerInstance@@UAEPBVphBound@@H@Z
    ARTS_IMPORT virtual const phBound* GetBound(i32 arg1);

    // ?GetData@dgBangerInstance@@QAEPAVdgBangerData@@XZ
    ARTS_IMPORT dgBangerData* GetData();

    // ?GetEntity@dgBangerInstance@@UAEPAVdgPhysEntity@@XZ
    ARTS_IMPORT virtual dgPhysEntity* GetEntity();

    // ?GetVelocity@dgBangerInstance@@UAEABVVector3@@XZ
    ARTS_IMPORT virtual const Vector3& GetVelocity();

    // ?SetupGfxLights@dgBangerInstance@@UBEHABVMatrix34@@@Z
    ARTS_IMPORT virtual i32 SetupGfxLights(const Matrix34& arg1) const;

    // ?SetVariant@dgBangerInstance@@UAEXH@Z
    ARTS_IMPORT virtual void SetVariant(i32 arg1);

public:
    // Members from 0x014; everything below that belongs to lvlInstance.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u16 BangerId; // 0x014
};

check_size(dgBangerInstance, 0x18);
