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

class Vector2;
struct asMeshCardVertex;
struct asSparkPos;

class asMeshCardInfo
{
public:
    // ?Draw@asMeshCardInfo@@QAEXPBUasSparkPos@@H@Z
    ARTS_IMPORT void Draw(const asSparkPos* sparks, i32 sparkCount);

    // ?DrawShadows@asMeshCardInfo@@QAEXPBUasSparkPos@@H@Z
    ARTS_IMPORT void DrawShadows(const asSparkPos* arg1, i32 arg2);

    // ?Init@asMeshCardInfo@@QAEXHPAUasMeshCardVertex@@HHH@Z
    ARTS_IMPORT void Init(i32 arg1, asMeshCardVertex* arg2, i32 arg3, i32 arg4, i32 arg5);

    // ?sQuad@asMeshCardInfo@@2PAUasMeshCardVertex@@A
    ARTS_IMPORT static asMeshCardVertex* sQuad;

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i32 Count; // 0x000
    u32 PositionsIndex; // 0x004
    Vector2* Positions; // 0x008
    Vector2* TexCoords; // 0x00C
};

check_size(asMeshCardInfo, 0x10);
