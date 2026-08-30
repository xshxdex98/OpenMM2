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

class Timer
{
public:
    // ??0Timer@@QAE@XZ
    ARTS_IMPORT Timer();

    // ?BeginBenchmark@Timer@@SAXXZ
    ARTS_IMPORT static void BeginBenchmark();

    // ?EndBenchmark@Timer@@SAXXZ
    ARTS_IMPORT static void EndBenchmark();

    // ?QuickTicks@Timer@@SAIXZ
    ARTS_IMPORT static u32 QuickTicks();

    // ?Ticks@Timer@@SAKXZ
    ARTS_IMPORT static ulong Ticks();

    // ?TicksToSeconds@Timer@@2MA
    ARTS_IMPORT static f32 TicksToSeconds;

public:
    static f32& Access_TicksToMilliseconds() { return TicksToMilliseconds; }

private:
    // ?TicksToMilliseconds@Timer@@0MA
    ARTS_IMPORT static f32 TicksToMilliseconds;

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u32 StartTime; // 0x000
};

check_size(Timer, 0x4);
