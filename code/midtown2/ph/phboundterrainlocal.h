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

#include "phboundterrain.h"

class Matrix34;
class Vector3;
class phBoundHotdog;
class phBoundPolygonal;
class phColliderBase;
class phImpactBase;
class phIntersection;

class phBoundTerrainLocal : public phBoundTerrain
{
public:
    // ??0phBoundTerrainLocal@@QAE@XZ
    ARTS_IMPORT phBoundTerrainLocal();

    // ?FindImpactsHotdogToTerrainLocal@phBoundTerrainLocal@@QBEHPBVphBoundHotdog@@PBVMatrix34@@1PAVphColliderBase@@2PAVphImpactBase@@HABVVector3@@4@Z
    ARTS_IMPORT i32 FindImpactsHotdogToTerrainLocal(const phBoundHotdog* arg1, const Matrix34* arg2, const Matrix34* arg3, phColliderBase* arg4, phColliderBase* arg5, phImpactBase* arg6, i32 arg7, const Vector3& arg8, const Vector3& arg9) const;

    // ?TestBoundPolyTerrain@phBoundTerrainLocal@@QBEHPBVphBoundPolygonal@@PAVphColliderBase@@PBVMatrix34@@22PAVphIntersection@@HPAHPBVVector3@@_N@Z
    ARTS_IMPORT i32 TestBoundPolyTerrain(const phBoundPolygonal* arg1, phColliderBase* arg2, const Matrix34* arg3, const Matrix34* arg4, const Matrix34* arg5, phIntersection* arg6, i32 arg7, i32* arg8, const Vector3* arg9, bool arg10) const;

    // ?TestBoundTerrainEdgesVsPoly@phBoundTerrainLocal@@QBEHPBVphBoundPolygonal@@PAVphColliderBase@@PBVMatrix34@@2PAVphIntersection@@HPAH@Z
    ARTS_IMPORT i32 TestBoundTerrainEdgesVsPoly(const phBoundPolygonal* arg1, phColliderBase* arg2, const Matrix34* arg3, const Matrix34* arg4, phIntersection* arg5, i32 arg6, i32* arg7) const;

    // ?TestBoundTerrainPoly@phBoundTerrainLocal@@QBEHPBVphBoundPolygonal@@PBVMatrix34@@11PAVphColliderBase@@2PAVphIntersection@@3HPAH4ABVVector3@@_N@Z
    ARTS_IMPORT i32 TestBoundTerrainPoly(const phBoundPolygonal* arg1, const Matrix34* arg2, const Matrix34* arg3, const Matrix34* arg4, phColliderBase* arg5, phColliderBase* arg6, phIntersection* arg7, phIntersection* arg8, i32 arg9, i32* arg10, i32* arg11, const Vector3& arg12, bool arg13) const;
};

// ??_7phBoundTerrainLocal@@6B@
// vtable at 0x005B23A4
// check_size(phBoundTerrainLocal, 0xCC); // size known, members are not - cannot verify
