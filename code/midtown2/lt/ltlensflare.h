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

class Vector3;

class ltLensFlare
{
public:
    // ??0ltLensFlare@@QAE@H@Z
    ARTS_IMPORT ltLensFlare(i32 _Src);

    // ??1ltLensFlare@@QAE@XZ
    ARTS_IMPORT ~ltLensFlare();

    // ?Draw@ltLensFlare@@QAEXAAVVector3@@0M@Z
    ARTS_IMPORT void Draw(Vector3& arg1, Vector3& arg2, f32 arg3);

    // ?DrawBegin@ltLensFlare@@QAEXXZ
    ARTS_IMPORT void DrawBegin();

    // ?DrawEnd@ltLensFlare@@QAEXXZ
    ARTS_IMPORT void DrawEnd();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    unsigned __int8[32] _buffer; // 0x000
};

check_size(ltLensFlare, 0x20);
