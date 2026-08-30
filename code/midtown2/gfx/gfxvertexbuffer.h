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

struct IDirect3DVertexBuffer7;

class gfxVertexBuffer
{
public:
    // ??0gfxVertexBuffer@@QAE@II@Z
    ARTS_IMPORT gfxVertexBuffer(u32 arg1, u32 arg2);

    // ??0gfxVertexBuffer@@QAE@PAUIDirect3DVertexBuffer7@@@Z
    ARTS_IMPORT gfxVertexBuffer(IDirect3DVertexBuffer7* arg1);

    // ??1gfxVertexBuffer@@QAE@XZ
    ARTS_IMPORT ~gfxVertexBuffer();

    // ?KillAll@gfxVertexBuffer@@SAXXZ
    ARTS_IMPORT static void KillAll();

    // ?RestoreAll@gfxVertexBuffer@@SAXXZ
    ARTS_IMPORT static void RestoreAll();

    // ?sm_First@gfxVertexBuffer@@2PAU1@A
    ARTS_IMPORT static gfxVertexBuffer* sm_First;

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    gfxVertexBuffer* Prev; // 0x000
    void* m_Verts; // 0x004
    u16 VertCount; // 0x008
    u16 m_ElementSize; // 0x00A
    u32 m_FVF; // 0x00C
};

check_size(gfxVertexBuffer, 0x10);
