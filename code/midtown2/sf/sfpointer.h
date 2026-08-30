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
#include "vector7/vector2.h"

class gfxBitmap;
class uiWidget;

class sfPointer : public asNode
{
public:
    // ??0sfPointer@@QAE@XZ
    ARTS_IMPORT sfPointer();

    // ??1sfPointer@@UAE@XZ
    ARTS_IMPORT virtual ~sfPointer();

    // ?Cull@sfPointer@@UAEXXZ
    ARTS_IMPORT virtual void Cull();

    // ?GetPointerHeight@sfPointer@@QAEMXZ
    ARTS_IMPORT f32 GetPointerHeight();

    // ?Init@sfPointer@@QAEXXZ
    ARTS_IMPORT void Init();

    // ?ResChange@sfPointer@@UAEXHH@Z
    ARTS_IMPORT virtual void ResChange(i32 arg1, i32 arg2);

    // ?Update@sfPointer@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?WaitForRelease@sfPointer@@QAEXXZ
    ARTS_IMPORT void WaitForRelease();

private:
    // ?UpdateAttached@sfPointer@@AAEXXZ
    ARTS_IMPORT void UpdateAttached();

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    gfxBitmap* pCursorBitmap; // 0x018
    Vector2 CurrentX; // 0x01C
    Vector2 PrevX; // 0x024
    u32 ScreenW; // 0x02C
    u32 ScreenH; // 0x030
    u32 field_34; // 0x034
    u32 field_38; // 0x038
    u32 State; // 0x03C
    Vector2 field_40; // 0x040
    uiWidget* pSelectedWidget; // 0x048
};

// ??_7sfPointer@@6B@
// vtable at 0x005B3B30
check_size(sfPointer, 0x4C);
