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
class Vector4;
class aiPath;

class aiRailSet
{
public:
    // ??0aiRailSet@@QAE@XZ
    ARTS_IMPORT aiRailSet();

    // ??1aiRailSet@@QAE@XZ
    ARTS_IMPORT ~aiRailSet();

    // ?CalcRailPosition@aiRailSet@@QAEXAAVVector3@@M@Z
    ARTS_IMPORT void CalcRailPosition(Vector3& arg1, f32 arg2);

    // ?CalcRailPosOrient@aiRailSet@@QAEXAAVVector3@@0M@Z
    ARTS_IMPORT void CalcRailPosOrient(Vector3& arg1, Vector3& arg2, f32 arg3);

    // ?CalcXZDirection@aiRailSet@@QAEXAAVVector3@@0000M@Z
    ARTS_IMPORT void CalcXZDirection(Vector3& arg1, Vector3& arg2, Vector3& arg3, Vector3& arg4, Vector3& arg5, f32 arg6);

    // ?CalcXZPosition@aiRailSet@@QAEXAAVVector3@@0000M@Z
    ARTS_IMPORT void CalcXZPosition(Vector3& arg1, Vector3& arg2, Vector3& arg3, Vector3& arg4, Vector3& arg5, f32 arg6);

    // ?CalcXZPosOrient@aiRailSet@@QAEXAAVVector3@@00000M@Z
    ARTS_IMPORT void CalcXZPosOrient(Vector3& arg1, Vector3& arg2, Vector3& arg3, Vector3& arg4, Vector3& arg5, Vector3& arg6, f32 arg7);

    // ?ComputeXZCurve@aiRailSet@@QAEXAAVVector3@@000@Z
    ARTS_IMPORT void ComputeXZCurve(Vector3& arg1, Vector3& arg2, Vector3& arg3, Vector3& arg4);

    // ?ComputeXZCurve@aiRailSet@@QAEXAAVVector3@@000AAVVector4@@1@Z
    ARTS_IMPORT void ComputeXZCurve(Vector3& arg1, Vector3& arg2, Vector3& arg3, Vector3& arg4, Vector4& arg5, Vector4& arg6);

    // ?Dump@aiRailSet@@QAEXXZ
    ARTS_IMPORT void Dump();

    // ?ReplayDebug@aiRailSet@@QAEXXZ
    ARTS_IMPORT void ReplayDebug();

    // ?Reset@aiRailSet@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?SolveTurnType@aiRailSet@@QAEHPAVaiPath@@0@Z
    ARTS_IMPORT i32 SolveTurnType(aiPath* arg1, aiPath* arg2);

    // ?SolveXZCurve@aiRailSet@@QAEXAAVVector3@@0M@Z
    ARTS_IMPORT void SolveXZCurve(Vector3& arg1, Vector3& arg2, f32 arg3);

    // ?SolveXZPosition@aiRailSet@@QAEXAAVVector3@@M@Z
    ARTS_IMPORT void SolveXZPosition(Vector3& arg1, f32 arg2);
};

// check_size(aiRailSet, 0x80); // size known, members are not - cannot verify
