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

class HashTable;
class Stream;
class datAsciiTokenizer;
class phMaterial;

class phMaterialMgr
{
public:
    // ??1phMaterialMgr@@QAE@XZ
    ARTS_IMPORT ~phMaterialMgr();

    // ?Load@phMaterialMgr@@UAEPAVphMaterial@@PAVStream@@@Z
    ARTS_IMPORT virtual phMaterial* Load(Stream* arg1);

    // ?Load@phMaterialMgr@@UAEPAVphMaterial@@PAVdatAsciiTokenizer@@@Z
    ARTS_IMPORT virtual phMaterial* Load(datAsciiTokenizer* arg1);

    // ?AddToTable@phMaterialMgr@@QAEXPAVphMaterial@@@Z
    ARTS_IMPORT void AddToTable(phMaterial* arg1);

    // ?CreateInstance@phMaterialMgr@@SAAAV1@H@Z
    ARTS_IMPORT static phMaterialMgr& CreateInstance(i32 arg1);

    // ?Find@phMaterialMgr@@QAEPAVphMaterial@@PBD@Z
    ARTS_IMPORT phMaterial* Find(const char* arg1);

    // ?FindIndexOfName@phMaterialMgr@@QAEHPBD@Z
    ARTS_IMPORT i32 FindIndexOfName(const char* arg1);

    // ?FindNameOfIndex@phMaterialMgr@@QAEPBDH@Z
    ARTS_IMPORT const char* FindNameOfIndex(i32 arg1);

    // ?LoadTable@phMaterialMgr@@QAE_NPBD@Z
    ARTS_IMPORT bool LoadTable(const char* arg1);

    // ?Save@phMaterialMgr@@QAEXPAVphMaterial@@PAVdatAsciiTokenizer@@@Z
    ARTS_IMPORT void Save(phMaterial* arg1, datAsciiTokenizer* arg2);

protected:
    // ??0phMaterialMgr@@IAE@PAVphMaterial@@H@Z
    ARTS_IMPORT phMaterialMgr(phMaterial* arg1, i32 arg2);

    // ??0phMaterialMgr@@IAE@XZ
    ARTS_IMPORT phMaterialMgr();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    HashTable* field_4; // 0x004
    phMaterial** Materials; // 0x008
    i32 NameCount; // 0x00C
    u32 MaterialCount; // 0x010
    u32 MaxCount; // 0x014
    char (*) Names[32]; // 0x018
};

// ??_7phMaterialMgr@@6B@
// vtable at 0x005B2410
check_size(phMaterialMgr, 0x1C);
