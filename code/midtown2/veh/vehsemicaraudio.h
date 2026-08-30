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

#include "vehcaraudio.h"
#include "vehengineaudio.h"
#include "vehsurfaceaudio.h"

class AudSoundBase;
class Vector3;
class vehCarDamage;
class vehCarSim;

class vehSemiCarAudio : public vehCarAudio
{
public:
    // ??0vehSemiCarAudio@@QAE@PAVvehCarSim@@PAVvehCarDamage@@PAD_N3@Z
    ARTS_IMPORT vehSemiCarAudio(vehCarSim* arg1, vehCarDamage* arg2, char* arg3, bool arg4, bool arg5);

    // ??1vehSemiCarAudio@@UAE@XZ
    ARTS_IMPORT virtual ~vehSemiCarAudio();

    // ?AssignSounds@vehSemiCarAudio@@UAEXXZ
    ARTS_IMPORT virtual void AssignSounds();

    // ?UnAssignSounds@vehSemiCarAudio@@UAEXH@Z
    ARTS_IMPORT virtual void UnAssignSounds(i32 label_id);

    // ?UpdateAudio@vehSemiCarAudio@@UAEXXZ
    ARTS_IMPORT virtual void UpdateAudio();

    // ?Update@vehSemiCarAudio@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?SetNon3DParams@vehSemiCarAudio@@UAEXXZ
    ARTS_IMPORT virtual void SetNon3DParams();

    // ?Set3DParams@vehSemiCarAudio@@UAEXXZ
    ARTS_IMPORT virtual void Set3DParams();

    // ?EchoOff@vehSemiCarAudio@@QAEXXZ
    ARTS_IMPORT void EchoOff();

    // ?EchoOn@vehSemiCarAudio@@QAEXXZ
    ARTS_IMPORT void EchoOn();

    // ?Init@vehSemiCarAudio@@QAEXPAVvehCarSim@@PAVvehCarDamage@@PAD_N33@Z
    ARTS_IMPORT void Init(vehCarSim* arg1, vehCarDamage* arg2, char* arg3, bool arg4, bool arg5, bool arg6);

    // ?Load@vehSemiCarAudio@@QAEXPAD@Z
    ARTS_IMPORT void Load(char* label_id);

    // ?RemoveFromManager@vehSemiCarAudio@@QAEXXZ
    ARTS_IMPORT void RemoveFromManager();

    // ?Reset@vehSemiCarAudio@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?Set2DPan@vehSemiCarAudio@@QAEXM@Z
    ARTS_IMPORT void Set2DPan(f32 arg1);

    // ?UpdateAirBlow@vehSemiCarAudio@@QAEXXZ
    ARTS_IMPORT void UpdateAirBlow();

    // ?UpdateAudio3D@vehSemiCarAudio@@QAEXXZ
    ARTS_IMPORT void UpdateAudio3D();

    // ?UpdateAudioNon3D@vehSemiCarAudio@@QAEXXZ
    ARTS_IMPORT void UpdateAudioNon3D();

    // ?UpdateEcho@vehSemiCarAudio@@QAEXXZ
    ARTS_IMPORT void UpdateEcho();

    // ?UpdateReverse@vehSemiCarAudio@@QAEXXZ
    ARTS_IMPORT void UpdateReverse();

public:
    // Members from 0x130; everything below that belongs to vehCarAudio.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    f32 ReverseVolume; // 0x130
    f32 AirBrakeVolume; // 0x134
    AudSoundBase* ReverseSound; // 0x138
    AudSoundBase* AirBrakeSound; // 0x13C
    i32 field_140; // 0x140
    i32 field_144; // 0x144
};

// ??_7vehSemiCarAudio@@6B@
// vtable at 0x005B31B4
check_size(vehSemiCarAudio, 0x148);
