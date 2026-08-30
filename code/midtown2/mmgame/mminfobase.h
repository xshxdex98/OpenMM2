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

class datParser;

class mmInfoBase
{
public:
    // ??0mmInfoBase@@QAE@XZ
    ARTS_IMPORT mmInfoBase();

    // ??1mmInfoBase@@UAE@XZ
    ARTS_IMPORT virtual ~mmInfoBase();

    // ?FileIO@mmInfoBase@@UAEXAAVdatParser@@@Z
    ARTS_IMPORT virtual void FileIO(datParser& arg1);

    // ?Load@mmInfoBase@@QAEHPAD@Z
    ARTS_IMPORT i32 Load(char* arg1);

    // ?Save@mmInfoBase@@QAEHPAD@Z
    ARTS_IMPORT i32 Save(char* arg1);

    // ?SetIOPath@mmInfoBase@@QAEXPAD@Z
    ARTS_IMPORT void SetIOPath(char* arg1);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 field_4[128]; // 0x004
    u8 field_84[4]; // 0x084
};

// ??_7mmInfoBase@@6B@
// vtable at 0x005B0634
check_size(mmInfoBase, 0x88);
