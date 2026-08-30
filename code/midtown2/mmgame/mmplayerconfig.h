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

#include "mmgfxcfg.h"
#include "mminfobase.h"
#include "mmiodev.h"

class mmPlayerConfig : public mmInfoBase
{
public:
    // ??0mmPlayerConfig@@QAE@XZ
    ARTS_IMPORT mmPlayerConfig();

    // ??1mmPlayerConfig@@UAE@XZ
    ARTS_IMPORT virtual ~mmPlayerConfig();

    // ?DefaultAudio@mmPlayerConfig@@QAEXXZ
    ARTS_IMPORT void DefaultAudio();

    // ?DefaultControls@mmPlayerConfig@@QAEXXZ
    ARTS_IMPORT void DefaultControls();

    // ?DefaultGraphics@mmPlayerConfig@@QAEXXZ
    ARTS_IMPORT void DefaultGraphics();

    // ?DefaultViewSettings@mmPlayerConfig@@QAEXXZ
    ARTS_IMPORT void DefaultViewSettings();

    // ?GetAudio@mmPlayerConfig@@QAEXXZ
    ARTS_IMPORT void GetAudio();

    // ?GetControls@mmPlayerConfig@@QAEXXZ
    ARTS_IMPORT void GetControls();

    // ?GetCustom@mmPlayerConfig@@QAEXXZ
    ARTS_IMPORT void GetCustom();

    // ?GetGraphics@mmPlayerConfig@@QAEXXZ
    ARTS_IMPORT void GetGraphics();

    // ?GetViewSettings@mmPlayerConfig@@QAEXXZ
    ARTS_IMPORT void GetViewSettings();

    // ?Load@mmPlayerConfig@@QAEHPAD@Z
    ARTS_IMPORT i32 Load(char* arg1);

    // ?LoadBinary@mmPlayerConfig@@QAEHPAD@Z
    ARTS_IMPORT i32 LoadBinary(char* arg1);

    // ??4mmPlayerConfig@@QAEXAAV0@@Z
    ARTS_IMPORT void operator=(mmPlayerConfig& arg1);

    // ?Reset@mmPlayerConfig@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?Save@mmPlayerConfig@@QAEHPADH@Z
    ARTS_IMPORT i32 Save(char* arg1, i32 arg2);

    // ?SaveBinary@mmPlayerConfig@@QAEHPAD@Z
    ARTS_IMPORT i32 SaveBinary(char* arg1);

    // ?SetAudio@mmPlayerConfig@@QAEXXZ
    ARTS_IMPORT void SetAudio();

    // ?SetControls@mmPlayerConfig@@QAEXXZ
    ARTS_IMPORT void SetControls();

    // ?SetCustom@mmPlayerConfig@@QAEXXZ
    ARTS_IMPORT void SetCustom();

    // ?SetDefaults@mmPlayerConfig@@QAEXXZ
    ARTS_IMPORT void SetDefaults();

    // ?SetGraphics@mmPlayerConfig@@QAEXXZ
    ARTS_IMPORT void SetGraphics();

    // ?SetViewSettings@mmPlayerConfig@@QAEXXZ
    ARTS_IMPORT void SetViewSettings();

public:
    // Members from 0x088; everything below that belongs to mmInfoBase.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i32 Version; // 0x088
    mmGfxCFG GraphicsConfig; // 0x08C
    f32 SoundFXVolume; // 0x0C4
    f32 MusicVolume; // 0x0C8
    f32 AudioBalance; // 0x0CC
    i32 AudioFlags; // 0x0D0
    i32 NumAudioChannels; // 0x0D4
    i8 CurrentAudioDevice[200]; // 0x0D8
    i32 InputDevice; // 0x1A0
    i32 int1A4; // 0x1A4
    i32 int1A8; // 0x1A8
    i32 int1AC; // 0x1AC
    i32 int1B0; // 0x1B0
    i32 int1B4; // 0x1B4
    i32 int1B8; // 0x1B8
    i32 int1BC; // 0x1BC
    i32 int1C0; // 0x1C0
    i32 int1C4; // 0x1C4
    i32 int1C8; // 0x1C8
    i32 int1CC; // 0x1CC
    i32 int1D0; // 0x1D0
    i32 int1D4; // 0x1D4
    mmIODev IODevices[170]; // 0x1D8
    i8 field_7168[4]; // 0x7168
    i8 field_716C; // 0x716C
    i8 field_716D; // 0x716D
    i8 field_716E; // 0x716E
    i8 field_716F; // 0x716F
    i8 field_7170; // 0x7170
    u8 field_7171[7]; // 0x7171
};

// ??_7mmPlayerConfig@@6B@
// vtable at 0x005B5224
check_size(mmPlayerConfig, 0x7178);
