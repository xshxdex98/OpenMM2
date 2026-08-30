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
class mmIO;

class UIControlWidget : public uiWidget
{
public:
    // ??0UIControlWidget@@QAE@XZ
    ARTS_IMPORT UIControlWidget();

    // ??1UIControlWidget@@UAE@XZ
    ARTS_IMPORT virtual ~UIControlWidget();

    // ?Update@UIControlWidget@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?TurnOn@UIControlWidget@@UAEXXZ
    ARTS_IMPORT virtual void TurnOn();

    // ?TurnOff@UIControlWidget@@UAEXXZ
    ARTS_IMPORT virtual void TurnOff();

    // ?Action@UIControlWidget@@UAEXTeqEvent@@@Z
    ARTS_IMPORT virtual void Action(eqEvent arg1);

    // ?Switch@UIControlWidget@@UAEXH@Z
    ARTS_IMPORT virtual void Switch(i32 transformer);

    // ?DisableField@UIControlWidget@@QAEXXZ
    ARTS_IMPORT void DisableField();

    // ?EnableField@UIControlWidget@@QAEXXZ
    ARTS_IMPORT void EnableField();

    // ?Init@UIControlWidget@@QAEXMMMMMPAVmmIO@@VdatCallback@@@Z
    ARTS_IMPORT void Init(f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, mmIO* arg6, datCallback arg7);

    // ?SetPosition@UIControlWidget@@QAEXMMMM@Z
    ARTS_IMPORT void SetPosition(f32 arg1, f32 arg2, f32 arg3, f32 arg4);

    // ?UpdateField@UIControlWidget@@QAEXXZ
    ARTS_IMPORT void UpdateField();
};

// ??_7UIControlWidget@@6B@
// vtable at 0x005B35E8
// check_size(UIControlWidget, 0x6C); // size known, members are not - cannot verify
