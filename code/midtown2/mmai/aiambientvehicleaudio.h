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

class Aud3DManagerData;
class AudCreature;
class AudImpact;
class Vector3;
class aiEngineAudio;
class vehHornAudio;

class aiAmbientVehicleAudio : public Aud3DObject
{
public:
    // ??0aiAmbientVehicleAudio@@QAE@XZ
    ARTS_IMPORT aiAmbientVehicleAudio();

    // ??1aiAmbientVehicleAudio@@UAE@XZ
    ARTS_IMPORT virtual ~aiAmbientVehicleAudio();

    // ?AssignSounds@aiAmbientVehicleAudio@@UAEXXZ
    ARTS_IMPORT virtual void AssignSounds();

    // ?UnAssignSounds@aiAmbientVehicleAudio@@UAEXH@Z
    ARTS_IMPORT virtual void UnAssignSounds(i32 arg1);

    // ?UpdateAudio@aiAmbientVehicleAudio@@UAEXXZ
    ARTS_IMPORT virtual void UpdateAudio();

    // ?DeallocateStatics@aiAmbientVehicleAudio@@SAXXZ
    ARTS_IMPORT static void DeallocateStatics();

    // ?EchoOff@aiAmbientVehicleAudio@@QAEXXZ
    ARTS_IMPORT void EchoOff();

    // ?EchoOn@aiAmbientVehicleAudio@@QAEXXZ
    ARTS_IMPORT void EchoOn();

    // ?GetAudImpactPtr@aiAmbientVehicleAudio@@QAEPAVAudImpact@@XZ
    ARTS_IMPORT AudImpact* GetAudImpactPtr();

    // ?Init@aiAmbientVehicleAudio@@QAEXPADPAVVector3@@@Z
    ARTS_IMPORT void Init(char* arg1, Vector3* arg2);

    // ?InitStatics@aiAmbientVehicleAudio@@SAXXZ
    ARTS_IMPORT static void InitStatics();

    // ?LoadEngine@aiAmbientVehicleAudio@@QAE_NPAD0@Z
    ARTS_IMPORT bool LoadEngine(char* arg1, char* arg2);

    // ?LoadHorn@aiAmbientVehicleAudio@@QAE_NPAD0@Z
    ARTS_IMPORT bool LoadHorn(char* arg1, char* arg2);

    // ?LoadImpacts@aiAmbientVehicleAudio@@QAE_NPAD0@Z
    ARTS_IMPORT bool LoadImpacts(char* arg1, char* arg2);

    // ?LoadNumVFileChoices@aiAmbientVehicleAudio@@SAXPAD@Z
    ARTS_IMPORT static void LoadNumVFileChoices(char* arg1);

    // ?LoadVoices@aiAmbientVehicleAudio@@QAE_NPAD0_N@Z
    ARTS_IMPORT bool LoadVoices(char* arg1, char* arg2, bool arg3);

    // ?PlayAvoidanceHorn@aiAmbientVehicleAudio@@QAE_NXZ
    ARTS_IMPORT bool PlayAvoidanceHorn();

    // ?PlayAvoidanceReaction@aiAmbientVehicleAudio@@QAEXM@Z
    ARTS_IMPORT void PlayAvoidanceReaction(f32 arg1);

    // ?PlayImpactHorn@aiAmbientVehicleAudio@@QAEXM@Z
    ARTS_IMPORT void PlayImpactHorn(f32 arg1);

    // ?PlayImpactReaction@aiAmbientVehicleAudio@@QAEXM@Z
    ARTS_IMPORT void PlayImpactReaction(f32 arg1);

    // ?Reset@aiAmbientVehicleAudio@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?SetCSVCatString@aiAmbientVehicleAudio@@SAXPAD@Z
    ARTS_IMPORT static void SetCSVCatString(char* arg1);

    // ?UpdateAudio@aiAmbientVehicleAudio@@QAEHM@Z
    ARTS_IMPORT i32 UpdateAudio(f32 arg1);

