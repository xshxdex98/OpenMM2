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

class mmHUD;
class mmHudMap;
class mmPlayer;

class mmViewMgr : public asNode
{
public:
    // ??0mmViewMgr@@QAE@XZ
    ARTS_IMPORT mmViewMgr();

    // ??1mmViewMgr@@UAE@XZ
    ARTS_IMPORT virtual ~mmViewMgr();

    // ?Update@mmViewMgr@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Init@mmViewMgr@@QAEXPAVmmHUD@@PAVmmHudMap@@PAVmmPlayer@@@Z
    ARTS_IMPORT void Init(mmHUD* arg1, mmHudMap* arg2, mmPlayer* arg3);

    // ?SetViewSetting@mmViewMgr@@QAEXH@Z
    ARTS_IMPORT void SetViewSetting(i32 arg1);
};

// ??_7mmViewMgr@@6B@
// vtable at 0x005B0DE8
// check_size(mmViewMgr, 0x0); // TODO: no layout in the IDB type library
