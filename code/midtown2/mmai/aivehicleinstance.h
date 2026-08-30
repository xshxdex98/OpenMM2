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
class aiVehicleData;
class aiVehicleSpline;
class dgPhysEntity;
class modShader;
class modStatic;
class phBound;

class aiVehicleInstance : public lvlInstance
{
public:
    // ??0aiVehicleInstance@@QAE@PAVaiVehicleSpline@@PAD@Z
    ARTS_IMPORT aiVehicleInstance(aiVehicleSpline* arg1, char* arg2);

    // ?AttachEntity@aiVehicleInstance@@UAEPAVdgPhysEntity@@XZ
    ARTS_IMPORT virtual dgPhysEntity* AttachEntity();

    // ?Detach@aiVehicleInstance@@UAEXXZ
    ARTS_IMPORT virtual void Detach();

    // ?Draw@aiVehicleInstance@@UAEXH@Z
    ARTS_IMPORT virtual void Draw(i32 arg1);

    // ?DrawGlow@aiVehicleInstance@@UAEXXZ
    ARTS_IMPORT virtual void DrawGlow();

    // ?DrawPart@aiVehicleInstance@@QAEXAAVmodStatic@@ABVMatrix34@@PAVmodShader@@H@Z
    ARTS_IMPORT void DrawPart(modStatic& arg1, const Matrix34& arg2, modShader* arg3, i32 arg4);

    // ?DrawReflected@aiVehicleInstance@@UAEXM@Z
    ARTS_IMPORT virtual void DrawReflected(f32 arg1);

    // ?DrawShadow@aiVehicleInstance@@UAEXXZ
    ARTS_IMPORT virtual void DrawShadow();

    // ?DrawShadowMap@aiVehicleInstance@@UAEXXZ
    ARTS_IMPORT virtual void DrawShadowMap();

    // ?GetBound@aiVehicleInstance@@UAEPBVphBound@@H@Z
    ARTS_IMPORT virtual const phBound* GetBound(i32 arg1);

    // ?GetData@aiVehicleInstance@@QAEPAVaiVehicleData@@XZ
    ARTS_IMPORT aiVehicleData* GetData();

    // ?GetEntity@aiVehicleInstance@@UAEPAVdgPhysEntity@@XZ
    ARTS_IMPORT virtual dgPhysEntity* GetEntity();

    // ?GetMatrix@aiVehicleInstance@@UAEABVMatrix34@@AAV2@@Z
    ARTS_IMPORT virtual const Matrix34& GetMatrix(Matrix34& arg1);

    // ?GetPosition@aiVehicleInstance@@UAEABVVector3@@XZ
    ARTS_IMPORT virtual const Vector3& GetPosition();

    // ?Reset@aiVehicleInstance@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?SetColor@aiVehicleInstance@@QAEXXZ
    ARTS_IMPORT void SetColor();

    // ?SetMatrix@aiVehicleInstance@@UAEXABVMatrix34@@@Z
    ARTS_IMPORT virtual void SetMatrix(const Matrix34& arg1);

    // ?SizeOf@aiVehicleInstance@@UAEIXZ
    ARTS_IMPORT virtual u32 SizeOf();

private:
    // ?InitBreakable@aiVehicleInstance@@AAE_NPBD0H@Z
    ARTS_IMPORT bool InitBreakable(const char* arg1, const char* arg2, i32 arg3);
};

// ??_7aiVehicleInstance@@6B@
// vtable at 0x005B590C
// check_size(aiVehicleInstance, 0x3C); // size known, members are not - cannot verify
