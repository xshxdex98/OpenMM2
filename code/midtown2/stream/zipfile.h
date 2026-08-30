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

class zipFile
{
public:
    // ??0zipFile@@QAE@XZ
    ARTS_IMPORT zipFile();

    // ??1zipFile@@QAE@XZ
    ARTS_IMPORT ~zipFile();

    // ?EnumFiles@zipFile@@QAEHPBDP6AX0_NPAX@Z2@Z
    ARTS_IMPORT i32 EnumFiles(const char* arg1, void (__cdecl* arg2)(const char*,bool,void*), void* arg3);

    // ?Init@zipFile@@QAE_NPBD@Z
    ARTS_IMPORT bool Init(const char* arg1);

    // ?KillAll@zipFile@@SAXXZ
    ARTS_IMPORT static void KillAll();

    // ?Open@zipFile@@QAEHPBD@Z
    ARTS_IMPORT i32 Open(const char* arg1);

    // ?zipClose@zipFile@@SAHH@Z
    ARTS_IMPORT static i32 zipClose(i32 arg1);

    // ?zipCreate@zipFile@@SAHPBD@Z
    ARTS_IMPORT static i32 zipCreate(const char* arg1);

    // ?zipEnumFiles@zipFile@@SAHPBDP6AX0_NPAX@Z2@Z
    ARTS_IMPORT static i32 zipEnumFiles(const char* arg1, void (__cdecl* arg2)(const char*,bool,void*), void* arg3);

    // ?zipOpen@zipFile@@SAHPBD_N@Z
    ARTS_IMPORT static i32 zipOpen(const char* arg1, bool arg2);

    // ?zipRead@zipFile@@SAHHPAXH@Z
    ARTS_IMPORT static i32 zipRead(i32 arg1, void* arg2, i32 arg3);

    // ?zipSeek@zipFile@@SAHHHW4seekWhence@@@Z
    ARTS_IMPORT static i32 zipSeek(i32 arg1, i32 arg2, seekWhence arg3);

    // ?zipSize@zipFile@@SAHH@Z
    ARTS_IMPORT static i32 zipSize(i32 arg1);

    // ?zipWrite@zipFile@@SAHHPBXH@Z
    ARTS_IMPORT static i32 zipWrite(i32 arg1, const void* arg2, i32 arg3);

    // ?sm_LogOpen@zipFile@@2_NA
    ARTS_IMPORT static bool sm_LogOpen;

private:
    // ?internalRead@zipFile@@AAEHPAXH_N@Z
    ARTS_IMPORT i32 internalRead(void* arg1, i32 arg2, bool arg3);

    // ?internalSeek@zipFile@@AAEHH@Z
    ARTS_IMPORT i32 internalSeek(i32 arg1);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    zipFile* PrevFile; // 0x000
    i32 FileHandle; // 0x004
    i8* NamesBuffer; // 0x008
    u32* FileCrcs; // 0x00C
    zipEntry* Entries; // 0x010
    i32 EntryCount; // 0x014
    u32 CurrentOffset; // 0x018
};

check_size(zipFile, 0x1C);
