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

#include "datasciitokenizer.h"

class Stream;
class datBaseTokenizer;
class datBinTokenizer;

class datMultiTokenizer : public datAsciiTokenizer
{
public:
    // ??0datMultiTokenizer@@QAE@XZ
    ARTS_IMPORT datMultiTokenizer();

    // ?GetReadTokenizer@datMultiTokenizer@@QAEAAVdatBaseTokenizer@@PBDPAVStream@@00@Z
    ARTS_IMPORT datBaseTokenizer& GetReadTokenizer(const char* arg1, ::Stream* arg2, const char* arg3, const char* arg4);

    // ?GetWriteTokenizer@datMultiTokenizer@@QAEAAVdatBaseTokenizer@@PBDPAVStream@@_N0@Z
    ARTS_IMPORT datBaseTokenizer& GetWriteTokenizer(const char* arg1, ::Stream* arg2, bool arg3, const char* arg4);

public:
    // Members from 0x01C; everything below that belongs to datAsciiTokenizer.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 gap4[60]; // 0x01C
    datBinTokenizer* BinTokenizer; // 0x058
};

check_size(datMultiTokenizer, 0x5C);
