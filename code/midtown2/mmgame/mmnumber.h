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

class mmNumberFont;

class mmNumber : public asNode
{
public:
    // ??0mmNumber@@QAE@XZ
    ARTS_IMPORT mmNumber();

    // ??1mmNumber@@UAE@XZ
    ARTS_IMPORT virtual ~mmNumber();

    // ?Cull@mmNumber@@UAEXXZ
    ARTS_IMPORT virtual void Cull();

    // ?Update@mmNumber@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Init@mmNumber@@QAEXPAVmmNumberFont@@MM@Z
    ARTS_IMPORT void Init(mmNumberFont* arg1, f32 arg2, f32 arg3);

    // ?Printf@mmNumber@@QAAXPBDZZ
    ARTS_IMPORT void Printf(const char* arg1, ...);

    // ?SetString@mmNumber@@QAEXPAD@Z
    ARTS_IMPORT void SetString(char* arg1);
};

// ??_7mmNumber@@6B@
// vtable at 0x005B5324
// check_size(mmNumber, 0x0); // TODO: no layout in the IDB type library
