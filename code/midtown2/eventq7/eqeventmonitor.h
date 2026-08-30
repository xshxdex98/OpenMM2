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

class eqEventHandler;

class eqEventMonitor
{
public:
    // ??0eqEventMonitor@@QAE@XZ
    ARTS_IMPORT eqEventMonitor();

    // ??1eqEventMonitor@@UAE@XZ
    ARTS_IMPORT virtual ~eqEventMonitor();

    // ?Keyboard@eqEventMonitor@@UAEXPAXHHHH@Z
    ARTS_IMPORT virtual void Keyboard(void* arg1, i32 arg2, i32 arg3, i32 arg4, i32 arg5);

    // ?Mouse@eqEventMonitor@@UAEXPAXHHHHHHH@Z
    ARTS_IMPORT virtual void Mouse(void* arg1, i32 arg2, i32 arg3, i32 arg4, i32 arg5, i32 arg6, i32 arg7, i32 arg8);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    eqEventHandler* Handler; // 0x004
    u32 Index; // 0x008
};

// ??_7eqEventMonitor@@6B@
// vtable at 0x005B2964
check_size(eqEventMonitor, 0xC);
