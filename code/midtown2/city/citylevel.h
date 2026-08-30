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

#include "lvl/lvllevel.h"
#include "lvl/lvlsdl.h"
#include "lvl/lvlsky.h"
#include "vector7/vector3.h"

class Matrix34;
class Vector4;
class asParticles;
struct cityRoomRec;
class gfxTexture;
class gfxViewport;
class lvlIntersection;
class lvlLevelBound;
class lvlSegment;
class pvsSphere;

class cityLevel : public lvlLevel
{
public:
    // ??0cityLevel@@QAE@XZ
    ARTS_IMPORT cityLevel();

    // ??1cityLevel@@UAE@XZ
    ARTS_IMPORT virtual ~cityLevel();

    // ?Collide@cityLevel@@UAE_NHAAVlvlSegment@@PAVlvlIntersection@@@Z
    ARTS_IMPORT virtual bool Collide(i32 arg1, lvlSegment& arg2, lvlIntersection* arg3);

    // ?Draw@cityLevel@@UAEXABVgfxViewport@@I@Z
    ARTS_IMPORT virtual void Draw(const gfxViewport& arg1, u32 arg2);

    // ?EnablePVS@cityLevel@@QAEX_N@Z
    ARTS_IMPORT void EnablePVS(bool arg1);

    // ?EnableSky@cityLevel@@QAEX_N@Z
    ARTS_IMPORT void EnableSky(bool arg1);

    // ?FindRoomId@cityLevel@@UAEHABVVector3@@H@Z
    ARTS_IMPORT virtual i32 FindRoomId(const Vector3& arg1, i32 arg2);

    // ?GetBound@cityLevel@@UAEPBVlvlLevelBound@@XZ
    ARTS_IMPORT virtual const lvlLevelBound* GetBound();

    // ?GetBoundSphere@cityLevel@@UAE_NAAVVector4@@H@Z
    ARTS_IMPORT virtual bool GetBoundSphere(Vector4& arg1, i32 arg2);

    // ?GetEnvMap@cityLevel@@UAEPAVgfxTexture@@HABVVector3@@AAM@Z
    ARTS_IMPORT virtual gfxTexture* GetEnvMap(i32 arg1, const Vector3& arg2, f32& arg3);

    // ?GetLastStartRoom@cityLevel@@QAEHXZ
    ARTS_IMPORT i32 GetLastStartRoom();

    // ?GetLightingIntensity@cityLevel@@UBEMABVVector3@@@Z
    ARTS_IMPORT virtual f32 GetLightingIntensity(const Vector3& arg1) const;

    // ?GetNeighborCount@cityLevel@@UAEHH@Z
    ARTS_IMPORT virtual i32 GetNeighborCount(i32 arg1);

    // ?GetNeighbors@cityLevel@@UAEHPAHH@Z
    ARTS_IMPORT virtual i32 GetNeighbors(i32* arg1, i32 arg2);

    // ?GetRoomPerimeter@cityLevel@@UAEHHQAVVector3@@H@Z
    ARTS_IMPORT virtual i32 GetRoomPerimeter(i32 arg1, Vector3*const arg2, i32 arg3);

    // ?GetTouchedNeighbors@cityLevel@@UAEHPAHHHABVVector4@@@Z
    ARTS_IMPORT virtual i32 GetTouchedNeighbors(i32* arg1, i32 arg2, i32 arg3, const Vector4& arg4);

    // ?GetTouchedNeighborsR@cityLevel@@QAEHPAHHHABVVector4@@H@Z
    ARTS_IMPORT i32 GetTouchedNeighborsR(i32* arg1, i32 arg2, i32 arg3, const Vector4& arg4, i32 arg5);

    // ?GetVisitList@cityLevel@@UAEHPAHHABVVector3@@1HH@Z
    ARTS_IMPORT virtual i32 GetVisitList(i32* arg1, i32 arg2, const Vector3& arg3, const Vector3& arg4, i32 arg5, i32 arg6);

    // ?GetWaterLevel@cityLevel@@UBEMH@Z
    ARTS_IMPORT virtual f32 GetWaterLevel(i32 arg1) const;

    // ?Load@cityLevel@@UAEXPBD@Z
    ARTS_IMPORT virtual void Load(const char* arg1);

    // ?LoadPath@cityLevel@@SAXPBDABVMatrix34@@_N@Z
    ARTS_IMPORT static void LoadPath(const char* arg1, const Matrix34& arg2, bool arg3);

