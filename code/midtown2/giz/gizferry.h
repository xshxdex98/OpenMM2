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

#include "dg/dgunhitybangerinstance.h"

class dgPath;

class gizFerry : public dgUnhitYBangerInstance
{
public:
    // ??0gizFerry@@QAE@XZ
    ARTS_IMPORT gizFerry();

    // ??1gizFerry@@QAE@XZ
    ARTS_IMPORT ~gizFerry();

    // ?Reset@gizFerry@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Draw@gizFerry@@UAEXH@Z
    ARTS_IMPORT virtual void Draw(i32 arg1);

    // ?SizeOf@gizFerry@@UAEIXZ
    ARTS_IMPORT virtual u32 SizeOf();

    // ?Cull@gizFerry@@QAEXH@Z
    ARTS_IMPORT void Cull(i32 arg1);

    // ?Init@gizFerry@@QAEXPADPAVdgPath@@@Z
    ARTS_IMPORT void Init(char* arg1, dgPath* arg2);

    // ?SetSpeed@gizFerry@@QAEXM@Z
    ARTS_IMPORT void SetSpeed(f32 arg1);

    // ?Update@gizFerry@@QAEXXZ
    ARTS_IMPORT void Update();
};

// ??_7gizFerry@@6B@
// vtable at 0x005B6180
// check_size(gizFerry, 0x0); // TODO: no layout in the IDB type library
