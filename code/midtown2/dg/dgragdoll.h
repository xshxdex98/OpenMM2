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

class Matrix34;
class Vector3;
class crKinematics;
class dgRagdollData;

class dgRagdoll
{
public:
    // ??0dgRagdoll@@QAE@XZ
    ARTS_IMPORT dgRagdoll();

    // ??1dgRagdoll@@QAE@XZ
    ARTS_IMPORT ~dgRagdoll();

    // ?ApplyPushVelocity@dgRagdoll@@QAEXAAVVector3@@@Z
    ARTS_IMPORT void ApplyPushVelocity(Vector3& arg1);

    // ?Deactivate@dgRagdoll@@QAEXXZ
    ARTS_IMPORT void Deactivate();

    // ?GetMatrix@dgRagdoll@@QAEAAVMatrix34@@XZ
    ARTS_IMPORT Matrix34& GetMatrix();

    // ?Init@dgRagdoll@@QAEXABVdgRagdollData@@AAVcrKinematics@@PAVMatrix34@@@Z
    ARTS_IMPORT void Init(const dgRagdollData& arg0, crKinematics& arg2, Matrix34* arg3);

    // ?Reset@dgRagdoll@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?ResetGoals@dgRagdoll@@QAEXXZ
    ARTS_IMPORT void ResetGoals();

    // ?StartRagdoll@dgRagdoll@@QAEXXZ
    ARTS_IMPORT void StartRagdoll();

    // ?StopRagdoll@dgRagdoll@@QAEXXZ
    ARTS_IMPORT void StopRagdoll();

    // ?Update@dgRagdoll@@QAEXXZ
    ARTS_IMPORT void Update();
};

// check_size(dgRagdoll, 0x1); // size known, members are not - cannot verify
