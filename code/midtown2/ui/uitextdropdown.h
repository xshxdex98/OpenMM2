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
#include "stream/string.h"

#include "uiwidget.h"

struct LocString;

class UITextDropdown : public uiWidget
{
public:
    // ??0UITextDropdown@@QAE@XZ
    ARTS_IMPORT UITextDropdown();

    // ??1UITextDropdown@@UAE@XZ
    ARTS_IMPORT virtual ~UITextDropdown();

    // ?Cull@UITextDropdown@@UAEXXZ
    ARTS_IMPORT virtual void Cull();

    // ?Update@UITextDropdown@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Action@UITextDropdown@@UAEXTeqEvent@@@Z
    ARTS_IMPORT virtual void Action(eqEvent arg1);

    // ?CaptureAction@UITextDropdown@@UAEXTeqEvent@@@Z
    ARTS_IMPORT virtual void CaptureAction(eqEvent arg1);

    // ?Switch@UITextDropdown@@UAEXH@Z
    ARTS_IMPORT virtual void Switch(i32 arg1);

    // ?GetScreenHeight@UITextDropdown@@UAEMXZ
    ARTS_IMPORT virtual f32 GetScreenHeight();

    // ?AnyEnabled@UITextDropdown@@QAEHXZ
    ARTS_IMPORT i32 AnyEnabled();

    // ?AssignString@UITextDropdown@@QAEXVstring@@@Z
    ARTS_IMPORT void AssignString(string arg1);

    // ?GetValue@UITextDropdown@@QAEHXZ
    ARTS_IMPORT i32 GetValue();

    // ?Init@UITextDropdown@@QAEXPAULocString@@PAHMMMMVstring@@HHHVdatCallback@@PAD3@Z
    ARTS_IMPORT void Init(LocString* arg1, i32* arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, string arg7, i32 arg8, i32 arg9, i32 arg10, datCallback arg11, char* arg12, datCallback arg13);

    // ?SetData@UITextDropdown@@QAEXPAH@Z
    ARTS_IMPORT void SetData(i32* arg1);

    // ?SetDisabledMask@UITextDropdown@@QAEXJ@Z
    ARTS_IMPORT void SetDisabledMask(ilong arg1);

    // ?SetPos@UITextDropdown@@QAEXMM@Z
    ARTS_IMPORT void SetPos(f32 arg1, f32 arg2);

    // ?SetSliderFocus@UITextDropdown@@QAEXH@Z
    ARTS_IMPORT void SetSliderFocus(i32 arg1);

    // ?SetText@UITextDropdown@@QAEXPAULocString@@@Z
    ARTS_IMPORT void SetText(LocString* arg1);

    // ?SetValue@UITextDropdown@@QAEHH@Z
    ARTS_IMPORT i32 SetValue(i32 arg1);
};

// ??_7UITextDropdown@@6B@
// vtable at 0x005B33E0
// check_size(UITextDropdown, 0x6C); // size known, members are not - cannot verify
