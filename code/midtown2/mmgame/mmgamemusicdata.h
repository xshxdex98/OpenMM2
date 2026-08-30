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

class Stream;

class mmGameMusicData
{
public:
    // ?LoadMusic@mmGameMusicData@@UAE_NPAD@Z
    ARTS_IMPORT virtual bool LoadMusic(char* arg1);

    // ?GetNumDMusicChoiceGroups@mmGameMusicData@@QAEFPAVStream@@@Z
    ARTS_IMPORT i16 GetNumDMusicChoiceGroups(Stream* arg1);

    // ?Load@mmGameMusicData@@QAE_NPAD0@Z
    ARTS_IMPORT bool Load(char* arg1, char* arg2);

    // ?RandomizeNumber@mmGameMusicData@@QAEFF@Z
    ARTS_IMPORT i16 RandomizeNumber(i16 arg1);

private:
    // ?LoadAmbientSFX@mmGameMusicData@@AAE_NPAD@Z
    ARTS_IMPORT bool LoadAmbientSFX(char* pos_old);

    // ?LoadAmbientSFXSegments@mmGameMusicData@@AAE_NPAVStream@@F@Z
    ARTS_IMPORT bool LoadAmbientSFXSegments(Stream* stream, i16 arg2);
};

// ??_7mmGameMusicData@@6B@
// vtable at 0x005B0614
// check_size(mmGameMusicData, 0x1); // size known, members are not - cannot verify
