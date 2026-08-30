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

#include "gfximage.h"

class IDirectDrawPalette;
struct IDirectDrawSurface7;
class gfxTextureCacheEntry;
class gfxTextureCachePool;

class gfxTexture
{
public:
    // ?Blit@gfxTexture@@QAEXHHPAV1@HHHH@Z
    ARTS_IMPORT void Blit(i32 arg1, i32 arg2, gfxTexture* arg3, i32 arg4, i32 arg5, i32 arg6, i32 arg7);

    // ?CalcMIPMapEnv@gfxTexture@@SAXHHAAG0@Z
    ARTS_EXPORT static void CalcMIPMapEnv(i32 arg1, i32 arg2, u16& arg3, u16& arg4);

    // ?Clone@gfxTexture@@QAEPAV1@XZ
    ARTS_IMPORT gfxTexture* Clone();

    // ?Create@gfxTexture@@SAPAV1@HHW4gfxImageFormat@gfxImage@@0H@Z
    ARTS_IMPORT static gfxTexture* Create(i32 arg1, i32 arg2, gfxImage::gfxImageFormat arg3, gfxImage::gfxImageFormat arg4, i32 arg5);

    // ?Create@gfxTexture@@SAPAV1@PAVgfxImage@@_N@Z
    ARTS_IMPORT static gfxTexture* Create(gfxImage* arg1, bool arg2);

    // ?CreateCubeMap@gfxTexture@@SAPAV1@HHQAPAV1@@Z
    ARTS_IMPORT static gfxTexture* CreateCubeMap(i32 arg1, i32 arg2, gfxTexture**const arg3);

    // ?CreateFromTexture@gfxTexture@@QAEPAV1@PBDHHHH@Z
    ARTS_EXPORT gfxTexture* CreateFromTexture(const char* arg1, i32 arg2, i32 arg3, i32 arg4, i32 arg5);

    // ?CreateRenderTarget@gfxTexture@@SAPAV1@HHII@Z
    ARTS_IMPORT static gfxTexture* CreateRenderTarget(i32 arg1, i32 arg2, u32 arg3, u32 arg4);

    // ?EnableCache@gfxTexture@@SAX_N@Z
    ARTS_IMPORT static void EnableCache(bool arg1);

    // ?EnableTrilinearMipMaping@gfxTexture@@SAX_N@Z
    ARTS_EXPORT static void EnableTrilinearMipMaping(bool arg1);

    // ?FindEntry@gfxTexture@@QAEPAUIDirectDrawSurface7@@XZ
    ARTS_IMPORT IDirectDrawSurface7* FindEntry();

    // ?GetColor@gfxTexture@@QBEIXZ
    ARTS_IMPORT u32 GetColor() const;

    // ?GetDefaultMIPMapEnv@gfxTexture@@SAXAAIAAM@Z
    ARTS_EXPORT static void GetDefaultMIPMapEnv(u32& arg1, f32& arg2);

    // ?GetMIPMapEnv@gfxTexture@@QAEXAAIAAM@Z
    ARTS_EXPORT void GetMIPMapEnv(u32& arg1, f32& arg2);

    // ?InitCache@gfxTexture@@SAXXZ
    ARTS_IMPORT static void InitCache();

    // ?KillAll@gfxTexture@@SAXXZ
    ARTS_IMPORT static void KillAll();

    // ?Load@gfxTexture@@QAEXPAVgfxImage@@@Z
    ARTS_IMPORT void Load(gfxImage* arg1);

    // ?MakeAllUnresident@gfxTexture@@SAXXZ
    ARTS_IMPORT static void MakeAllUnresident();

    // ?MakeResident@gfxTexture@@QAEXXZ
    ARTS_IMPORT void MakeResident();

    // ?MakeUnresident@gfxTexture@@QAEXXZ
    ARTS_EXPORT void MakeUnresident();

    // ?PreLoad@gfxTexture@@QAEXXZ
    ARTS_IMPORT void PreLoad();

    // ?SetDefaultMIPMapEnv@gfxTexture@@SAXIM@Z
    ARTS_EXPORT static void SetDefaultMIPMapEnv(u32 arg1, f32 arg2);

    // ?SetLODs@gfxTexture@@SAXXZ
    ARTS_IMPORT static void SetLODs();

    // ?SetMIPMapEnv@gfxTexture@@QAEXIM@Z
    ARTS_EXPORT void SetMIPMapEnv(u32 arg1, f32 arg2);

    // ?SetName@gfxTexture@@QAEXPBD@Z
    ARTS_IMPORT void SetName(const char* arg1);

    // ?SetTexEnv@gfxTexture@@QAEXI@Z
    ARTS_EXPORT void SetTexEnv(u32 arg1);

    // ?ShutdownCache@gfxTexture@@SAXXZ
    ARTS_IMPORT static void ShutdownCache();

    // ?sm_EnableSetLOD@gfxTexture@@2_NA
    ARTS_IMPORT static bool sm_EnableSetLOD;

    // ?sm_Allow32@gfxTexture@@2_NA
    ARTS_IMPORT static bool sm_Allow32;

public:
    // The destructor is private in midtown2.exe; this reaches it without
    // changing its mangled name. It does not free the storage.
    void Access_Destroy() { this->~gfxTexture(); }

public:
    static bool& Access_sm_UseInternalCache() { return sm_UseInternalCache; }
    static gfxTextureCachePool*& Access_sm_FirstPool() { return sm_FirstPool; }
    static u8& Access_sm_LOD() { return sm_LOD; }
    static gfxTexture*& Access_sm_First() { return sm_First; }
    static gfxTexture*& Access_sm_FirstActive() { return sm_FirstActive; }

private:
    // ??0gfxTexture@@AAE@XZ
    ARTS_IMPORT gfxTexture();

    // ??1gfxTexture@@AAE@XZ
    ARTS_IMPORT ~gfxTexture();

    // ?MarkFirstUse@gfxTexture@@AAEXXZ
    ARTS_EXPORT void MarkFirstUse();

    // ?MarkHigherUse@gfxTexture@@AAEXXZ
    ARTS_IMPORT void MarkHigherUse();

    // ?sm_UseInternalCache@gfxTexture@@0_NA
    ARTS_IMPORT static bool sm_UseInternalCache;

    // ?sm_FirstPool@gfxTexture@@0PAVgfxTextureCachePool@@A
    ARTS_IMPORT static gfxTextureCachePool* sm_FirstPool;

    // ?sm_LOD@gfxTexture@@0EA
    ARTS_IMPORT static u8 sm_LOD;

    // ?sm_First@gfxTexture@@0PAV1@A
    ARTS_IMPORT static gfxTexture* sm_First;

    // ?sm_FirstActive@gfxTexture@@0PAV1@A
    ARTS_IMPORT static gfxTexture* sm_FirstActive;

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u32 VglBindIndex; // 0x000
    i8* Name; // 0x004
    u16 Width; // 0x008
    u16 Height; // 0x00A
    u32 TexEnv; // 0x00C
    u32 DominantColor; // 0x010
    IDirectDrawSurface7* m_Surface; // 0x014
    IDirectDrawPalette* m_Palette; // 0x018
    gfxTextureCacheEntry* CacheEntry; // 0x01C
    gfxTextureCachePool* CachePool; // 0x020
    u32 m_RefCount; // 0x024
    gfxTexture* m_Next; // 0x028
    gfxTexture* NextLOD; // 0x02C
    u8 m_LOD; // 0x030
    u8 m_MaxLOD; // 0x031
    u8 EndPadding[2]; // 0x032
};

check_size(gfxTexture, 0x34);
