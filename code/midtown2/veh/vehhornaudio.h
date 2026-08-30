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
class vehHornAudioTiming;

class vehHornAudio
{
public:
    // ??0vehHornAudio@@QAE@PAV0@@Z
    ARTS_IMPORT vehHornAudio(vehHornAudio* arg1);

    // ??0vehHornAudio@@QAE@XZ
    ARTS_IMPORT vehHornAudio();

    // ??1vehHornAudio@@QAE@XZ
    ARTS_IMPORT ~vehHornAudio();

    // ?AddToHash@vehHornAudio@@SAXPADH@Z
    ARTS_IMPORT static void AddToHash(char* arg1, i32 arg2);

    // ?AllocTiming@vehHornAudio@@QAEXXZ
    ARTS_IMPORT void AllocTiming();

    // ?AssignSounds@vehHornAudio@@QAEXH@Z
    ARTS_IMPORT void AssignSounds(i32 arg1);

    // ?EchoOff@vehHornAudio@@QAEXXZ
    ARTS_IMPORT void EchoOff();

    // ?EchoOn@vehHornAudio@@QAEXM@Z
    ARTS_IMPORT void EchoOn(f32 arg1);

    // ?GetHornAudioTimingPtr@vehHornAudio@@QAEPAVvehHornAudioTiming@@H@Z
    ARTS_IMPORT vehHornAudioTiming* GetHornAudioTimingPtr(i32 arg1);

    // ?GetNumTimings@vehHornAudio@@QAEHXZ
    ARTS_IMPORT i32 GetNumTimings();

    // ?IsPlaying@vehHornAudio@@QAE_NXZ
    ARTS_IMPORT bool IsPlaying();

    // ?Load@vehHornAudio@@SAHPAPAV1@PAD1PAH@Z
    ARTS_IMPORT static i32 Load(vehHornAudio** arg1, char* arg2, char* arg3, i32* arg4);

    // ?PlayAvoidance@vehHornAudio@@QAE_NXZ
    ARTS_IMPORT bool PlayAvoidance();

    // ?PlayImpact@vehHornAudio@@QAEXM@Z
    ARTS_IMPORT void PlayImpact(f32 arg1);

    // ?Reset@vehHornAudio@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?UnAssignSounds@vehHornAudio@@QAEXXZ
    ARTS_IMPORT void UnAssignSounds();

    // ?Update@vehHornAudio@@QAEXXZ
    ARTS_IMPORT void Update();

    // ?UpdateDoppler@vehHornAudio@@QAEXMMM@Z
    ARTS_IMPORT void UpdateDoppler(f32 arg1, f32 arg2, f32 arg3);

    // ?UpdateEcho@vehHornAudio@@QAEXXZ
    ARTS_IMPORT void UpdateEcho();

private:
    // ?ReadCSV@vehHornAudio@@AAEHPAVStream@@@Z
    ARTS_IMPORT i32 ReadCSV(Stream* arg1);
};

// check_size(vehHornAudio, 0x0); // TODO: no layout in the IDB type library
