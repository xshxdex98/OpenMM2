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

class aiEngineAudio
{
public:
    // ??0aiEngineAudio@@QAE@PAV0@@Z
    ARTS_IMPORT aiEngineAudio(aiEngineAudio* arg1);

    // ??0aiEngineAudio@@QAE@XZ
    ARTS_IMPORT aiEngineAudio();

    // ??1aiEngineAudio@@QAE@XZ
    ARTS_IMPORT ~aiEngineAudio();

    // ?AddToHash@aiEngineAudio@@SAXPADH@Z
    ARTS_IMPORT static void AddToHash(char* arg1, i32 arg2);

    // ?AssignSounds@aiEngineAudio@@QAEXH@Z
    ARTS_IMPORT void AssignSounds(i32 arg1);

    // ?CalculatePitch@aiEngineAudio@@QAEXMMH@Z
    ARTS_IMPORT void CalculatePitch(f32 arg1, f32 arg2, i32 arg3);

    // ?EchoOff@aiEngineAudio@@QAEXXZ
    ARTS_IMPORT void EchoOff();

    // ?EchoOn@aiEngineAudio@@QAEXM@Z
    ARTS_IMPORT void EchoOn(f32 arg1);

    // ?Load@aiEngineAudio@@SAHPAPAV1@PAD1PAH@Z
    ARTS_IMPORT static i32 Load(aiEngineAudio** arg1, char* arg2, char* arg3, i32* arg4);

    // ?UnAssignSounds@aiEngineAudio@@QAEXXZ
    ARTS_IMPORT void UnAssignSounds();

    // ?UpdateDoppler@aiEngineAudio@@QAEXMMM@Z
    ARTS_IMPORT void UpdateDoppler(f32 arg1, f32 arg2, f32 arg3);

    // ?UpdateEcho@aiEngineAudio@@QAEXXZ
    ARTS_IMPORT void UpdateEcho();

private:
    // ?ReadCSV@aiEngineAudio@@AAEHPAVStream@@@Z
    ARTS_IMPORT i32 ReadCSV(Stream* arg1);
};

// check_size(aiEngineAudio, 0x0); // TODO: no layout in the IDB type library
