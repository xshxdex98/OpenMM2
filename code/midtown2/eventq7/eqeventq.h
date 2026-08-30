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

#include "eqeventmonitor.h"

union eqEvent;

class eqEventQ
{
public:
    // ??0eqEventQ@@QAE@H@Z
    ARTS_IMPORT eqEventQ(i32 arg1);

    // ??1eqEventQ@@UAE@XZ
    ARTS_IMPORT virtual ~eqEventQ();

    // ?Keyboard@eqEventQ@@UAEXPAXHHHH@Z
    ARTS_IMPORT virtual void Keyboard(void* arg1, i32 arg2, i32 arg3, i32 arg4, i32 arg5);

    // ?Mouse@eqEventQ@@UAEXPAXHHHHHHH@Z
    ARTS_IMPORT virtual void Mouse(void* arg1, i32 arg2, i32 arg3, i32 arg4, i32 arg5, i32 arg6, i32 arg7, i32 arg8);

    // ?Pop@eqEventQ@@QAEHPATeqEvent@@@Z
    ARTS_IMPORT i32 Pop(eqEvent* arg1);

private:
    // ?Queue@eqEventQ@@AAEXAATeqEvent@@@Z
    ARTS_IMPORT void Queue(eqEvent& arg1);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    eqEventMonitor eqeventmonitor0; // 0x000
    u32 Values; // 0x00C
    i32 field_10; // 0x010
    u32 ValueCount; // 0x014
    u32 field_18; // 0x018
    u32 field_1C; // 0x01C
    u32 InQueue; // 0x020
};

// ??_7eqEventQ@@6B@
// vtable at 0x005B29E0
check_size(eqEventQ, 0x24);
