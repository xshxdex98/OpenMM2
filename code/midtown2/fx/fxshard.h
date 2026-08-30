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

#include "vector7/matrix34.h"
#include "vector7/vector3.h"

class modShader;

class fxShard
{
public:
    // ??0fxShard@@QAE@XZ
    ARTS_IMPORT fxShard();

    // ??1fxShard@@QAE@XZ
    ARTS_IMPORT ~fxShard();

    // ?AddShard@fxShard@@QAEXVVector3@@00M@Z
    ARTS_IMPORT void AddShard(Vector3 arg1, Vector3 arg2, Vector3 arg3, f32 arg4);

    // ?Draw@fxShard@@QAEXPAVmodShader@@@Z
    ARTS_IMPORT void Draw(modShader* c);

    // ?Update@fxShard@@QAEXXZ
    ARTS_IMPORT void Update();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    Vector3 Velocity; // 0x000
    u8 pad_4[8]; // 0x004
    Vector3 RotationAxis; // 0x00C
    u8 pad_10[8]; // 0x010
    f32 AliveTime; // 0x018
    f32 RotationSpeed; // 0x01C
    Matrix34 Matrix; // 0x020
    u8 pad_24[44]; // 0x024
    f32 TexUCoord; // 0x050
    f32 TexVCoord; // 0x054
    i32 dword_58; // 0x058
};

check_size(fxShard, 0x5C);
