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

class Stream;
struct tagSirenData;
class vehCarDamage;
class vehCarSim;

class vehPoliceCarAudio : public vehCarAudio
{
public:
    // ??0vehPoliceCarAudio@@QAE@PAVvehCarSim@@PAVvehCarDamage@@PBD2_N@Z
    ARTS_IMPORT vehPoliceCarAudio(vehCarSim* arg1, vehCarDamage* arg2, const char* arg3, const char* arg4, bool arg5);

    // ??1vehPoliceCarAudio@@UAE@XZ
    ARTS_IMPORT virtual ~vehPoliceCarAudio();

    // ?AssignSounds@vehPoliceCarAudio@@UAEXXZ
    ARTS_IMPORT virtual void AssignSounds();

    // ?UnAssignSounds@vehPoliceCarAudio@@UAEXH@Z
    ARTS_IMPORT virtual void UnAssignSounds(i32 arg1);

    // ?UpdateAudio@vehPoliceCarAudio@@UAEXXZ
    ARTS_IMPORT virtual void UpdateAudio();

    // ?Update@vehPoliceCarAudio@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?SetNon3DParams@vehPoliceCarAudio@@UAEXXZ
    ARTS_IMPORT virtual void SetNon3DParams();

    // ?Set3DParams@vehPoliceCarAudio@@UAEXXZ
    ARTS_IMPORT virtual void Set3DParams();

    // ?DamageSiren@vehPoliceCarAudio@@QAEXXZ
    ARTS_IMPORT void DamageSiren();

    // ?EchoOff@vehPoliceCarAudio@@QAEXXZ
    ARTS_IMPORT void EchoOff();

    // ?EchoOn@vehPoliceCarAudio@@QAEXXZ
    ARTS_IMPORT void EchoOn();

    // ?ExplosionIsPlaying@vehPoliceCarAudio@@QAEHXZ
    ARTS_IMPORT i32 ExplosionIsPlaying();

    // ?GetNumCopsPursuingPlayer@vehPoliceCarAudio@@SAHXZ
    ARTS_IMPORT static i32 GetNumCopsPursuingPlayer();

    // ?Init@vehPoliceCarAudio@@QAEXPAVvehCarSim@@PAVvehCarDamage@@PBD2_N@Z
    ARTS_IMPORT void Init(vehCarSim* arg1, vehCarDamage* arg2, const char* arg3, const char* arg4, bool arg5);

    // ?PlayExplosion@vehPoliceCarAudio@@QAEXXZ
    ARTS_IMPORT void PlayExplosion();

    // ?PlayVoice@vehPoliceCarAudio@@QAEXXZ
    ARTS_IMPORT void PlayVoice();

    // ?RandomizeNumber@vehPoliceCarAudio@@QAEFXZ
    ARTS_IMPORT i16 RandomizeNumber();

    // ?RemoveFromManager@vehPoliceCarAudio@@QAEXXZ
    ARTS_IMPORT void RemoveFromManager();

    // ?Reset@vehPoliceCarAudio@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?Set2DPan@vehPoliceCarAudio@@QAEXM@Z
    ARTS_IMPORT void Set2DPan(f32 arg1);

    // ?StartSiren@vehPoliceCarAudio@@QAEXH@Z
    ARTS_IMPORT void StartSiren(i32 arg1);

    // ?StopSiren@vehPoliceCarAudio@@QAEXXZ
    ARTS_IMPORT void StopSiren();

    // ?UpdateAudio3D@vehPoliceCarAudio@@QAEHM@Z
    ARTS_IMPORT i32 UpdateAudio3D(f32 arg1);

    // ?UpdateAudio3D@vehPoliceCarAudio@@QAEXXZ
    ARTS_IMPORT void UpdateAudio3D();

    // ?UpdateAudioNon3D@vehPoliceCarAudio@@QAEXXZ
    ARTS_IMPORT void UpdateAudioNon3D();

    // ?UpdateEcho@vehPoliceCarAudio@@QAEXXZ
    ARTS_IMPORT void UpdateEcho();

    // ?UpdateExplosion@vehPoliceCarAudio@@QAEXMM@Z
    ARTS_IMPORT void UpdateExplosion(f32 arg1, f32 arg2);

    // ?UpdateSiren@vehPoliceCarAudio@@QAEHMMM@Z
    ARTS_IMPORT i32 UpdateSiren(f32 arg1, f32 arg2, f32 arg3);

    // ?UpdateSiren@vehPoliceCarAudio@@QAEXXZ
    ARTS_IMPORT void UpdateSiren();

public:
    static i32& Access_s_iNumCopsPursuingPlayer() { return s_iNumCopsPursuingPlayer; }

private:
    // ?DeallocateSirenPlayInfo@vehPoliceCarAudio@@AAEXPAUtagSirenData@@@Z
    ARTS_IMPORT void DeallocateSirenPlayInfo(tagSirenData* arg1);

    // ?FluctuateSiren@vehPoliceCarAudio@@AAEXXZ
    ARTS_IMPORT void FluctuateSiren();

    // ?Load@vehPoliceCarAudio@@AAEXPBD@Z
    ARTS_IMPORT void Load(const char* arg1);

    // ?ReadSirenData@vehPoliceCarAudio@@AAEXPAUtagSirenData@@PAD@Z
    ARTS_IMPORT void ReadSirenData(tagSirenData* arg1, char* arg2);

    // ?ReadSirenData@vehPoliceCarAudio@@AAEXPAVStream@@@Z
    ARTS_IMPORT void ReadSirenData(Stream* arg1);

    // ?ReadSirenPlayInfo@vehPoliceCarAudio@@AAEXPAUtagSirenData@@PAD@Z
    ARTS_IMPORT void ReadSirenPlayInfo(tagSirenData* arg1, char* arg2);

    // ?s_iNumCopsPursuingPlayer@vehPoliceCarAudio@@0HA
    ARTS_IMPORT static i32 s_iNumCopsPursuingPlayer;
};

// ??_7vehPoliceCarAudio@@6B@
// vtable at 0x005B2E18
// check_size(vehPoliceCarAudio, 0x248); // size known, members are not - cannot verify
