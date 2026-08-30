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

#include "vector7/matrix34.h"

#include "asnode.h"

class asLinearCS : public asNode
{
public:
    // ??0asLinearCS@@QAE@XZ
    ARTS_IMPORT asLinearCS();

    // ??1asLinearCS@@UAE@XZ
    ARTS_IMPORT virtual ~asLinearCS();

    // ?Cull@asLinearCS@@UAEXXZ
    ARTS_IMPORT virtual void Cull();

    // ?Update@asLinearCS@@UAEXXZ
    ARTS_IMPORT virtual void Update();

public:
    static asLinearCS& Access_RootFrame() { return RootFrame; }
    static asLinearCS**& Access_FrameStack() { return FrameStack; }
    static Matrix34*& Access_CurrentMatrix() { return CurrentMatrix; }

private:
    // ?RootFrame@asLinearCS@@0V1@A
    ARTS_IMPORT static asLinearCS RootFrame;

    // ?FrameStack@asLinearCS@@0PAPAV1@A
    ARTS_IMPORT static asLinearCS** FrameStack;

    // ?CurrentMatrix@asLinearCS@@0PAVMatrix34@@A
    ARTS_IMPORT static Matrix34* CurrentMatrix;

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    Matrix34 field_18; // 0x018
    Matrix34 field_48; // 0x048
    i32 field_78; // 0x078
    i32 field_7C; // 0x07C
};

// ??_7asLinearCS@@6B@
// vtable at 0x005B29AC
check_size(asLinearCS, 0x80);
