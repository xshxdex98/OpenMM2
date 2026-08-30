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

class Matrix34;
class Matrix44;
class Vector3;
class Vector4;
class cltLight;
class dgPhysEntity;
class gfxViewport;
class phBound;

class lvlInstance
{
public:
    struct GeomTableEntry;

    // ??0lvlInstance@@QAE@XZ
    ARTS_IMPORT lvlInstance();

    // ??1lvlInstance@@QAE@XZ
    ARTS_IMPORT ~lvlInstance();

    // ?Reset@lvlInstance@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?IsVisible@lvlInstance@@UAEHABVgfxViewport@@@Z
    ARTS_IMPORT virtual i32 IsVisible(const gfxViewport& viewport);

    // ?SetVariant@lvlInstance@@UAEXH@Z
    ARTS_IMPORT virtual void SetVariant(i32 arg1);

    // ?GetRadius@lvlInstance@@UAE?BMXZ
    ARTS_IMPORT virtual const f32 GetRadius();

    // ?GetEntity@lvlInstance@@UAEPAVdgPhysEntity@@XZ
    ARTS_IMPORT virtual dgPhysEntity* GetEntity();

    // ?AttachEntity@lvlInstance@@UAEPAVdgPhysEntity@@XZ
    ARTS_IMPORT virtual dgPhysEntity* AttachEntity();

    // ?GetVelocity@lvlInstance@@UAEABVVector3@@XZ
    ARTS_IMPORT virtual const Vector3& GetVelocity();

    // ?Detach@lvlInstance@@UAEXXZ
    ARTS_IMPORT virtual void Detach();

    // ?DrawShadow@lvlInstance@@UAEXXZ
    ARTS_IMPORT virtual void DrawShadow();

    // ?DrawShadowMap@lvlInstance@@UAEXXZ
    ARTS_IMPORT virtual void DrawShadowMap();

    // ?DrawGlow@lvlInstance@@UAEXXZ
    ARTS_IMPORT virtual void DrawGlow();

    // ?DrawReflected@lvlInstance@@UAEXM@Z
    ARTS_IMPORT virtual void DrawReflected(f32 arg1);

    // ?DrawReflectedParts@lvlInstance@@UAEXH@Z
    ARTS_IMPORT virtual void DrawReflectedParts(i32 arg1);

    // ?Init@lvlInstance@@UAEHPBDABVMatrix34@@H@Z
    ARTS_IMPORT virtual i32 Init(const char* arg1, const Matrix34& arg2, i32 arg3);

    // ?IsLandmark@lvlInstance@@UAE_NXZ
    ARTS_IMPORT virtual bool IsLandmark();

    // ?IsCollidable@lvlInstance@@UAE_NXZ
    ARTS_IMPORT virtual bool IsCollidable();

    // ?IsTerrainCollidable@lvlInstance@@UAE_NXZ
    ARTS_IMPORT virtual bool IsTerrainCollidable();

    // ?GetNumLightSources@lvlInstance@@UAEHXZ
    ARTS_IMPORT virtual i32 GetNumLightSources();

    // ?GetLightInfo@lvlInstance@@UAEXHPAVcltLight@@@Z
    ARTS_IMPORT virtual void GetLightInfo(i32 arg1, cltLight* arg2);

    // ?SetupGfxLights@lvlInstance@@UAEHABVMatrix34@@@Z
    ARTS_IMPORT virtual i32 SetupGfxLights(const Matrix34& arg1);

    // ?GetBound@lvlInstance@@UAEPBVphBound@@H@Z
    ARTS_IMPORT virtual const phBound* GetBound(i32 arg1);

    // ?AddGeom@lvlInstance@@QAEHPBD0H@Z
    ARTS_IMPORT i32 AddGeom(const char* arg1, const char* arg2, i32 flags);

    // ?AddSphere@lvlInstance@@SAHM@Z
    ARTS_IMPORT static i32 AddSphere(f32 arg1);

    // ?BeginGeom@lvlInstance@@QAE_NPBD0H@Z
    ARTS_IMPORT bool BeginGeom(const char* name, const char* arg2, i32 flags);

    // ?DrawPhysics@lvlInstance@@UAEXABVVector3@@@Z
    ARTS_IMPORT virtual void DrawPhysics(const Vector3& atlas);

