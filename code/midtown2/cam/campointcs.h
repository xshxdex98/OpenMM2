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

#include "vector7/vector3.h"

#include "camcarcs.h"

class camPointCS : public camCarCS
{
public:
    // ??0camPointCS@@QAE@XZ
    ARTS_IMPORT camPointCS();

    // ??1camPointCS@@UAE@XZ
    ARTS_IMPORT virtual ~camPointCS();

    // ?GetClassName@camPointCS@@UAEPADXZ
    ARTS_IMPORT virtual char* GetClassName();

    // ?GetPos@camPointCS@@QAE?AVVector3@@XZ
    ARTS_IMPORT Vector3 GetPos();

    // ?MakeActive@camPointCS@@UAEXXZ
    ARTS_IMPORT virtual void MakeActive();

    // ?Reset@camPointCS@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?SetAppRate@camPointCS@@QAEXM@Z
    ARTS_IMPORT void SetAppRate(f32 arg1);

    // ?SetMaxDist@camPointCS@@QAEXM@Z
    ARTS_IMPORT void SetMaxDist(f32 arg1);

    // ?SetMinDist@camPointCS@@QAEXM@Z
    ARTS_IMPORT void SetMinDist(f32 arg1);

    // ?SetPos@camPointCS@@QAEXAAVVector3@@@Z
    ARTS_IMPORT void SetPos(Vector3& arg1);

    // ?SetVel@camPointCS@@QAEXAAVVector3@@@Z
    ARTS_IMPORT void SetVel(Vector3& arg1);

    // ?Update@camPointCS@@UAEXXZ
    ARTS_IMPORT virtual void Update();
};

// ??_7camPointCS@@6B@
// vtable at 0x005B4FD8
// check_size(camPointCS, 0x13C); // size known, members are not - cannot verify
