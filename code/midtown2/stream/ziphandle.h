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

enum seekWhence : i32;

class zipEntry;
class zipFile;

class zipHandle
{
public:
    // ?Read@zipHandle@@QAEHPAXH@Z
    ARTS_IMPORT i32 Read(void* arg1, i32 arg2);

    // ?Seek@zipHandle@@QAEHHW4seekWhence@@@Z
    ARTS_IMPORT i32 Seek(i32 arg1, seekWhence arg2);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    zipFile* pZipFile; // 0x000
    zipEntry* pZipEntry; // 0x004
    u32 CurrentOffset; // 0x008
    u32 CurrentRawDataSize; // 0x00C
    z_stream Inflater; // 0x010
};

check_size(zipHandle, 0x48);
