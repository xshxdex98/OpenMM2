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

#include "ascullable.h"

class Vector3;
class asSparkLut;

class asLineSparks : public asCullable
{
public:
    // ??0asLineSparks@@QAE@XZ
    ARTS_IMPORT asLineSparks();

    // ??1asLineSparks@@UAE@XZ
    ARTS_IMPORT virtual ~asLineSparks();

    // ?Draw@asLineSparks@@QAEXXZ
    ARTS_IMPORT void Draw();

    // ?Init@asLineSparks@@QAEXHPAD@Z
    ARTS_IMPORT void Init(i32 arg1, char* arg2);

    // ?RadialBlast@asLineSparks@@QAEXHAAVVector3@@0@Z
    ARTS_IMPORT void RadialBlast(i32 arg1, Vector3& arg2, Vector3& arg3);

    // ?Update@asLineSparks@@QAEXM@Z
    ARTS_IMPORT void Update(f32 arg1);

    // ?Update@asLineSparks@@QAEXXZ
    ARTS_IMPORT void Update();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i32 field_4; // 0x004
    asSparkLut* asSparkLut; // 0x008
    Vector3* StartPoints; // 0x00C
    Vector3* EndPoints; // 0x010
    Vector3* Velocity; // 0x014
    f32 field_18; // 0x018
    f32 field_1C; // 0x01C
    f32 field_20; // 0x020
    u8* ColorIndexthing; // 0x024
    u8* MaxLifeTime; // 0x028
    i32* Colors; // 0x02C
    u32 MaxSparkCount; // 0x030
    i32 SparkCount; // 0x034
    f32 field_38; // 0x038
    f32 field_3C; // 0x03C
    f32 field_40; // 0x040
    f32 field_44; // 0x044
    f32 field_48; // 0x048
    f32 field_4C; // 0x04C
    f32 field_50; // 0x050
    f32 CurrentTime; // 0x054
    f32 FrameSpeed; // 0x058
    f32 field_5C; // 0x05C
};

// ??_7asLineSparks@@6B@
// vtable at 0x005B1968
check_size(asLineSparks, 0x60);
