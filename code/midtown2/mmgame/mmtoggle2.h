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

class mmToggle2 : public asNode
{
public:
    // ??0mmToggle2@@QAE@XZ
    ARTS_IMPORT mmToggle2();

    // ??1mmToggle2@@UAE@XZ
    ARTS_IMPORT virtual ~mmToggle2();

    // ?Cull@mmToggle2@@UAEXXZ
    ARTS_IMPORT virtual void Cull();

    // ?Update@mmToggle2@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Init@mmToggle2@@QAEXPADMMPAH@Z
    ARTS_IMPORT void Init(char* arg1, f32 arg2, f32 arg3, i32* arg4);

    // ?SetPosition@mmToggle2@@QAEXMM@Z
    ARTS_IMPORT void SetPosition(f32 arg1, f32 arg2);

private:
    // ?LoadBitmap@mmToggle2@@AAEXPAD@Z
    ARTS_IMPORT void LoadBitmap(char* arg1);
};

// ??_7mmToggle2@@6B@
// vtable at 0x005B3CA0
// check_size(mmToggle2, 0x0); // TODO: no layout in the IDB type library
