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

class mmToggle : public asNode
{
public:
    // ??0mmToggle@@QAE@XZ
    ARTS_IMPORT mmToggle();

    // ??1mmToggle@@UAE@XZ
    ARTS_IMPORT virtual ~mmToggle();

    // ?Cull@mmToggle@@UAEXXZ
    ARTS_IMPORT virtual void Cull();

    // ?Init@mmToggle@@QAEXPADMMMM@Z
    ARTS_IMPORT void Init(char* arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5);

    // ?Update@mmToggle@@UAEXXZ
    ARTS_IMPORT virtual void Update();

private:
    // ?LoadBitmap@mmToggle@@AAEXPAD@Z
    ARTS_IMPORT void LoadBitmap(char* arg1);

    // ?SetSize@mmToggle@@AAEXMM@Z
    ARTS_IMPORT void SetSize(f32 arg1, f32 arg2);
};

// ??_7mmToggle@@6B@
// vtable at 0x005B3C6C
// check_size(mmToggle, 0x0); // TODO: no layout in the IDB type library
