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
    ARTS_IMPORT void Draw(modShader* arg1);

    // ?Update@fxShard@@QAEXXZ
    ARTS_IMPORT void Update();
};

// check_size(fxShard, 0x5C); // size known, members are not - cannot verify
