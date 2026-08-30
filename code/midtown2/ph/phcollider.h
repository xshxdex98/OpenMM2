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

#include "phcolliderbase.h"

class InstanceData;
class Matrix34;
class phBound;
class phInertialCS;
class phSleep;

class phCollider : public phColliderBase
{
public:
    // ?Init@phCollider@@QAEXPAVMatrix34@@PBVphBound@@@Z
    ARTS_IMPORT void Init(Matrix34* atlas, const phBound* rhs);

    // ?Init@phCollider@@QAEXPBVphBound@@PAVInstanceData@@PAVphInertialCS@@PAVphSleep@@@Z
    ARTS_IMPORT void Init(const phBound* arg1, InstanceData* arg2, phInertialCS* arg3, phSleep* arg4);

    // ?Init@phCollider@@QAEXPBVphBound@@PAVMatrix34@@@Z
    ARTS_IMPORT void Init(const phBound* arg1, Matrix34* arg2);

    // ?Init@phCollider@@QAEXPBVphBound@@PAVMatrix34@@PAVphInertialCS@@PAVphSleep@@@Z
    ARTS_IMPORT void Init(const phBound* arg1, Matrix34* arg2, phInertialCS* arg3, phSleep* arg4);

    // ?Init@phCollider@@QAEXPBVphBound@@PAVphInertialCS@@PAVphSleep@@@Z
    ARTS_IMPORT void Init(const phBound* arg1, phInertialCS* arg2, phSleep* arg3);

    // ?InitArray@phCollider@@SAXQAV1@H@Z
    ARTS_IMPORT static void InitArray(phCollider*const src, i32 dst);

    // ?Reset@phCollider@@QAEXXZ
    ARTS_IMPORT void Reset();
};

// ??_7phCollider@@6B@
// vtable at 0x005B1CB0
// check_size(phCollider, 0x74); // size known, members are not - cannot verify
