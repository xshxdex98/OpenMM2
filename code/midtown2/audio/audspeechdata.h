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

class AudSoundBase;

class AudSpeechData
{
public:
    // ??0AudSpeechData@@QAE@PADMM_N00@Z
    ARTS_IMPORT AudSpeechData(char* arg1, f32 arg2, f32 arg3, bool arg4, char* arg5, char* arg6);

    // ??1AudSpeechData@@QAE@XZ
    ARTS_IMPORT ~AudSpeechData();

    // ?GetName@AudSpeechData@@QAEPADH@Z
    ARTS_IMPORT char* GetName(i32 arg1);

    // ?GetOneShot@AudSpeechData@@QAEPAVAudSoundBase@@H@Z
    ARTS_IMPORT AudSoundBase* GetOneShot(i32 arg1);

    // ?GetRandomName@AudSpeechData@@QAEPADXZ
    ARTS_IMPORT char* GetRandomName();

    // ?GetRandomOneShot@AudSpeechData@@QAEPAVAudSoundBase@@XZ
    ARTS_IMPORT AudSoundBase* GetRandomOneShot();

    // ?SetVolume@AudSpeechData@@QAEXM@Z
    ARTS_IMPORT void SetVolume(f32 arg1);

private:
    // ?LoadOneShots@AudSpeechData@@AAEXPAD0@Z
    ARTS_IMPORT void LoadOneShots(char* arg1, char* arg2);
};

// check_size(AudSpeechData, 0x0); // TODO: no layout in the IDB type library
