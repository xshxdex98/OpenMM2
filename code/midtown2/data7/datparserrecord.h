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

class datCallback;

class datParserRecord
{
public:
    // ??1datParserRecord@@QAE@XZ
    ARTS_IMPORT ~datParserRecord();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u32 Type; // 0x000
    u16 ArrayCount; // 0x004
    u16 ValueSize; // 0x006
    i8 Name[64]; // 0x008
    void* Data; // 0x048
    datCallback* Callback; // 0x04C
    datParserRecord* Next; // 0x050
};

check_size(datParserRecord, 0x54);
