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

class mmVehInfo
{
public:
    // ??0mmVehInfo@@QAE@XZ
    ARTS_IMPORT mmVehInfo();

    // ??1mmVehInfo@@UAE@XZ
    ARTS_IMPORT virtual ~mmVehInfo();

    // ?ComputeTuningCRC@mmVehInfo@@QAEXXZ
    ARTS_IMPORT void ComputeTuningCRC();

    // ?GetTuningCRC@mmVehInfo@@QAEKXZ
    ARTS_IMPORT ulong GetTuningCRC();

    // ?HasColorVariations@mmVehInfo@@QAEHXZ
    ARTS_IMPORT i32 HasColorVariations();

    // ?IsValid@mmVehInfo@@QAEHXZ
    ARTS_IMPORT i32 IsValid();

    // ?Load@mmVehInfo@@QAEHPAD@Z
    ARTS_IMPORT i32 Load(char* arg1);

    // ?Print@mmVehInfo@@QAEXXZ
    ARTS_IMPORT void Print();
};

// ??_7mmVehInfo@@6B@
// vtable at 0x005B5248
// check_size(mmVehInfo, 0x124); // size known, members are not - cannot verify
