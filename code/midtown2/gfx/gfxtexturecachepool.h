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

class gfxTexture;
class gfxTextureCacheEntry;

class gfxTextureCachePool
{
public:
    // ??0gfxTextureCachePool@@QAE@PAV0@@Z
    ARTS_IMPORT gfxTextureCachePool(gfxTextureCachePool* arg1);

    // ??1gfxTextureCachePool@@QAE@XZ
    ARTS_IMPORT ~gfxTextureCachePool();

    // ?AddSlot@gfxTextureCachePool@@QAE_NXZ
    ARTS_IMPORT bool AddSlot();

    // ?FindEntry@gfxTextureCachePool@@QAEXPAVgfxTexture@@@Z
    ARTS_IMPORT void FindEntry(gfxTexture* arg1);

    // ?Init@gfxTextureCachePool@@QAEXPAVgfxTexture@@@Z
    ARTS_IMPORT void Init(gfxTexture* arg1);

    // ?IsCompatibleWith@gfxTextureCachePool@@QAE_NPAVgfxTexture@@@Z
    ARTS_IMPORT bool IsCompatibleWith(gfxTexture* arg1);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u16 Width; // 0x000
    u16 Height; // 0x002
    u16 MipMapCount; // 0x004
    u16 TextureCount; // 0x006
    u16 EntryCount; // 0x008
    u16 HasNoSurface; // 0x00A
    gfxTextureCacheEntry* FirstEntry; // 0x00C
    gfxTextureCachePool* PrevPool; // 0x010
    DDPIXELFORMAT PixelFormat; // 0x014
};

check_size(gfxTextureCachePool, 0x34);
