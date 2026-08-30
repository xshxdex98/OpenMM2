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

union eqEvent;

class UIVScrollBar : public uiWidget
{
public:
    // ??0UIVScrollBar@@QAE@XZ
    ARTS_IMPORT UIVScrollBar();

    // ??1UIVScrollBar@@UAE@XZ
    ARTS_IMPORT virtual ~UIVScrollBar();

    // ?Cull@UIVScrollBar@@UAEXXZ
    ARTS_IMPORT virtual void Cull();

    // ?Update@UIVScrollBar@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Disable@UIVScrollBar@@UAEXXZ
    ARTS_IMPORT virtual void Disable();

    // ?Enable@UIVScrollBar@@UAEXXZ
    ARTS_IMPORT virtual void Enable();

    // ?Action@UIVScrollBar@@UAEXTeqEvent@@@Z
    ARTS_IMPORT virtual void Action(eqEvent arg1);

    // ?Switch@UIVScrollBar@@UAEXH@Z
    ARTS_IMPORT virtual void Switch(i32 arg1);

    // ?SetPosition@UIVScrollBar@@UAEXMM@Z
    ARTS_IMPORT virtual void SetPosition(f32 arg1, f32 arg2);

    // ?Dec@UIVScrollBar@@QAEMXZ
    ARTS_IMPORT f32 Dec();

    // ?EvalMouseXY@UIVScrollBar@@QAEXMM@Z
    ARTS_IMPORT void EvalMouseXY(f32 arg1, f32 arg2);

    // ?Inc@UIVScrollBar@@QAEMXZ
    ARTS_IMPORT f32 Inc();

    // ?Init@UIVScrollBar@@QAEXMMMMHMMPAHVdatCallback@@H@Z
    ARTS_IMPORT void Init(f32 arg1, f32 arg2, f32 arg3, f32 arg4, i32 arg5, f32 arg6, f32 arg7, i32* arg8, datCallback arg9, i32 arg10);

    // ?LoadBitmap@UIVScrollBar@@QAEXXZ
    ARTS_IMPORT void LoadBitmap();

    // ?SetHotSpots@UIVScrollBar@@QAEXXZ
    ARTS_IMPORT void SetHotSpots();

    // ?SetRange@UIVScrollBar@@QAEXMM@Z
    ARTS_IMPORT void SetRange(f32 arg1, f32 arg2);

    // ?SetStep@UIVScrollBar@@QAEXHM@Z
    ARTS_IMPORT void SetStep(i32 arg1, f32 arg2);

    // ?SetTrough@UIVScrollBar@@QAEXMM@Z
    ARTS_IMPORT void SetTrough(f32 arg1, f32 arg2);
};

// ??_7UIVScrollBar@@6B@
// vtable at 0x005B3ACC
// check_size(UIVScrollBar, 0x0); // TODO: no layout in the IDB type library
