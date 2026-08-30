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

class mmCCSpeech
{
public:
    // ??0mmCCSpeech@@QAE@XZ
    ARTS_IMPORT mmCCSpeech();

    // ??1mmCCSpeech@@UAE@XZ
    ARTS_IMPORT virtual ~mmCCSpeech();

    // ?LoadCheckPointIndexInfo@mmCCSpeech@@QAE_NPAD@Z
    ARTS_IMPORT bool LoadCheckPointIndexInfo(char* renderer);

    // ?LoadGroup@mmCCSpeech@@QAE_NPADH@Z
    ARTS_IMPORT bool LoadGroup(char* renderer, i32 DrawList);

    // ?LoadPreRace@mmCCSpeech@@QAE_NPAD@Z
    ARTS_IMPORT bool LoadPreRace(char* arg1);

    // ?LoadResults@mmCCSpeech@@QAE_NPAD@Z
    ARTS_IMPORT bool LoadResults(char* arg1);

    // ?LoadUnlock@mmCCSpeech@@QAE_NPAD@Z
    ARTS_IMPORT bool LoadUnlock(char* arg1);

    // ?PlayCheckPoint@mmCCSpeech@@QAEXHM@Z
    ARTS_IMPORT void PlayCheckPoint(i32 arg1, f32 arg2);

    // ?PlayPreRace@mmCCSpeech@@QAEXXZ
    ARTS_IMPORT void PlayPreRace();

    // ?PlayResults@mmCCSpeech@@QAEXH@Z
    ARTS_IMPORT void PlayResults(i32 arg1);

    // ?PlayResultsPoor@mmCCSpeech@@QAEXXZ
    ARTS_IMPORT void PlayResultsPoor();

    // ?PlayResultsWin@mmCCSpeech@@QAEXXZ
    ARTS_IMPORT void PlayResultsWin();

    // ?PlayUnlock@mmCCSpeech@@QAEXXZ
    ARTS_IMPORT void PlayUnlock();

    // ?SetSubPath@mmCCSpeech@@QAE_NPAD@Z
    ARTS_IMPORT bool SetSubPath(char* renderer);

private:
    // ?CheckRaceLoadSanity@mmCCSpeech@@AAEXMH@Z
    ARTS_IMPORT void CheckRaceLoadSanity(f32 arg1, i32 arg2);

    // ?locstrnicmp@mmCCSpeech@@AAEHPAD0@Z
    ARTS_IMPORT i32 locstrnicmp(char* arg1, char* arg2);

    // ?SetReadState@mmCCSpeech@@AAE_NPADMPAH@Z
    ARTS_IMPORT bool SetReadState(char* renderer, f32 DrawList, i32* cull_rect);
};

// check_size(mmCCSpeech, 0x0); // TODO: no layout in the IDB type library
