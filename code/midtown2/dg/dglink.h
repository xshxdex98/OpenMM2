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

class Vector3;
class crIKGoal;
class dgLinkData;
class dgRagdoll;

class dgLink
{
public:
    // ??0dgLink@@QAE@XZ
    ARTS_IMPORT dgLink();

    // ?Bounce@dgLink@@QAEXAAVVector3@@0@Z
    ARTS_IMPORT void Bounce(Vector3& arg1, Vector3& arg2);

    // ?Init@dgLink@@QAEXAAVdgLinkData@@AAVcrIKGoal@@AAVVector3@@AAVdgRagdoll@@@Z
    ARTS_IMPORT void Init(dgLinkData& arg1, crIKGoal& arg2, Vector3& arg3, dgRagdoll& arg4);

    // ?Reset@dgLink@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?ResetGoal@dgLink@@QAEXXZ
    ARTS_IMPORT void ResetGoal();

    // ?SetMode@dgLink@@QAEXH@Z
    ARTS_IMPORT void SetMode(i32 arg1);

    // ?Update@dgLink@@QAEXXZ
    ARTS_IMPORT void Update();

    // ?UpdateDamping@dgLink@@QAEXXZ
    ARTS_IMPORT void UpdateDamping();

    // ?UpdateFree@dgLink@@QAEXXZ
    ARTS_IMPORT void UpdateFree();

    // ?UpdateGrasp@dgLink@@QAEXXZ
    ARTS_IMPORT void UpdateGrasp();

    // ?UpdateTendon@dgLink@@QAEXXZ
    ARTS_IMPORT void UpdateTendon();
};

// check_size(dgLink, 0x0); // TODO: no layout in the IDB type library
