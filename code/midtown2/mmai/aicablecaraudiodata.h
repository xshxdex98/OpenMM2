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

class aiCableCarAudioData
{
public:
    // ??0aiCableCarAudioData@@QAE@XZ
    ARTS_IMPORT aiCableCarAudioData();

    // ??1aiCableCarAudioData@@QAE@XZ
    ARTS_IMPORT ~aiCableCarAudioData();

    // ?AssignSounds@aiCableCarAudioData@@QAEXH@Z
    ARTS_IMPORT void AssignSounds(i32 arg1);

    // ?GetCrossIntersectionPtr@aiCableCarAudioData@@QAEPAVAudSoundBase@@XZ
    ARTS_IMPORT AudSoundBase* GetCrossIntersectionPtr();

    // ?GetGetSpeedPtr@aiCableCarAudioData@@QAEPAVAudSoundBase@@XZ
    ARTS_IMPORT AudSoundBase* GetGetSpeedPtr();

    // ?GetGoPtr@aiCableCarAudioData@@QAEPAVAudSoundBase@@XZ
    ARTS_IMPORT AudSoundBase* GetGoPtr();

    // ?GetLoopPtr@aiCableCarAudioData@@QAEPAVAudSoundBase@@XZ
    ARTS_IMPORT AudSoundBase* GetLoopPtr();

    // ?GetStopPtr@aiCableCarAudioData@@QAEPAVAudSoundBase@@XZ
    ARTS_IMPORT AudSoundBase* GetStopPtr();

    // ?Stop@aiCableCarAudioData@@QAEXXZ
    ARTS_IMPORT void Stop();

    // ?UnAssignSounds@aiCableCarAudioData@@QAEXXZ
    ARTS_IMPORT void UnAssignSounds();

    // ?UpdatePlay@aiCableCarAudioData@@QAEXMMM@Z
    ARTS_IMPORT void UpdatePlay(f32 id, f32 text, f32 offsetX);

    // ?UpdateState@aiCableCarAudioData@@QAEXMM@Z
    ARTS_IMPORT void UpdateState(f32 arg1, f32 arg2);
};

// check_size(aiCableCarAudioData, 0x0); // TODO: no layout in the IDB type library
