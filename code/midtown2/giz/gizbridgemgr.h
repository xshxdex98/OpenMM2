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

class Vector3;
class gizBridge;

class gizBridgeMgr : public asNode
{
public:
    // ??0gizBridgeMgr@@QAE@XZ
    ARTS_IMPORT gizBridgeMgr();

    // ??1gizBridgeMgr@@UAE@XZ
    ARTS_IMPORT virtual ~gizBridgeMgr();

    // ?Cull@gizBridgeMgr@@UAEXXZ
    ARTS_IMPORT virtual void Cull();

    // ?Update@gizBridgeMgr@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@gizBridgeMgr@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?AddProximityTrigger@gizBridgeMgr@@QAEXPAVVector3@@@Z
    ARTS_IMPORT void AddProximityTrigger(Vector3* arg1);

    // ?GetBridges@gizBridgeMgr@@QAEPAVgizBridge@@H@Z
    ARTS_IMPORT gizBridge* GetBridges(i32 index);

    // ?Init@gizBridgeMgr@@QAE_NPAD00@Z
    ARTS_IMPORT bool Init(char* arg1, char* arg2, char* arg3);

private:
    // ?CheckProximity@gizBridgeMgr@@AAE_NPAVgizBridge@@@Z
    ARTS_IMPORT bool CheckProximity(gizBridge* arg1);

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u32 BridgeCount2; // 0x018
    u32 field_1C; // 0x01C
    gizBridge* lpBridgeArray; // 0x020
    u32 BridgeCount; // 0x024
    Vector3* field_28; // 0x028
    u32 field_2C; // 0x02C
    u32 field_30; // 0x030
    u32 field_34; // 0x034
    u32 field_38; // 0x038
    u32 field_3C; // 0x03C
    u32 field_40; // 0x040
    u32 field_44; // 0x044
    u32 field_48; // 0x048
    f32 MaxDrawDistance; // 0x04C
};

// ??_7gizBridgeMgr@@6B@
// vtable at 0x005B6004
check_size(gizBridgeMgr, 0x50);
