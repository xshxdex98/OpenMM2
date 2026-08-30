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

class Matrix34;
class Vector3;
class gfxTexture;

class lvlTrackManager
{
public:
    // ??0lvlTrackManager@@QAE@XZ
    ARTS_IMPORT lvlTrackManager();

    // ??1lvlTrackManager@@QAE@XZ
    ARTS_IMPORT ~lvlTrackManager();

    // ?Draw@lvlTrackManager@@QAEXXZ
    ARTS_IMPORT void Draw();

    // ?Init@lvlTrackManager@@QAEXABVMatrix34@@MH@Z
    ARTS_IMPORT void Init(const Matrix34& arg1, f32 arg2, i32 arg3);

    // ?Reset@lvlTrackManager@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?Update@lvlTrackManager@@QAEXABVVector3@@0PAVgfxTexture@@@Z
    ARTS_IMPORT void Update(const Vector3& info, const Vector3& unicode_codepoint, gfxTexture* offset);

private:
    // ?AddVertex@lvlTrackManager@@AAEXPAVgfxTexture@@MABVVector3@@1@Z
    ARTS_IMPORT void AddVertex(gfxTexture* arg1, f32 arg2, const Vector3& arg3, const Vector3& arg4);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i32 MaxNumTracks; // 0x000
    i32 field_4; // 0x004
    i32 field_8; // 0x008
    f32 HalfWheelWidth; // 0x00C
    f32 OneDivWheelWidth; // 0x010
    i32 TracksData; // 0x014
    i32 field_18; // 0x018
    i32 field_1C; // 0x01C
    i32 field_20; // 0x020
    i32 field_24; // 0x024
    i32 field_28; // 0x028
    i32 field_2C; // 0x02C
    i32 field_30; // 0x030
    i32 field_34; // 0x034
    i32 field_38; // 0x038
    i32 field_3C; // 0x03C
    i32 field_40; // 0x040
    i32 field_44; // 0x044
    i32 field_48; // 0x048
    i32 field_4C; // 0x04C
    Matrix34* WheelMatrixPtr; // 0x050
    i32 field_54; // 0x054
};

check_size(lvlTrackManager, 0x58);
