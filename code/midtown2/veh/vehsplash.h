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

class Vector3;
class phInertialCS;

class vehSplash : public asNode
{
public:
    // ??0vehSplash@@QAE@XZ
    ARTS_IMPORT vehSplash();

    // ??1vehSplash@@UAE@XZ
    ARTS_IMPORT virtual ~vehSplash();

    // ?Update@vehSplash@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@vehSplash@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Activate@vehSplash@@QAEXM@Z
    ARTS_IMPORT void Activate(f32 arg1);

    // ?Init@vehSplash@@QAEXPAVphInertialCS@@ABVVector3@@1@Z
    ARTS_IMPORT void Init(phInertialCS* arg1, const Vector3& arg2, const Vector3& arg3);
};

// ??_7vehSplash@@6B@
// vtable at 0x005B2EEC
// check_size(vehSplash, 0x18); // size known, members are not - cannot verify
