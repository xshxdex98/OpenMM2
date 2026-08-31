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

#include "uiwidget.h"

class Card2D;
struct LocString;
class mmTextNode;

class UIButton : public uiWidget
{
public:
    // ??0UIButton@@QAE@XZ
    ARTS_IMPORT UIButton();

    // ??1UIButton@@UAE@XZ
    ARTS_IMPORT virtual ~UIButton();

    // ?Update@UIButton@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Disable@UIButton@@UAEXXZ
    ARTS_IMPORT virtual void Disable();

    // ?Enable@UIButton@@UAEXXZ
    ARTS_IMPORT virtual void Enable();

    // ?SetReadOnly@UIButton@@UAEXH@Z
    ARTS_IMPORT virtual void SetReadOnly(i32 arg1);

    // ?Action@UIButton@@UAEXTeqEvent@@@Z
    ARTS_IMPORT virtual void Action(eqEvent arg1);

    // ?Switch@UIButton@@UAEXH@Z
    ARTS_IMPORT virtual void Switch(i32 arg1);

    // ?Init@UIButton@@QAEXPAULocString@@HMMMMHVdatCallback@@@Z
    ARTS_IMPORT void Init(LocString* arg1, i32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, i32 arg7, datCallback arg8);

    // ?SetString@UIButton@@QAEXPAULocString@@@Z
    ARTS_IMPORT void SetString(LocString* arg1);

    // ?SetType@UIButton@@QAEXH@Z
    ARTS_IMPORT void SetType(i32 type);

    // ?TestHit@UIButton@@QAEHMM@Z
    ARTS_IMPORT i32 TestHit(f32 arg1, f32 arg2);

private:
    // ?DrawOff@UIButton@@AAEXXZ
    ARTS_IMPORT void DrawOff();

    // ?DrawOn@UIButton@@AAEXXZ
    ARTS_IMPORT void DrawOn();

public:
    // Members from 0x06C; everything below that belongs to uiWidget.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 field_6C[16]; // 0x06C
    mmTextNode* pTextNode; // 0x07C
    Card2D* pCard2D; // 0x080
    u32* field_84; // 0x084
    u32 field_88; // 0x088
    u32 Type; // 0x08C
    u32 TextIndex; // 0x090
    u32 Flags; // 0x094
    datCallback Callback; // 0x098
};

// ??_7UIButton@@6B@
// vtable at 0x005B36B0
check_size(UIButton, 0xA4);
