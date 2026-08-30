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
class dgPath;

class gizPathspline
{
public:
    // ??0gizPathspline@@QAE@XZ
    ARTS_IMPORT gizPathspline();

    // ??1gizPathspline@@QAE@XZ
    ARTS_IMPORT ~gizPathspline();

    // ?GetCurrRatio@gizPathspline@@QAEMXZ
    ARTS_IMPORT f32 GetCurrRatio();

    // ?GetNumVertex@gizPathspline@@QAEHXZ
    ARTS_IMPORT i32 GetNumVertex();

    // ?GetVertex@gizPathspline@@QAEAAVVector3@@H@Z
    ARTS_IMPORT Vector3& GetVertex(i32 arg1);

    // ?Init@gizPathspline@@QAEXPAVdgPath@@M@Z
    ARTS_IMPORT void Init(dgPath* arg1, f32 arg2);

    // ?Reset@gizPathspline@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?SetSpeed@gizPathspline@@QAEXM@Z
    ARTS_IMPORT void SetSpeed(f32 arg1);

    // ?Update@gizPathspline@@QAEXAAVVector3@@0M@Z
    ARTS_IMPORT void Update(Vector3& arg1, Vector3& arg2, f32 arg3);

    // ?UpdateRatio@gizPathspline@@QAEXAAVVector3@@0M@Z
    ARTS_IMPORT void UpdateRatio(Vector3& arg1, Vector3& arg2, f32 arg3);

protected:
    // ?Compute@gizPathspline@@IAEXAAVVector3@@000@Z
    ARTS_IMPORT void Compute(Vector3& arg1, Vector3& arg2, Vector3& arg3, Vector3& arg4);

    // ?ComputePath@gizPathspline@@IAEXHHH@Z
    ARTS_IMPORT void ComputePath(i32 arg1, i32 name, i32 arg3);

    // ?DecrementPath@gizPathspline@@IAEXXZ
    ARTS_IMPORT void DecrementPath();

    // ?IncrementPath@gizPathspline@@IAEXXZ
    ARTS_IMPORT void IncrementPath();

    // ?Solve@gizPathspline@@IAEXAAVVector3@@0M@Z
    ARTS_IMPORT void Solve(Vector3& name, Vector3& arg2, f32 arg3);
};

// check_size(gizPathspline, 0x0); // TODO: no layout in the IDB type library
