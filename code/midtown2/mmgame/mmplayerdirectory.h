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

#include "mminfobase.h"

class mmPlayerDirectory : public mmInfoBase
{
public:
    // ??0mmPlayerDirectory@@QAE@XZ
    ARTS_IMPORT mmPlayerDirectory();

    // ??1mmPlayerDirectory@@UAE@XZ
    ARTS_IMPORT virtual ~mmPlayerDirectory();

    // ?AddPlayer@mmPlayerDirectory@@QAEHPAD@Z
    ARTS_IMPORT i32 AddPlayer(char* getter);

    // ?FindPlayer@mmPlayerDirectory@@QAEHPAD@Z
    ARTS_IMPORT i32 FindPlayer(char* arg1);

    // ?GetFileName@mmPlayerDirectory@@QAEPADH@Z
    ARTS_IMPORT char* GetFileName(i32 arg1);

    // ?GetFileName@mmPlayerDirectory@@QAEPADPAD@Z
    ARTS_IMPORT char* GetFileName(char* arg1);

    // ?GetLastPlayer@mmPlayerDirectory@@QAEPADXZ
    ARTS_IMPORT char* GetLastPlayer();

    // ?GetNumPlayers@mmPlayerDirectory@@QAEHXZ
    ARTS_IMPORT i32 GetNumPlayers();

    // ?GetPlayer@mmPlayerDirectory@@QAEPADH@Z
    ARTS_IMPORT char* GetPlayer(i32 arg1);

    // ?GetPlayerName@mmPlayerDirectory@@QAEPADPAD@Z
    ARTS_IMPORT char* GetPlayerName(char* arg1);

    // ?Load@mmPlayerDirectory@@QAEHPAD@Z
    ARTS_IMPORT i32 Load(char* arg1);

    // ?LoadBinary@mmPlayerDirectory@@QAEHPAD@Z
    ARTS_IMPORT i32 LoadBinary(char* getter);

    // ?NewDirectory@mmPlayerDirectory@@QAEXH@Z
    ARTS_IMPORT void NewDirectory(i32 getter);

    // ?RemovePlayer@mmPlayerDirectory@@QAEHPAD@Z
    ARTS_IMPORT i32 RemovePlayer(char* arg1);

    // ?Save@mmPlayerDirectory@@QAEHPADH@Z
    ARTS_IMPORT i32 Save(char* arg1, i32 arg2);

    // ?SaveBinary@mmPlayerDirectory@@QAEHPAD@Z
    ARTS_IMPORT i32 SaveBinary(char* arg1);

    // ?SetLastPlayer@mmPlayerDirectory@@QAEXPAD@Z
    ARTS_IMPORT void SetLastPlayer(char* arg1);

    // ?SetPlayer@mmPlayerDirectory@@QAEXHPAD0@Z
    ARTS_IMPORT void SetPlayer(i32 arg1, char* arg2, char* arg3);

private:
    // ?CreatePlayer@mmPlayerDirectory@@AAEXPAD0@Z
    ARTS_IMPORT void CreatePlayer(char* getter, char* transformer);

    // ?MakeFileName@mmPlayerDirectory@@AAEPADXZ
    ARTS_IMPORT char* MakeFileName();

public:
    // Members from 0x088; everything below that belongs to mmInfoBase.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i32 field_88; // 0x088
    void* field_8C; // 0x08C
    u8 field_90[4]; // 0x090
    u8 field_94[4]; // 0x094
    u8 field_98[4]; // 0x098
    u8 field_9C[4]; // 0x09C
    u8 field_A0[68]; // 0x0A0
};

// ??_7mmPlayerDirectory@@6B@
// vtable at 0x005B522C
check_size(mmPlayerDirectory, 0xE4);
