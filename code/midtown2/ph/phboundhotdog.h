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

#include "phbound.h"
#include "phmaterial.h"

class Matrix34;
class phBoundPolygonal;
class phBoundSphere;
class phBound_vtbl;
class phColliderBase;
class phImpactBase;
class phIntersection;
class phIntersectionPoint;
class phSegment;

class phBoundHotdog : public phBound
{
public:
    // ??0phBoundHotdog@@QAE@MM@Z
    ARTS_IMPORT phBoundHotdog(f32 arg1, f32 arg2);

    // ??0phBoundHotdog@@QAE@XZ
    ARTS_IMPORT phBoundHotdog();

    // ??1phBoundHotdog@@QAE@XZ
    ARTS_IMPORT ~phBoundHotdog();

    // ?GetMaterial@phBoundHotdog@@UBEPBVphMaterial@@H@Z
    ARTS_IMPORT virtual const phMaterial* GetMaterial(i32 arg1) const;

    // ?TestProbePoint@phBoundHotdog@@UBE_NAAVphSegment@@PAVphIntersectionPoint@@M@Z
    ARTS_IMPORT virtual bool TestProbePoint(phSegment& arg1, phIntersectionPoint* arg2, f32 arg3) const;

    // ?TestAIPoint@phBoundHotdog@@UBE_NAAVphSegment@@PAVphIntersectionPoint@@@Z
    ARTS_IMPORT virtual bool TestAIPoint(phSegment& flags, phIntersectionPoint* window) const;

    // ?TestEdge@phBoundHotdog@@UBEHAAVphSegment@@PAVphIntersection@@H@Z
    ARTS_IMPORT virtual i32 TestEdge(phSegment& arg1, phIntersection* arg2, i32 arg3) const;

    // ?TestProbe@phBoundHotdog@@UBE_NAAVphSegment@@PAVphIntersection@@M@Z
    ARTS_IMPORT virtual bool TestProbe(phSegment& arg1, phIntersection* arg2, f32 arg3) const;

    // ?TestSphere@phBoundHotdog@@UBE_NABVVector3@@MAAV2@1AAM@Z
    ARTS_IMPORT virtual bool TestSphere(const Vector3& arg1, f32 arg2, Vector3& arg3, Vector3& arg4, f32& arg5) const;

    // ?CalculateBoundingBox@phBoundHotdog@@QAEXXZ
    ARTS_IMPORT void CalculateBoundingBox();

    // ?CreateOffset@phBoundHotdog@@QAEXABVVector3@@@Z
    ARTS_IMPORT void CreateOffset(const Vector3& id);

    // ?FindHotdogIsectNormal@phBoundHotdog@@QBE?AVVector3@@ABV2@H@Z
    ARTS_IMPORT Vector3 FindHotdogIsectNormal(const Vector3& arg1, i32 arg2) const;

    // ?FindImpactsHotdogToHotdog@phBoundHotdog@@QBEHPBV1@PBVMatrix34@@1PAVphColliderBase@@2PAVphImpactBase@@H@Z
    ARTS_IMPORT i32 FindImpactsHotdogToHotdog(const phBoundHotdog* arg1, const Matrix34* arg2, const Matrix34* arg3, phColliderBase* arg4, phColliderBase* arg5, phImpactBase* arg6, i32 arg7) const;

    // ?FindImpactsHotdogToPoly@phBoundHotdog@@QBEHPBVphBoundPolygonal@@PBVMatrix34@@1PAVphColliderBase@@2PAVphImpactBase@@HABVVector3@@4@Z
    ARTS_IMPORT i32 FindImpactsHotdogToPoly(const phBoundPolygonal* arg1, const Matrix34* arg2, const Matrix34* arg3, phColliderBase* arg4, phColliderBase* arg5, phImpactBase* arg6, i32 arg7, const Vector3& arg8, const Vector3& arg9) const;

    // ?FindImpactSphereToHotdog@phBoundHotdog@@QBE_NPBVphBoundSphere@@PBVMatrix34@@1PAVphColliderBase@@2PAVphImpactBase@@ABVVector3@@@Z
    ARTS_IMPORT bool FindImpactSphereToHotdog(const phBoundSphere* arg1, const Matrix34* arg2, const Matrix34* arg3, phColliderBase* arg4, phColliderBase* arg5, phImpactBase* arg6, const Vector3& arg7) const;

    // ?IsInsideHotdog@phBoundHotdog@@QBE_NABVVector3@@@Z
    ARTS_IMPORT bool IsInsideHotdog(const Vector3& arg1) const;

    // ?Load@phBoundHotdog@@QAE_NPBD@Z
    ARTS_IMPORT bool Load(const char* label);

    // ?ScaleBoundingBox@phBoundHotdog@@QAEXMM@Z
    ARTS_IMPORT void ScaleBoundingBox(f32 arg1, f32 arg2);

    // ?SegmentToHotdogIntersections@phBoundHotdog@@QBEHABVVector3@@0PAM111PAH2@Z
    ARTS_IMPORT i32 SegmentToHotdogIntersections(const Vector3& arg1, const Vector3& arg2, f32* arg3, f32* arg4, f32* arg5, f32* arg6, i32* arg7, i32* arg8) const;

    // ?SetBoundingBox@phBoundHotdog@@QAEXMM@Z
    ARTS_IMPORT void SetBoundingBox(f32 arg1, f32 arg2);

    // ?SetSize@phBoundHotdog@@QAEXMM@Z
    ARTS_IMPORT void SetSize(f32 arg1, f32 arg2);

    // ?ShiftCentroid@phBoundHotdog@@QAEXABVVector3@@@Z
    ARTS_IMPORT void ShiftCentroid(const Vector3& arg1);

public:
    // Members from 0x04C; everything below that belongs to phBound.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    f32 Radius; // 0x04C
    f32 Height; // 0x050
    phMaterial Material; // 0x054
    u8[44] pad_58; // 0x058
};

// ??_7phBoundHotdog@@6B@
// vtable at 0x005B21B8
check_size(phBoundHotdog, 0x84);
