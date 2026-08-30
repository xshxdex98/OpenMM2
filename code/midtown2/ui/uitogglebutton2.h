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

#include "uibutton.h"

struct LocString;
union eqEvent;

class UIToggleButton2 : public UIButton
{
public:
    // ??0UIToggleButton2@@QAE@XZ
    ARTS_IMPORT UIToggleButton2();

    // ??1UIToggleButton2@@UAE@XZ
    ARTS_IMPORT virtual ~UIToggleButton2();

    // ?Update@UIToggleButton2@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Disable@UIToggleButton2@@UAEXXZ
    ARTS_IMPORT virtual void Disable();

    // ?Enable@UIToggleButton2@@UAEXXZ
    ARTS_IMPORT virtual void Enable();

    // ?Action@UIToggleButton2@@UAEXTeqEvent@@@Z
    ARTS_IMPORT virtual void Action(eqEvent arg1);

    // ?Switch@UIToggleButton2@@UAEXH@Z
    ARTS_IMPORT virtual void Switch(i32 arg1);

    // ?DoToggle@UIToggleButton2@@UAEXXZ
    ARTS_IMPORT virtual void DoToggle();

    // ?Init@UIToggleButton2@@QAEXPAULocString@@PAHMMMMHHVdatCallback@@@Z
    ARTS_IMPORT void Init(LocString* arg1, i32* arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, i32 arg7, i32 arg8, datCallback arg9);

    // ?Unkill@UIToggleButton2@@QAEXXZ
    ARTS_IMPORT void Unkill();

protected:
    // ?DrawOff@UIToggleButton2@@MAEXXZ
    ARTS_IMPORT virtual void DrawOff();

    // ?DrawOn@UIToggleButton2@@MAEXXZ
    ARTS_IMPORT virtual void DrawOn();
};

// ??_7UIToggleButton2@@6B@
// vtable at 0x005B3914
// check_size(UIToggleButton2, 0x0); // TODO: no layout in the IDB type library
