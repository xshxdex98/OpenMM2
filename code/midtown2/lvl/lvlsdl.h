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

#include "sdl/sdlcommon.h"

class Matrix34;
class Vector3;
class lvlIntersection;
class lvlSegment;
class phBoundPolygonal;
class phColliderBase;
class sdlPage16;

class lvlSDL : public sdlCommon
{
public:
    // ??0lvlSDL@@QAE@XZ
    ARTS_IMPORT lvlSDL();

    // ??1lvlSDL@@QAE@XZ
    ARTS_IMPORT ~lvlSDL();

    // ?CollideAI@lvlSDL@@UBE_NAAVlvlSegment@@PAVlvlIntersection@@PAHH@Z
    ARTS_IMPORT virtual bool CollideAI(lvlSegment& arg1, lvlIntersection* arg2, i32* arg3, i32 arg4) const;

    // ?CollideEdge@lvlSDL@@UBEHAAVlvlSegment@@PAVlvlIntersection@@HPAHH@Z
    ARTS_IMPORT virtual i32 CollideEdge(lvlSegment& arg1, lvlIntersection* arg2, i32 arg3, i32* arg4, i32 arg5) const;

    // ?CollidePolyToLevel@lvlSDL@@UBEHPBVphBoundPolygonal@@PAHHPAVphColliderBase@@PBVMatrix34@@3PAVlvlIntersection@@H1_N@Z
    ARTS_IMPORT virtual i32 CollidePolyToLevel(const phBoundPolygonal* arg1, i32* arg2, i32 arg3, phColliderBase* arg4, const Matrix34* arg5, const Matrix34* arg6, lvlIntersection* arg7, i32 arg8, i32* arg9, bool arg10) const;

    // ?CollideProbe@lvlSDL@@UBE_NAAVlvlSegment@@PAVlvlIntersection@@M@Z
    ARTS_IMPORT virtual bool CollideProbe(lvlSegment& arg1, lvlIntersection* arg2, f32 arg3) const;

    // ?Enumerate@lvlSDL@@QBEXHP6AXABV1@HHHPBGPAX@Z2@Z
    ARTS_IMPORT void Enumerate(i32 arg1, void (__cdecl* arg2)(const lvlSDL&,i32,i32,i32,unsigned const i16*,void*), void* arg3) const;

    // ?GetVertex@lvlSDL@@UBEABVVector3@@H@Z
    ARTS_IMPORT virtual const Vector3& GetVertex(i32 arg1) const;

    // ?LoadBinary@lvlSDL@@QAE_NPBD@Z
    ARTS_IMPORT bool LoadBinary(const char* arg1);

    // ?Propulate@lvlSDL@@SAXABV1@HHHPBGPAX@Z
    ARTS_IMPORT static void Propulate(const lvlSDL& arg1, i32 arg2, i32 arg3, i32 arg4, unsigned const i16* arg5, void* arg6);

private:
    // ?IsoLerp@lvlSDL@@CA_NAAVVector3@@AAHPBV2@HPBGHM_N4@Z
    ARTS_IMPORT static bool IsoLerp(Vector3& arg1, i32& arg2, const Vector3* arg3, i32 arg4, unsigned const i16* arg5, i32 arg6, f32 arg7, bool arg8, bool arg9);

public:
    // Members from 0x04C; everything below that belongs to sdlCommon.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u32 PageCount; // 0x04C
    u32 MaterialCountThing; // 0x050
    sdlPage16** Pages; // 0x054
    u8* RoomFlags; // 0x058
    u8* PropRules; // 0x05C
};

// ??_7lvlSDL@@6B@
// vtable at 0x005B1868
check_size(lvlSDL, 0x60);
