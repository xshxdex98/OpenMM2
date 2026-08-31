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

#include "arts7/asnode.h"
#include "vector7/matrix44.h"

class Base_vtbl;
class Matrix34;
struct OppIconInfo;
class gfxTexture;

class mmIcons : public asNode
{
public:
    // ??0mmIcons@@QAE@XZ
    ARTS_IMPORT mmIcons();

    // ??1mmIcons@@UAE@XZ
    ARTS_IMPORT virtual ~mmIcons();

    // ?Cull@mmIcons@@UAEXXZ
    ARTS_IMPORT virtual void Cull();

    // ?Update@mmIcons@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Init@mmIcons@@QAEXPAVMatrix34@@MMH@Z
    ARTS_IMPORT void Init(Matrix34* arg1, f32 arg2, f32 arg3, i32 arg4);

    // ?RegisterOpponents@mmIcons@@QAEXPAUOppIconInfo@@HPAX@Z
    ARTS_IMPORT void RegisterOpponents(OppIconInfo* arg1, i32 arg2, void* h);

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    gfxTexture* IconTexture; // 0x018
    Matrix44 TriangleMtx; // 0x01C
    Matrix44 IconMtx; // 0x05C
    i32 OpponentCount; // 0x09C
    f32 MinDistance; // 0x0A0
    f32 MaxDistance; // 0x0A4
    Matrix34* Transform; // 0x0A8
    OppIconInfo* IconInfo; // 0x0AC
    f32 field_B0; // 0x0B0
    i32 IconMode; // 0x0B4
    i32 field_B8; // 0x0B8
};

// ??_7mmIcons@@6B@
// vtable at 0x005B0E1C
check_size(mmIcons, 0xBC);
