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

#include "ui/optionsbase.h"

struct AudioOptions : public OptionsBase
{
public:
    // ??0AudioOptions@@QAE@H@Z
    ARTS_IMPORT AudioOptions(i32 arg1);

    // ??1AudioOptions@@UAE@XZ
    ARTS_IMPORT virtual ~AudioOptions();

    // ?PreSetup@AudioOptions@@UAEXXZ
    ARTS_IMPORT virtual void PreSetup();

    // ?CancelAction@AudioOptions@@UAEXXZ
    ARTS_IMPORT virtual void CancelAction();

    // ?DoneAction@AudioOptions@@UAEXXZ
    ARTS_IMPORT virtual void DoneAction();

    // ?ResetDefaultAction@AudioOptions@@UAEXXZ
    ARTS_IMPORT virtual void ResetDefaultAction();

    // ?StoreCurrentSetup@AudioOptions@@UAEXXZ
    ARTS_IMPORT virtual void StoreCurrentSetup();

    // ?FindDevice@AudioOptions@@QAEHPAD@Z
    ARTS_IMPORT i32 FindDevice(char* arg1);

    // ?FocusDescription@AudioOptions@@QAEXHH@Z
    ARTS_IMPORT void FocusDescription(i32 arg1, i32 arg2);

    // ?GetCurrentDeviceName@AudioOptions@@QAEPADXZ
    ARTS_IMPORT char* GetCurrentDeviceName();

    // ?LoadUIMusicCSV@AudioOptions@@QAEXPAD@Z
    ARTS_IMPORT void LoadUIMusicCSV(char* arg1);

    // ?ResetAmbient@AudioOptions@@QAEXXZ
    ARTS_IMPORT void ResetAmbient();

    // ?ResetCommentary@AudioOptions@@QAEXXZ
    ARTS_IMPORT void ResetCommentary();

    // ?ResetMusic@AudioOptions@@QAEXXZ
    ARTS_IMPORT void ResetMusic();

    // ?ResetSoundFX@AudioOptions@@QAEXXZ
    ARTS_IMPORT void ResetSoundFX();

    // ?ResetSoundQuality@AudioOptions@@QAEXXZ
    ARTS_IMPORT void ResetSoundQuality();

    // ?ResetStereo@AudioOptions@@QAEXXZ
    ARTS_IMPORT void ResetStereo();

    // ?SetAmbient@AudioOptions@@QAEXXZ
    ARTS_IMPORT void SetAmbient();

    // ?SetAudioState@AudioOptions@@QAEXXZ
    ARTS_IMPORT void SetAudioState();

    // ?SetBalance@AudioOptions@@QAEXXZ
    ARTS_IMPORT void SetBalance();

    // ?SetCommentary@AudioOptions@@QAEXXZ
    ARTS_IMPORT void SetCommentary();

    // ?SetDevice@AudioOptions@@QAEXH@Z
    ARTS_IMPORT void SetDevice(i32 getter1);

    // ?SetMusic@AudioOptions@@QAEXXZ
    ARTS_IMPORT void SetMusic();

    // ?SetMusicVolume@AudioOptions@@QAEXXZ
    ARTS_IMPORT void SetMusicVolume();

    // ?SetQuality@AudioOptions@@QAEXXZ
    ARTS_IMPORT void SetQuality();

    // ?SetSFXVolume@AudioOptions@@QAEXXZ
    ARTS_IMPORT void SetSFXVolume();

    // ?SetSoundFX@AudioOptions@@QAEXXZ
    ARTS_IMPORT void SetSoundFX();

    // ?SetStereoFX@AudioOptions@@QAEXXZ
    ARTS_IMPORT void SetStereoFX();

    // ?ToggleAmbient@AudioOptions@@QAEXXZ
    ARTS_IMPORT void ToggleAmbient();

    // ?ToggleMusic@AudioOptions@@QAEXXZ
    ARTS_IMPORT void ToggleMusic();
};

// ??_7AudioOptions@@6B@
// vtable at 0x005B3D9C
// check_size(AudioOptions, 0x8C); // size known, members are not - cannot verify
