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

class Card2D;
struct LocString;
class UIMenu;
union eqEvent;
class mmTextNode;

class UITextField : public uiWidget
{
public:
    // ??0UITextField@@QAE@XZ
    ARTS_IMPORT UITextField();

    // ??1UITextField@@UAE@XZ
    ARTS_IMPORT virtual ~UITextField();

    // ?Update@UITextField@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Action@UITextField@@UAEXTeqEvent@@@Z
    ARTS_IMPORT virtual void Action(eqEvent arg1);

    // ?CaptureAction@UITextField@@UAEXTeqEvent@@@Z
    ARTS_IMPORT virtual void CaptureAction(eqEvent arg1);

    // ?Switch@UITextField@@UAEXH@Z
    ARTS_IMPORT virtual void Switch(i32 arg1);

    // ?ClearField@UITextField@@QAEXXZ
    ARTS_IMPORT void ClearField();

    // ?Init@UITextField@@QAEXPAULocString@@PADMMMMHHHHHVdatCallback@@PAVUIMenu@@@Z
    ARTS_IMPORT void Init(LocString* arg1, char* arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, i32 arg7, i32 arg8, i32 arg9, i32 arg10, i32 arg11, datCallback arg12, UIMenu* arg13);

    // ?IsValidChar@UITextField@@QAEHG@Z
    ARTS_IMPORT i32 IsValidChar(u16 label_id);

    // ?KeyAction@UITextField@@QAEXPATeqEvent@@@Z
    ARTS_IMPORT void KeyAction(eqEvent* arg1);

    // ?SetCompositionWindow@UITextField@@QAEXXZ
    ARTS_IMPORT void SetCompositionWindow();

    // ?SetField@UITextField@@QAEXPAD@Z
    ARTS_IMPORT void SetField(char* arg1);

    // ?SetText@UITextField@@QAEXPAULocString@@@Z
    ARTS_IMPORT void SetText(LocString* arg1);

    // ?SetTextField@UITextField@@QAEXXZ
    ARTS_IMPORT void SetTextField();

    // ?ToggleField@UITextField@@QAEXH@Z
    ARTS_IMPORT void ToggleField(i32 arg1);

    // ?WmCharHandler@UITextField@@QAEXE@Z
    ARTS_IMPORT void WmCharHandler(u8 TestChar);

public:
    // Members from 0x06C; everything below that belongs to uiWidget.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u32 Flags; // 0x06C
    u32 field_70; // 0x070
    mmTextNode* pTextNode; // 0x074
    mmTextNode* pLabelText; // 0x078
    u32 field_7C; // 0x07C
    Card2D* pCard; // 0x080
    i8* pInputBuffer; // 0x084
    i8 FieldBuffer[256]; // 0x088
    i8 FieldFormat[16]; // 0x188
    i8* pCurrentInput; // 0x198
    u32 field_19C; // 0x19C
    u32 FontIndex; // 0x1A0
    u32 DefaultInputLength; // 0x1A4
    u32 MaxFieldLength; // 0x1A8
    u32 field_1AC; // 0x1AC
    u32 TextIndex; // 0x1B0
    u32 Toggled; // 0x1B4
    u32 field_1B8; // 0x1B8
    u32 field_1BC; // 0x1BC
    u32 field_1C0; // 0x1C0
    u32 field_1C4; // 0x1C4
    Vector2 Pos; // 0x1C8
    Vector2 Size; // 0x1D0
    datCallback Callback; // 0x1D8
};

// ??_7UITextField@@6B@
// vtable at 0x005B3310
check_size(UITextField, 0x1E4);
