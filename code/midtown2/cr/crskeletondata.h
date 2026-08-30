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

class crBoneData;

class crSkeletonData
{
public:
    // ??0crSkeletonData@@QAE@XZ
    ARTS_IMPORT crSkeletonData();

    // ??1crSkeletonData@@QAE@XZ
    ARTS_IMPORT ~crSkeletonData();

    // ?FindBone@crSkeletonData@@QBEPAVcrBoneData@@PBDHD@Z
    ARTS_IMPORT crBoneData* FindBone(const char* arg1, i32 arg2, char arg3) const;

    // ?HowMany@crSkeletonData@@QBEHPBDD@Z
    ARTS_IMPORT i32 HowMany(const char* arg1, char arg2) const;

    // ?Load@crSkeletonData@@QAE_NPBD@Z
    ARTS_IMPORT bool Load(const char* rhs);

private:
    // ?InitMirror@crSkeletonData@@AAEXXZ
    ARTS_IMPORT void InitMirror();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u32 field_0; // 0x000
    u32 field_4; // 0x004
};

check_size(crSkeletonData, 0x8);
