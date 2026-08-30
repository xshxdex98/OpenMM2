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

class mmGfxCFG
{
public:
    // ??0mmGfxCFG@@QAE@XZ
    ARTS_IMPORT mmGfxCFG();

    // ?Get@mmGfxCFG@@QAEXXZ
    ARTS_IMPORT void Get();

    // ?Set@mmGfxCFG@@QAEXXZ
    ARTS_IMPORT void Set();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u32 field_0; // 0x000
    u32 ObjectDetail; // 0x004
    u32 field_8; // 0x008
    u32 UseEnvMap; // 0x00C
    u32 CloudShadows; // 0x010
    u32 EnableSky; // 0x014
    f32 FarClip; // 0x018
    f32 LightQuality; // 0x01C
    f32 field_20; // 0x020
    u32 field_24; // 0x024
    u32 field_28; // 0x028
    u32 field_2C; // 0x02C
    u32 field_30; // 0x030
    u32 field_34; // 0x034
};

check_size(mmGfxCFG, 0x38);
