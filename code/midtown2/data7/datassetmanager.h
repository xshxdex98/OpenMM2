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

class Stream;

class datAssetManager
{
public:
    // ?BaseName@datAssetManager@@SAXPBDPADH@Z
    ARTS_IMPORT static void BaseName(const char* arg1, char* arg2, i32 arg3);

    // ?Create@datAssetManager@@SAPAVStream@@PBD00_N@Z
    ARTS_IMPORT static Stream* Create(const char* arg1, const char* arg2, const char* arg3, bool arg4);

    // ?Create@datAssetManager@@SAPAVStream@@PBD0_N@Z
    ARTS_IMPORT static Stream* Create(const char* arg1, const char* arg2, bool arg3);

    // ?EnumFiles@datAssetManager@@SAHPBDP6AX0_NPAX@Z21@Z
    ARTS_IMPORT static i32 EnumFiles(const char* arg1, void (__cdecl* arg2)(const char*,bool,void*), void* arg3, bool arg4);

    // ?Exists@datAssetManager@@SA_NPBD00@Z
    ARTS_IMPORT static bool Exists(const char* arg1, const char* arg2, const char* arg3);

    // ?Exists@datAssetManager@@SA_NPBD0@Z
    ARTS_IMPORT static bool Exists(const char* arg1, const char* arg2);

    // ?FileName@datAssetManager@@SAPBDPBD@Z
    ARTS_IMPORT static const char* FileName(const char* arg1);

    // ?FullPath@datAssetManager@@SAXPADHPBD11@Z
    ARTS_IMPORT static void FullPath(char* arg1, i32 arg2, const char* arg3, const char* arg4, const char* arg5);

    // ?FullPath@datAssetManager@@SAXPADHPBD1@Z
    ARTS_IMPORT static void FullPath(char* arg1, i32 arg2, const char* arg3, const char* arg4);

    // ?Open@datAssetManager@@SAPAVStream@@PBD00_N1@Z
    ARTS_IMPORT static Stream* Open(const char* arg1, const char* arg2, const char* arg3, bool arg4, bool arg5);

    // ?Open@datAssetManager@@SAPAVStream@@PBD0_N1@Z
    ARTS_IMPORT static Stream* Open(const char* arg1, const char* arg2, bool arg3, bool arg4);

    // ?SetPath@datAssetManager@@SAXPBD@Z
    ARTS_IMPORT static void SetPath(const char* arg1);

public:
    static bool& Access_sm_IgnorePrefix() { return sm_IgnorePrefix; }
    static char*& Access_sm_Path() { return sm_Path; }

private:
    // ?sm_IgnorePrefix@datAssetManager@@0_NA
    ARTS_IMPORT static bool sm_IgnorePrefix;

    // ?sm_Path@datAssetManager@@0PADA
    ARTS_IMPORT static char* sm_Path;
};

// check_size(datAssetManager, 0x1); // size known, members are not - cannot verify