    // ?UpdateHorn@aiAmbientVehicleAudio@@QAEXXZ
    ARTS_IMPORT void UpdateHorn();

    // ?UpdateStatics@aiAmbientVehicleAudio@@SAXM@Z
    ARTS_IMPORT static void UpdateStatics(f32 arg1);

public:
    static char*& Access_s_CSVCatString() { return s_CSVCatString; }
    static Aud3DManagerData<vehHornAudio>**& Access_s_ppHornAudioContainer() { return s_ppHornAudioContainer; }
    static Aud3DManagerData<AudCreature>**& Access_s_ppAudCreatureContainer() { return s_ppAudCreatureContainer; }
    static Aud3DManagerData<aiEngineAudio>**& Access_s_ppEngineAudioContainer() { return s_ppEngineAudioContainer; }
    static Aud3DManagerData<AudImpact>**& Access_s_ppAudImpactContainer() { return s_ppAudImpactContainer; }
    static i32& Access_s_iNumHornAudioContainers() { return s_iNumHornAudioContainers; }
    static i32& Access_s_iNumEngineAudioContainers() { return s_iNumEngineAudioContainers; }
    static i32& Access_s_iNumAudCreatureContainers() { return s_iNumAudCreatureContainers; }
    static i32& Access_s_iNumAudImpactContainers() { return s_iNumAudImpactContainers; }
    static i32& Access_s_iVoiceFileNum() { return s_iVoiceFileNum; }

private:
    // ?UpdateEcho@aiAmbientVehicleAudio@@AAEXXZ
    ARTS_IMPORT void UpdateEcho();

    // ?UpdateVoices@aiAmbientVehicleAudio@@CAXHM@Z
    ARTS_IMPORT static void UpdateVoices(i32 arg1, f32 arg2);

    // ?s_CSVCatString@aiAmbientVehicleAudio@@0PADA
    ARTS_IMPORT static char* s_CSVCatString;

    // ?s_ppHornAudioContainer@aiAmbientVehicleAudio@@0PAPAV?$Aud3DManagerData@VvehHornAudio@@@@A
    ARTS_IMPORT static Aud3DManagerData<vehHornAudio>** s_ppHornAudioContainer;

    // ?s_ppAudCreatureContainer@aiAmbientVehicleAudio@@0PAPAV?$Aud3DManagerData@VAudCreature@@@@A
    ARTS_IMPORT static Aud3DManagerData<AudCreature>** s_ppAudCreatureContainer;

    // ?s_ppEngineAudioContainer@aiAmbientVehicleAudio@@0PAPAV?$Aud3DManagerData@VaiEngineAudio@@@@A
    ARTS_IMPORT static Aud3DManagerData<aiEngineAudio>** s_ppEngineAudioContainer;

    // ?s_ppAudImpactContainer@aiAmbientVehicleAudio@@0PAPAV?$Aud3DManagerData@VAudImpact@@@@A
    ARTS_IMPORT static Aud3DManagerData<AudImpact>** s_ppAudImpactContainer;

    // ?s_iNumHornAudioContainers@aiAmbientVehicleAudio@@0HA
    ARTS_IMPORT static i32 s_iNumHornAudioContainers;

    // ?s_iNumEngineAudioContainers@aiAmbientVehicleAudio@@0HA
    ARTS_IMPORT static i32 s_iNumEngineAudioContainers;

    // ?s_iNumAudCreatureContainers@aiAmbientVehicleAudio@@0HA
    ARTS_IMPORT static i32 s_iNumAudCreatureContainers;

    // ?s_iNumAudImpactContainers@aiAmbientVehicleAudio@@0HA
    ARTS_IMPORT static i32 s_iNumAudImpactContainers;

    // ?s_iVoiceFileNum@aiAmbientVehicleAudio@@0HA
    ARTS_IMPORT static i32 s_iVoiceFileNum;
};

// ??_7aiAmbientVehicleAudio@@6B@
// vtable at 0x005B2D74
// check_size(aiAmbientVehicleAudio, 0x0); // TODO: no layout in the IDB type library
