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

class ccStatus : public asNode
{
public:
    // ??0ccStatus@@QAE@XZ
    ARTS_IMPORT ccStatus();

    // ??1ccStatus@@UAE@XZ
    ARTS_IMPORT virtual ~ccStatus();

    // ?Cull@ccStatus@@UAEXXZ
    ARTS_IMPORT virtual void Cull();

    // ?Update@ccStatus@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?LoadBitmap@ccStatus@@QAEXPADHHH@Z
    ARTS_IMPORT void LoadBitmap(char* name, i32 arg2, i32 arg3, i32 arg4);

    // ?SetStatus@ccStatus@@QAEXF@Z
    ARTS_IMPORT void SetStatus(i16 arg1);
};

// ??_7ccStatus@@6B@
// vtable at 0x005B43DC
// check_size(ccStatus, 0x18); // size known, members are not - cannot verify
