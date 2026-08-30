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

class mmPlayerRecord : public mmInfoBase
{
public:
    // ??0mmPlayerRecord@@QAE@ABV0@@Z
    ARTS_IMPORT mmPlayerRecord(const mmPlayerRecord& arg1);

    // ??0mmPlayerRecord@@QAE@XZ
    ARTS_IMPORT mmPlayerRecord();

    // ??1mmPlayerRecord@@UAE@XZ
    ARTS_IMPORT virtual ~mmPlayerRecord();

    // ?ComputeCRC@mmPlayerRecord@@QAEIXZ
    ARTS_IMPORT u32 ComputeCRC();

    // ?LoadBinary@mmPlayerRecord@@QAEHPAVStream@@@Z
    ARTS_IMPORT i32 LoadBinary(Stream* arg1);

    // ??4mmPlayerRecord@@QAEXAAV0@@Z
    ARTS_IMPORT void operator=(mmPlayerRecord& arg1);

    // ??8mmPlayerRecord@@QAEHABV0@@Z
    ARTS_IMPORT i32 operator==(const mmPlayerRecord& arg1);

    // ?Reset@mmPlayerRecord@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?SaveBinary@mmPlayerRecord@@QAEHPAVStream@@@Z
    ARTS_IMPORT i32 SaveBinary(Stream* arg1);

    // ?SizeOf@mmPlayerRecord@@2HA
    ARTS_IMPORT static i32 SizeOf;
};

// ??_7mmPlayerRecord@@6B@
// vtable at 0x005B062C
// check_size(mmPlayerRecord, 0x0); // TODO: no layout in the IDB type library
