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
class phBoundPolygonal;
class phIntersectionPoint;
class phSegment;

class phCollisionPrim
{
public:
    // ?SegmentSphereTest@phCollisionPrim@@SA_NABVVector3@@M00@Z
    ARTS_IMPORT static bool SegmentSphereTest(const Vector3& arg1, f32 arg2, const Vector3& arg3, const Vector3& arg4);

    // ?SegmentSphereTest@phCollisionPrim@@SA_NABVVector3@@MABVphSegment@@@Z
    ARTS_IMPORT static bool SegmentSphereTest(const Vector3& arg1, f32 arg2, const phSegment& arg3);

    // ?SegmentSphereTest@phCollisionPrim@@SA_NMABVVector3@@0@Z
    ARTS_IMPORT static bool SegmentSphereTest(f32 arg1, const Vector3& arg2, const Vector3& arg3);

    // ?SegmentSphereTest@phCollisionPrim@@SA_NMABVphSegment@@@Z
    ARTS_IMPORT static bool SegmentSphereTest(f32 arg1, const phSegment& arg2);

    // ?SegmentToSphere@phCollisionPrim@@SA_NABVVector3@@MABVphSegment@@PAVphIntersectionPoint@@@Z
    ARTS_IMPORT static bool SegmentToSphere(const Vector3& arg1, f32 arg2, const phSegment& arg3, phIntersectionPoint* arg4);

    // ?SphereToPolygonal@phCollisionPrim@@SA_NABVVector3@@MPBVphBoundPolygonal@@AAV2@2AAM@Z
    ARTS_IMPORT static bool SphereToPolygonal(const Vector3& arg1, f32 arg2, const phBoundPolygonal* arg3, Vector3& arg4, Vector3& arg5, f32& arg6);

    // ?SphereToPolygonal@phCollisionPrim@@SA_NABVVector3@@MPBVphBoundPolygonal@@ABVMatrix34@@AAV2@3AAM@Z
    ARTS_IMPORT static bool SphereToPolygonal(const Vector3& arg1, f32 arg2, const phBoundPolygonal* arg3, const Matrix34& arg4, Vector3& arg5, Vector3& arg6, f32& arg7);
};

// check_size(phCollisionPrim, 0x0); // TODO: no layout in the IDB type library
