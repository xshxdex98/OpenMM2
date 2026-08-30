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

#include "arts7/asnode.h"
#include "misc/timer.h"

class mmTimer : public asNode
{
public:
    // ??0mmTimer@@QAE@XZ
    ARTS_IMPORT mmTimer();

    // ??1mmTimer@@UAE@XZ
    ARTS_IMPORT virtual ~mmTimer();

    // ?GetTime@mmTimer@@QAEMXZ
    ARTS_IMPORT f32 GetTime();

    // ?Init@mmTimer@@QAEXHMH@Z
    ARTS_IMPORT void Init(i32 arg1, f32 arg2, i32 arg3);

    // ?Reset@mmTimer@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Start@mmTimer@@QAEXXZ
    ARTS_IMPORT void Start();

    // ?StartStop@mmTimer@@QAEXXZ
    ARTS_IMPORT void StartStop();

    // ?Stop@mmTimer@@QAEXXZ
    ARTS_IMPORT void Stop();

    // ?Update@mmTimer@@UAEXXZ
    ARTS_IMPORT virtual void Update();

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i32 field_18; // 0x018
    i32 field_1C; // 0x01C
    i32 field_20; // 0x020
    i32 field_24; // 0x024
    i32 field_28; // 0x028
    Timer field_2C; // 0x02C
};

// ??_7mmTimer@@6B@
// vtable at 0x005B0C78
check_size(mmTimer, 0x30);
