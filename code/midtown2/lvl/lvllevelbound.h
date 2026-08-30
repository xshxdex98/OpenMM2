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

#include "ph/phbound.h"

class Matrix34;
class Vector3;
class lvlIntersection;
class phBoundBox;
class phBoundHotdog;
class phBoundPolygonal;
class phBoundSphere;
class phColliderBase;
class phImpactBase;
class phIntersection;
class phIntersectionPoint;
class phMaterial;
class phSegment;

class lvlLevelBound : public phBound
{
public:
    // ?AllocateState@lvlLevelBound@@UBEPADXZ
    ARTS_IMPORT virtual char* AllocateState() const;

    // ?CollidePolyToLevel@lvlLevelBound@@UBEHPBVphBoundPolygonal@@PAHHPAVphColliderBase@@PBVMatrix34@@3PAVlvlIntersection@@H1_N@Z
    ARTS_IMPORT virtual i32 CollidePolyToLevel(const phBoundPolygonal* arg1, i32* arg2, i32 arg3, phColliderBase* arg4, const Matrix34* arg5, const Matrix34* arg6, lvlIntersection* arg7, i32 arg8, i32* arg9, bool arg10) const;

    // ?GetEdgeCosine@lvlLevelBound@@UBEMH@Z
    ARTS_IMPORT virtual f32 GetEdgeCosine(i32 arg1) const;

    // ?GetEdgeNormal@lvlLevelBound@@UBEABVVector3@@HAAV2@@Z
    ARTS_IMPORT virtual const Vector3& GetEdgeNormal(i32 arg1, Vector3& arg2) const;

    // ?GetMaterial@lvlLevelBound@@UBEPBVphMaterial@@H@Z
    ARTS_IMPORT virtual const phMaterial* GetMaterial(i32 arg1) const;

    // ?TestAI@lvlLevelBound@@UBE_NAAVphSegment@@PAVphIntersection@@@Z
    ARTS_IMPORT virtual bool TestAI(phSegment& arg1, phIntersection* arg2) const;

    // ?TestAIPoint@lvlLevelBound@@UBE_NAAVphSegment@@PAVphIntersectionPoint@@@Z
    ARTS_IMPORT virtual bool TestAIPoint(phSegment& arg1, phIntersectionPoint* arg2) const;

    // ?TestEdge@lvlLevelBound@@UBEHAAVphSegment@@PAVphIntersection@@H@Z
    ARTS_IMPORT virtual i32 TestEdge(phSegment& arg1, phIntersection* arg2, i32 arg3) const;

    // ?TestEdgePoint@lvlLevelBound@@UBEHAAVphSegment@@PAVphIntersectionPoint@@H@Z
    ARTS_IMPORT virtual i32 TestEdgePoint(phSegment& arg1, phIntersectionPoint* arg2, i32 arg3) const;

    // ?TestProbe@lvlLevelBound@@UBE_NAAVphSegment@@PAVphIntersection@@M@Z
    ARTS_IMPORT virtual bool TestProbe(phSegment& arg1, phIntersection* arg2, f32 arg3) const;

    // ?TestProbePoint@lvlLevelBound@@UBE_NAAVphSegment@@PAVphIntersectionPoint@@M@Z
    ARTS_IMPORT virtual bool TestProbePoint(phSegment& arg1, phIntersectionPoint* arg2, f32 arg3) const;

    // ?TestSphere@lvlLevelBound@@UBE_NABVVector3@@MAAV2@1AAM@Z
    ARTS_IMPORT virtual bool TestSphere(const Vector3& arg1, f32 arg2, Vector3& arg3, Vector3& arg4, f32& arg5) const;

    // ?TrivialCollideBoxToLevel@lvlLevelBound@@UBE_NPBVphBoundBox@@HPAVphColliderBase@@PBVMatrix34@@2@Z
    ARTS_IMPORT virtual bool TrivialCollideBoxToLevel(const phBoundBox* arg1, i32 arg2, phColliderBase* arg3, const Matrix34* arg4, const Matrix34* arg5) const;

private:
    // ?FindImpactsHotdogOneRoom@lvlLevelBound@@EBEHHQBVVector3@@PBVphBoundHotdog@@PBVMatrix34@@2PAVphColliderBase@@3PAVphImpactBase@@HABV2@@Z
    ARTS_IMPORT virtual i32 FindImpactsHotdogOneRoom(i32 arg1, const Vector3*const arg2, const phBoundHotdog* arg3, const Matrix34* arg4, const Matrix34* arg5, phColliderBase* arg6, phColliderBase* arg7, phImpactBase* arg8, i32 arg9, const Vector3& arg10) const;

    // ?FindImpactsSphereOneRoom@lvlLevelBound@@EBEHHPBVphBoundSphere@@ABVVector3@@PAVphColliderBase@@2PAVphImpactBase@@H@Z
    ARTS_IMPORT virtual i32 FindImpactsSphereOneRoom(i32 arg1, const phBoundSphere* arg2, const Vector3& arg3, phColliderBase* arg4, phColliderBase* arg5, phImpactBase* arg6, i32 arg7) const;
};

// check_size(lvlLevelBound, 0x4C); // size known, members are not - cannot verify
