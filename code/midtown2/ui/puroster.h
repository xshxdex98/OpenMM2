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

#include "pumenubase.h"

class PURoster : public PUMenuBase
{
public:
    // ??0PURoster@@QAE@HMMMM@Z
    ARTS_IMPORT PURoster(i32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5);

    // ??1PURoster@@UAE@XZ
    ARTS_IMPORT virtual ~PURoster();

    // ?AddName@PURoster@@QAEXPBDKH@Z
    ARTS_IMPORT void AddName(const char* arg1, ulong arg2, i32 arg3);

    // ?BootButtonCB@PURoster@@QAEXXZ
    ARTS_IMPORT void BootButtonCB();

    // ?ClearNames@PURoster@@QAEXXZ
    ARTS_IMPORT void ClearNames();

    // ?FindRosterName@PURoster@@QAEHK@Z
    ARTS_IMPORT i32 FindRosterName(ulong arg1);

    // ?FindRosterName@PURoster@@QAEHPBD@Z
    ARTS_IMPORT i32 FindRosterName(const char* arg1);

    // ?PostNames@PURoster@@QAEXXZ
    ARTS_IMPORT void PostNames();

    // ?RemoveName@PURoster@@QAEXK@Z
    ARTS_IMPORT void RemoveName(ulong arg1);

    // ?RemoveName@PURoster@@QAEXPBDK@Z
    ARTS_IMPORT void RemoveName(const char* arg1, ulong arg2);

    // ?Reset@PURoster@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?SetBootCB@PURoster@@QAEXVdatCallback@@@Z
    ARTS_IMPORT void SetBootCB(datCallback arg1);

    // ?SetHost@PURoster@@QAEXH@Z
    ARTS_IMPORT void SetHost(i32 arg1);
};

// ??_7PURoster@@6B@
// vtable at 0x005B4768
// check_size(PURoster, 0x0); // TODO: no layout in the IDB type library
