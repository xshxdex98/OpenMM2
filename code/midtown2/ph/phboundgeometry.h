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

#include "phboundpolygonal.h"

class Vector3;
class phHotEdge;
class phMaterial;
class phPolyPair;
class phPolygon;

class phBoundGeometry : public phBoundPolygonal
{
public:
    // ??0phBoundGeometry@@QAE@XZ
    ARTS_IMPORT phBoundGeometry();

    // ??1phBoundGeometry@@QAE@XZ
    ARTS_IMPORT ~phBoundGeometry();

    // ?CalculatePolyNormals@phBoundGeometry@@QAEXXZ
    ARTS_IMPORT void CalculatePolyNormals();

    // ?ComputeEdgeNormals@phBoundGeometry@@QAEXXZ
    ARTS_IMPORT void ComputeEdgeNormals();

    // ?ComputeEdgeNums@phBoundGeometry@@QAEXXZ
    ARTS_IMPORT void ComputeEdgeNums();

    // ?ComputeEdges@phBoundGeometry@@QAEXXZ
    ARTS_IMPORT void ComputeEdges();

    // ?GetEdgeCosine@phBoundGeometry@@UBEMH@Z
    ARTS_IMPORT virtual f32 GetEdgeCosine(i32 arg1) const;

    // ?GetEdgeNormal@phBoundGeometry@@UBEABVVector3@@HAAV2@@Z
    ARTS_IMPORT virtual const Vector3& GetEdgeNormal(i32 arg1, Vector3& arg2) const;

    // ?GetMaterial@phBoundGeometry@@UBEPBVphMaterial@@H@Z
    ARTS_IMPORT virtual const phMaterial* GetMaterial(i32 arg1) const;

    // ?GhostSection@phBoundGeometry@@QAE_NPBV1@PBVVector3@@HMM@Z
    ARTS_IMPORT bool GhostSection(const phBoundGeometry* arg1, const Vector3* arg2, i32 arg3, f32 arg4, f32 arg5);

    // ?Init@phBoundGeometry@@QAEXHHHH@Z
    ARTS_IMPORT void Init(i32 arg1, i32 arg2, i32 arg3, i32 arg4);

    // ?Load@phBoundGeometry@@QAE_NPBDPBVVector3@@@Z
    ARTS_IMPORT bool Load(const char* arg1, const Vector3* arg2);

    // ?LoadBinary@phBoundGeometry@@QAE_NPBD@Z
    ARTS_IMPORT bool LoadBinary(const char* arg1);

    // ?OverlapRegion@phBoundGeometry@@QAE_NPBVVector3@@HMM@Z
    ARTS_IMPORT bool OverlapRegion(const Vector3* arg1, i32 arg2, f32 arg3, f32 arg4);

    // ?PostLoadCompute@phBoundGeometry@@UAEXXZ
    ARTS_IMPORT virtual void PostLoadCompute();

    // ?ReComputeEdgeNormals@phBoundGeometry@@QAEXXZ
    ARTS_IMPORT void ReComputeEdgeNormals();

    // ?ScaleSize@phBoundGeometry@@QAEXMMM@Z
    ARTS_IMPORT void ScaleSize(f32 arg1, f32 arg2, f32 arg3);

    // ?SetQuickTestInfo@phBoundGeometry@@QAEXXZ
    ARTS_IMPORT void SetQuickTestInfo();

    // ?ShiftCentroid@phBoundGeometry@@QAEXABVVector3@@@Z
    ARTS_IMPORT void ShiftCentroid(const Vector3& arg1);

protected:
    // ?EdgeInList@phBoundGeometry@@KAJHHJPAVphHotEdge@@@Z
    ARTS_IMPORT static ilong EdgeInList(i32 arg1, i32 arg2, ilong arg3, phHotEdge* arg4);

public:
    // Members from 0x04C; everything below that belongs to phBoundPolygonal.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u32 VertCount; // 0x04C
    u32 PolyCount; // 0x050
    u32 EdgeCount; // 0x054
    Vector3* Verts2; // 0x058
    phPolygon* Polys; // 0x05C
    phPolyPair* Edges2; // 0x060
    Vector3* Verts; // 0x064
    u32* Materials; // 0x068
    phPolyPair* Edges; // 0x06C
    u32 field_70; // 0x070
    u32 field_74; // 0x074
    u8 field_78; // 0x078
};

// ??_7phBoundGeometry@@6B@
// vtable at 0x005B2248
check_size(phBoundGeometry, 0x7C);
