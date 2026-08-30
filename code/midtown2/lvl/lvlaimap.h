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

class Stream;
struct lvlAiCurrent;
class lvlAiRoad;
class lvlSDL;

class lvlAiMap
{
public:
    // ?Delete@lvlAiMap@@SAXXZ
    ARTS_IMPORT static void Delete();

    // ?GetCablecarVertex@lvlAiMap@@SA?AVVector3@@HHH@Z
    ARTS_IMPORT static Vector3 GetCablecarVertex(i32 arg1, i32 arg2, i32 arg3);

    // ?GetIntersectionType@lvlAiMap@@SAHH@Z
    ARTS_IMPORT static i32 GetIntersectionType(i32 arg1);

    // ?GetNormal@lvlAiMap@@SA?AVVector3@@H@Z
    ARTS_IMPORT static Vector3 GetNormal(i32 arg1);

    // ?GetNumCablecarLanes@lvlAiMap@@SAHH@Z
    ARTS_IMPORT static i32 GetNumCablecarLanes(i32 arg1);

    // ?GetNumLanes@lvlAiMap@@SAHH@Z
    ARTS_IMPORT static i32 GetNumLanes(i32 arg1);

    // ?GetNumRoads@lvlAiMap@@SAIXZ
    ARTS_IMPORT static u32 GetNumRoads();

    // ?GetNumRooms@lvlAiMap@@SAHXZ
    ARTS_IMPORT static i32 GetNumRooms();

    // ?GetNumSidewalks@lvlAiMap@@SAHH@Z
    ARTS_IMPORT static i32 GetNumSidewalks(i32 arg1);

    // ?GetNumSubwayLanes@lvlAiMap@@SAHH@Z
    ARTS_IMPORT static i32 GetNumSubwayLanes(i32 arg1);

    // ?GetNumVertexs@lvlAiMap@@SAHXZ
    ARTS_IMPORT static i32 GetNumVertexs();

    // ?GetRoad@lvlAiMap@@SAPAVlvlAiRoad@@H@Z
    ARTS_IMPORT static lvlAiRoad* GetRoad(i32 arg1);

    // ?GetRoadIntersection@lvlAiMap@@SAHH@Z
    ARTS_IMPORT static i32 GetRoadIntersection(i32 arg1);

    // ?GetRoom@lvlAiMap@@SAHH@Z
    ARTS_IMPORT static i32 GetRoom(i32 arg1);

    // ?GetRoomChop@lvlAiMap@@SA_NH@Z
    ARTS_IMPORT static bool GetRoomChop(i32 arg1);

    // ?GetSidewalkVertex@lvlAiMap@@SA?AVVector3@@HHH@Z
    ARTS_IMPORT static Vector3 GetSidewalkVertex(i32 arg1, i32 arg2, i32 arg3);

    // ?GetSidewalkVertexMulti@lvlAiMap@@SA?AVVector3@@HHH@Z
    ARTS_IMPORT static Vector3 GetSidewalkVertexMulti(i32 arg1, i32 arg2, i32 arg3);

    // ?GetSidewalkVertexSingle@lvlAiMap@@SA?AVVector3@@HHH@Z
    ARTS_IMPORT static Vector3 GetSidewalkVertexSingle(i32 arg1, i32 arg2, i32 arg3);

    // ?GetStopLightName@lvlAiMap@@SAPADH@Z
    ARTS_IMPORT static char* GetStopLightName(i32 font_cfg_template);

    // ?GetStopLightPos@lvlAiMap@@SAXHAAVVector3@@0_N@Z
    ARTS_IMPORT static void GetStopLightPos(i32 font_cfg, Vector3& rhs, Vector3& arg3, bool arg4);

    // ?GetStopLightType@lvlAiMap@@SAHH@Z
    ARTS_IMPORT static i32 GetStopLightType(i32 arg1);

    // ?GetSubwayVertex@lvlAiMap@@SA?AVVector3@@HHH@Z
    ARTS_IMPORT static Vector3 GetSubwayVertex(i32 arg1, i32 arg2, i32 arg3);

    // ?GetVertex@lvlAiMap@@SA?AVVector3@@HHH@Z
    ARTS_IMPORT static Vector3 GetVertex(i32 arg1, i32 arg2, i32 arg3);

    // ?GetVertexMulti@lvlAiMap@@SA?AVVector3@@HHH@Z
    ARTS_IMPORT static Vector3 GetVertexMulti(i32 arg1, i32 arg2, i32 arg3);

    // ?GetVertexSingle@lvlAiMap@@SA?AVVector3@@HHH@Z
    ARTS_IMPORT static Vector3 GetVertexSingle(i32 arg1, i32 arg2, i32 arg3);

    // ?GetVertexSingleCenter@lvlAiMap@@SA?AVVector3@@HHH@Z
    ARTS_IMPORT static Vector3 GetVertexSingleCenter(i32 arg1, i32 arg2, i32 arg3);

    // ?Init@lvlAiMap@@SAXH@Z
    ARTS_IMPORT static void Init(i32 arg1);

    // ?IsAlley@lvlAiMap@@SAHXZ
    ARTS_IMPORT static i32 IsAlley();

    // ?IsBlocked@lvlAiMap@@SAHH@Z
    ARTS_IMPORT static i32 IsBlocked(i32 arg1);

    // ?IsDivided@lvlAiMap@@SAHXZ
    ARTS_IMPORT static i32 IsDivided();

    // ?IsFreeway@lvlAiMap@@SAHXZ
    ARTS_IMPORT static i32 IsFreeway();

    // ?IsPedBlocked@lvlAiMap@@SAHH@Z
    ARTS_IMPORT static i32 IsPedBlocked(i32 arg1);

    // ?LoadBinary@lvlAiMap@@SAXPAVStream@@@Z
    ARTS_IMPORT static void LoadBinary(Stream* arg1);

    // ?LoadCurrent@lvlAiMap@@SAXABVlvlSDL@@HHHPBGPAX@Z
    ARTS_IMPORT static void LoadCurrent(const lvlSDL& arg1, i32 arg2, i32 arg3, i32 arg4, const u16* arg5, void* arg6);

    // ?SetRoad@lvlAiMap@@SAXABVlvlSDL@@H_N@Z
    ARTS_IMPORT static void SetRoad(const lvlSDL& arg1, i32 arg2, bool arg3);

    // ?SetRoad@lvlAiMap@@SAXPBVlvlSDL@@H_N@Z
    ARTS_IMPORT static void SetRoad(const lvlSDL* arg1, i32 arg2, bool arg3);

    // ?m_BevelMode@lvlAiMap@@2_NA
    ARTS_IMPORT static bool m_BevelMode;

    // ?m_Current@lvlAiMap@@2UlvlAiCurrent@@A
    ARTS_IMPORT static lvlAiCurrent m_Current;

    // ?m_RoadCount@lvlAiMap@@2IA
    ARTS_IMPORT static u32 m_RoadCount;

    // ?m_AiRoads@lvlAiMap@@2PAVlvlAiRoad@@A
    ARTS_IMPORT static lvlAiRoad* m_AiRoads;
};

// check_size(lvlAiMap, 0x1); // size known, members are not - cannot verify