    // ?EndGeom@lvlInstance@@QAEXXZ
    ARTS_IMPORT void EndGeom();

    // ?GetBoundSphere@lvlInstance@@QAEAAVVector4@@AAV2@@Z
    ARTS_IMPORT Vector4& GetBoundSphere(Vector4& arg1);

    // ?InitBoundTerrain@lvlInstance@@QAE_NPBD@Z
    ARTS_IMPORT bool InitBoundTerrain(const char* arg1);

    // ?InitBoundTerrainLocal@lvlInstance@@QAE_NPBD@Z
    ARTS_IMPORT bool InitBoundTerrainLocal(const char* arg1);

    // ?InitGhost@lvlInstance@@QAEHPBDABVMatrix34@@@Z
    ARTS_IMPORT i32 InitGhost(const char* arg1, const Matrix34& arg2);

    // ?InitGhostBound@lvlInstance@@QAE_NPAVphBound@@PBVVector3@@H@Z
    ARTS_IMPORT bool InitGhostBound(phBound* bound, const Vector3* arg2, i32 arg3);

    // ?LoadBoundOnLastEntry@lvlInstance@@QAE_NPBD@Z
    ARTS_IMPORT bool LoadBoundOnLastEntry(const char* arg1);

    // ?NeedGhostBound@lvlInstance@@QAE_NPBVVector3@@H@Z
    ARTS_IMPORT bool NeedGhostBound(const Vector3* arg1, i32 arg2);

    // ??3lvlInstance@@SAXPAX@Z
    ARTS_IMPORT static void operator delete(void* arg1);

    // ??2lvlInstance@@SAPAXI@Z
    ARTS_IMPORT static void* operator new(u32 size);

    // ?Optimize@lvlInstance@@QAEXH@Z
    ARTS_IMPORT void Optimize(i32 arg1);

    // ?PreLoadShader@lvlInstance@@QAEXH@Z
    ARTS_IMPORT void PreLoadShader(i32 arg1);

    // ?ResetAll@lvlInstance@@SAXXZ
    ARTS_IMPORT static void ResetAll();

    // ?ResetInstanceHeap@lvlInstance@@SAXXZ
    ARTS_IMPORT static void ResetInstanceHeap();

    // ?SetShadowBillboardMtx@lvlInstance@@SAXAAVMatrix44@@@Z
    ARTS_IMPORT static void SetShadowBillboardMtx(Matrix44& arg1);

    // ?sm_ObjNoDrawThresh@lvlInstance@@2MA
    ARTS_IMPORT static f32 sm_ObjNoDrawThresh;

    // ?sm_ObjMaxThresh@lvlInstance@@2MA
    ARTS_IMPORT static f32 sm_ObjMaxThresh;

    // ?sm_ObjVLowThresh@lvlInstance@@2MA
    ARTS_IMPORT static f32 sm_ObjVLowThresh;

    // ?sm_ObjLowThresh@lvlInstance@@2MA
    ARTS_IMPORT static f32 sm_ObjLowThresh;

    // ?sm_ObjMedThresh@lvlInstance@@2MA
    ARTS_IMPORT static f32 sm_ObjMedThresh;

protected:
    // ?CreateTempBounds@lvlInstance@@KAXXZ
    ARTS_IMPORT static void CreateTempBounds();

    // ?DeleteTempBounds@lvlInstance@@KAXXZ
    ARTS_IMPORT static void DeleteTempBounds();

    // ?GetGeomSet@lvlInstance@@KAHPBD0H@Z
    ARTS_IMPORT static i32 GetGeomSet(const char* arg1, const char* arg2, i32 flags);

public:
    static bool& Access_sm_EnableShadows() { return sm_EnableShadows; }

private:
    // ?sm_EnableShadows@lvlInstance@@0_NA
    ARTS_IMPORT static bool sm_EnableShadows;

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 field_4; // 0x004
    u8 ShaderSet; // 0x005
    u16 field_6; // 0x006
    u16 Flags; // 0x008
    u16 GeomSet; // 0x00A
    u32 field_C; // 0x00C
    lvlInstance* Next; // 0x010
};

// ??_7lvlInstance@@6B@
// vtable at 0x005B1988
check_size(lvlInstance, 0x14);
