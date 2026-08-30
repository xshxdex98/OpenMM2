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

#include "audio/dmusicmanager.h"

class MMDMusicManager : public DMusicManager
{
public:
    // ??0MMDMusicManager@@QAE@XZ
    ARTS_IMPORT MMDMusicManager();

    // ??1MMDMusicManager@@QAE@XZ
    ARTS_IMPORT ~MMDMusicManager();

    // ?CreateSpeedRanges@MMDMusicManager@@QAEXH@Z
    ARTS_IMPORT void CreateSpeedRanges(i32 arg1);

    // ?EchoOff@MMDMusicManager@@QAEXXZ
    ARTS_IMPORT void EchoOff();

    // ?EchoOn@MMDMusicManager@@QAEXMM@Z
    ARTS_IMPORT void EchoOn(f32 renderer, f32 DrawList);

    // ?Init@MMDMusicManager@@QAEXHK@Z
    ARTS_IMPORT void Init(i32 arg1, ulong arg2);

    // ?MatchMusicToPlayerSpeed@MMDMusicManager@@QAEXM@Z
    ARTS_IMPORT void MatchMusicToPlayerSpeed(f32 arg1);

    // ?Reset@MMDMusicManager@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?SetSpeedEventRange@MMDMusicManager@@QAEXMMEH@Z
    ARTS_IMPORT void SetSpeedEventRange(f32 arg1, f32 arg2, u8 arg3, i32 arg4);

    // ?Update@MMDMusicManager@@QAEXXZ
    ARTS_IMPORT void Update();

    // ?UpdateAmbientSFX@MMDMusicManager@@QAEXXZ
    ARTS_IMPORT void UpdateAmbientSFX();

    // ?UpdateMusic@MMDMusicManager@@QAEXMH_N@Z
    ARTS_IMPORT void UpdateMusic(f32 renderer, i32 DrawList, bool cull_rect);

    // ?UpdateSeconds@MMDMusicManager@@QAEXXZ
    ARTS_IMPORT void UpdateSeconds();

    // ?UpdateSpeedEvents@MMDMusicManager@@QAEXXZ
    ARTS_IMPORT void UpdateSpeedEvents();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 pad_1[87]; // 0x001
};

check_size(MMDMusicManager, 0x58);
