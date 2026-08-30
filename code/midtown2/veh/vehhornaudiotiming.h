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

class vehHornAudioTiming
{
public:
    // ??0vehHornAudioTiming@@QAE@PAV0@@Z
    ARTS_IMPORT vehHornAudioTiming(vehHornAudioTiming* arg1);

    // ??0vehHornAudioTiming@@QAE@XZ
    ARTS_IMPORT vehHornAudioTiming();

    // ??1vehHornAudioTiming@@QAE@XZ
    ARTS_IMPORT ~vehHornAudioTiming();

    // ?AllocPlayPause@vehHornAudioTiming@@QAEXMM@Z
    ARTS_IMPORT void AllocPlayPause(f32 arg1, f32 arg2);

    // ?Play@vehHornAudioTiming@@QAEXPAVAudSoundBase@@@Z
    ARTS_IMPORT void Play(AudSoundBase* arg1);

    // ?Reset@vehHornAudioTiming@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?Stop@vehHornAudioTiming@@QAEXPAVAudSoundBase@@@Z
    ARTS_IMPORT void Stop(AudSoundBase* arg1);

    // ?Update@vehHornAudioTiming@@QAEXPAVAudSoundBase@@@Z
    ARTS_IMPORT void Update(AudSoundBase* arg1);
};

// check_size(vehHornAudioTiming, 0x0); // TODO: no layout in the IDB type library
