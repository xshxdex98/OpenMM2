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

#include "stream/hashtable.h"

class datArgParser
{
public:
    // ?AddReplace@datArgParser@@SAXPBDPAD@Z
    ARTS_IMPORT static void AddReplace(const char* arg1, char* arg2);

    // ?Get@datArgParser@@SA_NPBD@Z
    ARTS_IMPORT static bool Get(const char* arg1);

    // ?Get@datArgParser@@SA_NPBDIAAH@Z
    ARTS_IMPORT static bool Get(const char* argName, u32 index, i32& lpIntOut);

    // ?Get@datArgParser@@SA_NPBDIAAM@Z
    ARTS_IMPORT static bool Get(const char* entryName, u32 index, f32& lpFloatOut);

    // ?Get@datArgParser@@SA_NPBDIPAPBD@Z
    ARTS_IMPORT static bool Get(const char* entryName, u32 index, const char** lpStringOut);

    // ?GetNum@datArgParser@@SAHPBD@Z
    ARTS_IMPORT static i32 GetNum(const char* arg1);

    // ?Init@datArgParser@@SAXHPAPAD@Z
    ARTS_IMPORT static void Init(i32 argc, char** argv);

    // ?Init@datArgParser@@SAXXZ
    ARTS_IMPORT static void Init();

    // ?Kill@datArgParser@@SAXXZ
    ARTS_IMPORT static void Kill();

    // ?RestoreFromArchive@datArgParser@@SAXPAD@Z
    ARTS_IMPORT static void RestoreFromArchive(char* archiveString);

    // ?SaveToArchive@datArgParser@@SAHPAD@Z
    ARTS_IMPORT static i32 SaveToArchive(char* label_id);

    // ?Argv@datArgParser@@2PAPADA
    ARTS_IMPORT static char** Argv;

    // ?Argc@datArgParser@@2HA
    ARTS_IMPORT static i32 Argc;

public:
    static HashTable& Access_ArgHash() { return ArgHash; }

private:
    // ?ArgHash@datArgParser@@0VHashTable@@A
    ARTS_IMPORT static HashTable ArgHash;
};

// check_size(datArgParser, 0x1); // size known, members are not - cannot verify
