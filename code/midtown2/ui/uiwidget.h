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
#include "vector7/vector4.h"

struct LocString;
class UIMenu;
union eqEvent;
class mmToolTip;

class uiWidget : public asNode
{
public:
    // ??0uiWidget@@QAE@XZ
    ARTS_IMPORT uiWidget();

    // ??1uiWidget@@UAE@XZ
    ARTS_IMPORT virtual ~uiWidget();

    // ?Disable@uiWidget@@UAEXXZ
    ARTS_IMPORT virtual void Disable();

    // ?Enable@uiWidget@@UAEXXZ
    ARTS_IMPORT virtual void Enable();

    // ?TurnOn@uiWidget@@UAEXXZ
    ARTS_IMPORT virtual void TurnOn();

    // ?TurnOff@uiWidget@@UAEXXZ
    ARTS_IMPORT virtual void TurnOff();

    // ?SetReadOnly@uiWidget@@UAEXH@Z
    ARTS_IMPORT virtual void SetReadOnly(i32 arg1);

    // ?Action@uiWidget@@UAEXTeqEvent@@@Z
    ARTS_IMPORT virtual void Action(eqEvent arg1);

    // ?CaptureAction@uiWidget@@UAEXTeqEvent@@@Z
    ARTS_IMPORT virtual void CaptureAction(eqEvent arg1);

    // ?Switch@uiWidget@@UAEXH@Z
    ARTS_IMPORT virtual void Switch(i32 arg1);

    // ?EvalMouseX@uiWidget@@UAEXM@Z
    ARTS_EXPORT virtual void EvalMouseX(f32 arg1);

    // ?ReturnDescription@uiWidget@@UAEPADXZ
    ARTS_EXPORT virtual char* ReturnDescription();

    // ?SetPosition@uiWidget@@UAEXMM@Z
    ARTS_EXPORT virtual void SetPosition(f32 arg1, f32 arg2);

    // ?GetScreenHeight@uiWidget@@UAEMXZ
    ARTS_EXPORT virtual f32 GetScreenHeight();

    // ?AddToolTip@uiWidget@@QAEXPAVUIMenu@@PAULocString@@@Z
    ARTS_EXPORT void AddToolTip(UIMenu* arg1, LocString* arg2);

    // ?ResetToolTip@uiWidget@@QAEXXZ
    ARTS_IMPORT void ResetToolTip();

    // ?SetToolTipText@uiWidget@@QAEXPAULocString@@@Z
    ARTS_IMPORT void SetToolTipText(LocString* arg1);

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    UIMenu* pParent; // 0x018
    Vector2 MinPos; // 0x01C
    Vector2 MaxPos; // 0x024
    Vector2 LastMousePos; // 0x02C
    u32 Selected; // 0x034
    u32 LastMouseAction; // 0x038
    u32 field_3C; // 0x03C
    u32 WidgetID; // 0x040
    i32 ReadOnly; // 0x044
    i8* pTooltipText; // 0x048
    u32 field_4C; // 0x04C
    Vector4 Position; // 0x050
    i32 Enabled; // 0x060
    u32 field_64; // 0x064
    mmToolTip* pTooltip; // 0x068
};

// ??_7uiWidget@@6B@
// vtable at 0x005B3374
check_size(uiWidget, 0x6C);
