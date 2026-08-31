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

class Base_vtbl;
class audManager;
class audSoundBankHdr;

class AudManagerBase : public asNode
{
public:
    // ??0AudManagerBase@@QAE@XZ
    ARTS_IMPORT AudManagerBase();

    // ??1AudManagerBase@@UAE@XZ
    ARTS_IMPORT virtual ~AudManagerBase();

    // ?Update@AudManagerBase@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?UpdatePaused@AudManagerBase@@UAEXXZ
    ARTS_IMPORT virtual void UpdatePaused();

    // ?Disable@AudManagerBase@@QAEXXZ
    ARTS_IMPORT void Disable();

    // ?Enable@AudManagerBase@@QAEXXZ
    ARTS_IMPORT void Enable();

    // ?GetAgeAudioManager@AudManagerBase@@QAEPAVaudManager@@XZ
    ARTS_IMPORT audManager* GetAgeAudioManager();

    // ?GetMasterSFXVolume@AudManagerBase@@QAEMXZ
    ARTS_IMPORT f32 GetMasterSFXVolume();

    // ?Init@AudManagerBase@@QAEXHFFH@Z
    ARTS_IMPORT void Init(i32 arg1, i16 arg2, i16 arg3, i32 arg4);

    // ?IsEnabled@AudManagerBase@@QAE_NXZ
    ARTS_IMPORT bool IsEnabled();

    // ?IsStereo@AudManagerBase@@QAEHXZ
    ARTS_IMPORT i32 IsStereo();

    // ?LoadSoundBank@AudManagerBase@@QAEPAVaudSoundBankHdr@@PAD@Z
    ARTS_IMPORT audSoundBankHdr* LoadSoundBank(char* arg1);

    // ?RandomizeNumber@AudManagerBase@@QAEMM@Z
    ARTS_IMPORT f32 RandomizeNumber(f32 arg1);

    // ?RandomizeNumber@AudManagerBase@@QAEMMM@Z
    ARTS_IMPORT f32 RandomizeNumber(f32 arg1, f32 arg2);

    // ?RestartAudio@AudManagerBase@@QAEXHHH@Z
    ARTS_IMPORT void RestartAudio(i32 arg1, i32 arg2, i32 arg3);

    // ?SetReverb@AudManagerBase@@QAEXH@Z
    ARTS_IMPORT void SetReverb(i32 arg1);

    // ?SetReverbFlags@AudManagerBase@@QAEXHH@Z
    ARTS_IMPORT void SetReverbFlags(i32 arg1, i32 arg2);

    // ?SetStereoFlag@AudManagerBase@@QAEXH@Z
    ARTS_IMPORT void SetStereoFlag(i32 stereo);

    // ?ShutDownAudio@AudManagerBase@@QAEXXZ
    ARTS_IMPORT void ShutDownAudio();

    // ?SoundFXOn@AudManagerBase@@QAE_NXZ
    ARTS_IMPORT bool SoundFXOn();

    // ?StopAllSounds@AudManagerBase@@QAEXXZ
    ARTS_IMPORT void StopAllSounds();
};

// ??_7AudManagerBase@@6B@
// vtable at 0x005B4C18
// check_size(AudManagerBase, 0x18); // size known, members are not - cannot verify
