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

#include "vector7/matrix34.h"

#include "phimpactbase.h"

class Vector3;
class datCallback;
class phBound;
class phContact;
class phInertialCS;
class phIntersection;
class phIntersectionPoint;
class phSegment;

class phColliderBase
{
public:
    // ?ApplyAirResistance@phColliderBase@@QAEXMABVVector3@@PBVphBound@@@Z
    ARTS_IMPORT void ApplyAirResistance(f32 arg1, const Vector3& arg2, const phBound* arg3);

    // ?ApplyGravity@phColliderBase@@QAEXXZ
    ARTS_IMPORT void ApplyGravity();

    // ?CalcMaxMoved@phColliderBase@@QAEXXZ
    ARTS_IMPORT void CalcMaxMoved();

    // ?CallBoundCallback@phColliderBase@@QAEXPBVphImpactBase@@PBVphContact@@ABVVector3@@2_NPBVMatrix34@@@Z
    ARTS_IMPORT void CallBoundCallback(const phImpactBase* arg1, const phContact* arg2, const Vector3& arg3, const Vector3& arg4, bool arg5, const Matrix34* arg6);

    // ?ColliderIsActive@phColliderBase@@QBE_NXZ
    ARTS_IMPORT bool ColliderIsActive() const;

    // ?Contact@phColliderBase@@UAEXABVVector3@@PBVphContact@@@Z
    ARTS_IMPORT virtual void Contact(const Vector3& arg1, const phContact* arg2);

    // ?Contact@phColliderBase@@UAEXABVVector3@@PBVphContact@@ABVMatrix34@@@Z
    ARTS_IMPORT virtual void Contact(const Vector3& arg1, const phContact* arg2, const Matrix34& arg3);

    // ?Contact@phColliderBase@@UAEXPBVphImpactBase@@ABVVector3@@11ABVMatrix34@@@Z
    ARTS_IMPORT virtual void Contact(const phImpactBase* arg1, const Vector3& arg2, const Vector3& arg3, const Vector3& arg4, const Matrix34& arg5);

    // ?CopyLastMatrix@phColliderBase@@QBEXPAVMatrix34@@J@Z
    ARTS_IMPORT void CopyLastMatrix(Matrix34* arg1, ilong arg2) const;

    // ?GetBoundCBImpactInfo@phColliderBase@@QBEXPBVphImpactBase@@PBVphContact@@PAUBoundCallbackData@2@PAVVector3@@@Z
    ARTS_IMPORT void GetBoundCBImpactInfo(const phImpactBase* arg1, const phContact* arg2, phImpactBase::BoundCallbackData* arg3, Vector3* arg4) const;

    // ?GetDisp@phColliderBase@@QBEXABVVector3@@HHHPAV2@@Z
    ARTS_IMPORT void GetDisp(const Vector3& arg1, i32 arg2, i32 arg3, i32 arg4, Vector3* arg5) const;

    // ?GetDisp@phColliderBase@@QBEXABVVector3@@PAV2@@Z
    ARTS_IMPORT void GetDisp(const Vector3& arg1, Vector3* arg2) const;

    // ?GetDisp@phColliderBase@@QBEXABVVector3@@PAV2@J@Z
    ARTS_IMPORT void GetDisp(const Vector3& arg1, Vector3* arg2, ilong arg3) const;

    // ?GetInvMassMatrix@phColliderBase@@UBEXABVVector3@@0AAVMatrix34@@H@Z
    ARTS_IMPORT virtual void GetInvMassMatrix(const Vector3& arg1, const Vector3& arg2, Matrix34& arg3, i32 arg4) const;

    // ?GetInvMassMatrix@phColliderBase@@UBEXABVVector3@@AAVMatrix34@@H@Z
    ARTS_IMPORT virtual void GetInvMassMatrix(const Vector3& arg1, Matrix34& arg2, i32 arg3) const;

    // ?GetLocalVelocity@phColliderBase@@QBEXABVVector3@@HHHAAV2@@Z
    ARTS_IMPORT void GetLocalVelocity(const Vector3& arg1, i32 arg2, i32 arg3, i32 arg4, Vector3& arg5) const;

    // ?Impact@phColliderBase@@UAEXPBVphImpactBase@@ABVVector3@@1@Z
    ARTS_IMPORT virtual void Impact(const phImpactBase* arg1, const Vector3& arg2, const Vector3& arg3);

    // ?Impact@phColliderBase@@UAEXPBVphImpactBase@@ABVVector3@@@Z
    ARTS_IMPORT virtual void Impact(const phImpactBase* arg1, const Vector3& arg2);

    // ?Reset@phColliderBase@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?SetBoundCB@phColliderBase@@QAEXPAVdatCallback@@@Z
    ARTS_IMPORT void SetBoundCB(datCallback* arg1);

    // ?SetColliderPenetration@phColliderBase@@SAXM@Z
    ARTS_IMPORT static void SetColliderPenetration(f32 arg1);

    // ?SetHitCB@phColliderBase@@QAEXPAVdatCallback@@@Z
    ARTS_IMPORT void SetHitCB(datCallback* arg1);

    // ?SetImpactCB@phColliderBase@@QAEXPAVdatCallback@@@Z
    ARTS_IMPORT void SetImpactCB(datCallback* arg1);

    // ?TestSegment@phColliderBase@@QBEHABVphSegment@@PAVphIntersection@@H@Z
    ARTS_IMPORT i32 TestSegment(const phSegment& arg1, phIntersection* arg2, i32 arg3) const;

    // ?TestSegmentPoint@phColliderBase@@QBEHABVphSegment@@PAVphIntersectionPoint@@H@Z
    ARTS_IMPORT i32 TestSegmentPoint(const phSegment& arg1, phIntersectionPoint* arg2, i32 arg3) const;

    // ?ToWorldCoords@phColliderBase@@QBEXPAVphIntersection@@H@Z
    ARTS_IMPORT void ToWorldCoords(phIntersection* arg1, i32 arg2) const;

    // ?Update@phColliderBase@@QAEXXZ
    ARTS_IMPORT void Update();

    // ?UpdateMtx@phColliderBase@@QAEXXZ
    ARTS_IMPORT void UpdateMtx();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i32 field_4; // 0x004
    i32 field_8; // 0x008
    i8 field_C; // 0x00C
    i32 field_10; // 0x010
    datCallback* pImpactCallback; // 0x014
    i32 field_18; // 0x018
    i32 field_1C; // 0x01C
    void* field_20; // 0x020
    phInertialCS* pInertialCS; // 0x024
    Matrix34* pSourceMatrix; // 0x028
    Matrix34 Matrix; // 0x02C
    u8 field_5C; // 0x05C
    u8 field_5D; // 0x05D
    f32 field_60; // 0x060
    u8 field_64[4]; // 0x064
    u32 field_68; // 0x068
    u32 field_6C; // 0x06C
    u32 field_70; // 0x070
};

// ??_7phColliderBase@@6B@
// vtable at 0x005B1CCC
check_size(phColliderBase, 0x74);
