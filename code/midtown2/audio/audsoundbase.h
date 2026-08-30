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

#include "arts7/asnode.h"

class Vector3;
class audManager;
class audObject;

class AudSoundBase : public asNode
{
public:
    // ??0AudSoundBase@@QAE@IHF@Z
    ARTS_IMPORT AudSoundBase(u32 arg1, i32 arg2, i16 arg3);

    // ??0AudSoundBase@@QAE@PAV0@@Z
    ARTS_IMPORT AudSoundBase(AudSoundBase* arg1);

    // ??0AudSoundBase@@QAE@XZ
    ARTS_IMPORT AudSoundBase();

    // ??1AudSoundBase@@UAE@XZ
    ARTS_IMPORT virtual ~AudSoundBase();

    // ?Update@AudSoundBase@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?AllOneShotLayersPlaying@AudSoundBase@@QAEHXZ
    ARTS_IMPORT i32 AllOneShotLayersPlaying();

    // ?DeallocateStatics@AudSoundBase@@SAXXZ
    ARTS_IMPORT static void DeallocateStatics();

    // ?Disable3DMode@AudSoundBase@@QAEXXZ
    ARTS_IMPORT void Disable3DMode();

    // ?DisableEchoEffect@AudSoundBase@@QAEXXZ
    ARTS_IMPORT void DisableEchoEffect();

    // ?DisableEffect@AudSoundBase@@QAEXH@Z
    ARTS_IMPORT void DisableEffect(i32 arg1);

    // ?Enable3DMode@AudSoundBase@@QAEXXZ
    ARTS_IMPORT void Enable3DMode();

    // ?Get2DFlags@AudSoundBase@@SAIXZ
    ARTS_IMPORT static u32 Get2DFlags();

    // ?Get3DFlags@AudSoundBase@@SAIXZ
    ARTS_IMPORT static u32 Get3DFlags();

    // ?GetEffect@AudSoundBase@@QAEHXZ
    ARTS_IMPORT i32 GetEffect();

    // ?GetFreqChange2DFlags@AudSoundBase@@SAIXZ
    ARTS_IMPORT static u32 GetFreqChange2DFlags();

    // ?GetFreqChange3DFlags@AudSoundBase@@SAIXZ
    ARTS_IMPORT static u32 GetFreqChange3DFlags();

    // ?GetNumSoundHandles@AudSoundBase@@QAEHXZ
    ARTS_IMPORT i32 GetNumSoundHandles();

    // ?GetSoft2DFlags@AudSoundBase@@SAIXZ
    ARTS_IMPORT static u32 GetSoft2DFlags();

    // ?GetSoftFreqChange2DFlags@AudSoundBase@@SAIXZ
    ARTS_IMPORT static u32 GetSoftFreqChange2DFlags();

    // ?GetSoundHandleIndex@AudSoundBase@@QAEHXZ
    ARTS_IMPORT i32 GetSoundHandleIndex();

    // ?GetSubPathAndExtension@AudSoundBase@@QAEXPAD0@Z
    ARTS_IMPORT void GetSubPathAndExtension(char* arg1, char* arg2);

    // ?IsPlaying@AudSoundBase@@QAE_NXZ
    ARTS_IMPORT bool IsPlaying();

    // ?Load@AudSoundBase@@QAEHPADH_N@Z
    ARTS_IMPORT i32 Load(char* arg1, i32 arg2, bool arg3);

    // ?PauseResume@AudSoundBase@@QAEX_N@Z
    ARTS_IMPORT void PauseResume(bool arg1);

    // ?PlayLoop@AudSoundBase@@QAEXMM@Z
    ARTS_IMPORT void PlayLoop(f32 arg1, f32 arg2);

    // ?PlayOnce@AudSoundBase@@QAEXMM@Z
    ARTS_IMPORT void PlayOnce(f32 arg1, f32 arg2);

    // ?ReadyOneShotLayerBuf@AudSoundBase@@QAEHXZ
    ARTS_IMPORT i32 ReadyOneShotLayerBuf();

    // ?SetAgeAudioManagerPtr@AudSoundBase@@SAXPAVaudManager@@@Z
    ARTS_IMPORT static void SetAgeAudioManagerPtr(audManager* arg1);

    // ?SetAllPanning@AudSoundBase@@QAEXM@Z
    ARTS_IMPORT void SetAllPanning(f32 arg1);

    // ?SetAllVolumes@AudSoundBase@@QAEXM@Z
    ARTS_IMPORT void SetAllVolumes(f32 arg1);

