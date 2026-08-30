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

class mmCircuitHUD : public asNode
{
public:
    // ??0mmCircuitHUD@@QAE@XZ
    ARTS_IMPORT mmCircuitHUD();

    // ??1mmCircuitHUD@@UAE@XZ
    ARTS_IMPORT virtual ~mmCircuitHUD();

    // ?Update@mmCircuitHUD@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@mmCircuitHUD@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?PostUpdate@mmCircuitHUD@@UAEXXZ
    ARTS_IMPORT virtual void PostUpdate();

    // ?Init@mmCircuitHUD@@QAEXXZ
    ARTS_IMPORT void Init();

    // ?SetLapTime@mmCircuitHUD@@QAEXHMPADH@Z
    ARTS_IMPORT void SetLapTime(i32 arg1, f32 arg2, char* arg3, i32 arg4);

    // ?SetStandings@mmCircuitHUD@@QAEXHH@Z
    ARTS_IMPORT void SetStandings(i32 arg1, i32 arg2);

    // ?SetWPCleared@mmCircuitHUD@@QAEXHH@Z
    ARTS_IMPORT void SetWPCleared(i32 arg1, i32 arg2);
};

// ??_7mmCircuitHUD@@6B@
// vtable at 0x005B0FE4
// check_size(mmCircuitHUD, 0x0); // TODO: no layout in the IDB type library
