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

class Stream;
class Vector3;
class Vector4;
class gfxTexture;
class lvlIntersection;
class lvlSegment;
class sdlPerimeterVertex;
class sdlPoly;

class sdlPage16
{
public:
    // ??0sdlPage16@@QAE@HH@Z
    ARTS_IMPORT sdlPage16(i32 perimeterVertexCount, i32 vertexIndicesCount);

    // ?ArcMap@sdlPage16@@QBEXPAMPBGHHH@Z
    ARTS_IMPORT void ArcMap(f32* arg1, unsigned const i16* arg2, i32 arg3, i32 arg4, i32 arg5) const;

    // ?Collect@sdlPage16@@QBEHPBVVector4@@PAVsdlPoly@@HAAH@Z
    ARTS_IMPORT i32 Collect(const Vector4* arg1, sdlPoly* arg2, i32 arg3, i32& arg4) const;

    // ?CollideSegment@sdlPage16@@QBE_NPBVVector4@@AAVlvlSegment@@AAVlvlIntersection@@MM@Z
    ARTS_IMPORT bool CollideSegment(const Vector4* arg1, lvlSegment& arg2, lvlIntersection& arg3, f32 arg4, f32 arg5) const;

    // ?ComputeBoundSphere@sdlPage16@@QBEXAAVVector4@@@Z
    ARTS_IMPORT void ComputeBoundSphere(Vector4& font) const;

    // ?Draw@sdlPage16@@QBEXHI@Z
    ARTS_IMPORT void Draw(i32 arg1, u32 arg2) const;

    // ?FindBoundingIsoParams@sdlPage16@@SAXPBVVector4@@PBVVector3@@PBGHHAAH3@Z
    ARTS_IMPORT static void FindBoundingIsoParams(const Vector4* arg1, const Vector3* arg2, unsigned const i16* arg3, i32 arg4, i32 arg5, i32& arg6, i32& arg7);

    // ?GetCentroid@sdlPage16@@QBEXAAVVector3@@@Z
    ARTS_IMPORT void GetCentroid(Vector3& out) const;

    // ?GetCodedVertex@sdlPage16@@QBEABVVector3@@H@Z
    ARTS_IMPORT const Vector3& GetCodedVertex(i32 index) const;

    // ?GetDrawnSDLPrims@sdlPage16@@QBEHHPAHH@Z
    ARTS_IMPORT i32 GetDrawnSDLPrims(i32 lod, i32* pOutput, i32 _1D4C) const;

    // ?GetFloat@sdlPage16@@QBEMH@Z
    ARTS_IMPORT f32 GetFloat(i32 index) const;

    // ?GetPerimeterCount@sdlPage16@@QBEHXZ
    ARTS_IMPORT i32 GetPerimeterCount() const;

    // ?GetPerimeterVertexIndex@sdlPage16@@QBEHH@Z
    ARTS_IMPORT i32 GetPerimeterVertexIndex(i32 index) const;

    // ?GetTexture@sdlPage16@@QBEPAVgfxTexture@@H@Z
    ARTS_IMPORT gfxTexture* GetTexture(i32 arg1) const;

    // ?LoadBinary@sdlPage16@@SAPAV1@PAVStream@@@Z
    ARTS_IMPORT static sdlPage16* LoadBinary(Stream* stream);

    // ?PointInPerimeter@sdlPage16@@QBE_NMM@Z
    ARTS_IMPORT bool PointInPerimeter(f32 x, f32 z) const;

    // ?WallMap@sdlPage16@@QBEXPAMPBGMHH@Z
    ARTS_IMPORT void WallMap(f32* arg1, unsigned const i16* arg2, f32 arg3, i32 arg4, i32 arg5) const;

private:
    // ?GetShadedColor@sdlPage16@@DAIII@Z
    ARTS_IMPORT static u32 GetShadedColor(u32 arg1, u32 arg2);

    // ?GetShadedColor@sdlPage16@@DAIIII@Z
    ARTS_IMPORT static u32 GetShadedColor(u32 arg0, u32 arg2, u32 arg3);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    byte bool0; // 0x000
    byte PerimeterVerticesCount; // 0x001
    byte bool2; // 0x002
    byte bool3; // 0x003
    Vector3* CodedVertices; // 0x004
    f32* Floats; // 0x008
    u32 field_C; // 0x00C
    sdlPerimeterVertex* PerimeterVertices; // 0x010
    u16* CodedVertexIndices; // 0x014
    gfxTexture** Textures; // 0x018
    u32 field_1C; // 0x01C
};

check_size(sdlPage16, 0x20);
