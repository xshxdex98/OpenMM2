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

struct coreFileMethods;

class Stream
{
public:
    // ?Close@Stream@@QAEHXZ
    ARTS_IMPORT i32 Close();

    // ?Create@Stream@@SAPAV1@PBD@Z
    ARTS_IMPORT static Stream* Create(const char* arg1);

    // ?Create@Stream@@SAPAV1@PBDPBUcoreFileMethods@@@Z
    ARTS_IMPORT static Stream* Create(const char* arg1, const coreFileMethods* arg2);

    // ?DumpOpenFiles@Stream@@SAXXZ
    ARTS_IMPORT static void DumpOpenFiles();

    // ?Flush@Stream@@QAEHXZ
    ARTS_IMPORT i32 Flush();

    // ?GetCh@Stream@@QAEHXZ
    ARTS_IMPORT i32 GetCh();

    // ?Open@Stream@@SAPAV1@PBDPBUcoreFileMethods@@_N@Z
    ARTS_IMPORT static Stream* Open(const char* arg1, const coreFileMethods* arg2, bool arg3);

    // ?Open@Stream@@SAPAV1@PBD_N@Z
    ARTS_IMPORT static Stream* Open(const char* arg1, bool arg2);

    // ?PreLoad@Stream@@SAPAV1@PAV1@@Z
    ARTS_IMPORT static Stream* PreLoad(Stream* arg1);

    // ?PutCh@Stream@@QAEHE@Z
    ARTS_IMPORT i32 PutCh(u8 arg1);

    // ?Read@Stream@@QAEHPAXH@Z
    ARTS_IMPORT i32 Read(void* arg1, i32 arg2);

    // ?Seek@Stream@@QAEHH@Z
    ARTS_IMPORT i32 Seek(i32 arg1);

    // ?Size@Stream@@QAEHXZ
    ARTS_IMPORT i32 Size();

    // ?Tell@Stream@@QAEHXZ
    ARTS_IMPORT i32 Tell();

    // ?Write@Stream@@QAEHPBXH@Z
    ARTS_IMPORT i32 Write(const void* arg1, i32 arg2);

public:
    static const coreFileMethods*const& Access_sm_DefaultOpenMethods() { return sm_DefaultOpenMethods; }
    static const coreFileMethods*const& Access_sm_DefaultCreateMethods() { return sm_DefaultCreateMethods; }
    static Stream*& Access_sm_Streams() { return sm_Streams; }

private:
    // ?AllocStream@Stream@@CAPAV1@PBDHPBUcoreFileMethods@@@Z
    ARTS_IMPORT static Stream* AllocStream(const char* arg1, i32 arg2, const coreFileMethods* arg3);

    // ?sm_DefaultOpenMethods@Stream@@0PBUcoreFileMethods@@B
    ARTS_IMPORT static const coreFileMethods*const sm_DefaultOpenMethods;

    // ?sm_DefaultCreateMethods@Stream@@0PBUcoreFileMethods@@B
    ARTS_IMPORT static const coreFileMethods*const sm_DefaultCreateMethods;

    // ?sm_Streams@Stream@@0PAV1@A
    ARTS_IMPORT static Stream* sm_Streams;

    // ?sm_Buffers@Stream@@0PAY0BAAA@DA
    ARTS_IMPORT static char (*sm_Buffers)[4096];

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    coreFileMethods* Methods; // 0x000
    i32 Handle; // 0x004
    i8* Buffer; // 0x008
    i32 CurrentFileOffset; // 0x00C
    i32 CurrentBufferOffset; // 0x010
    i32 CurrentBufferSize; // 0x014
    i32 BufferSize; // 0x018
};

check_size(Stream, 0x1C);
