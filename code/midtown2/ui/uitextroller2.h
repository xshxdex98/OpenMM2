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
#include "stream/string.h"

#include "uiwidget.h"

struct LocString;
class UIMenu;
union eqEvent;
class gfxBitmap;
class mmTextNode;

class UITextRoller2 : public uiWidget
{
public:
    // ??0UITextRoller2@@QAE@XZ
    ARTS_IMPORT UITextRoller2();

    // ??1UITextRoller2@@UAE@XZ
    ARTS_IMPORT virtual ~UITextRoller2();

    // ?Action@UITextRoller2@@UAEXTeqEvent@@@Z
    ARTS_IMPORT virtual void Action(eqEvent arg1);

    // ?Cull@UITextRoller2@@UAEXXZ
    ARTS_IMPORT virtual void Cull();

    // ?Dec@UITextRoller2@@QAEHXZ
    ARTS_IMPORT i32 Dec();

    // ?EvalMouseXY@UITextRoller2@@QAEXMM@Z
    ARTS_IMPORT void EvalMouseXY(f32 arg1, f32 arg2);

    // ?Inc@UITextRoller2@@QAEHXZ
    ARTS_IMPORT i32 Inc();

    // ?Init@UITextRoller2@@QAEXPAULocString@@PAHMMMMVstring@@HHHHVdatCallback@@PAVUIMenu@@@Z
    ARTS_IMPORT void Init(LocString* arg1, i32* arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, string arg7, i32 arg8, i32 arg9, i32 arg10, i32 arg11, datCallback arg12, UIMenu* arg13);

    // ?SetData@UITextRoller2@@QAEXPAH@Z
    ARTS_IMPORT void SetData(i32* arg1);

    // ?SetString@UITextRoller2@@QAEXVstring@@@Z
    ARTS_IMPORT void SetString(string arg1);

    // ?SetText@UITextRoller2@@QAEXXZ
    ARTS_IMPORT void SetText();

    // ?SetValue@UITextRoller2@@QAEHH@Z
    ARTS_IMPORT i32 SetValue(i32 arg1);

    // ?Switch@UITextRoller2@@UAEXH@Z
    ARTS_IMPORT virtual void Switch(i32 arg1);

    // ?Update@UITextRoller2@@UAEXXZ
    ARTS_IMPORT virtual void Update();

public:
    // Members from 0x06C; everything below that belongs to uiWidget.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 field_6C[8]; // 0x06C
    mmTextNode* field_74; // 0x074
    mmTextNode* field_78; // 0x078
    u8 field_7C[4]; // 0x07C
    i8 field_80[16]; // 0x080
    i32* CurrentValue; // 0x090
    i32 NewValue; // 0x094
    u8 field_98[4]; // 0x098
    i32 MaximumValue; // 0x09C
    i32 ClickedDown; // 0x0A0
    i32 MaxNumber; // 0x0A4
    u8 field_A8[4]; // 0x0A8
    gfxBitmap* RollerUpGfxBitmap; // 0x0AC
    gfxBitmap* RollerDownGfxBitmap; // 0x0B0
    gfxBitmap* DropFrameGfxBitmap; // 0x0B4
    gfxBitmap* BgGfxBitmap; // 0x0B8
    f32 field_BC; // 0x0BC
    f32 field_C0; // 0x0C0
    f32 field_C4; // 0x0C4
    f32 field_C8; // 0x0C8
    u8 field_CC[4]; // 0x0CC
    u8 field_D0[4]; // 0x0D0
    u8 field_D4[4]; // 0x0D4
    u8 field_D8[4]; // 0x0D8
    u8 field_DC[4]; // 0x0DC
    u8 field_E0[4]; // 0x0E0
    u8 field_E4[4]; // 0x0E4
    u8 field_E8[4]; // 0x0E8
    u8 field_EC[4]; // 0x0EC
    i32 field_F0; // 0x0F0
    i32 field_F4; // 0x0F4
    u8 field_F8[4]; // 0x0F8
    i32 field_FC; // 0x0FC
    i32 field_100; // 0x100
    i32 field_104; // 0x104
    u8 field_108[4]; // 0x108
    u8 field_10C[4]; // 0x10C
    i32 field_110; // 0x110
    i32 field_114; // 0x114
    u8 field_118[4]; // 0x118
    u8 field_11C[4]; // 0x11C
    string field_120; // 0x120
    datCallback field_128; // 0x128
};

// ??_7UITextRoller2@@6B@
// vtable at 0x005B34B4
check_size(UITextRoller2, 0x134);
