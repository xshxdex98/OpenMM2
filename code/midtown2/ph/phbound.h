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

class Matrix34;
class phImpactBase;
class phIntersection;
class phIntersectionPoint;
class phSegment;

class phBound
{
public:
    // ??0phBound@@QAE@H@Z
    ARTS_IMPORT phBound(i32 arg1);

    // ?CenterBound@phBound@@UAEXXZ
    ARTS_IMPORT virtual void CenterBound();

    // ?GetNumMaterials@phBound@@UBEHXZ
    ARTS_IMPORT virtual i32 GetNumMaterials() const;

    // ?SetFriction@phBound@@UAEMXZ
    ARTS_EXPORT virtual f32 SetFriction();

    // ?SetFriction@phBound@@UAEXM@Z
    ARTS_EXPORT virtual void SetFriction(f32 arg1);

    // ?SetElasticity@phBound@@UAEMXZ
    ARTS_EXPORT virtual f32 SetElasticity();

    // ?SetElasticity@phBound@@UAEXM@Z
    ARTS_EXPORT virtual void SetElasticity(f32 arg1);

    // ?TestSphere@phBound@@UBE_NABVVector3@@MAAVphImpactBase@@@Z
    ARTS_EXPORT virtual bool TestSphere(const Vector3& arg1, f32 arg2, phImpactBase& arg3) const;

    // ?TestSphere@phBound@@UBE_NABVVector3@@MAAV2@1AAM@Z
    ARTS_EXPORT virtual bool TestSphere(const Vector3& arg1, f32 arg2, Vector3& arg3, Vector3& arg4, f32& arg5) const;

    // ?ModifyInvMassMatrix@phBound@@UBEXPAVMatrix34@@HABVVector3@@1@Z
    ARTS_EXPORT virtual void ModifyInvMassMatrix(Matrix34* arg1, i32 arg2, const Vector3& arg3, const Vector3& arg4) const;

    // ?ModifyInvMassMatrix@phBound@@UBEXPAVMatrix34@@HABVVector3@@@Z
    ARTS_EXPORT virtual void ModifyInvMassMatrix(Matrix34* arg1, i32 arg2, const Vector3& arg3) const;

    // ?EffectiveMass@phBound@@UBEMABVVector3@@0HM@Z
    ARTS_EXPORT virtual f32 EffectiveMass(const Vector3& arg1, const Vector3& arg2, i32 arg3, f32 arg4) const;

    // ?GetVertex@phBound@@UBEABVVector3@@H@Z
    ARTS_IMPORT virtual const Vector3& GetVertex(i32 arg1) const;

    // ?CalculateSphereFromBoundingBox@phBound@@QAEXXZ
    ARTS_IMPORT void CalculateSphereFromBoundingBox();

    // ?GetCenter@phBound@@QBE?AVVector3@@PBVMatrix34@@@Z
    ARTS_IMPORT Vector3 GetCenter(const Matrix34* arg1) const;

    // ?GetCenter@phBound@@QBEXPBVMatrix34@@PAVVector3@@@Z
    ARTS_IMPORT void GetCenter(const Matrix34* arg1, Vector3* arg2) const;

    // ?GetFricElas@phBound@@QBEXPBVphIntersection@@PAM1@Z
    ARTS_IMPORT void GetFricElas(const phIntersection* arg1, f32* arg2, f32* arg3) const;

    // ?GetFricElas@phBound@@SAXPBV1@PBVphIntersection@@01PAM2@Z
    ARTS_IMPORT static void GetFricElas(const phBound* arg1, const phIntersection* arg2, const phBound* arg3, const phIntersection* arg4, f32* arg5, f32* arg6);

    // ?IsOffset@phBound@@QBE?B_NXZ
    ARTS_IMPORT const bool IsOffset() const;

    // ?SetFlexibility@phBound@@QAEXM@Z
    ARTS_IMPORT void SetFlexibility(f32 arg1);

    // ?SetOffset@phBound@@QAEXABVVector3@@@Z
    ARTS_IMPORT void SetOffset(const Vector3& arg1);

    // ?SetPenetration@phBound@@QAEXXZ
    ARTS_IMPORT void SetPenetration();

    // ?TestSegment@phBound@@QBEHAAVphSegment@@PAVphIntersection@@H@Z
    ARTS_IMPORT i32 TestSegment(phSegment& arg1, phIntersection* arg2, i32 arg3) const;

    // ?TestSegmentPoint@phBound@@QBEHAAVphSegment@@PAVphIntersectionPoint@@H@Z
    ARTS_IMPORT i32 TestSegmentPoint(phSegment& arg1, phIntersectionPoint* arg2, i32 arg3) const;

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u32 MaterialCount; // 0x004
    u32 Type; // 0x008
    Vector3 field_C; // 0x00C
    Vector3 field_18; // 0x018
    u8 IsOffset_; // 0x024
    Vector3 Offset; // 0x028
    f32 Radius; // 0x034
    u32 field_38; // 0x038
    u8 field_3C; // 0x03C
    f32 field_40; // 0x040
    f32 field_44; // 0x044
    f32 field_48; // 0x048
};

// ??_7phBound@@6B@
// vtable at 0x005B22F0
check_size(phBound, 0x4C);
