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

class gizSailboatMgr : public asNode
{
public:
    // ??0gizSailboatMgr@@QAE@XZ
    ARTS_IMPORT gizSailboatMgr();

    // ??1gizSailboatMgr@@UAE@XZ
    ARTS_IMPORT virtual ~gizSailboatMgr();

    // ?Update@gizSailboatMgr@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@gizSailboatMgr@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Init@gizSailboatMgr@@QAE_NPAD00@Z
    ARTS_IMPORT bool Init(char* arg1, char* arg2, char* arg3);

private:
    // ?ApplyTuning@gizSailboatMgr@@AAEXXZ
    ARTS_IMPORT void ApplyTuning();

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u32 nBoatCount; // 0x018
    u8 field_1C[4]; // 0x01C
    u32 pBoatArray; // 0x020
    f32 fAvgSpeed; // 0x024
    f32 fSpeedVariance; // 0x028
};

// ??_7gizSailboatMgr@@6B@
// vtable at 0x005B60A0
check_size(gizSailboatMgr, 0x2C);
