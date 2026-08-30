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

class Matrix34;
class Vector3;
class phBound;
class phBoundHotdog;
class phBoundPolygonal;
class phBoundSphere;
class phColliderBase;
class phForceSphere;
class phImpact;
class phIntersection;

class phCollision
{
public:
    // ?GetRelDisp@phCollision@@SAXPBVMatrix34@@00ABVVector3@@PAV3@@Z
    ARTS_IMPORT static void GetRelDisp(const Matrix34* arg1, const Matrix34* arg2, const Matrix34* arg3, const Vector3& arg4, Vector3* arg5);

    // ?SphereApplyCenterForceToHotdog@phCollision@@SAXPBVphForceSphere@@AAVphColliderBase@@PBVphBoundHotdog@@1@Z
    ARTS_IMPORT static void SphereApplyCenterForceToHotdog(const phForceSphere* arg1, phColliderBase& arg2, const phBoundHotdog* arg3, phColliderBase& arg4);

    // ?SphereApplyCenterForceToPoly@phCollision@@SA_NPBVphForceSphere@@AAVphColliderBase@@PBVphBoundPolygonal@@1@Z
    ARTS_IMPORT static bool SphereApplyCenterForceToPoly(const phForceSphere* arg1, phColliderBase& arg2, const phBoundPolygonal* arg3, phColliderBase& arg4);

    // ?SphereApplyCenterForceToSphere@phCollision@@SAXPBVphForceSphere@@AAVphColliderBase@@PBVphBoundSphere@@1@Z
    ARTS_IMPORT static void SphereApplyCenterForceToSphere(const phForceSphere* label, phColliderBase& v, const phBoundSphere* step, phColliderBase& step_fast);

    // ?TestBoundForce@phCollision@@SAXPBVphBound@@AAVphColliderBase@@01PAVphImpact@@HABVVector3@@@Z
    ARTS_IMPORT static void TestBoundForce(const phBound* arg1, phColliderBase& arg2, const phBound* arg3, phColliderBase& arg4, phImpact* arg5, i32 arg6, const Vector3& arg7);

    // ?TestBoundGeneric@phCollision@@SAHPBVphBound@@0PBVMatrix34@@111PAVphIntersection@@2PAVphImpact@@HHABVVector3@@_N@Z
    ARTS_IMPORT static i32 TestBoundGeneric(const phBound* arg1, const phBound* arg2, const Matrix34* arg3, const Matrix34* arg4, const Matrix34* arg5, const Matrix34* arg6, phIntersection* arg7, phIntersection* arg8, phImpact* arg9, i32 arg10, i32 arg11, const Vector3& arg12, bool arg13);

    // ?TestBoundGeneric@phCollision@@SAHPBVphBound@@AAVphColliderBase@@01PAVphIntersection@@2PAVphImpact@@HHABVVector3@@@Z
    ARTS_IMPORT static i32 TestBoundGeneric(const phBound* arg1, phColliderBase& arg2, const phBound* arg3, phColliderBase& arg4, phIntersection* arg5, phIntersection* arg6, phImpact* arg7, i32 arg8, i32 arg9, const Vector3& arg10);
};

// check_size(phCollision, 0x0); // TODO: no layout in the IDB type library
