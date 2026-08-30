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

class mmCCSpeech;
class mmCNRSpeech;
class mmRaceSpeech;

class mmSpeechContainer
{
public:
    // ??0mmSpeechContainer@@QAE@HHPAD@Z
    ARTS_IMPORT mmSpeechContainer(i32 arg1, i32 arg2, char* arg3);

    // ??1mmSpeechContainer@@QAE@XZ
    ARTS_IMPORT ~mmSpeechContainer();

    // ?GetCCSpeechPtr@mmSpeechContainer@@QAEPAVmmCCSpeech@@XZ
    ARTS_IMPORT mmCCSpeech* GetCCSpeechPtr();

    // ?GetCNRSpeechPtr@mmSpeechContainer@@QAEPAVmmCNRSpeech@@XZ
    ARTS_IMPORT mmCNRSpeech* GetCNRSpeechPtr();

    // ?GetRaceSpeechPtr@mmSpeechContainer@@QAEPAVmmRaceSpeech@@XZ
    ARTS_IMPORT mmRaceSpeech* GetRaceSpeechPtr();

    // ?Stop@mmSpeechContainer@@QAEXXZ
    ARTS_IMPORT void Stop();

    // ?Update@mmSpeechContainer@@QAEXXZ
    ARTS_IMPORT void Update();

private:
    // ?InitCC@mmSpeechContainer@@AAEXHPAD@Z
    ARTS_IMPORT void InitCC(i32 arg1, char* arg2);

    // ?InitCNR@mmSpeechContainer@@AAEXPAD@Z
    ARTS_IMPORT void InitCNR(char* renderer);

    // ?InitRace@mmSpeechContainer@@AAEXHHPAD@Z
    ARTS_IMPORT void InitRace(i32 arg1, i32 arg2, char* arg3);
};

// check_size(mmSpeechContainer, 0x0); // TODO: no layout in the IDB type library
