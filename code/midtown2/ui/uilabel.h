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

#include "uiwidget.h"

struct LocString;
union eqEvent;
class mmTextNode;

class UILabel : public uiWidget
{
public:
    // ??0UILabel@@QAE@XZ
    ARTS_IMPORT UILabel();

    // ??1UILabel@@UAE@XZ
    ARTS_IMPORT virtual ~UILabel();

    // ?Action@UILabel@@UAEXTeqEvent@@@Z
    ARTS_IMPORT virtual void Action(eqEvent arg1);

    // ?Init@UILabel@@QAEXPAULocString@@HMMMMH@Z
    ARTS_IMPORT void Init(LocString* arg1, i32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, i32 arg7);

    // ?SetBlink@UILabel@@QAEXH@Z
    ARTS_IMPORT void SetBlink(i32 arg1);

    // ?SetText@UILabel@@QAEXPAULocString@@@Z
    ARTS_IMPORT void SetText(LocString* arg1);

    // ?Switch@UILabel@@UAEXH@Z
    ARTS_IMPORT virtual void Switch(i32 arg1);

    // ?SwitchState@UILabel@@QAEXH@Z
    ARTS_IMPORT void SwitchState(i32 arg1);

    // ?Update@UILabel@@UAEXXZ
    ARTS_IMPORT virtual void Update();

public:
    // Members from 0x06C; everything below that belongs to uiWidget.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    mmTextNode* pTextNode; // 0x06C
    HFONT hFont; // 0x070
    u32 Flags; // 0x074
    u32 State; // 0x078
    f32 ElapsedTime; // 0x07C
};

// ??_7UILabel@@6B@
// vtable at 0x005B3714
check_size(UILabel, 0x80);
