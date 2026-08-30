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
class datBaseTokenizer_vTable;

class datBaseTokenizer
{
public:
    // ?CheckToken@datBaseTokenizer@@QAE_NPBD_N@Z
    ARTS_IMPORT bool CheckToken(const char* arg1, bool arg2);

    // ?GetBlock@datBaseTokenizer@@QAEHPADH@Z
    ARTS_IMPORT i32 GetBlock(char* arg1, i32 arg2);

    // ?GetToken@datBaseTokenizer@@QAEHPADH@Z
    ARTS_IMPORT i32 GetToken(char* arg1, i32 arg2);

    // ?GetTokenToChar@datBaseTokenizer@@QAEHPADHD@Z
    ARTS_IMPORT i32 GetTokenToChar(char* arg1, i32 arg2, char arg3);

    // ?IgnoreToken@datBaseTokenizer@@QAEXXZ
    ARTS_IMPORT void IgnoreToken();

    // ?Init@datBaseTokenizer@@QAEXPBDPAVStream@@@Z
    ARTS_IMPORT void Init(const char* arg1, Stream* arg2);

    // ?MatchToken@datBaseTokenizer@@QAEXPBD@Z
    ARTS_IMPORT void MatchToken(const char* arg1);

    // ?Pop@datBaseTokenizer@@QAEXXZ
    ARTS_IMPORT void Pop();

    // ?Put@datBaseTokenizer@@UAE_ND@Z
    ARTS_IMPORT virtual bool Put(char arg1);

    // ?PutStr@datBaseTokenizer@@QAA_NPBDZZ
    ARTS_IMPORT bool PutStr(const char* arg1, ...);

    // ?SkipToEndOfLine@datBaseTokenizer@@QAEXXZ
    ARTS_IMPORT void SkipToEndOfLine();

    // ?CommentChar@datBaseTokenizer@@2HA
    ARTS_IMPORT static i32 CommentChar;

protected:
    // ?SkipComment@datBaseTokenizer@@IAEXXZ
    ARTS_IMPORT void SkipComment();

private:
    // ?GetTokenCh@datBaseTokenizer@@AAEHXZ
    ARTS_IMPORT i32 GetTokenCh();

    // ?PushBack@datBaseTokenizer@@AAEXPBDH@Z
    ARTS_IMPORT void PushBack(const char* arg1, i32 arg2);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i8* Name; // 0x004
    u32 TokenCount; // 0x008
    Stream* Stream; // 0x00C
    u32 CurrentToken; // 0x010
    u32 CachedTokens; // 0x014
    i8 TokenBuffer[1]; // 0x018
};

// ??_7datBaseTokenizer@@6B@
// vtable at 0x005B22A4
check_size(datBaseTokenizer, 0x1C);
