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
    ARTS_IMPORT void CalcRailPosition(Vector3& DrawList, f32 __formal);

    // ?CalcRailPosOrient@aiRailSet@@QAEXAAVVector3@@0M@Z
    ARTS_IMPORT void CalcRailPosOrient(Vector3& DrawList, Vector3& __formal, f32 uv);

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
    ARTS_IMPORT i32 SolveTurnType(aiPath* DrawList, aiPath* __formal);

    // ?SolveXZCurve@aiRailSet@@QAEXAAVVector3@@0M@Z
    ARTS_IMPORT void SolveXZCurve(Vector3& arg1, Vector3& arg2, f32 arg3);

    // ?SolveXZPosition@aiRailSet@@QAEXAAVVector3@@M@Z
    ARTS_IMPORT void SolveXZPosition(Vector3& DrawList, f32 __formal);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    f32 BackBumperDist; // 0x000
    f32 FrontBumperDist; // 0x004
    f32 LSideDist; // 0x008
    f32 RSideDist; // 0x00C
    i16 EnterInt; // 0x010
    u8[2] pad_12; // 0x012
    f32 RoadDist; // 0x014
    f32 ResumeDist; // 0x018
    f32 SubSectionDist; // 0x01C
    f32 TurnDist; // 0x020
    f32 LaneRandomness; // 0x024
    i16 WaitCount; // 0x028
    i16 SSIdx; // 0x02A
    i16 CurLane; // 0x02C
    i16 NextLane; // 0x02E
    i16 TargetLane; // 0x030
    u8[2] pad_32; // 0x032
    i32 RailType; // 0x034
    aiPath* NextLink; // 0x038
    aiPath* CurLink; // 0x03C
    i16 field_40; // 0x040
    i16 field_42; // 0x042
    f32 CurAccelFactor; // 0x044
    f32 ExheedLimit; // 0x048
    f32 TargetVelocity; // 0x04C
    f32 CarReactDist; // 0x050
    f32 VehicleAccelFactor; // 0x054
    f32 SeparationDist; // 0x058
    f32 IntersectionReactDist; // 0x05C
    f32 field_60; // 0x060
    f32 field_64; // 0x064
    f32 field_68; // 0x068
    f32 field_6C; // 0x06C
    f32 field_70; // 0x070
    f32 field_74; // 0x074
    f32 field_78; // 0x078
    f32 field_7C; // 0x07C
};

check_size(aiRailSet, 0x80);
