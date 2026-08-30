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

struct LocString;

class mmNumberFont
{
public:
    // ??0mmNumberFont@@QAE@PAD@Z
    ARTS_IMPORT mmNumberFont(char* arg1);

    // ??1mmNumberFont@@QAE@XZ
    ARTS_IMPORT ~mmNumberFont();

    // ?LoadFont@mmNumberFont@@QAEXPADHI@Z
    ARTS_IMPORT void LoadFont(char* arg1, i32 arg2, u32 arg3);

    // ?LoadLocFont@mmNumberFont@@QAEXPADPAULocString@@HI@Z
    ARTS_IMPORT void LoadLocFont(char* arg1, LocString* arg2, i32 arg3, u32 arg4);
};

// check_size(mmNumberFont, 0x0); // TODO: no layout in the IDB type library
