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

class mmCNRSpeech
{
public:
    // ??0mmCNRSpeech@@QAE@XZ
    ARTS_IMPORT mmCNRSpeech();

    // ??1mmCNRSpeech@@QAE@XZ
    ARTS_IMPORT ~mmCNRSpeech();

    // ?LoadGroup@mmCNRSpeech@@QAE_NPADH@Z
    ARTS_IMPORT bool LoadGroup(char* arg1, i32 arg2);

    // ?Play@mmCNRSpeech@@QAEXH@Z
    ARTS_IMPORT void Play(i32 arg1);

    // ?Play@mmCNRSpeech@@QAEXPAD@Z
    ARTS_IMPORT void Play(char* _First);

private:
    // ?locstrnicmp@mmCNRSpeech@@AAEHPAD0@Z
    ARTS_IMPORT i32 locstrnicmp(char* arg1, char* arg2);

    // ?SetReadState@mmCNRSpeech@@AAE_NPAD@Z
    ARTS_IMPORT bool SetReadState(char* _Count);
};

// check_size(mmCNRSpeech, 0x1); // size known, members are not - cannot verify
