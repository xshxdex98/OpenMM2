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

class mmPlayerCityRecord : public mmInfoBase
{
public:
    // ??0mmPlayerCityRecord@@QAE@XZ
    ARTS_IMPORT mmPlayerCityRecord();

    // ??1mmPlayerCityRecord@@UAE@XZ
    ARTS_IMPORT virtual ~mmPlayerCityRecord();

    // ?Close@mmPlayerCityRecord@@QAEHXZ
    ARTS_IMPORT i32 Close();

    // ?ComputeCRC@mmPlayerCityRecord@@QAEIXZ
    ARTS_IMPORT u32 ComputeCRC();

    // ?GetNumPassed@mmPlayerCityRecord@@QAEHH@Z
    ARTS_IMPORT i32 GetNumPassed(i32 getter);

    // ?GetNumRaces@mmPlayerCityRecord@@QAEHH@Z
    ARTS_IMPORT i32 GetNumRaces(i32 arg1);

    // ?GetPassedMask@mmPlayerCityRecord@@QAEKH@Z
    ARTS_IMPORT ulong GetPassedMask(i32 arg1);

    // ?GetRecord@mmPlayerCityRecord@@QAEHAAVmmPlayerRecord@@HH@Z
    ARTS_IMPORT i32 GetRecord(mmPlayerRecord& arg1, i32 arg2, i32 position);

    // ?InitCityRecord@mmPlayerCityRecord@@QAEXPADMHHHH@Z
    ARTS_IMPORT void InitCityRecord(char* arg1, f32 arg2, i32 arg3, i32 arg4, i32 arg5, i32 arg6);

    // ?NewRecord@mmPlayerCityRecord@@QAEHVmmPlayerRecord@@HH@Z
    ARTS_IMPORT i32 NewRecord(mmPlayerRecord arg1, i32 arg2, i32 arg3);

    // ?Open@mmPlayerCityRecord@@QAEHPADM@Z
    ARTS_IMPORT i32 Open(char* getter, f32 transformer);

    // ?Reset@mmPlayerCityRecord@@QAEXXZ
    ARTS_IMPORT void Reset();

private:
    // ?GetFileOffset@mmPlayerCityRecord@@AAEJHH@Z
    ARTS_IMPORT ilong GetFileOffset(i32 arg1, i32 arg2);
};

// ??_7mmPlayerCityRecord@@6B@
// vtable at 0x005B5234
// check_size(mmPlayerCityRecord, 0x0); // TODO: no layout in the IDB type library
