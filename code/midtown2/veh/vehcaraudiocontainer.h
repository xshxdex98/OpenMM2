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

class AudImpact;
class Stream;
class vehCarAudio;
class vehCarDamage;
class vehCarSim;
class vehNitroCarAudio;
class vehPoliceCarAudio;
class vehSemiCarAudio;

class vehCarAudioContainer
{
public:
    // ??0vehCarAudioContainer@@QAE@PBDPAVvehCarSim@@PAVvehCarDamage@@H@Z
    ARTS_IMPORT vehCarAudioContainer(const char* arg1, vehCarSim* arg2, vehCarDamage* arg3, i32 arg4);

    // ??0vehCarAudioContainer@@QAE@XZ
    ARTS_IMPORT vehCarAudioContainer();

    // ??1vehCarAudioContainer@@QAE@XZ
    ARTS_IMPORT ~vehCarAudioContainer();

    // ?DeallocateStatics@vehCarAudioContainer@@SAXXZ
    ARTS_IMPORT static void DeallocateStatics();

    // ?GetAudImpactPtr@vehCarAudioContainer@@QAEPAVAudImpact@@XZ
    ARTS_IMPORT AudImpact* GetAudImpactPtr();

    // ?GetCarAudioPtr@vehCarAudioContainer@@QAEPAVvehCarAudio@@XZ
    ARTS_IMPORT vehCarAudio* GetCarAudioPtr();

    // ?GetNitroCarAudioPtr@vehCarAudioContainer@@QAEPAVvehNitroCarAudio@@XZ
    ARTS_IMPORT vehNitroCarAudio* GetNitroCarAudioPtr();

    // ?GetPoliceCarAudioPtr@vehCarAudioContainer@@QAEPAVvehPoliceCarAudio@@XZ
    ARTS_IMPORT vehPoliceCarAudio* GetPoliceCarAudioPtr();

    // ?GetSemiCarAudioPtr@vehCarAudioContainer@@QAEPAVvehSemiCarAudio@@XZ
    ARTS_IMPORT vehSemiCarAudio* GetSemiCarAudioPtr();

    // ?Init@vehCarAudioContainer@@QAEXPBDPAVvehCarSim@@PAVvehCarDamage@@H@Z
    ARTS_IMPORT void Init(const char* arg1, vehCarSim* arg2, vehCarDamage* arg3, i32 arg4);

    // ?InitNitro@vehCarAudioContainer@@QAEXPBDPAVvehCarSim@@PAVvehCarDamage@@H@Z
    ARTS_IMPORT void InitNitro(const char* arg1, vehCarSim* arg2, vehCarDamage* arg3, i32 arg4);

    // ?InitPolice@vehCarAudioContainer@@QAEXPBDPAVvehCarSim@@PAVvehCarDamage@@H@Z
    ARTS_IMPORT void InitPolice(const char* arg1, vehCarSim* arg2, vehCarDamage* arg3, i32 arg4);

    // ?InitSemi@vehCarAudioContainer@@QAEXPBDPAVvehCarSim@@PAVvehCarDamage@@H@Z
    ARTS_IMPORT void InitSemi(const char* arg1, vehCarSim* arg2, vehCarDamage* arg3, i32 arg4);

    // ?InitStatics@vehCarAudioContainer@@SAXXZ
    ARTS_IMPORT static void InitStatics();

    // ?Is3D@vehCarAudioContainer@@QAE_NXZ
    ARTS_IMPORT bool Is3D();

    // ?IsAirBorne@vehCarAudioContainer@@QAE_NXZ
    ARTS_IMPORT bool IsAirBorne();

    // ?IsPlayer@vehCarAudioContainer@@QAE_NXZ
    ARTS_IMPORT bool IsPlayer();

    // ?IsPolice@vehCarAudioContainer@@SA_NPBD@Z
    ARTS_IMPORT static bool IsPolice(const char* arg1);

    // ?IsSemiOrBus@vehCarAudioContainer@@SA_NPBD@Z
    ARTS_IMPORT static bool IsSemiOrBus(const char* label_id);

