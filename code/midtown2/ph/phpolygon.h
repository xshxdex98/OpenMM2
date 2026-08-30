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
#include "vector7/vector4.h"

class phIntersectionPoint;
class phSegment;

class phPolygon
{
public:
    // ??0phPolygon@@QAE@XZ
    ARTS_IMPORT phPolygon();

    // ?CalculateNormal@phPolygon@@QAEXPBVVector3@@@Z
    ARTS_IMPORT void CalculateNormal(const Vector3* scale);

    // ?DetectSegmentDirected@phPolygon@@QBEHPBVVector3@@ABV2@1@Z
    ARTS_IMPORT i32 DetectSegmentDirected(const Vector3* arg1, const Vector3& arg2, const Vector3& arg3) const;

    // ?DetectSegmentUndirected@phPolygon@@QBEHPBVVector3@@ABVVector4@@1@Z
    ARTS_IMPORT i32 DetectSegmentUndirected(const Vector3* arg1, const Vector4& arg2, const Vector4& arg3) const;

    // ?InitQuad@phPolygon@@QAEXGGGGPBVVector3@@@Z
    ARTS_IMPORT void InitQuad(u16 arg1, u16 arg2, u16 arg3, u16 arg4, const Vector3* arg5);

    // ?InitTriangle@phPolygon@@QAEXGGGPBVVector3@@@Z
    ARTS_IMPORT void InitTriangle(u16 arg1, u16 arg2, u16 arg3, const Vector3* scale);

    // ?Rotate@phPolygon@@QAEXXZ
    ARTS_IMPORT void Rotate();

    // ?TestSegmentDirected@phPolygon@@QBEHPBVVector3@@ABVphSegment@@PAVphIntersectionPoint@@M@Z
    ARTS_IMPORT i32 TestSegmentDirected(const Vector3* arg1, const phSegment& arg2, phIntersectionPoint* arg3, f32 arg4) const;

    // ?TestSegmentUndirected@phPolygon@@QBEHPBVVector3@@ABVphSegment@@PAVphIntersectionPoint@@MM@Z
    ARTS_IMPORT i32 TestSegmentUndirected(const Vector3* arg1, const phSegment& arg2, phIntersectionPoint* arg3, f32 arg4, f32 arg5) const;

protected:
    // ?ComputeEdgeNormalCross@phPolygon@@IAEXPAVVector4@@PBVVector3@@@Z
    ARTS_IMPORT void ComputeEdgeNormalCross(Vector4* arg1, const Vector3* arg2);

    // ?SegEdgeCheckDirected@phPolygon@@KA_NABVVector3@@00ABVVector4@@1@Z
    ARTS_IMPORT static bool SegEdgeCheckDirected(const Vector3& arg1, const Vector3& arg2, const Vector3& arg3, const Vector4& arg4, const Vector4& arg5);

    // ?SegEdgeCheckUndirected@phPolygon@@KA_NABVVector3@@00ABVVector4@@1@Z
    ARTS_IMPORT static bool SegEdgeCheckUndirected(const Vector3& arg1, const Vector3& arg2, const Vector3& arg3, const Vector4& arg4, const Vector4& arg5);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    Vector3 Position; // 0x000
    f32 field_C; // 0x00C
    Vector4 EdgeNormalCross[4]; // 0x010
    u16 Polys[8]; // 0x050
};

check_size(phPolygon, 0x60);
