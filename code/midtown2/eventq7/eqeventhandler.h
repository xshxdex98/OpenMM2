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

class eqEventMonitor;

class eqEventHandler
{
public:
    // ??0eqEventHandler@@QAE@XZ
    ARTS_IMPORT eqEventHandler();

    // ??1eqEventHandler@@UAE@XZ
    ARTS_IMPORT virtual ~eqEventHandler();

    // ?AddClient@eqEventHandler@@QAEXPAVeqEventMonitor@@@Z
    ARTS_IMPORT void AddClient(eqEventMonitor* arg1);

    // ?BeginGfx@eqEventHandler@@QAEHHHH@Z
    ARTS_IMPORT i32 BeginGfx(i32 arg1, i32 arg2, i32 arg3);

    // ?ClearActive@eqEventHandler@@QAEXH@Z
    ARTS_IMPORT void ClearActive(i32 arg1);

    // ?EKeyName@eqEventHandler@@SAPADH@Z
    ARTS_IMPORT static char* EKeyName(i32 arg1);

    // ?EndGfx@eqEventHandler@@QAEXXZ
    ARTS_IMPORT void EndGfx();

    // ?MinimizeApp@eqEventHandler@@QAEXH@Z
    ARTS_IMPORT void MinimizeApp(i32 arg1);

    // ?RemoveClient@eqEventHandler@@QAEXPAVeqEventMonitor@@@Z
    ARTS_IMPORT void RemoveClient(eqEventMonitor* arg1);

    // ?RestoreApp@eqEventHandler@@QAEXXZ
    ARTS_IMPORT void RestoreApp();

    // ?SetActive@eqEventHandler@@QAEXH@Z
    ARTS_IMPORT void SetActive(i32 arg1);

    // ?Update@eqEventHandler@@QAEXXZ
    ARTS_IMPORT void Update();

    // ?SuperQ@eqEventHandler@@2PAV1@A
    ARTS_IMPORT static eqEventHandler* SuperQ;

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u32 field_4; // 0x004
    u32 field_8; // 0x008
    u32 field_C; // 0x00C
    eqEventMonitor* Clients[8]; // 0x010
    i32 MouseX; // 0x030
    i32 MouseY; // 0x034
    u32 field_38; // 0x038
    u32 field_3C; // 0x03C
    u32 field_40; // 0x040
    u32 field_44; // 0x044
    i8 field_48[256]; // 0x048
    u8 field_148[24]; // 0x148
};

// ??_7eqEventHandler@@6B@
// vtable at 0x005B2960
check_size(eqEventHandler, 0x160);
