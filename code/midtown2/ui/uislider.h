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

#include "uiwidget.h"

struct LocString;
class UIMenu;
union eqEvent;

class UISlider : public uiWidget
{
public:
    // ??0UISlider@@QAE@XZ
    ARTS_IMPORT UISlider();

    // ??1UISlider@@UAE@XZ
    ARTS_IMPORT virtual ~UISlider();

    // ?Update@UISlider@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Action@UISlider@@UAEXTeqEvent@@@Z
    ARTS_IMPORT virtual void Action(eqEvent arg1);

    // ?Switch@UISlider@@UAEXH@Z
    ARTS_IMPORT virtual void Switch(i32 arg1);

    // ?SetPosition@UISlider@@UAEXMM@Z
    ARTS_IMPORT virtual void SetPosition(f32 arg1, f32 arg2);

    // ?GetScreenHeight@UISlider@@UAEMXZ
    ARTS_IMPORT virtual f32 GetScreenHeight();

    // ?EvalMouseXY@UISlider@@QAEXMM@Z
    ARTS_IMPORT void EvalMouseXY(f32 arg1, f32 arg2);

    // ?GetFudgeWidth@UISlider@@QAEMXZ
    ARTS_IMPORT f32 GetFudgeWidth();

    // ?GetValue@UISlider@@QAEMXZ
    ARTS_IMPORT f32 GetValue();

    // ?Init@UISlider@@QAEXPAULocString@@PAMMMMMMMHHHHVdatCallback@@PAVUIMenu@@2@Z
    ARTS_IMPORT void Init(LocString* arg1, f32* arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, i32 arg9, i32 arg10, i32 arg11, i32 arg12, datCallback arg13, UIMenu* arg14, datCallback arg15);

    // ?IsReadWrite@UISlider@@QAEHXZ
    ARTS_IMPORT i32 IsReadWrite();

    // ?SetData@UISlider@@QAEXM@Z
    ARTS_IMPORT void SetData(f32 arg1);

    // ?SetMouseParams@UISlider@@QAEXM@Z
    ARTS_IMPORT void SetMouseParams(f32 arg1);

    // ?SetReadWrite@UISlider@@QAEXH@Z
    ARTS_IMPORT void SetReadWrite(i32 arg1);

    // ?SetText@UISlider@@QAEXPAULocString@@@Z
    ARTS_IMPORT void SetText(LocString* arg1);

    // ?SetValue@UISlider@@QAEMM@Z
    ARTS_IMPORT f32 SetValue(f32 arg1);

    // ?TestHit@UISlider@@QAEHMM@Z
    ARTS_IMPORT i32 TestHit(f32 arg1, f32 arg2);
};

// ??_7UISlider@@6B@
// vtable at 0x005B364C
// check_size(UISlider, 0x6C); // size known, members are not - cannot verify
