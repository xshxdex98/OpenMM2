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

#include "phbound.h"

class Matrix34;
class Vector3;
class Vector4;
class phBoundSphere;
class phColliderBase;
class phImpactBase;
class phIntersection;
class phIntersectionPoint;
class phSegment;

class phBoundPolygonal : public phBound
{
public:
    struct DispSegment;
    struct Segment;

    // ?CenterBound@phBoundPolygonal@@UAEXXZ
    ARTS_IMPORT virtual void CenterBound();

    // ?TestProbePoint@phBoundPolygonal@@UBE_NAAVphSegment@@PAVphIntersectionPoint@@M@Z
    ARTS_IMPORT virtual bool TestProbePoint(phSegment& arg1, phIntersectionPoint* arg2, f32 arg3) const;

    // ?TestAIPoint@phBoundPolygonal@@UBE_NAAVphSegment@@PAVphIntersectionPoint@@@Z
    ARTS_IMPORT virtual bool TestAIPoint(phSegment& arg1, phIntersectionPoint* arg2) const;

    // ?TestEdge@phBoundPolygonal@@UBEHAAVphSegment@@PAVphIntersection@@H@Z
    ARTS_IMPORT virtual i32 TestEdge(phSegment& arg1, phIntersection* arg2, i32 arg3) const;

    // ?TestProbe@phBoundPolygonal@@UBE_NAAVphSegment@@PAVphIntersection@@M@Z
    ARTS_IMPORT virtual bool TestProbe(phSegment& arg1, phIntersection* arg2, f32 arg3) const;

    // ?TestSphere@phBoundPolygonal@@UBE_NABVVector3@@MAAV2@1AAM@Z
    ARTS_IMPORT virtual bool TestSphere(const Vector3& arg1, f32 arg2, Vector3& arg3, Vector3& arg4, f32& arg5) const;

    // ?GetVertex@phBoundPolygonal@@UBEABVVector3@@H@Z
    ARTS_IMPORT virtual const Vector3& GetVertex(i32 arg1) const;

    // ?TestBoundPolyPoly@phBoundPolygonal@@UBEHPBV1@PBVMatrix34@@111PAVphColliderBase@@2PAVphIntersection@@3HPAH4ABVVector3@@_N@Z
    ARTS_IMPORT virtual i32 TestBoundPolyPoly(const phBoundPolygonal* arg1, const Matrix34* arg2, const Matrix34* arg3, const Matrix34* arg4, const Matrix34* arg5, phColliderBase* arg6, phColliderBase* arg7, phIntersection* arg8, phIntersection* arg9, i32 arg10, i32* arg11, i32* arg12, const Vector3& arg13, bool arg14) const;

    // ?TestBoundPolyPolyUseDot@phBoundPolygonal@@UBEHPBV1@PAVphColliderBase@@PBVMatrix34@@222PAVphIntersection@@HPAHMPBVVector3@@_N@Z
    ARTS_IMPORT virtual i32 TestBoundPolyPolyUseDot(const phBoundPolygonal* arg1, phColliderBase* arg2, const Matrix34* arg3, const Matrix34* arg4, const Matrix34* arg5, const Matrix34* arg6, phIntersection* arg7, i32 arg8, i32* arg9, f32 arg10, const Vector3* arg11, bool arg12) const;

    // ?TestBoundPolyPolyUseDotSmall@phBoundPolygonal@@UBEHPBV1@PAVphColliderBase@@PBVMatrix34@@222PAVphIntersection@@PAHMABVVector3@@_N@Z
    ARTS_IMPORT virtual i32 TestBoundPolyPolyUseDotSmall(const phBoundPolygonal* arg1, phColliderBase* arg2, const Matrix34* arg3, const Matrix34* arg4, const Matrix34* arg5, const Matrix34* arg6, phIntersection* arg7, i32* arg8, f32 arg9, const Vector3& arg10, bool arg11) const;

    // ?BackupAbyPenetration@phBoundPolygonal@@SAXAAVphSegment@@@Z
    ARTS_IMPORT static void BackupAbyPenetration(phSegment& plot_type);

