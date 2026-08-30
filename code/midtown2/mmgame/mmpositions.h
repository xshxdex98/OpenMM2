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
class Vector4;

class mmPositions
{
public:
    // ??0mmPositions@@QAE@XZ
    ARTS_IMPORT mmPositions();

    // ??1mmPositions@@QAE@XZ
    ARTS_IMPORT ~mmPositions();

    // ?Dump@mmPositions@@QAEHPAD@Z
    ARTS_IMPORT i32 Dump(char* arg1);

    // ?GetCount@mmPositions@@QAEHXZ
    ARTS_IMPORT i32 GetCount();

    // ?GetFrameRate@mmPositions@@QAEHH@Z
    ARTS_IMPORT i32 GetFrameRate(i32 arg1);

    // ?GetVector4@mmPositions@@QAEAAVVector4@@H@Z
    ARTS_IMPORT Vector4& GetVector4(i32 arg1);

    // ?Init@mmPositions@@QAEXH@Z
    ARTS_IMPORT void Init(i32 arg1);

    // ?Load@mmPositions@@QAEHPAD@Z
    ARTS_IMPORT i32 Load(char* arg1);

    // ?Recall@mmPositions@@QAEHHPAVMatrix34@@PAHPAD@Z
    ARTS_IMPORT i32 Recall(i32 arg1, Matrix34* arg2, i32* arg3, char* arg4);

    // ?Register@mmPositions@@QAEHAAVVector4@@HHHHPAD@Z
    ARTS_IMPORT i32 Register(Vector4& arg1, i32 arg2, i32 arg3, i32 arg4, i32 arg5, char* arg6);
};

// check_size(mmPositions, 0x0); // TODO: no layout in the IDB type library
