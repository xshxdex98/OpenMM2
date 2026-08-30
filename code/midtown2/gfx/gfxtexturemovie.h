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

class gfxTexture;
class gfxTextureMovieClient;

class gfxTextureMovie
{
public:
    // ?AddClient@gfxTextureMovie@@QAEXPAPAVgfxTexture@@@Z
    ARTS_IMPORT void AddClient(gfxTexture** gfxTexture);

    // ?KillAll@gfxTextureMovie@@SAXXZ
    ARTS_IMPORT static void KillAll();

    // ?Update@gfxTextureMovie@@QAEXM@Z
    ARTS_IMPORT void Update(f32 elapsed);

    // ?UpdateAll@gfxTextureMovie@@SAXM@Z
    ARTS_IMPORT static void UpdateAll(f32 delta);

public:
    static gfxTextureMovie*& Access_sm_First() { return sm_First; }

protected:
    // ?sm_First@gfxTextureMovie@@1PAV1@A
    ARTS_IMPORT static gfxTextureMovie* sm_First;

private:
    // ??0gfxTextureMovie@@AAE@PBDH@Z
    ARTS_IMPORT gfxTextureMovie(const char* arg1, i32 arg2);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i8* TextureName; // 0x000
    gfxTextureMovie* PreviousMovie; // 0x004
    gfxTexture** FrameArray; // 0x008
    gfxTextureMovieClient* FirstClient; // 0x00C
    u8 FrameCount; // 0x010
    u8 FrameIndex; // 0x011
    u8 field_12; // 0x012
    u8 field_13; // 0x013
    f32 CurrentDelta; // 0x014
    f32 FrameTime; // 0x018
};

check_size(gfxTextureMovie, 0x1C);
