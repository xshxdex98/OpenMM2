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
class Vector3;
class vehEngineSampleWrapper;

class vehEngineAudio
{
public:
    // ??0vehEngineAudio@@QAE@XZ
    ARTS_IMPORT vehEngineAudio();

    // ??1vehEngineAudio@@UAE@XZ
    ARTS_IMPORT virtual ~vehEngineAudio();

    // ?AssignSounds@vehEngineAudio@@QAEXH@Z
    ARTS_IMPORT void AssignSounds(i32 arg1);

    // ?Disable3DMode@vehEngineAudio@@QAEXXZ
    ARTS_IMPORT void Disable3DMode();

    // ?EchoOff@vehEngineAudio@@QAEXXZ
    ARTS_IMPORT void EchoOff();

    // ?EchoOn@vehEngineAudio@@QAEXM@Z
    ARTS_IMPORT void EchoOn(f32 arg1);

    // ?Enable3DMode@vehEngineAudio@@QAEXPAVVector3@@@Z
    ARTS_IMPORT void Enable3DMode(Vector3* arg1);

    // ?GetSoundIndex@vehEngineAudio@@QAEHH@Z
    ARTS_IMPORT i32 GetSoundIndex(i32 arg1);

    // ?Load@vehEngineAudio@@QAEHPAVStream@@@Z
    ARTS_IMPORT i32 Load(Stream* arg1);

    // ?SetPan@vehEngineAudio@@QAEXM@Z
    ARTS_IMPORT void SetPan(f32 arg1);

    // ?Silence@vehEngineAudio@@QAEXH@Z
    ARTS_IMPORT void Silence(i32 arg1);

    // ?Stop@vehEngineAudio@@QAEXXZ
    ARTS_IMPORT void Stop();

    // ?UnAssignSounds@vehEngineAudio@@QAEXXZ
    ARTS_IMPORT void UnAssignSounds();

    // ?UpdateRPM@vehEngineAudio@@QAEXM@Z
    ARTS_IMPORT void UpdateRPM(f32 arg1);

    // ?UpdateRPM@vehEngineAudio@@QAEXMMMM@Z
    ARTS_IMPORT void UpdateRPM(f32 arg1, f32 arg2, f32 arg3, f32 arg4);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i32 SampleCount; // 0x000
    vehEngineSampleWrapper** Samples; // 0x004
};

check_size(vehEngineAudio, 0x8);
