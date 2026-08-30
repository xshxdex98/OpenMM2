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

struct HWND__;
class MixerCTL_vtbl;

class MixerCTL
{
public:
    // ??1MixerCTL@@UAE@XZ
    ARTS_IMPORT virtual ~MixerCTL();

    // ?AssignCDBalance@MixerCTL@@QAEXM@Z
    ARTS_IMPORT void AssignCDBalance(f32 arg1);

    // ?AssignCDVolume@MixerCTL@@QAEXM@Z
    ARTS_IMPORT void AssignCDVolume(f32 arg1);

    // ?AssignMixerBalance@MixerCTL@@QAEKMK@Z
    ARTS_IMPORT ulong AssignMixerBalance(f32 renderer, ulong DrawList);

    // ?AssignMixerVolume@MixerCTL@@QAEKMK@Z
    ARTS_IMPORT ulong AssignMixerVolume(f32 renderer, ulong DrawList);

    // ?AssignWaveBalance@MixerCTL@@QAEXM@Z
    ARTS_IMPORT void AssignWaveBalance(f32 arg1);

    // ?AssignWaveVolume@MixerCTL@@QAEXM@Z
    ARTS_IMPORT void AssignWaveVolume(f32 arg1);

    // ?GetCDBalance@MixerCTL@@QAEMXZ
    ARTS_IMPORT f32 GetCDBalance();

    // ?GetCDVolume@MixerCTL@@QAEMXZ
    ARTS_IMPORT f32 GetCDVolume();

    // ?GetMixerBalance@MixerCTL@@QAEMK@Z
    ARTS_IMPORT f32 GetMixerBalance(ulong arg1);

    // ?GetWaveBalance@MixerCTL@@QAEMXZ
    ARTS_IMPORT f32 GetWaveBalance();

    // ?GetWaveVolume@MixerCTL@@QAEMXZ
    ARTS_IMPORT f32 GetWaveVolume();

    // ?Init@MixerCTL@@QAEKXZ
    ARTS_IMPORT ulong Init();

    // ?RefreshAll@MixerCTL@@QAEXK@Z
    ARTS_IMPORT void RefreshAll(ulong arg1);

    // ?SetDeviceNum@MixerCTL@@QAEXI@Z
    ARTS_IMPORT void SetDeviceNum(u32 arg1);

private:
    // ?GetErrorMessage@MixerCTL@@AAEPADK@Z
    ARTS_IMPORT char* GetErrorMessage(ulong arg1);

    // ?WindowProc@MixerCTL@@AAEJPAUHWND__@@IIJ@Z
    ARTS_IMPORT ilong WindowProc(HWND__* arg1, u32 arg2, u32 arg3, ilong arg4);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    f32 field_4; // 0x004
    f32 field_8; // 0x008
    f32 field_C; // 0x00C
    f32 field_10; // 0x010
    f32 field_14; // 0x014
    f32 field_18; // 0x018
    HWND__* window; // 0x01C
    i32 field_20; // 0x020
    i32 field_24; // 0x024
    u32 mixer_id; // 0x028
    f32 wave_balance; // 0x02C
    f32 cd_balance; // 0x030
};

// ??_7MixerCTL@@6B@
// vtable at 0x005B4E70
check_size(MixerCTL, 0x34);