    // ?LoadProp@cityLevel@@SAXHPBDABVMatrix34@@@Z
    ARTS_IMPORT static void LoadProp(i32 arg1, const char* arg2, const Matrix34& arg3);

    // ?PostDraw@cityLevel@@UAEXXZ
    ARTS_IMPORT virtual void PostDraw();

    // ?PreDraw@cityLevel@@UAEXXZ
    ARTS_IMPORT virtual void PreDraw();

    // ?SetObjectDetail@cityLevel@@UAEXH@Z
    ARTS_IMPORT virtual void SetObjectDetail(i32 arg1);

    // ?SetPtxHeight@cityLevel@@UAEXAAVasParticles@@@Z
    ARTS_IMPORT virtual void SetPtxHeight(asParticles& arg1);

    // ?SetupLighting@cityLevel@@SAXABVVector3@@@Z
    ARTS_IMPORT static void SetupLighting(const Vector3& arg1);

    // ?Update@cityLevel@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?sm_SDLVLowThresh@cityLevel@@2MA
    ARTS_IMPORT static f32 sm_SDLVLowThresh;

    // ?sm_SDLLowThresh@cityLevel@@2MA
    ARTS_IMPORT static f32 sm_SDLLowThresh;

    // ?sm_SDLMedThresh@cityLevel@@2MA
    ARTS_IMPORT static f32 sm_SDLMedThresh;

    // ?sm_LightQuality@cityLevel@@2HA
    ARTS_IMPORT static i32 sm_LightQuality;

protected:
    // ?DecompressPvs@cityLevel@@IAEXH@Z
    ARTS_IMPORT void DecompressPvs(i32 arg1);

    // ?DrawRooms@cityLevel@@IAEXABVgfxViewport@@IPBUcityRoomRec@@H@Z
    ARTS_IMPORT void DrawRooms(const gfxViewport& arg1, u32 arg2, const cityRoomRec* arg3, i32 arg4);

    // ?FullProbe@cityLevel@@IAEHABVVector3@@@Z
    ARTS_IMPORT i32 FullProbe(const Vector3& arg1);

    // ?InitFullProbe@cityLevel@@IAEXHH@Z
    ARTS_IMPORT void InitFullProbe(i32 arg1, i32 arg2);

    // ?IsInRoomCheckWarps@cityLevel@@IAE_NABVVector3@@H@Z
    ARTS_IMPORT bool IsInRoomCheckWarps(const Vector3& arg1, i32 arg2);

    // ?SetupPerRoomLighting@cityLevel@@IAEXH@Z
    ARTS_IMPORT void SetupPerRoomLighting(i32 arg1);

public:
    static i32& Access_sm_LastPvsRoom() { return sm_LastPvsRoom; }
    static lvlSky& Access_sm_Sky() { return sm_Sky; }
    static lvlSDL& Access_SDL() { return SDL; }
    static Vector3& Access_sm_Lighting() { return sm_Lighting; }
    static u8*& Access_sm_PvsBuffer() { return sm_PvsBuffer; }

private:
    // ?AddWarp@cityLevel@@CAXHH@Z
    ARTS_IMPORT static void AddWarp(i32 arg1, i32 arg2);

    // ?LoadPathSet@cityLevel@@CAXPBDPBD@Z
    ARTS_IMPORT static void LoadPathSet(const char* arg1, const char* arg2);

    // ?sm_LastPvsRoom@cityLevel@@0HA
    ARTS_IMPORT static i32 sm_LastPvsRoom;

    // ?sm_Sky@cityLevel@@0VlvlSky@@A
    ARTS_IMPORT static lvlSky sm_Sky;

    // ?SDL@cityLevel@@0VlvlSDL@@A
    ARTS_IMPORT static lvlSDL SDL;

    // ?sm_Lighting@cityLevel@@0VVector3@@A
    ARTS_IMPORT static Vector3 sm_Lighting;

    // ?sm_PvsBuffer@cityLevel@@0PAEA
    ARTS_IMPORT static u8* sm_PvsBuffer;

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i8 field_4[4]; // 0x004
    pvsSphere** pSpheres; // 0x008
    u32 PvsIndex; // 0x00C
    u8 field_10[8]; // 0x010
    i8* field_18; // 0x018
    u8 field_1C; // 0x01C
};

// ??_7cityLevel@@6B@
// vtable at 0x005B16A0
check_size(cityLevel, 0x20);
