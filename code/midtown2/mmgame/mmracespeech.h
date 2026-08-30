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

class mmRaceSpeech
{
public:
    // ??0mmRaceSpeech@@QAE@XZ
    ARTS_IMPORT mmRaceSpeech();

    // ??1mmRaceSpeech@@QAE@XZ
    ARTS_IMPORT ~mmRaceSpeech();

    // ?LoadCityInfo@mmRaceSpeech@@QAE_NPAD@Z
    ARTS_IMPORT bool LoadCityInfo(char* arg1);

    // ?LoadGroup@mmRaceSpeech@@QAE_NPADH@Z
    ARTS_IMPORT bool LoadGroup(char* arg1, i32 arg2);

    // ?LoadPreRace@mmRaceSpeech@@QAE_NPAD@Z
    ARTS_IMPORT bool LoadPreRace(char* arg1);

    // ?LoadResults@mmRaceSpeech@@QAE_NPAD@Z
    ARTS_IMPORT bool LoadResults(char* arg1);

    // ?LoadTextureUnlock@mmRaceSpeech@@QAE_NPAD@Z
    ARTS_IMPORT bool LoadTextureUnlock(char* arg1);

    // ?LoadVehicleUnlock@mmRaceSpeech@@QAE_NPAD@Z
    ARTS_IMPORT bool LoadVehicleUnlock(char* arg1);

    // ?PlayDamagePenalty@mmRaceSpeech@@QAEXXZ
    ARTS_IMPORT void PlayDamagePenalty();

    // ?PlayFinalCheckPoint@mmRaceSpeech@@QAEXXZ
    ARTS_IMPORT void PlayFinalCheckPoint();

    // ?PlayFinalLap@mmRaceSpeech@@QAEXXZ
    ARTS_IMPORT void PlayFinalLap();

    // ?PlayPreRace@mmRaceSpeech@@QAEXXZ
    ARTS_IMPORT void PlayPreRace();

    // ?PlayRaceProgress@mmRaceSpeech@@QAEXXZ
    ARTS_IMPORT void PlayRaceProgress();

    // ?PlayResults@mmRaceSpeech@@QAEXHH@Z
    ARTS_IMPORT void PlayResults(i32 arg1, i32 arg2);

    // ?PlayResultsMid@mmRaceSpeech@@QAEXXZ
    ARTS_IMPORT void PlayResultsMid();

    // ?PlayResultsPoor@mmRaceSpeech@@QAEXXZ
    ARTS_IMPORT void PlayResultsPoor();

    // ?PlayResultsWin@mmRaceSpeech@@QAEXXZ
    ARTS_IMPORT void PlayResultsWin();

    // ?PlayUnlockRace@mmRaceSpeech@@QAEXXZ
    ARTS_IMPORT void PlayUnlockRace();

    // ?PlayUnlockTexture@mmRaceSpeech@@QAEXXZ
    ARTS_IMPORT void PlayUnlockTexture();

    // ?PlayUnlockVehicle@mmRaceSpeech@@QAEXXZ
    ARTS_IMPORT void PlayUnlockVehicle();

private:
    // ?CheckRaceLoadSanity@mmRaceSpeech@@AAEXMH@Z
    ARTS_IMPORT void CheckRaceLoadSanity(f32 arg1, i32 arg2);

    // ?locstrnicmp@mmRaceSpeech@@AAEHPAD0@Z
    ARTS_IMPORT i32 locstrnicmp(char* arg1, char* arg2);

    // ?SetReadState@mmRaceSpeech@@AAE_NPADMPAH@Z
    ARTS_IMPORT bool SetReadState(char* arg1, f32 arg2, i32* arg3);
};

// check_size(mmRaceSpeech, 0x1); // size known, members are not - cannot verify
