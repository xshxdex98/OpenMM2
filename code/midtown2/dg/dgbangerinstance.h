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
    // ?SetVariant@dgBangerInstance@@UAEXH@Z
    ARTS_IMPORT virtual void SetVariant(i32 arg1);

    // ?GetEntity@dgBangerInstance@@UAEPAVdgPhysEntity@@XZ
    ARTS_IMPORT virtual dgPhysEntity* GetEntity();

    // ?AttachEntity@dgBangerInstance@@UAEPAVdgPhysEntity@@XZ
    ARTS_IMPORT virtual dgPhysEntity* AttachEntity();

    // ?GetVelocity@dgBangerInstance@@UAEABVVector3@@XZ
    ARTS_IMPORT virtual const Vector3& GetVelocity();

    // ?Draw@dgBangerInstance@@UAEXH@Z
    ARTS_IMPORT virtual void Draw(i32 lod);

    // ?DrawShadow@dgBangerInstance@@UAEXXZ
    ARTS_EXPORT virtual void DrawShadow();

    // ?DrawShadowMap@dgBangerInstance@@UAEXXZ
    ARTS_IMPORT virtual void DrawShadowMap();

    // ?DrawGlow@dgBangerInstance@@UAEXXZ
    ARTS_IMPORT virtual void DrawGlow();

    // ?DrawReflected@dgBangerInstance@@UAEXM@Z
    ARTS_EXPORT virtual void DrawReflected(f32 arg1);

    // ?GetBound@dgBangerInstance@@UAEPBVphBound@@H@Z
    ARTS_IMPORT virtual const phBound* GetBound(i32 arg1);

    // ?ComputeLod@dgBangerInstance@@UAEHMM@Z
    ARTS_IMPORT virtual i32 ComputeLod(f32 arg1, f32 arg2);

    // ?SetupGfxLights@dgBangerInstance@@UBEHABVMatrix34@@@Z
    ARTS_EXPORT virtual i32 SetupGfxLights(const Matrix34& arg1) const;

    // ?DrawTree@dgBangerInstance@@QAEXH@Z
    ARTS_IMPORT void DrawTree(i32 arg1);

    // ?GetData@dgBangerInstance@@QAEPAVdgBangerData@@XZ
    ARTS_IMPORT dgBangerData* GetData();

public:
    // Members from 0x014; everything below that belongs to lvlInstance.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u16 DataPack; // 0x014
    u8 pad_16[2]; // 0x016
    i8 gap[16]; // 0x018
};

check_size(dgBangerInstance, 0x28);
