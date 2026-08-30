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

#include "aslinearcs.h"

class Bank;

class asDofCS : public asLinearCS
{
public:
    // ??0asDofCS@@QAE@XZ
    ARTS_IMPORT asDofCS();

    // ??1asDofCS@@UAE@XZ
    ARTS_IMPORT virtual ~asDofCS();

    // ?AddWidgets@asDofCS@@QAEXPAVBank@@@Z
    ARTS_IMPORT void AddWidgets(Bank* arg1);

    // ??4asDofCS@@QAEXABV0@@Z
    ARTS_IMPORT void operator=(const asDofCS& arg1);

    // ?Reset@asDofCS@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?SetTime@asDofCS@@QAEXMMH@Z
    ARTS_IMPORT void SetTime(f32 arg1, f32 arg2, i32 arg3);

    // ?Update@asDofCS@@UAEXXZ
    ARTS_IMPORT virtual void Update();
};

// ??_7asDofCS@@6B@
// vtable at 0x005B86E8
// check_size(asDofCS, 0x0); // TODO: no layout in the IDB type library
