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

#include "arts7/asnode.h"
#include "vector7/matrix34.h"
#include "vector7/vector3.h"

class mmPlayer;

class mmWaypoints : public asNode
{
public:
    // ??0mmWaypoints@@QAE@XZ
    ARTS_IMPORT mmWaypoints();

    // ??1mmWaypoints@@UAE@XZ
    ARTS_IMPORT virtual ~mmWaypoints();

    // ?Cull@mmWaypoints@@UAEXXZ
    ARTS_IMPORT virtual void Cull();

    // ?Update@mmWaypoints@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@mmWaypoints@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?AIWPHit@mmWaypoints@@QAEHHHVMatrix34@@VVector3@@M@Z
    ARTS_IMPORT i32 AIWPHit(i32 arg1, i32 arg2, Matrix34 arg3, Vector3 arg4, f32 arg5);

    // ?AnyDrawn@mmWaypoints@@QAEHXZ
    ARTS_IMPORT i32 AnyDrawn();

    // ?AnyWPHits@mmWaypoints@@QAEHH@Z
    ARTS_IMPORT i32 AnyWPHits(i32 arg1);

    // ?DeactivateFinish@mmWaypoints@@QAEXXZ
    ARTS_IMPORT void DeactivateFinish();

    // ?DisplayHUDMessage@mmWaypoints@@QAEXH@Z
    ARTS_IMPORT void DisplayHUDMessage(i32 arg1);

    // ?GetClosestWaypoint@mmWaypoints@@QAEXXZ
    ARTS_IMPORT void GetClosestWaypoint();

    // ?GetCurrentDistance@mmWaypoints@@QAEMXZ
    ARTS_IMPORT f32 GetCurrentDistance();

    // ?GetHeading@mmWaypoints@@QAEMH@Z
    ARTS_IMPORT f32 GetHeading(i32 arg1);

    // ?GetHitRoom@mmWaypoints@@QAEHH@Z
    ARTS_IMPORT i32 GetHitRoom(i32 arg1);

    // ?GetLastWaypoint@mmWaypoints@@QAEXXZ
    ARTS_IMPORT void GetLastWaypoint();

    // ?GetNextWaypoint@mmWaypoints@@QAEXXZ
    ARTS_IMPORT void GetNextWaypoint();

    // ?GetStart@mmWaypoints@@QAEXAAVVector3@@@Z
    ARTS_IMPORT void GetStart(Vector3& arg1);

    // ?GetStartAngle@mmWaypoints@@QAEMXZ
    ARTS_IMPORT f32 GetStartAngle();

    // ?GetWaypoint@mmWaypoints@@QAEXHAAVVector3@@@Z
    ARTS_IMPORT void GetWaypoint(i32 arg1, Vector3& arg2);

    // ?Init@mmWaypoints@@QAEHPAVmmPlayer@@PADHHHHI@Z
    ARTS_IMPORT i32 Init(mmPlayer* arg1, char* arg2, i32 arg3, i32 arg4, i32 arg5, i32 arg6, u32 arg7);

    // ?InitStatic@mmWaypoints@@QAEXPAVmmPlayer@@HI@Z
    ARTS_IMPORT void InitStatic(mmPlayer* arg1, i32 arg2, u32 arg3);

    // ?LoadCSV@mmWaypoints@@QAEHPADH@Z
    ARTS_IMPORT i32 LoadCSV(char* node, i32 pos);

    // ?ReInit@mmWaypoints@@QAEXHPADI@Z
    ARTS_IMPORT void ReInit(i32 ctx, char* parent_node, u32 split_axis);

    // ?ResetAllTags@mmWaypoints@@QAEXXZ
    ARTS_IMPORT void ResetAllTags();

    // ?SaveCSV@mmWaypoints@@QAEHPAD@Z
    ARTS_IMPORT i32 SaveCSV(char* arg1);

    // ?SetArrow@mmWaypoints@@QAEXXZ
    ARTS_IMPORT void SetArrow();

protected:
    // ?ClearWaypoint@mmWaypoints@@IAEXH@Z
    ARTS_IMPORT void ClearWaypoint(i32 ctx);

    // ?CycleCurrentWaypoint@mmWaypoints@@IAEXH@Z
    ARTS_IMPORT void CycleCurrentWaypoint(i32 arg1);

    // ?GenerateHitRooms@mmWaypoints@@IAEXXZ
    ARTS_IMPORT void GenerateHitRooms();

    // ?SetCurrentGoals@mmWaypoints@@IAEXH@Z
    ARTS_IMPORT void SetCurrentGoals(i32 node);

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i32 field_18; // 0x018
    u8 field_1C; // 0x01C
    u8 field_1D; // 0x01D
    u8 field_1E; // 0x01E
    u8 field_1F[1]; // 0x01F
    i32 field_20; // 0x020
    i32 field_24; // 0x024
    i32 field_28; // 0x028
    i32 field_2C; // 0x02C
    i32 field_30; // 0x030
    i32 field_34; // 0x034
    i32 field_38; // 0x038
    i32 field_3C; // 0x03C
    i32 field_40; // 0x040
    void* field_44; // 0x044
    i32 field_48; // 0x048
    i32 field_4C; // 0x04C
    i32 field_50; // 0x050
    i32 field_54; // 0x054
    u8 field_58[40]; // 0x058
    i32 field_80; // 0x080
    i32 field_84; // 0x084
    void* field_88; // 0x088
    void* field_8C; // 0x08C
};

// ??_7mmWaypoints@@6B@
// vtable at 0x005B0F34
check_size(mmWaypoints, 0x90);
