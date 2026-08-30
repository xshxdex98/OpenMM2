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
class crBone;
class datTokenizer;

class crBoneData
{
public:
    // ??0crBoneData@@QAE@XZ
    ARTS_IMPORT crBoneData();

    // ?AddChild@crBoneData@@QAEXPAV1@@Z
    ARTS_IMPORT void AddChild(crBoneData* arg1);

    // ?ApplyLimits@crBoneData@@QBEXAAVMatrix34@@@Z
    ARTS_IMPORT void ApplyLimits(Matrix34& arg1) const;

    // ?Load@crBoneData@@QAE_NAAVdatTokenizer@@PAPAV1@AAH@Z
    ARTS_IMPORT bool Load(datTokenizer& instance, crBoneData** arg2, i32& arg3);

    // ?Transform@crBoneData@@QBEXPBVMatrix34@@PAVcrBone@@@Z
    ARTS_IMPORT void Transform(const Matrix34* arg1, crBone* arg2) const;
};

// check_size(crBoneData, 0x0); // TODO: no layout in the IDB type library
