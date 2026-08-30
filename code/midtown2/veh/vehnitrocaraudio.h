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

class vehNitroCarAudio : public vehCarAudio
{
public:
    // ??0vehNitroCarAudio@@QAE@PAVvehCarSim@@PAVvehCarDamage@@PAD_N3@Z
    ARTS_IMPORT vehNitroCarAudio(vehCarSim* arg1, vehCarDamage* arg2, char* arg3, bool arg4, bool arg5);

    // ??1vehNitroCarAudio@@UAE@XZ
    ARTS_IMPORT virtual ~vehNitroCarAudio();

    // ?AssignSounds@vehNitroCarAudio@@UAEXXZ
    ARTS_IMPORT virtual void AssignSounds();

    // ?UnAssignSounds@vehNitroCarAudio@@UAEXH@Z
    ARTS_IMPORT virtual void UnAssignSounds(i32 arg1);

    // ?UpdateAudio@vehNitroCarAudio@@UAEXXZ
    ARTS_IMPORT virtual void UpdateAudio();

    // ?Update@vehNitroCarAudio@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?SetNon3DParams@vehNitroCarAudio@@UAEXXZ
    ARTS_IMPORT virtual void SetNon3DParams();

    // ?Set3DParams@vehNitroCarAudio@@UAEXXZ
    ARTS_IMPORT virtual void Set3DParams();

    // ?EchoOff@vehNitroCarAudio@@QAEXXZ
    ARTS_IMPORT void EchoOff();

    // ?EchoOn@vehNitroCarAudio@@QAEXXZ
    ARTS_IMPORT void EchoOn();

    // ?Init@vehNitroCarAudio@@QAEXPAVvehCarSim@@PAVvehCarDamage@@PAD_N33@Z
    ARTS_IMPORT void Init(vehCarSim* arg1, vehCarDamage* arg2, char* arg3, bool arg4, bool arg5, bool arg6);

    // ?Load@vehNitroCarAudio@@QAEXPAD@Z
    ARTS_IMPORT void Load(char* label_id);

    // ?PlayNitro@vehNitroCarAudio@@QAEXXZ
    ARTS_IMPORT void PlayNitro();

    // ?RemoveFromManager@vehNitroCarAudio@@QAEXXZ
    ARTS_IMPORT void RemoveFromManager();

    // ?Reset@vehNitroCarAudio@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?Set2DPan@vehNitroCarAudio@@QAEXM@Z
    ARTS_IMPORT void Set2DPan(f32 arg1);

    // ?StopNitro@vehNitroCarAudio@@QAEXXZ
    ARTS_IMPORT void StopNitro();

    // ?UpdateAudio3D@vehNitroCarAudio@@QAEXXZ
    ARTS_IMPORT void UpdateAudio3D();

    // ?UpdateAudioNon3D@vehNitroCarAudio@@QAEXXZ
    ARTS_IMPORT void UpdateAudioNon3D();

    // ?UpdateEcho@vehNitroCarAudio@@QAEXXZ
    ARTS_IMPORT void UpdateEcho();

public:
    // Members from 0x130; everything below that belongs to vehCarAudio.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    f32 NitroVolume; // 0x130
    i32 field_134; // 0x134
    AudSoundBase* NitroSound; // 0x138
};

// ??_7vehNitroCarAudio@@6B@
// vtable at 0x005B31D8
check_size(vehNitroCarAudio, 0x13C);
