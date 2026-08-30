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
class gfxImage;

class gfxBitmap
{
public:
    // ?Clear@gfxBitmap@@QAEXI@Z
    ARTS_EXPORT void Clear(u32 fillColor);

    // ?Create@gfxBitmap@@SAPAV1@HH_N@Z
    ARTS_EXPORT static gfxBitmap* Create(i32 width, i32 height, bool arg3);

    // ?Create@gfxBitmap@@SAPAV1@PAVgfxImage@@_N@Z
    ARTS_EXPORT static gfxBitmap* Create(gfxImage* image, bool unused);

    // ?KillAll@gfxBitmap@@SAXXZ
    ARTS_IMPORT static void KillAll();

    // ?Load@gfxBitmap@@QAE_NPAVgfxImage@@@Z
    ARTS_EXPORT bool Load(gfxImage* image);

    // ?SetName@gfxBitmap@@QAEXPBD@Z
    ARTS_IMPORT void SetName(const char* name);

public:
    // The destructor is private in midtown2.exe; this reaches it without
    // changing its mangled name. It does not free the storage.
    void Access_Destroy() { this->~gfxBitmap(); }

public:
    static gfxBitmap*& Access_sm_First() { return sm_First; }

private:
    // ??0gfxBitmap@@AAE@HH@Z
    ARTS_IMPORT gfxBitmap(i32 width, i32 height);

    // ??1gfxBitmap@@AAE@XZ
    ARTS_IMPORT ~gfxBitmap();

    // ?sm_First@gfxBitmap@@0PAV1@A
    ARTS_IMPORT static gfxBitmap* sm_First;

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i8* Name; // 0x000
    u16 Width; // 0x004
    u16 Height; // 0x006
    IDirectDrawSurface7* Surface; // 0x008
    u32 RefCount; // 0x00C
    gfxBitmap* Prev; // 0x010
};

check_size(gfxBitmap, 0x14);