    // ?FindImpactsPolyToPoly@phBoundPolygonal@@QBEHPBV1@PBVMatrix34@@111PAVphColliderBase@@2PAVphIntersection@@3PAVphImpactBase@@HHPAH5@Z
    ARTS_IMPORT i32 FindImpactsPolyToPoly(const phBoundPolygonal* arg1, const Matrix34* arg2, const Matrix34* arg3, const Matrix34* arg4, const Matrix34* arg5, phColliderBase* arg6, phColliderBase* arg7, phIntersection* arg8, phIntersection* arg9, phImpactBase* arg10, i32 arg11, i32 arg12, i32* arg13, i32* arg14) const;

    // ?FindImpactsSphereToPoly@phBoundPolygonal@@QBEHPBVphBoundSphere@@PBVMatrix34@@1PAVphColliderBase@@2PAVphImpactBase@@HABVVector3@@4@Z
    ARTS_IMPORT i32 FindImpactsSphereToPoly(const phBoundSphere* arg1, const Matrix34* arg2, const Matrix34* arg3, phColliderBase* arg4, phColliderBase* arg5, phImpactBase* arg6, i32 arg7, const Vector3& arg8, const Vector3& arg9) const;

    // ?GetAllSegments@phBoundPolygonal@@QBEXMPAUDispSegment@1@PAUSegment@1@ABVMatrix34@@2MABVVector3@@PAEPAVVector4@@AAH6@Z
    ARTS_IMPORT void GetAllSegments(f32 arg1, phBoundPolygonal::DispSegment* arg2, phBoundPolygonal::Segment* arg3, const Matrix34& arg4, const Matrix34& arg5, f32 arg6, const Vector3& arg7, u8* arg8, Vector4* arg9, i32& arg10, i32& arg11) const;

    // ?GetNextSegment@phBoundPolygonal@@QBEHAAVphSegment@@PAJPBVMatrix34@@2AAH3@Z
    ARTS_IMPORT i32 GetNextSegment(phSegment& arg1, ilong* arg2, const Matrix34* arg3, const Matrix34* arg4, i32& arg5, i32& arg6) const;

    // ?GetNextSegment@phBoundPolygonal@@QBEHAAVphSegment@@PAJPBVMatrix34@@2MPBVVector3@@PAEPAV4@AAH6@Z
    ARTS_IMPORT i32 GetNextSegment(phSegment& arg1, ilong* arg2, const Matrix34* arg3, const Matrix34* arg4, f32 arg5, const Vector3* arg6, u8* arg7, Vector3* arg8, i32& arg9, i32& arg10) const;

    // ?MaxDot@phBoundPolygonal@@QBEMABVVector3@@ABVMatrix34@@AAV2@@Z
    ARTS_IMPORT f32 MaxDot(const Vector3& arg1, const Matrix34& arg2, Vector3& arg3) const;

    // ?MinDot@phBoundPolygonal@@QBEMABVVector3@@ABVMatrix34@@AAV2@@Z
    ARTS_IMPORT f32 MinDot(const Vector3& arg1, const Matrix34& arg2, Vector3& arg3) const;

    // ?RewindSegments@phBoundPolygonal@@QBEX_NPBVMatrix34@@111PAV2@2AAH3@Z
    ARTS_IMPORT void RewindSegments(bool arg1, const Matrix34* arg2, const Matrix34* arg3, const Matrix34* arg4, const Matrix34* arg5, Matrix34* arg6, Matrix34* arg7, i32& arg8, i32& arg9) const;

    // ?RewindSegments@phBoundPolygonal@@QBEX_NPBVMatrix34@@1PAV2@2AAH3@Z
    ARTS_IMPORT void RewindSegments(bool arg1, const Matrix34* arg2, const Matrix34* arg3, Matrix34* arg4, Matrix34* arg5, i32& arg6, i32& arg7) const;

protected:
    // ?AddInteriorEdges@phBoundPolygonal@@IBEHPAVphIntersection@@H0HH@Z
    ARTS_IMPORT i32 AddInteriorEdges(phIntersection* arg1, i32 arg2, phIntersection* arg3, i32 arg4, i32 arg5) const;

