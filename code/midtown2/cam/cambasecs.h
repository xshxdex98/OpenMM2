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

class Vector3;
class datParser;

class camBaseCS : public asNode
{
public:
    // ??0camBaseCS@@QAE@XZ
    ARTS_IMPORT camBaseCS();

    // ??1camBaseCS@@UAE@XZ
    ARTS_IMPORT virtual ~camBaseCS();

    // ?FileIO@camBaseCS@@UAEXAAVdatParser@@@Z
    ARTS_IMPORT virtual void FileIO(datParser& arg1);

    // ?AfterLoad@camBaseCS@@UAEXXZ
    ARTS_IMPORT virtual void AfterLoad();

    // ?GetDirName@camBaseCS@@UAEPBDXZ
    ARTS_IMPORT virtual const char* GetDirName();

    // ?MakeActive@camBaseCS@@UAEXXZ
    ARTS_IMPORT virtual void MakeActive();

    // ?UpdateInput@camBaseCS@@UAEXXZ
    ARTS_IMPORT virtual void UpdateInput();

    // ?ForceMatrixDelta@camBaseCS@@UAEXABVVector3@@@Z
    ARTS_IMPORT virtual void ForceMatrixDelta(const Vector3& arg1);

    // ?SetST@camBaseCS@@UAEXPAM@Z
    ARTS_IMPORT virtual void SetST(f32* arg1);

    // ?IsViewCSInTransition@camBaseCS@@QAEHXZ
    ARTS_IMPORT i32 IsViewCSInTransition();

    // ?UpdateView@camBaseCS@@QAEXXZ
    ARTS_IMPORT void UpdateView();

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u16 field_18; // 0x018
    u8 field_1A[2]; // 0x01A
    Matrix34 field_1C; // 0x01C
    Matrix34 field_4C; // 0x04C
    i32 field_7C; // 0x07C
    f32 BlendTime; // 0x080
    f32 BlendGoal; // 0x084
    f32 CameraFOV; // 0x088
    f32 CameraNear; // 0x08C
};

// ??_7camBaseCS@@6B@
// vtable at 0x005B5130
check_size(camBaseCS, 0x90);
