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

#include "vector7/vector3.h"

#include "phboundpolygonal.h"
#include "phpolygon.h"

class Matrix34;
class phBoundSphere;
class phColliderBase;
class phImpactBase;
class phIntersection;
class phIntersectionPoint;
class phMaterial;
class phSegment;

class phBoundBox : public phBoundPolygonal
{
public:
    // ??0phBoundBox@@QAE@ABVVector3@@@Z
    ARTS_IMPORT phBoundBox(const Vector3& arg1);

    // ??0phBoundBox@@QAE@XZ
    ARTS_IMPORT phBoundBox();

    // ??1phBoundBox@@QAE@XZ
    ARTS_IMPORT ~phBoundBox();

    // ?GetMaterial@phBoundBox@@UBEPBVphMaterial@@H@Z
    ARTS_IMPORT virtual const phMaterial* GetMaterial(i32 arg1) const;

    // ?TestProbePoint@phBoundBox@@UBE_NAAVphSegment@@PAVphIntersectionPoint@@M@Z
    ARTS_IMPORT virtual bool TestProbePoint(phSegment& id, phIntersectionPoint* pos, f32 dock_node) const;

    // ?TestAIPoint@phBoundBox@@UBE_NAAVphSegment@@PAVphIntersectionPoint@@@Z
    ARTS_IMPORT virtual bool TestAIPoint(phSegment& arg1, phIntersectionPoint* arg2) const;

    // ?TestEdge@phBoundBox@@UBEHAAVphSegment@@PAVphIntersection@@H@Z
    ARTS_IMPORT virtual i32 TestEdge(phSegment& label, phIntersection* size_arg, i32 flags) const;

    // ?TestProbe@phBoundBox@@UBE_NAAVphSegment@@PAVphIntersection@@M@Z
    ARTS_IMPORT virtual bool TestProbe(phSegment& arg1, phIntersection* arg2, f32 arg3) const;

    // ?GetEdgeCosine@phBoundBox@@UBEMH@Z
    ARTS_IMPORT virtual f32 GetEdgeCosine(i32 arg1) const;

    // ?GetEdgeNormal@phBoundBox@@UBEABVVector3@@HAAV2@@Z
    ARTS_IMPORT virtual const Vector3& GetEdgeNormal(i32 arg1, Vector3& arg2) const;

    // ?CreateOffset@phBoundBox@@QAEXABVVector3@@@Z
    ARTS_IMPORT void CreateOffset(const Vector3& arg1);

    // ?FindImpactsBoxToBox@phBoundBox@@QBEHPBV1@PBVMatrix34@@111PAVphColliderBase@@2PAVphImpactBase@@HABVVector3@@@Z
    ARTS_IMPORT i32 FindImpactsBoxToBox(const phBoundBox* arg1, const Matrix34* arg2, const Matrix34* arg3, const Matrix34* arg4, const Matrix34* arg5, phColliderBase* arg6, phColliderBase* arg7, phImpactBase* arg8, i32 arg9, const Vector3& arg10) const;

    // ?FindImpactsBoxToBoxOffset@phBoundBox@@QBEHPBV1@PBVMatrix34@@111PAVphColliderBase@@2PAVphImpactBase@@HABVVector3@@@Z
    ARTS_IMPORT i32 FindImpactsBoxToBoxOffset(const phBoundBox* arg1, const Matrix34* arg2, const Matrix34* arg3, const Matrix34* arg4, const Matrix34* arg5, phColliderBase* arg6, phColliderBase* arg7, phImpactBase* arg8, i32 arg9, const Vector3& arg10) const;

    // ?FindImpactSphereToBox@phBoundBox@@QBEHPBVphBoundSphere@@ABVMatrix34@@1PAVphColliderBase@@2PAVphImpactBase@@ABVVector3@@4@Z
    ARTS_IMPORT i32 FindImpactSphereToBox(const phBoundSphere* arg1, const Matrix34& arg2, const Matrix34& arg3, phColliderBase* arg4, phColliderBase* arg5, phImpactBase* arg6, const Vector3& arg7, const Vector3& arg8) const;

    // ?Load@phBoundBox@@QAE_NPBD@Z
    ARTS_IMPORT bool Load(const char* arg1);

    // ?ProbeVsBox@phBoundBox@@SA_NABVVector3@@AAVphSegment@@PAVphIntersectionPoint@@PAJM@Z
    ARTS_IMPORT static bool ProbeVsBox(const Vector3& arg1, phSegment& arg2, phIntersectionPoint* arg3, ilong* arg4, f32 arg5);

