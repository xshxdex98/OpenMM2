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
class datBaseTokenizer;
class datCallback;
class datParserRecord;

class datParser
{
public:
    // ??0datParser@@QAE@PBD@Z
    ARTS_IMPORT datParser(const char* filename);

    // ??1datParser@@QAE@XZ
    ARTS_IMPORT ~datParser();

    // ?AddParser@datParser@@QAEPAV1@PBDPAVdatCallback@@@Z
    ARTS_IMPORT datParser* AddParser(const char* name, datCallback* callback);

    // ?AddValue@datParser@@QAEXPBDPAHPAVdatCallback@@@Z
    ARTS_IMPORT void AddValue(const char* arg1, i32* pData, datCallback* callback);

    // ?AddValue@datParser@@QAEXPBDPAMPAVdatCallback@@@Z
    ARTS_IMPORT void AddValue(const char* arg1, f32* arg2, datCallback* callback);

    // ?Load@datParser@@QAE_NPAVStream@@PBD@Z
    ARTS_IMPORT bool Load(Stream* arg1, const char* arg2);

    // ?Load@datParser@@QAE_NPBD00@Z
    ARTS_IMPORT bool Load(const char* prefix, const char* path, const char* ext);

    // ?Load@datParser@@QAE_NPBD0@Z
    ARTS_IMPORT bool Load(const char* path, const char* extension);

    // ?Save@datParser@@QAE_NPAVStream@@PBD_N@Z
    ARTS_IMPORT bool Save(Stream* stream, const char* filename, bool arg3);

    // ?Save@datParser@@QAE_NPBD00_N@Z
    ARTS_IMPORT bool Save(const char* arg1, const char* arg2, const char* arg3, bool arg4);

    // ?Save@datParser@@QAE_NPBD0_N@Z
    ARTS_IMPORT bool Save(const char* arg1, const char* arg2, bool arg3);

private:
    // ?AddRecord@datParser@@AAEAAVdatParserRecord@@HPBDPAXGPAVdatCallback@@@Z
    ARTS_IMPORT datParserRecord& AddRecord(i32 arg1, const char* arg2, void* arg3, u16 arg4, datCallback* arg5);

    // ?Indent@datParser@@AAEXAAVdatBaseTokenizer@@@Z
    ARTS_IMPORT void Indent(datBaseTokenizer& arg1);

    // ?Read@datParser@@AAEXAAVdatBaseTokenizer@@@Z
    ARTS_IMPORT void Read(datBaseTokenizer& arg1);

    // ?Write@datParser@@AAEXAAVdatBaseTokenizer@@@Z
    ARTS_IMPORT void Write(datBaseTokenizer& arg1);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i8 Name[64]; // 0x000
    u32 EntryCount; // 0x040
    u32 field_44; // 0x044
    datParserRecord* FirstRecord; // 0x048
    u32 field_4C; // 0x04C
    u32 RefCount; // 0x050
};

check_size(datParser, 0x54);