    // ?PlayHorn@vehCarAudioContainer@@QAEXXZ
    ARTS_IMPORT void PlayHorn();

    // ?PlayNitro@vehCarAudioContainer@@QAEXXZ
    ARTS_IMPORT void PlayNitro();

    // ?RegisterTypes@vehCarAudioContainer@@SAXPBD@Z
    ARTS_IMPORT static void RegisterTypes(const char* fileName);

    // ?RemoveNetVehicleAudio@vehCarAudioContainer@@QAEXXZ
    ARTS_IMPORT void RemoveNetVehicleAudio();

    // ?Reset@vehCarAudioContainer@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?Set2DPan@vehCarAudioContainer@@QAEXM@Z
    ARTS_IMPORT void Set2DPan(f32 label_id);

    // ?Set3D@vehCarAudioContainer@@QAEX_N@Z
    ARTS_IMPORT void Set3D(bool arg1);

    // ?SetSirenCSVName@vehCarAudioContainer@@SAXPBD@Z
    ARTS_IMPORT static void SetSirenCSVName(const char* arg1);

    // ?SilenceEngine@vehCarAudioContainer@@QAEXH@Z
    ARTS_IMPORT void SilenceEngine(i32 arg1);

    // ?StartSiren@vehCarAudioContainer@@QAEXXZ
    ARTS_IMPORT void StartSiren();

    // ?StopHorn@vehCarAudioContainer@@QAEXXZ
    ARTS_IMPORT void StopHorn();

    // ?StopNitro@vehCarAudioContainer@@QAEXXZ
    ARTS_IMPORT void StopNitro();

    // ?StopSiren@vehCarAudioContainer@@QAEXXZ
    ARTS_IMPORT void StopSiren();

    // ?Update@vehCarAudioContainer@@QAEXXZ
    ARTS_IMPORT void Update();

public:
    static i32& Access_s_iNumPoliceNames() { return s_iNumPoliceNames; }
    static i32& Access_s_iNumSemiNames() { return s_iNumSemiNames; }
    static char*& Access_s_acSirenCSVName() { return s_acSirenCSVName; }
    static bool& Access_s_bTypesRegistered() { return s_bTypesRegistered; }
    static bool& Access_s_bAlwaysNitro() { return s_bAlwaysNitro; }
    static char**& Access_s_ppSemiName() { return s_ppSemiName; }
    static char**& Access_s_ppPoliceName() { return s_ppPoliceName; }

private:
    // ?RegisterPoliceNames@vehCarAudioContainer@@CAXPAVStream@@PAD@Z
    ARTS_IMPORT static void RegisterPoliceNames(Stream* stream, char* names);

    // ?RegisterSemiNames@vehCarAudioContainer@@CAXPAVStream@@PAD@Z
    ARTS_IMPORT static void RegisterSemiNames(Stream* stream, char* names);

    // ?s_iNumPoliceNames@vehCarAudioContainer@@0HA
    ARTS_IMPORT static i32 s_iNumPoliceNames;

    // ?s_iNumSemiNames@vehCarAudioContainer@@0HA
    ARTS_IMPORT static i32 s_iNumSemiNames;

    // ?s_acSirenCSVName@vehCarAudioContainer@@0PADA
    ARTS_IMPORT static char* s_acSirenCSVName;

    // ?s_bTypesRegistered@vehCarAudioContainer@@0_NA
    ARTS_IMPORT static bool s_bTypesRegistered;

    // ?s_bAlwaysNitro@vehCarAudioContainer@@0_NA
    ARTS_IMPORT static bool s_bAlwaysNitro;

    // ?s_ppSemiName@vehCarAudioContainer@@0PAPADA
    ARTS_IMPORT static char** s_ppSemiName;

    // ?s_ppPoliceName@vehCarAudioContainer@@0PAPADA
    ARTS_IMPORT static char** s_ppPoliceName;
};

// check_size(vehCarAudioContainer, 0x18); // size known, members are not - cannot verify
