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

class Stream;

class mmRecord : public mmInfoBase
{
public:
    // ??0mmRecord@@QAE@XZ
    ARTS_IMPORT mmRecord();

    // ??1mmRecord@@UAE@XZ
    ARTS_IMPORT virtual ~mmRecord();

    // ?ComputeCRC@mmRecord@@QAEIXZ
    ARTS_IMPORT u32 ComputeCRC();

    // ?GetCarName@mmRecord@@QAEPADXZ
    ARTS_IMPORT char* GetCarName();

    // ?GetName@mmRecord@@QAEPADXZ
    ARTS_IMPORT char* GetName();

    // ?GetPassed@mmRecord@@QAEHXZ
    ARTS_IMPORT i32 GetPassed();

    // ?GetScore@mmRecord@@QAEHXZ
    ARTS_IMPORT i32 GetScore();

    // ?GetTime@mmRecord@@QAEMXZ
    ARTS_IMPORT f32 GetTime();

    // ?LoadBinary@mmRecord@@QAEHPAVStream@@@Z
    ARTS_IMPORT i32 LoadBinary(Stream* getter);

    // ??4mmRecord@@QAEXAAV0@@Z
    ARTS_IMPORT void operator=(mmRecord& getter);

    // ?Reset@mmRecord@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?SaveBinary@mmRecord@@QAEHPAVStream@@@Z
    ARTS_IMPORT i32 SaveBinary(Stream* Data);

    // ?SetCarName@mmRecord@@QAEXPAD@Z
    ARTS_IMPORT void SetCarName(char* arg1);

    // ?SetName@mmRecord@@QAEXPAD@Z
    ARTS_IMPORT void SetName(char* arg1);

    // ?SetPassed@mmRecord@@QAEXH@Z
    ARTS_IMPORT void SetPassed(i32 arg1);

    // ?SetScore@mmRecord@@QAEXH@Z
    ARTS_IMPORT void SetScore(i32 arg1);

    // ?SetTime@mmRecord@@QAEXM@Z
    ARTS_IMPORT void SetTime(f32 arg1);

    // ?SizeOf@mmRecord@@2HA
    ARTS_IMPORT static i32 SizeOf;
};

// ??_7mmRecord@@6B@
// vtable at 0x005B5210
// check_size(mmRecord, 0x88); // size known, members are not - cannot verify
