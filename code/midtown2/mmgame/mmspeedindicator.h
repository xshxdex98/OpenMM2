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
class vehCarSim;

class mmSpeedIndicator
{
public:
    // ??0mmSpeedIndicator@@QAE@XZ
    ARTS_IMPORT mmSpeedIndicator();

    // ?Draw@mmSpeedIndicator@@QAEXXZ
    ARTS_IMPORT void Draw();

    // ?Init@mmSpeedIndicator@@QAEXPAVmmExternalView@@PAVvehCarSim@@@Z
    ARTS_IMPORT void Init(mmExternalView* sz, vehCarSim* arg2);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    gfxBitmap [10]* BitmapSpeeds; // 0x000
    u8[36] pad_4; // 0x004
    i32 DestX; // 0x028
    i32 DestY; // 0x02C
    mmExternalView* ExternalView; // 0x030
    vehCarSim* CarSim; // 0x034
};

check_size(mmSpeedIndicator, 0x38);
