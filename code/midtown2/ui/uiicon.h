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

#include "data7/datcallback.h"
#include "vector7/vector2.h"

#include "uiwidget.h"

class gfxBitmap;

class UIIcon : public uiWidget
{
public:
    // ??0UIIcon@@QAE@XZ
    ARTS_IMPORT UIIcon();

    // ??1UIIcon@@UAE@XZ
    ARTS_IMPORT virtual ~UIIcon();

    // ?Cull@UIIcon@@UAEXXZ
    ARTS_IMPORT virtual void Cull();

    // ?GetHitArea@UIIcon@@QAEXAAM0@Z
    ARTS_IMPORT void GetHitArea(f32& arg1, f32& arg2);

    // ?Init@UIIcon@@QAEXPADMM@Z
    ARTS_IMPORT void Init(char* arg1, f32 arg2, f32 arg3);

    // ?LoadBitchmap@UIIcon@@QAEXPAD@Z
    ARTS_IMPORT void LoadBitchmap(char* arg1);

    // ?LoadBitmap@UIIcon@@QAEXPAD@Z
    ARTS_IMPORT void LoadBitmap(char* arg1);

    // ?Switch@UIIcon@@UAEXH@Z
    ARTS_IMPORT virtual void Switch(i32 arg1);

    // ?Update@UIIcon@@UAEXXZ
    ARTS_IMPORT virtual void Update();

public:
    // Members from 0x06C; everything below that belongs to uiWidget.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    Vector2 Pos; // 0x06C
    gfxBitmap* pBitmap; // 0x074
    datCallback Callback; // 0x078
};

// ??_7UIIcon@@6B@
// vtable at 0x005B37DC
check_size(UIIcon, 0x84);
