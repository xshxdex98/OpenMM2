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

#include "phboundgeometry.h"

class Matrix34;
class Vector3;
class Vector4;
class phBoundHotdog;
class phBoundPolygonal;
class phBoundSphere;
class phColliderBase;
class phImpactBase;
class phIntersection;
class phIntersectionPoint;
class phSegment;

class phBoundTerrain : public phBoundGeometry
{
public:
    // ??0phBoundTerrain@@QAE@XZ
    ARTS_IMPORT phBoundTerrain();

    // ??1phBoundTerrain@@QAE@XZ
    ARTS_IMPORT ~phBoundTerrain();

    // ?TestProbePoint@phBoundTerrain@@UBE_NAAVphSegment@@PAVphIntersectionPoint@@M@Z
    ARTS_IMPORT virtual bool TestProbePoint(phSegment& arg1, phIntersectionPoint* arg2, f32 arg3) const;

    // ?TestAIPoint@phBoundTerrain@@UBE_NAAVphSegment@@PAVphIntersectionPoint@@@Z
    ARTS_IMPORT virtual bool TestAIPoint(phSegment& arg1, phIntersectionPoint* arg2) const;

    // ?TestEdge@phBoundTerrain@@UBEHAAVphSegment@@PAVphIntersection@@H@Z
    ARTS_IMPORT virtual i32 TestEdge(phSegment& arg1, phIntersection* arg2, i32 arg3) const;

    // ?TestProbe@phBoundTerrain@@UBE_NAAVphSegment@@PAVphIntersection@@M@Z
    ARTS_IMPORT virtual bool TestProbe(phSegment& arg1, phIntersection* arg2, f32 arg3) const;

    // ?TestSphere@phBoundTerrain@@UBE_NABVVector3@@MAAV2@1AAM@Z
    ARTS_IMPORT virtual bool TestSphere(const Vector3& arg1, f32 arg2, Vector3& arg3, Vector3& arg4, f32& arg5) const;

    // ?PostLoadCompute@phBoundTerrain@@UAEXXZ
    ARTS_IMPORT virtual void PostLoadCompute();

    // ?ClearPolyTouched@phBoundTerrain@@QBEXXZ
    ARTS_IMPORT void ClearPolyTouched() const;

    // ?FindImpactsHotdogToTerrain@phBoundTerrain@@QBEHPBVphBoundHotdog@@PBVMatrix34@@1PAVphColliderBase@@2PAVphImpactBase@@HABVVector3@@4@Z
    ARTS_IMPORT i32 FindImpactsHotdogToTerrain(const phBoundHotdog* arg1, const Matrix34* arg2, const Matrix34* arg3, phColliderBase* arg4, phColliderBase* arg5, phImpactBase* arg6, i32 arg7, const Vector3& arg8, const Vector3& arg9) const;

    // ?FindImpactsSphereToTerrain@phBoundTerrain@@QBEHPBVphBoundSphere@@PBVMatrix34@@1PAVphColliderBase@@2PAVphImpactBase@@HABVVector3@@4@Z
    ARTS_IMPORT i32 FindImpactsSphereToTerrain(const phBoundSphere* arg1, const Matrix34* arg2, const Matrix34* arg3, phColliderBase* arg4, phColliderBase* arg5, phImpactBase* arg6, i32 arg7, const Vector3& arg8, const Vector3& arg9) const;

    // ?InitPolyIterator@phBoundTerrain@@QBEXABVVector3@@M@Z
    ARTS_IMPORT void InitPolyIterator(const Vector3& arg1, f32 arg2) const;

    // ?InitPolyIterator@phBoundTerrain@@QBEXABVphSegment@@@Z
    ARTS_IMPORT void InitPolyIterator(const phSegment& arg1) const;

    // ?Load@phBoundTerrain@@QAE_NPBD@Z
    ARTS_IMPORT bool Load(const char* arg1);

    // ?PackNormal@phBoundTerrain@@SAXAAIABVVector4@@@Z
    ARTS_IMPORT static void PackNormal(u32& arg1, const Vector4& arg2);

    // ?Save@phBoundTerrain@@QAE_NPBD@Z
    ARTS_IMPORT bool Save(const char* arg1);

    // ?SetHotEdges@phBoundTerrain@@QAEX_N@Z
    ARTS_IMPORT void SetHotEdges(bool arg1);

    // ?TestBoundPolyTerrain@phBoundTerrain@@QBEHPBVphBoundPolygonal@@PAVphColliderBase@@PBVMatrix34@@2PAVphIntersection@@HPAHPBVVector3@@_N@Z
    ARTS_IMPORT i32 TestBoundPolyTerrain(const phBoundPolygonal* arg1, phColliderBase* arg2, const Matrix34* arg3, const Matrix34* arg4, phIntersection* arg5, i32 arg6, i32* arg7, const Vector3* arg8, bool arg9) const;

    // ?TestBoundTerrainEdgesVsPoly@phBoundTerrain@@QBEHPBVphBoundPolygonal@@PAVphColliderBase@@PBVMatrix34@@PAVphIntersection@@HPAH@Z
    ARTS_IMPORT i32 TestBoundTerrainEdgesVsPoly(const phBoundPolygonal* arg1, phColliderBase* arg2, const Matrix34* arg3, phIntersection* arg4, i32 arg5, i32* arg6) const;

    // ?TestBoundTerrainPoly@phBoundTerrain@@QBEHPBVphBoundPolygonal@@PBVMatrix34@@1PAVphColliderBase@@2PAVphIntersection@@3HPAH4ABVVector3@@_N@Z
    ARTS_IMPORT i32 TestBoundTerrainPoly(const phBoundPolygonal* arg1, const Matrix34* arg2, const Matrix34* arg3, phColliderBase* arg4, phColliderBase* arg5, phIntersection* arg6, phIntersection* arg7, i32 arg8, i32* arg9, i32* arg10, const Vector3& arg11, bool arg12) const;

    // ?UnpackNormal@phBoundTerrain@@SAXAAVVector4@@I@Z
    ARTS_IMPORT static void UnpackNormal(Vector4& arg1, u32 arg2);

protected:
    // ?CalculateBuckets@phBoundTerrain@@IBEXMMMM@Z
    ARTS_IMPORT void CalculateBuckets(f32 arg1, f32 arg2, f32 arg3, f32 arg4) const;

public:
    // Members from 0x07C; everything below that belongs to phBoundGeometry.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 field_7C; // 0x07C
    u8 field_7D[15]; // 0x07D
    i32 field_8C; // 0x08C
    i32 field_90; // 0x090
    i32 field_94; // 0x094
    u8 field_98[8]; // 0x098
    i32 field_A0; // 0x0A0
    i32 field_A4; // 0x0A4
    i32 field_A8; // 0x0A8
    i32 field_AC; // 0x0AC
    i32 field_B0; // 0x0B0
    i32 field_B4; // 0x0B4
    i32 field_B8; // 0x0B8
    i32 field_BC; // 0x0BC
    i32 field_C0; // 0x0C0
    u8 field_C4[4]; // 0x0C4
    i32 field_C8; // 0x0C8
};

// ??_7phBoundTerrain@@6B@
// vtable at 0x005B2344
check_size(phBoundTerrain, 0xCC);
