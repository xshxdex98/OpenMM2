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
class phBound_vtbl;
class phColliderBase;
class phImpactBase;
class phIntersection;
class phIntersectionPoint;
class phSegment;

class phBoundSphere : public phBound
{
public:
    // ??0phBoundSphere@@QAE@M@Z
    ARTS_IMPORT phBoundSphere(f32 arg1);

    // ??0phBoundSphere@@QAE@XZ
    ARTS_IMPORT phBoundSphere();

    // ??1phBoundSphere@@QAE@XZ
    ARTS_IMPORT ~phBoundSphere();

    // ?GetMaterial@phBoundSphere@@UBEPBVphMaterial@@H@Z
    ARTS_IMPORT virtual const phMaterial* GetMaterial(i32 arg1) const;

    // ?TestProbePoint@phBoundSphere@@UBE_NAAVphSegment@@PAVphIntersectionPoint@@M@Z
    ARTS_IMPORT virtual bool TestProbePoint(phSegment& arg1, phIntersectionPoint* arg2, f32 arg3) const;

    // ?TestAIPoint@phBoundSphere@@UBE_NAAVphSegment@@PAVphIntersectionPoint@@@Z
    ARTS_IMPORT virtual bool TestAIPoint(phSegment& arg1, phIntersectionPoint* arg2) const;

    // ?TestEdge@phBoundSphere@@UBEHAAVphSegment@@PAVphIntersection@@H@Z
    ARTS_IMPORT virtual i32 TestEdge(phSegment& arg1, phIntersection* arg2, i32 arg3) const;

    // ?TestProbe@phBoundSphere@@UBE_NAAVphSegment@@PAVphIntersection@@M@Z
    ARTS_IMPORT virtual bool TestProbe(phSegment& arg1, phIntersection* arg2, f32 arg3) const;

    // ?TestSphere@phBoundSphere@@UBE_NABVVector3@@MAAVphImpactBase@@@Z
    ARTS_IMPORT virtual bool TestSphere(const Vector3& arg1, f32 arg2, phImpactBase& arg3) const;

    // ?TestSphere@phBoundSphere@@UBE_NABVVector3@@MAAV2@1AAM@Z
    ARTS_IMPORT virtual bool TestSphere(const Vector3& arg1, f32 arg2, Vector3& arg3, Vector3& arg4, f32& arg5) const;

    // ?CreateOffset@phBoundSphere@@QAEXABVVector3@@@Z
    ARTS_IMPORT void CreateOffset(const Vector3& arg1);

    // ?FindImpactSphereToSphere@phBoundSphere@@QBE_NPBV1@PBVMatrix34@@1PAVphColliderBase@@2PAVphImpactBase@@ABVVector3@@@Z
    ARTS_IMPORT bool FindImpactSphereToSphere(const phBoundSphere* arg1, const Matrix34* arg2, const Matrix34* arg3, phColliderBase* arg4, phColliderBase* arg5, phImpactBase* arg6, const Vector3& arg7) const;

    // ?Load@phBoundSphere@@QAE_NPBD@Z
    ARTS_IMPORT bool Load(const char* path);

    // ?ScaleRadius@phBoundSphere@@QAEXM@Z
    ARTS_IMPORT void ScaleRadius(f32 arg1);

    // ?SetRadius@phBoundSphere@@QAEXM@Z
    ARTS_IMPORT void SetRadius(f32 arg1);

    // ?ShiftCentroid@phBoundSphere@@QAEXABVVector3@@@Z
    ARTS_IMPORT void ShiftCentroid(const Vector3& arg1);

public:
    // Members from 0x04C; everything below that belongs to phBound.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    f32 Radius; // 0x04C
    phMaterial Material; // 0x050
    u8[44] pad_54; // 0x054
};

// ??_7phBoundSphere@@6B@
// vtable at 0x005B2204
check_size(phBoundSphere, 0x80);
