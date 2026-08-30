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
class dgPath;
class gfxTexture;

class dgRoadDecalInstance : public lvlInstance
{
public:
    // ??0dgRoadDecalInstance@@QAE@PBDABVdgPath@@@Z
    ARTS_IMPORT dgRoadDecalInstance(const char* arg1, const dgPath& arg2);

    // ??1dgRoadDecalInstance@@QAE@XZ
    ARTS_IMPORT ~dgRoadDecalInstance();

    // ?GetPosition@dgRoadDecalInstance@@UAEABVVector3@@XZ
    ARTS_IMPORT virtual const Vector3& GetPosition();

    // ?GetMatrix@dgRoadDecalInstance@@UAEABVMatrix34@@AAV2@@Z
    ARTS_IMPORT virtual const Matrix34& GetMatrix(Matrix34& arg1);

    // ?SetMatrix@dgRoadDecalInstance@@UAEXABVMatrix34@@@Z
    ARTS_IMPORT virtual void SetMatrix(const Matrix34& arg1);

    // ?Draw@dgRoadDecalInstance@@UAEXH@Z
    ARTS_IMPORT virtual void Draw(i32 arg1);

    // ?DrawShadow@dgRoadDecalInstance@@UAEXXZ
    ARTS_IMPORT virtual void DrawShadow();

    // ?SizeOf@dgRoadDecalInstance@@UAEIXZ
    ARTS_IMPORT virtual u32 SizeOf();

public:
    // Members from 0x014; everything below that belongs to lvlInstance.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u32 VertCount; // 0x014
    Vector3* pVerts; // 0x018
    f32* pUVs; // 0x01C
    gfxTexture* pTexture; // 0x020
};

// ??_7dgRoadDecalInstance@@6B@
// vtable at 0x005B16FC
check_size(dgRoadDecalInstance, 0x24);
