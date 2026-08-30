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

class aiIntersection;
class lvlInstance;

class aiTrafficLightSet : public asNode
{
public:
    // ??0aiTrafficLightSet@@QAE@PAVaiIntersection@@@Z
    ARTS_IMPORT aiTrafficLightSet(aiIntersection* arg1);

    // ??1aiTrafficLightSet@@UAE@XZ
    ARTS_IMPORT virtual ~aiTrafficLightSet();

    // ?Update@aiTrafficLightSet@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@aiTrafficLightSet@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?SetFourWay@aiTrafficLightSet@@QAEXXZ
    ARTS_IMPORT void SetFourWay();

public:
    static i16& Access_ObjCount() { return ObjCount; }

private:
    // ?ObjCount@aiTrafficLightSet@@0FA
    ARTS_IMPORT static i16 ObjCount;

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    lvlInstance** LvlInstances; // 0x018
    u32 field_1C; // 0x01C
    u16 IntersectionId; // 0x020
    u16 field_22; // 0x022
    u16 InstanceCount; // 0x024
    u16 field_26; // 0x026
    u32 field_28; // 0x028
    f32 field_2C; // 0x02C
    u32 field_30; // 0x030
};

// ??_7aiTrafficLightSet@@6B@
// vtable at 0x005B5528
check_size(aiTrafficLightSet, 0x34);
