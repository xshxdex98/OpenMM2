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

struct IDirectDrawSurface7;
class gfxTexture;

class gfxTextureCacheEntry
{
public:
    // ??1gfxTextureCacheEntry@@QAE@XZ
    ARTS_IMPORT ~gfxTextureCacheEntry();

    // ?Evict@gfxTextureCacheEntry@@QAEXXZ
    ARTS_IMPORT void Evict();

    // ?Lease@gfxTextureCacheEntry@@QAEXPAVgfxTexture@@@Z
    ARTS_IMPORT void Lease(gfxTexture* texture);

public:
    static u32& Access_sm_CurrentTime() { return sm_CurrentTime; }

private:
    // ??0gfxTextureCacheEntry@@AAE@PAUIDirectDrawSurface7@@PAV0@@Z
    ARTS_IMPORT gfxTextureCacheEntry(IDirectDrawSurface7* arg1, gfxTextureCacheEntry* arg2);

    // ?sm_CurrentTime@gfxTextureCacheEntry@@0IA
    ARTS_IMPORT static u32 sm_CurrentTime;

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    gfxTexture* Texture; // 0x000
    IDirectDrawSurface7* Surface; // 0x004
    u32 LastAccessTime; // 0x008
    gfxTextureCacheEntry* PrevEntry; // 0x00C
};

check_size(gfxTextureCacheEntry, 0x10);