    // ?SetDefExtension@AudSoundBase@@SAXPAD@Z
    ARTS_IMPORT static void SetDefExtension(char* arg1);

    // ?SetDefSubPath@AudSoundBase@@SAXPAD@Z
    ARTS_IMPORT static void SetDefSubPath(char* arg1);

    // ?SetDelayTime@AudSoundBase@@QAEXM@Z
    ARTS_IMPORT void SetDelayTime(f32 arg1);

    // ?SetEchoAttenuation@AudSoundBase@@QAEXM@Z
    ARTS_IMPORT void SetEchoAttenuation(f32 arg1);

    // ?SetEchoEffect@AudSoundBase@@QAEXXZ
    ARTS_IMPORT void SetEchoEffect();

    // ?SetEchoFrequency@AudSoundBase@@QAEXMH@Z
    ARTS_IMPORT void SetEchoFrequency(f32 arg1, i32 arg2);

    // ?SetEffect@AudSoundBase@@QAEXH@Z
    ARTS_IMPORT void SetEffect(i32 arg1);

    // ?SetExtension@AudSoundBase@@QAEXPAD@Z
    ARTS_IMPORT void SetExtension(char* arg1);

    // ?SetFrequency@AudSoundBase@@QAEXM@Z
    ARTS_IMPORT void SetFrequency(f32 arg1);

    // ?SetPan@AudSoundBase@@QAEXMH@Z
    ARTS_IMPORT void SetPan(f32 arg1, i32 arg2);

    // ?SetPlayPosition@AudSoundBase@@QAEXI@Z
    ARTS_IMPORT void SetPlayPosition(u32 arg1);

    // ?SetPositionPtr@AudSoundBase@@QAEXPAVVector3@@@Z
    ARTS_IMPORT void SetPositionPtr(Vector3* arg1);

    // ?SetPriority@AudSoundBase@@QAEXH@Z
    ARTS_IMPORT void SetPriority(i32 arg1);

    // ?SetRamp@AudSoundBase@@QAEXMM@Z
    ARTS_IMPORT void SetRamp(f32 arg1, f32 arg2);

    // ?SetSoundHandleIndex@AudSoundBase@@QAEXH@Z
    ARTS_IMPORT void SetSoundHandleIndex(i32 arg1);

    // ?SetSubPath@AudSoundBase@@QAEXPAD@Z
    ARTS_IMPORT void SetSubPath(char* arg1);

    // ?SetupOneShotLayerBuffers@AudSoundBase@@QAEXHMH@Z
    ARTS_IMPORT void SetupOneShotLayerBuffers(i32 arg1, f32 arg2, i32 arg3);

    // ?SetVolume@AudSoundBase@@QAEXM@Z
    ARTS_IMPORT void SetVolume(f32 arg1);

    // ?Stop@AudSoundBase@@QAEXXZ
    ARTS_IMPORT void Stop();

    // ?UpdateEcho@AudSoundBase@@QAEXXZ
    ARTS_IMPORT void UpdateEcho();

public:
    static audManager*& Access_s_pAgeAudioManager() { return s_pAgeAudioManager; }
    static char*& Access_s_pacDefSubPath() { return s_pacDefSubPath; }
    static char*& Access_s_pacDefExtension() { return s_pacDefExtension; }

private:
    // ?AutoUpdateParamsOff@AudSoundBase@@AAEXH@Z
    ARTS_IMPORT void AutoUpdateParamsOff(i32 arg1);

    // ?CreateDuplicateObject@AudSoundBase@@AAEPAVaudObject@@H@Z
    ARTS_IMPORT audObject* CreateDuplicateObject(i32 arg1);

    // ?SetEchoEffect@AudSoundBase@@AAEXH@Z
    ARTS_IMPORT void SetEchoEffect(i32 arg1);

    // ?s_pAgeAudioManager@AudSoundBase@@0PAVaudManager@@A
    ARTS_IMPORT static audManager* s_pAgeAudioManager;

    // ?s_pacDefSubPath@AudSoundBase@@0PADA
    ARTS_IMPORT static char* s_pacDefSubPath;

    // ?s_pacDefExtension@AudSoundBase@@0PADA
    ARTS_IMPORT static char* s_pacDefExtension;
};

// ??_7AudSoundBase@@6B@
// vtable at 0x005B4B48
// check_size(AudSoundBase, 0x58); // size known, members are not - cannot verify
