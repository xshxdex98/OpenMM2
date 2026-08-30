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
union eqEvent;

class mmIODev : public mmInfoBase
{
public:
    // ??0mmIODev@@QAE@XZ
    ARTS_IMPORT mmIODev();

    // ??1mmIODev@@UAE@XZ
    ARTS_IMPORT virtual ~mmIODev();

    // ?Assign@mmIODev@@QAEXHHH@Z
    ARTS_IMPORT void Assign(i32 arg1, i32 arg2, i32 arg3);

    // ?GetComponentType@mmIODev@@QAEHHH@Z
    ARTS_IMPORT i32 GetComponentType(i32 arg1, i32 arg2);

    // ?GetDescription@mmIODev@@QAEXPAD@Z
    ARTS_IMPORT void GetDescription(char* arg1);

    // ?Init@mmIODev@@QAEXH_J@Z
    ARTS_IMPORT void Init(i32 arg1, i64 arg2);

    // ??8mmIODev@@QAEHPATeqEvent@@@Z
    ARTS_IMPORT i32 operator==(eqEvent* arg1);

    // ?Print@mmIODev@@QAEXH@Z
    ARTS_IMPORT void Print(i32 arg1);

    // ?Read@mmIODev@@QAEHPAVStream@@@Z
    ARTS_IMPORT i32 Read(Stream* arg1);

    // ?ReadBinary@mmIODev@@QAEHPAVStream@@@Z
    ARTS_IMPORT i32 ReadBinary(Stream* arg1);

    // ?SanityCheckioType@mmIODev@@QAEHHHH@Z
    ARTS_IMPORT i32 SanityCheckioType(i32 arg1, i32 arg2, i32 arg3);

    // ?Write@mmIODev@@QAEHPAVStream@@@Z
    ARTS_IMPORT i32 Write(Stream* arg1);

    // ?WriteBinary@mmIODev@@QAEHPAVStream@@@Z
    ARTS_IMPORT i32 WriteBinary(Stream* arg1);

public:
    // Members from 0x088; everything below that belongs to mmInfoBase.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i32 int88; // 0x088
    u8 gap89[12]; // 0x08C
    i32 int98; // 0x098
    i32 int9C; // 0x09C
    i32 intA0; // 0x0A0
    i32 intA4; // 0x0A4
};

// ??_7mmIODev@@6B@
// vtable at 0x005B5294
check_size(mmIODev, 0xA8);
