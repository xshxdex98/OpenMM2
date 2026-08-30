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

#include "mmwphud.h"

class mmCollideHUD : public mmWPHUD
{
public:
    // ??0mmCollideHUD@@QAE@XZ
    ARTS_IMPORT mmCollideHUD();

    // ??1mmCollideHUD@@UAE@XZ
    ARTS_IMPORT virtual ~mmCollideHUD();

    // ?Update@mmCollideHUD@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@mmCollideHUD@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Init@mmCollideHUD@@QAEXHPAH0H@Z
    ARTS_IMPORT void Init(i32 arg1, i32* arg2, i32* outHeight, i32 outWidth);

    // ?SetBangerCount@mmCollideHUD@@QAEXH@Z
    ARTS_IMPORT void SetBangerCount(i32 arg1);

    // ?SetVehicleCount@mmCollideHUD@@QAEXH@Z
    ARTS_IMPORT void SetVehicleCount(i32 arg1);
};

// ??_7mmCollideHUD@@6B@
// vtable at 0x005B1064
// check_size(mmCollideHUD, 0x18); // size known, members are not - cannot verify