    // ?BackupDispByPenetration@phBoundPolygonal@@KAXAAVVector3@@ABV2@M@Z
    ARTS_IMPORT static void BackupDispByPenetration(Vector3& arg1, const Vector3& arg2, f32 arg3);

    // ?CheckSaveEdgeEdge@phBoundPolygonal@@KAXPAVphIntersection@@H_N1ABVVector3@@22MM1MM@Z
    ARTS_IMPORT static void CheckSaveEdgeEdge(phIntersection* arg1, i32 arg2, bool arg3, bool arg4, const Vector3& arg5, const Vector3& arg6, const Vector3& arg7, f32 arg8, f32 arg9, bool arg10, f32 arg11, f32 arg12);

    // ?DoEndPtSearch@phBoundPolygonal@@KAXPBVphBound@@PBVMatrix34@@111PAVphColliderBase@@2HPAVphIntersection@@HPAPAVphImpactBase@@AAH_N@Z
    ARTS_IMPORT static void DoEndPtSearch(const phBound* arg1, const Matrix34* arg2, const Matrix34* arg3, const Matrix34* arg4, const Matrix34* arg5, phColliderBase* arg6, phColliderBase* arg7, i32 arg8, phIntersection* arg9, i32 arg10, phImpactBase** arg11, i32& arg12, bool arg13);

    // ?FindImpacts@phBoundPolygonal@@IBEHPBVphBound@@PBVMatrix34@@111PAVphColliderBase@@2PAVphIntersection@@3HHPAVphImpactBase@@H@Z
    ARTS_IMPORT i32 FindImpacts(const phBound* arg1, const Matrix34* arg2, const Matrix34* arg3, const Matrix34* arg4, const Matrix34* arg5, phColliderBase* arg6, phColliderBase* arg7, phIntersection* arg8, phIntersection* arg9, i32 arg10, i32 arg11, phImpactBase* arg12, i32 arg13) const;

    // ?GetCollideEdgePoly@phBoundPolygonal@@KA_NPBVphIntersection@@ABVVector3@@ABVMatrix34@@21PAV3@3PAMPAHPA_N@Z
    ARTS_IMPORT static bool GetCollideEdgePoly(const phIntersection* arg1, const Vector3& arg2, const Matrix34& arg3, const Matrix34& arg4, const Vector3& arg5, Vector3* arg6, Vector3* arg7, f32* arg8, i32* arg9, bool* arg10);

    // ?GetNextEdgeIsect@phBoundPolygonal@@KAXAAHAAPAVphIntersection@@_N@Z
    ARTS_IMPORT static void GetNextEdgeIsect(i32& arg1, phIntersection*& arg2, bool arg3);

    // ?MakeBsInside@phBoundPolygonal@@KAXPAVphIntersection@@H@Z
    ARTS_IMPORT static void MakeBsInside(phIntersection* arg1, i32 arg2);

    // ?ResetVertNeedsH@phBoundPolygonal@@KAXPAVphIntersection@@0_N1@Z
    ARTS_IMPORT static void ResetVertNeedsH(phIntersection* arg1, phIntersection* arg2, bool arg3, bool arg4);

    // ?ResetVertNeedsH@phBoundPolygonal@@KAXPAVphIntersection@@0_N1M@Z
    ARTS_IMPORT static void ResetVertNeedsH(phIntersection* arg1, phIntersection* arg2, bool arg3, bool arg4, f32 arg5);

    // ?RetryVertPolyCollide@phBoundPolygonal@@KAXPBVphBound@@PAVphColliderBase@@1PAVphIntersection@@HPAPAVphImpactBase@@PAH_N@Z
    ARTS_IMPORT static void RetryVertPolyCollide(const phBound* v, phColliderBase* mn, phColliderBase* b, phIntersection* arg4, i32 arg5, phImpactBase** arg6, i32* mx, bool arg8);
};

// ??_7phBoundPolygonal@@6B@
// vtable at 0x005B2158
// check_size(phBoundPolygonal, 0x4C); // size known, members are not - cannot verify
