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

#include "audio/aud3dobject.h"

class AudImpact;
class vehCarDamage;
class vehCarSim;

class vehCarAudio : public Aud3DObject
{
public:
    // ??0vehCarAudio@@QAE@PAVvehCarSim@@PAVvehCarDamage@@PBD_N3@Z
    ARTS_IMPORT vehCarAudio(vehCarSim* arg1, vehCarDamage* arg2, const char* arg3, bool arg4, bool arg5);

    // ??1vehCarAudio@@UAE@XZ
    ARTS_IMPORT virtual ~vehCarAudio();

    // ?AssignSounds@vehCarAudio@@UAEXXZ
    ARTS_IMPORT virtual void AssignSounds();

    // ?UnAssignSounds@vehCarAudio@@UAEXH@Z
    ARTS_IMPORT virtual void UnAssignSounds(i32 arg1);

    // ?UpdateAudio@vehCarAudio@@UAEXXZ
    ARTS_IMPORT virtual void UpdateAudio();

    // ?Update@vehCarAudio@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?SetNon3DParams@vehCarAudio@@UAEXXZ
    ARTS_IMPORT virtual void SetNon3DParams();

    // ?Set3DParams@vehCarAudio@@UAEXXZ
    ARTS_IMPORT virtual void Set3DParams();

    // ?DeallocateStatics@vehCarAudio@@SAXXZ
    ARTS_IMPORT static void DeallocateStatics();

    // ?EchoOff@vehCarAudio@@QAEXXZ
    ARTS_IMPORT void EchoOff();

    // ?EchoOn@vehCarAudio@@QAEXXZ
    ARTS_IMPORT void EchoOn();

    // ?GetAudImpactPtr@vehCarAudio@@QAEPAVAudImpact@@XZ
    ARTS_IMPORT AudImpact* GetAudImpactPtr();

    // ?GetCurrentGear@vehCarAudio@@QAEHXZ
    ARTS_IMPORT i32 GetCurrentGear();

    // ?GetSpeed@vehCarAudio@@QAEMXZ
    ARTS_IMPORT f32 GetSpeed();

    // ?Init@vehCarAudio@@QAEXPAVvehCarSim@@PAVvehCarDamage@@PBD_N33@Z
    ARTS_IMPORT void Init(vehCarSim* arg1, vehCarDamage* arg2, const char* arg3, bool arg4, bool arg5, bool arg6);

    // ?InitStatics@vehCarAudio@@SAXXZ
    ARTS_IMPORT static void InitStatics();

    // ?IsAirBorne@vehCarAudio@@QAE_NXZ
    ARTS_IMPORT bool IsAirBorne();

    // ?IsBrakeing@vehCarAudio@@QAE_NXZ
    ARTS_IMPORT bool IsBrakeing();

    // ?Load@vehCarAudio@@QAEHPBD_N@Z
    ARTS_IMPORT i32 Load(const char* arg1, bool arg2);

    // ?PlayHorn@vehCarAudio@@QAEXXZ
    ARTS_IMPORT void PlayHorn();

    // ?RemoveFromManager@vehCarAudio@@QAEXXZ
    ARTS_IMPORT void RemoveFromManager();

    // ?Reset@vehCarAudio@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?Set2DPan@vehCarAudio@@QAEXM@Z
    ARTS_IMPORT void Set2DPan(f32 arg1);

    // ?SetMinAmpSpeed@vehCarAudio@@QAEXM@Z
    ARTS_IMPORT void SetMinAmpSpeed(f32 arg1);

    // ?StopHorn@vehCarAudio@@QAEXXZ
    ARTS_IMPORT void StopHorn();

    // ?UpdateAudio3D@vehCarAudio@@QAEHM@Z
    ARTS_IMPORT i32 UpdateAudio3D(f32 arg1);

    // ?UpdateAudio3D@vehCarAudio@@QAEHXZ
    ARTS_IMPORT i32 UpdateAudio3D();

    // ?UpdateAudioNon3D@vehCarAudio@@QAEXXZ
    ARTS_IMPORT void UpdateAudioNon3D();

    // ?UpdateEcho@vehCarAudio@@QAEXXZ
    ARTS_IMPORT void UpdateEcho();

    // ?UpdateGear@vehCarAudio@@QAEXXZ
    ARTS_IMPORT void UpdateGear();

private:
    // ?LoadImpacts@vehCarAudio@@AAE_NPAD0@Z
    ARTS_IMPORT bool LoadImpacts(char* arg1, char* arg2);
};

// ??_7vehCarAudio@@6B@
// vtable at 0x005B3190
// check_size(vehCarAudio, 0x130); // size known, members are not - cannot verify
