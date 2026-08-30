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

class Matrix34;
class Matrix44;
class crSkeletonData;

class crSkeleton
{
public:
    // ??0crSkeleton@@QAE@XZ
    ARTS_IMPORT crSkeleton();

    // ??1crSkeleton@@QAE@XZ
    ARTS_IMPORT ~crSkeleton();

    // ?Attach@crSkeleton@@QBEXQAVMatrix34@@@Z
    ARTS_IMPORT void Attach(Matrix34*const size) const;

    // ?Attach@crSkeleton@@QBEXQAVMatrix44@@@Z
    ARTS_IMPORT void Attach(Matrix44*const arg1) const;

    // ?Init@crSkeleton@@QAEXABVcrSkeletonData@@PAVMatrix34@@@Z
    ARTS_IMPORT void Init(const crSkeletonData& arg1, Matrix34* arg2);

    // ?Update@crSkeleton@@QAEXXZ
    ARTS_IMPORT void Update();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i32* pMatrixCount; // 0x000
    Matrix44* field_4; // 0x004
    Matrix34* Matrices; // 0x008
    u32 field_C; // 0x00C
};

check_size(crSkeleton, 0x10);
