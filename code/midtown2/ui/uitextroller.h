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
class UIMenu;

class UITextRoller : public uiWidget
{
public:
    // ??0UITextRoller@@QAE@XZ
    ARTS_IMPORT UITextRoller();

    // ??1UITextRoller@@UAE@XZ
    ARTS_IMPORT virtual ~UITextRoller();

    // ?Cull@UITextRoller@@UAEXXZ
    ARTS_IMPORT virtual void Cull();

    // ?Update@UITextRoller@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Action@UITextRoller@@UAEXTeqEvent@@@Z
    ARTS_IMPORT virtual void Action(eqEvent arg1);

    // ?Switch@UITextRoller@@UAEXH@Z
    ARTS_IMPORT virtual void Switch(i32 arg1);

    // ?Dec@UITextRoller@@QAEHXZ
    ARTS_IMPORT i32 Dec();

    // ?EvalMouseXY@UITextRoller@@QAEXMM@Z
    ARTS_IMPORT void EvalMouseXY(f32 arg1, f32 arg2);

    // ?Inc@UITextRoller@@QAEHXZ
    ARTS_IMPORT i32 Inc();

    // ?Init@UITextRoller@@QAEXPAULocString@@PAHMMMMVstring@@HHHHVdatCallback@@PAVUIMenu@@@Z
    ARTS_IMPORT void Init(LocString* arg1, i32* arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, string arg7, i32 arg8, i32 arg9, i32 arg10, i32 arg11, datCallback arg12, UIMenu* arg13);

    // ?SetString@UITextRoller@@QAEXVstring@@@Z
    ARTS_IMPORT void SetString(string arg1);

    // ?SetText@UITextRoller@@QAEXXZ
    ARTS_IMPORT void SetText();

    // ?SetValue@UITextRoller@@QAEHH@Z
    ARTS_IMPORT i32 SetValue(i32 label_id);
};

// ??_7UITextRoller@@6B@
// vtable at 0x005B344C
// check_size(UITextRoller, 0x6C); // size known, members are not - cannot verify
