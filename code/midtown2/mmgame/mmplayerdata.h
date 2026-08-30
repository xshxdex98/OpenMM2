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

#include "mminfobase.h"
#include "mmplayerrecord.h"

class mmPlayerData : public mmInfoBase
{
public:
    // ??0mmPlayerData@@QAE@XZ
    ARTS_IMPORT mmPlayerData();

    // ??1mmPlayerData@@UAE@XZ
    ARTS_IMPORT virtual ~mmPlayerData();

    // ?CloseCityRecord@mmPlayerData@@QAEHXZ
    ARTS_IMPORT i32 CloseCityRecord();

    // ?ComputeCRC@mmPlayerData@@QAEIXZ
    ARTS_IMPORT u32 ComputeCRC();

    // ?GetCheckpointProgress@mmPlayerData@@QAEHPADH@Z
    ARTS_IMPORT i32 GetCheckpointProgress(char* arg1, i32 arg2);

    // ?GetCity@mmPlayerData@@QAEPADXZ
    ARTS_IMPORT char* GetCity();

    // ?GetFileName@mmPlayerData@@QAEPADXZ
    ARTS_IMPORT char* GetFileName();

    // ?GetName@mmPlayerData@@QAEPADXZ
    ARTS_IMPORT char* GetName();

    // ?GetNetName@mmPlayerData@@QAEPADXZ
    ARTS_IMPORT char* GetNetName();

    // ?GetNumPassed@mmPlayerData@@QAEHPADH@Z
    ARTS_IMPORT i32 GetNumPassed(char* arg1, i32 arg2);

    // ?GetPassedMask@mmPlayerData@@QAEHPADH@Z
    ARTS_IMPORT i32 GetPassedMask(char* arg1, i32 arg2);

    // ?GetProgress@mmPlayerData@@QAEHPADH@Z
    ARTS_IMPORT i32 GetProgress(char* arg1, i32 arg2);

    // ?GetScore@mmPlayerData@@QAEHPADH@Z
    ARTS_IMPORT i32 GetScore(char* getter, i32 transformer);

    // ?GetTagID@mmPlayerData@@QAEMXZ
    ARTS_IMPORT f32 GetTagID();

    // ?GetTotalPassed@mmPlayerData@@QAEHPAD@Z
    ARTS_IMPORT i32 GetTotalPassed(char* arg1);

    // ?GetTotalScore@mmPlayerData@@QAEHPAD@Z
    ARTS_IMPORT i32 GetTotalScore(char* getter);

    // ?Load@mmPlayerData@@QAEHPAD@Z
    ARTS_IMPORT i32 Load(char* arg1);

    // ?LoadBinary@mmPlayerData@@QAEHPAD@Z
    ARTS_IMPORT i32 LoadBinary(char* getter);

    // ?OpenCityRecord@mmPlayerData@@QAEHPAD@Z
    ARTS_IMPORT i32 OpenCityRecord(char* arg1);

    // ??4mmPlayerData@@QAEXAAV0@@Z
    ARTS_IMPORT void operator=(mmPlayerData& getter);

    // ?RegisterFinish@mmPlayerData@@QAEHVmmPlayerRecord@@HH@Z
    ARTS_IMPORT i32 RegisterFinish(mmPlayerRecord arg1, i32 arg2, i32 arg3);

    // ?Reset@mmPlayerData@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?ResolveCheckpointProgress@mmPlayerData@@QAEHPAD@Z
    ARTS_IMPORT i32 ResolveCheckpointProgress(char* arg1);

    // ?ResolveCrashProgress@mmPlayerData@@QAEHPAD@Z
    ARTS_IMPORT i32 ResolveCrashProgress(char* getter);

    // ?Save@mmPlayerData@@QAEHPADH@Z
    ARTS_IMPORT i32 Save(char* arg1, i32 arg2);

    // ?SaveBinary@mmPlayerData@@QAEHPAD@Z
    ARTS_IMPORT i32 SaveBinary(char* getter);

    // ?SetCity@mmPlayerData@@QAEXPAD@Z
    ARTS_IMPORT void SetCity(char* arg1);

    // ?SetFileName@mmPlayerData@@QAEXPAD@Z
    ARTS_IMPORT void SetFileName(char* arg1);

    // ?SetName@mmPlayerData@@QAEXPAD@Z
    ARTS_IMPORT void SetName(char* arg1);

    // ?SetNetName@mmPlayerData@@QAEXPAD@Z
    ARTS_IMPORT void SetNetName(char* getter);

    // ?SetTagID@mmPlayerData@@QAEXM@Z
    ARTS_IMPORT void SetTagID(f32 arg1);
};

// ??_7mmPlayerData@@6B@
// vtable at 0x005B51F8
// check_size(mmPlayerData, 0x88); // size known, members are not - cannot verify
