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

#include "lvl/lvllevelbound.h"
#include "ph/phbound.h"
#include "vector7/vector3.h"

class lvlIntersectionPoint;
class lvlSegment;

class sdlCommon : public lvlLevelBound
{
public:
    // ??0sdlCommon@@QAE@XZ
    ARTS_IMPORT sdlCommon();

    // ??1sdlCommon@@QAE@XZ
    ARTS_IMPORT ~sdlCommon();

    // ?CollideEdgePoint@sdlCommon@@UBEHAAVlvlSegment@@PAVlvlIntersectionPoint@@H@Z
    ARTS_IMPORT virtual i32 CollideEdgePoint(lvlSegment& arg1, lvlIntersectionPoint* arg2, i32 arg3) const;

    // ?CollideProbePoint@sdlCommon@@UBE_NAAVlvlSegment@@PAVlvlIntersectionPoint@@M@Z
    ARTS_IMPORT virtual bool CollideProbePoint(lvlSegment& arg1, lvlIntersectionPoint* arg2, f32 arg3) const;

    // ?CollideAIPoint@sdlCommon@@UBE_NAAVlvlSegment@@PAVlvlIntersectionPoint@@@Z
    ARTS_IMPORT virtual bool CollideAIPoint(lvlSegment& arg1, lvlIntersectionPoint* arg2) const;

    // ?AllocateState@sdlCommon@@UBEPADXZ
    ARTS_IMPORT virtual char* AllocateState() const;

    // ?BACKFACE@sdlCommon@@SA_NABVVector3@@0@Z
    ARTS_IMPORT static bool BACKFACE(const Vector3& arg1, const Vector3& arg2);

    // ?UpdateLighting@sdlCommon@@SAXXZ
    ARTS_IMPORT static void UpdateLighting();

    // ?sm_CamPos@sdlCommon@@2VVector3@@A
    ARTS_IMPORT static Vector3 sm_CamPos;

    // ?sm_LightTable@sdlCommon@@2PAIA
    ARTS_IMPORT static u32* sm_LightTable;

public:
    static sdlCommon*& Access_sm_Instance() { return sm_Instance; }

private:
    // ?sm_Instance@sdlCommon@@0PAV1@A
    ARTS_IMPORT static sdlCommon* sm_Instance;

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    phBound Bound; // 0x000
};

// ??_7sdlCommon@@6B@
// vtable at 0x005B17CC
check_size(sdlCommon, 0x4C);
