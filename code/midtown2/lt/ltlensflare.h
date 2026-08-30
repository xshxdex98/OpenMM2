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
    ARTS_IMPORT ltLensFlare(i32 arg1);

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
    i32 field_0; // 0x000
    i32 field_4; // 0x004
    i32 field_8; // 0x008
    i32 field_C; // 0x00C
    i32 field_10; // 0x010
    f32 field_14; // 0x014
    i32 field_18; // 0x018
    f32 field_1C; // 0x01C
};

check_size(ltLensFlare, 0x20);
