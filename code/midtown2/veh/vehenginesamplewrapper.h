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

class vehEngineSampleWrapper
{
public:
    // ??0vehEngineSampleWrapper@@QAE@XZ
    ARTS_IMPORT vehEngineSampleWrapper();

    // ??1vehEngineSampleWrapper@@UAE@XZ
    ARTS_IMPORT virtual ~vehEngineSampleWrapper();

    // ?CalculatePitch@vehEngineSampleWrapper@@QAEMM@Z
    ARTS_IMPORT f32 CalculatePitch(f32 arg1);

    // ?CalculateVolume@vehEngineSampleWrapper@@QAEMM@Z
    ARTS_IMPORT f32 CalculateVolume(f32 arg1);

    // ?CalculateVolumeOld@vehEngineSampleWrapper@@QAEMM@Z
    ARTS_IMPORT f32 CalculateVolumeOld(f32 arg1);

    // ?DeallocateSoundPtr@vehEngineSampleWrapper@@QAEXXZ
    ARTS_IMPORT void DeallocateSoundPtr();

    // ?EchoOff@vehEngineSampleWrapper@@QAEXXZ
    ARTS_IMPORT void EchoOff();

    // ?EchoOn@vehEngineSampleWrapper@@QAEXM@Z
    ARTS_IMPORT void EchoOn(f32 arg1);

    // ?ParseCSVBuffer@vehEngineSampleWrapper@@QAEXPAD@Z
    ARTS_IMPORT void ParseCSVBuffer(char* arg1);

    // ?ParseCSVBufferOld@vehEngineSampleWrapper@@QAEXPAD@Z
    ARTS_IMPORT void ParseCSVBufferOld(char* arg1);

    // ?SetMaxVolume@vehEngineSampleWrapper@@QAEXM@Z
    ARTS_IMPORT void SetMaxVolume(f32 arg1);

    // ?SetPan@vehEngineSampleWrapper@@QAEXM@Z
    ARTS_IMPORT void SetPan(f32 arg1);

    // ?SetSoundPtr@vehEngineSampleWrapper@@QAEXPAVAudSoundBase@@@Z
    ARTS_IMPORT void SetSoundPtr(AudSoundBase* arg1);

    // ?Silence@vehEngineSampleWrapper@@QAEXH@Z
    ARTS_IMPORT void Silence(i32 arg1);

    // ?Stop@vehEngineSampleWrapper@@QAEXXZ
    ARTS_IMPORT void Stop();

    // ?UpdateRPM@vehEngineSampleWrapper@@QAEXM@Z
    ARTS_IMPORT void UpdateRPM(f32 arg1);

    // ?UpdateRPM@vehEngineSampleWrapper@@QAEXMMMM@Z
    ARTS_IMPORT void UpdateRPM(f32 arg1, f32 arg2, f32 arg3, f32 arg4);
};

// check_size(vehEngineSampleWrapper, 0x0); // TODO: no layout in the IDB type library
