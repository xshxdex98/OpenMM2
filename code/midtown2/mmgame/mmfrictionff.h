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

struct IDirectInputDevice2A;

class mmFrictionFF
{
public:
    // ?Init@mmFrictionFF@@UAEHPAUIDirectInputDevice2A@@@Z
    ARTS_IMPORT virtual i32 Init(IDirectInputDevice2A* getter);

    // ?Play@mmFrictionFF@@UAEHXZ
    ARTS_IMPORT virtual i32 Play();

    // ?Stop@mmFrictionFF@@UAEHXZ
    ARTS_IMPORT virtual i32 Stop();

    // ?SetValues@mmFrictionFF@@UAEHMM@Z
    ARTS_IMPORT virtual i32 SetValues(f32 getter, f32 transformer);

    // ?Assign@mmFrictionFF@@QAEHJ@Z
    ARTS_IMPORT i32 Assign(ilong getter);
};

// ??_7mmFrictionFF@@6B@
// vtable at 0x005B529C
// check_size(mmFrictionFF, 0x0); // TODO: no layout in the IDB type library
