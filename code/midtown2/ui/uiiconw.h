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
#include "eventq7/eqevent.h"
#include "vector7/vector2.h"

#include "uiwidget.h"

struct LocString;
class mmTextNode;

class UIIconW : public uiWidget
{
public:
    // ??0UIIconW@@QAE@XZ
    ARTS_IMPORT UIIconW();

    // ??1UIIconW@@UAE@XZ
    ARTS_IMPORT virtual ~UIIconW();

    // ?Update@UIIconW@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Action@UIIconW@@UAEXTeqEvent@@@Z
    ARTS_IMPORT virtual void Action(eqEvent arg1);

    // ?Switch@UIIconW@@UAEXH@Z
    ARTS_IMPORT virtual void Switch(i32 transformer);

    // ?Init@UIIconW@@QAEXPAULocString@@PADMMMMVdatCallback@@@Z
    ARTS_IMPORT void Init(LocString* arg1, char* arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, datCallback arg7);

public:
    // Members from 0x06C; everything below that belongs to uiWidget.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 field_6C[32]; // 0x06C
    UIIconW::UIIconW::mmToggle* pToggle; // 0x08C
    mmTextNode* pText; // 0x090
    datCallback Callback; // 0x094
    u32 field_A0; // 0x0A0
    Vector2 Size; // 0x0A4
};

// ??_7UIIconW@@6B@
// vtable at 0x005B3840
check_size(UIIconW, 0xAC);
