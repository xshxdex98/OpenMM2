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

class gfxBitmap;
class mmExternalView;

class mmLinearGauge
{
public:
    // ??0mmLinearGauge@@QAE@XZ
    ARTS_IMPORT mmLinearGauge();

    // ??1mmLinearGauge@@QAE@XZ
    ARTS_IMPORT ~mmLinearGauge();

    // ?Draw@mmLinearGauge@@QAEXXZ
    ARTS_IMPORT void Draw();

    // ?Init@mmLinearGauge@@QAEXPADPAM1HPAVmmExternalView@@@Z
    ARTS_IMPORT void Init(char* capacity, f32* arg2, f32* arg3, i32 arg4, mmExternalView* arg5);

    // ?InitOverlay@mmLinearGauge@@QAEXPAD@Z
    ARTS_IMPORT void InitOverlay(char* name);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    f32* MinValue; // 0x000
    f32* MaxValue; // 0x004
    i32 DestX; // 0x008
    i32 DestY; // 0x00C
    gfxBitmap* BitmapGauge; // 0x010
    gfxBitmap* BitmapLabel; // 0x014
    i32 DimensionType; // 0x018
    mmExternalView* ExternalView; // 0x01C
};

check_size(mmLinearGauge, 0x20);
