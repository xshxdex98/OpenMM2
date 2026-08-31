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
class fxTexelDamage;
class ltLight;
class modShader;
class modStatic;
class vehBreakableMgr;
class vehCar;

class vehCarModel : public lvlInstance
{
public:
    // ??0vehCarModel@@QAE@XZ
    ARTS_IMPORT vehCarModel();

    // ??1vehCarModel@@QAE@XZ
    ARTS_IMPORT ~vehCarModel();

    // ?Reset@vehCarModel@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?GetPosition@vehCarModel@@UAEABVVector3@@XZ
    ARTS_IMPORT virtual const Vector3& GetPosition();

    // ?GetMatrix@vehCarModel@@UAEABVMatrix34@@AAV2@@Z
    ARTS_IMPORT virtual const Matrix34& GetMatrix(Matrix34& arg1);

    // ?SetMatrix@vehCarModel@@UAEXABVMatrix34@@@Z
    ARTS_IMPORT virtual void SetMatrix(const Matrix34& arg1);

    // ?GetEntity@vehCarModel@@UAEPAVdgPhysEntity@@XZ
    ARTS_IMPORT virtual dgPhysEntity* GetEntity();

    // ?AttachEntity@vehCarModel@@UAEPAVdgPhysEntity@@XZ
    ARTS_EXPORT virtual dgPhysEntity* AttachEntity();

    // ?GetVelocity@vehCarModel@@UAEABVVector3@@XZ
    ARTS_IMPORT virtual const Vector3& GetVelocity();

    // ?Draw@vehCarModel@@UAEXH@Z
    ARTS_IMPORT virtual void Draw(i32 lod);

    // ?DrawShadow@vehCarModel@@UAEXXZ
    ARTS_IMPORT virtual void DrawShadow();

    // ?DrawShadowMap@vehCarModel@@UAEXXZ
    ARTS_IMPORT virtual void DrawShadowMap();

    // ?DrawGlow@vehCarModel@@UAEXXZ
    ARTS_IMPORT virtual void DrawGlow();

    // ?DrawReflected@vehCarModel@@UAEXM@Z
    ARTS_EXPORT virtual void DrawReflected(f32 arg1);

    // ?SizeOf@vehCarModel@@UAEIXZ
    ARTS_EXPORT virtual u32 SizeOf();

    // ?BreakElectrics@vehCarModel@@QAEXABVVector3@@@Z
    ARTS_IMPORT void BreakElectrics(const Vector3& arg1);

    // ?ClearDamage@vehCarModel@@QAEXXZ
    ARTS_IMPORT void ClearDamage();

    // ?DrawHeadlights@vehCarModel@@QAEX_N@Z
    ARTS_IMPORT void DrawHeadlights(bool arg1);

    // ?DrawPart@vehCarModel@@QAEXHHABVMatrix34@@PAVmodShader@@@Z
    ARTS_IMPORT void DrawPart(i32 arg1, i32 arg2, const Matrix34& arg3, modShader* arg4);

    // ?DrawPart@vehCarModel@@QAEXPAVmodStatic@@ABVMatrix34@@PAVmodShader@@@Z
    ARTS_IMPORT void DrawPart(modStatic* arg1, const Matrix34& arg2, modShader* arg3);

    // ?EjectOneshot@vehCarModel@@QAEXXZ
    ARTS_IMPORT void EjectOneshot();

    // ?GetTrailerHitch@vehCarModel@@QAE_NPAVVector3@@@Z
    ARTS_IMPORT bool GetTrailerHitch(Vector3* arg1);

    // ?GetVisible@vehCarModel@@QAE_NXZ
    ARTS_IMPORT bool GetVisible();

    // ?Init@vehCarModel@@QAEXPAVvehCar@@PBDH@Z
    ARTS_IMPORT void Init(vehCar* arg1, const char* arg2, i32 arg3);

    // ?InitBound@vehCarModel@@QAEXPBD_N@Z
    ARTS_IMPORT void InitBound(const char* arg1, bool arg2);

    // ?SetVisible@vehCarModel@@QAEX_N@Z
    ARTS_IMPORT void SetVisible(bool arg1);

protected:
    // ?GetSurfaceColor@vehCarModel@@IAE_NPAVmodStatic@@AAVVector3@@@Z
    ARTS_IMPORT bool GetSurfaceColor(modStatic* arg_0, Vector3& colorOut);

    // ?InitBreakable@vehCarModel@@IAE_NPAVvehBreakableMgr@@PBD1HH@Z
    ARTS_IMPORT bool InitBreakable(vehBreakableMgr* arg1, const char* arg2, const char* arg3, i32 arg4, i32 arg5);

    // ?InitSirenLight@vehCarModel@@IAEXPBD0H@Z
    ARTS_IMPORT void InitSirenLight(const char* arg1, const char* arg2, i32 arg3);

public:
    // Members from 0x014; everything below that belongs to lvlInstance.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 field_14; // 0x014
    u8 field_15; // 0x015
    u8 field_16; // 0x016
    u8 field_17; // 0x017
    vehCar* pCar; // 0x018
    void* field_1C; // 0x01C
    fxTexelDamage* pTexelDamage; // 0x020
    u8 ShaderSet; // 0x024
    u32 field_28; // 0x028
    f32 field_2C; // 0x02C
    Vector3 field_30; // 0x030
    Vector3 field_3C; // 0x03C
    Vector3 TrailerHitch; // 0x048
    u8 field_54[76]; // 0x054
    vehBreakableMgr* BreakableMgr; // 0x0A0
    u8 field_A4; // 0x0A4
    u8 field_A5; // 0x0A5
    u8 field_A6; // 0x0A6
    u8 field_A7; // 0x0A7
    u32 field_A8; // 0x0A8
    vehBreakableMgr* BreakableMgr2; // 0x0AC
    ltLight* Lights; // 0x0B0
    u32 field_B4; // 0x0B4
    u32 field_B8; // 0x0B8
    u32 field_BC; // 0x0BC
    u32 field_C0; // 0x0C0
    u32 field_C4; // 0x0C4
    u32 field_C8; // 0x0C8
};

// ??_7vehCarModel@@6B@
// vtable at 0x005B2CB0
check_size(vehCarModel, 0xCC);