    // ?ScaleSize@phBoundBox@@QAEXABVVector3@@@Z
    ARTS_IMPORT void ScaleSize(const Vector3& arg1);

    // ?SetQuickTestInfo@phBoundBox@@QAEXXZ
    ARTS_IMPORT void SetQuickTestInfo();

    // ?SetSize@phBoundBox@@QAEXABVVector3@@@Z
    ARTS_IMPORT void SetSize(const Vector3& arg1);

    // ?ShiftCentroid@phBoundBox@@QAEXABVVector3@@@Z
    ARTS_IMPORT void ShiftCentroid(const Vector3& arg1);

protected:
    // ?BoxToBoxFaceImpacts@phBoundBox@@IBEXABVVector3@@PAVphImpactBase@@H_NPBVMatrix34@@333PAVphColliderBase@@4@Z
    ARTS_IMPORT void BoxToBoxFaceImpacts(const Vector3& arg1, phImpactBase* arg2, i32 arg3, bool arg4, const Matrix34* arg5, const Matrix34* arg6, const Matrix34* arg7, const Matrix34* arg8, phColliderBase* arg9, phColliderBase* arg10) const;

    // ?TestProbeSlave@phBoundBox@@IBE_NAAVphSegment@@PAVphIntersectionPoint@@PAJM@Z
    ARTS_IMPORT bool TestProbeSlave(phSegment& str_id, phIntersectionPoint* dir, ilong* size, f32 arg4) const;

private:
    // ?AddEdgeChecks@phBoundBox@@CAXHH_N@Z
    ARTS_IMPORT static void AddEdgeChecks(i32 c, i32 lhs, bool rhs);

    // ?AvoidEdgeChecks@phBoundBox@@CAXHH@Z
    ARTS_IMPORT static void AvoidEdgeChecks(i32 arg1, i32 arg2);

    // ?AvoidEdgeChecks@phBoundBox@@CAXHH_N@Z
    ARTS_IMPORT static void AvoidEdgeChecks(i32 lhs, i32 rhs, bool arg3);

    // ?BoxToBoxFaceImpactsOffset@phBoundBox@@CAXABVVector3@@00PAVphImpactBase@@H_NPBVMatrix34@@333PAVphColliderBase@@4@Z
    ARTS_IMPORT static void BoxToBoxFaceImpactsOffset(const Vector3& arg1, const Vector3& arg2, const Vector3& arg3, phImpactBase* arg4, i32 arg5, bool arg6, const Matrix34* arg7, const Matrix34* arg8, const Matrix34* arg9, const Matrix34* arg10, phColliderBase* arg11, phColliderBase* arg12);

    // ?CheckFourFaceDotPattern@phBoundBox@@CA_NXZ
    ARTS_IMPORT static bool CheckFourFaceDotPattern();

    // ?FindFaceDots@phBoundBox@@CAXMPAH0@Z
    ARTS_IMPORT static void FindFaceDots(f32 count, i32* spacing, i32* clear);

    // ?MakeTransformedCorners@phBoundBox@@CAXABVVector3@@0_N@Z
    ARTS_IMPORT static void MakeTransformedCorners(const Vector3& arg1, const Vector3& arg2, bool arg3);

    // ?RemoveFaceDotZero@phBoundBox@@CAXXZ
    ARTS_IMPORT static void RemoveFaceDotZero();

    // ?RemoveFifthFaceDotZero@phBoundBox@@CAXXZ
    ARTS_IMPORT static void RemoveFifthFaceDotZero();

    // ?UseThisImpact@phBoundBox@@CA_NABVVector3@@0MPBVMatrix34@@111@Z
    ARTS_IMPORT static bool UseThisImpact(const Vector3& lhs, const Vector3& rhs, f32 arg3, const Matrix34* arg4, const Matrix34* arg5, const Matrix34* arg6, const Matrix34* arg7);

    // ?VerifyFaceDotPattern@phBoundBox@@CA_NXZ
    ARTS_IMPORT static bool VerifyFaceDotPattern();

public:
    // Members from 0x04C; everything below that belongs to phBoundPolygonal.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u32 field_4C; // 0x04C
    u32 field_50; // 0x050
    u32 field_54; // 0x054
    u32 field_58; // 0x058
    u32 field_5C; // 0x05C
    u32 field_60; // 0x060
    Vector3 field_64; // 0x064
    Vector3 vector370; // 0x070
    u8 field_7C[84]; // 0x07C
    phPolygon Polys[6]; // 0x0D0
    u32 field_310; // 0x310
};

// ??_7phBoundBox@@6B@
// vtable at 0x005B2100
check_size(phBoundBox, 0x314);
