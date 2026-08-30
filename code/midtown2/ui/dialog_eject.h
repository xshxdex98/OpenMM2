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

class Dialog_Eject : public PUMenuBase
{
public:
    // ??0Dialog_Eject@@QAE@HMMMMPAD@Z
    ARTS_IMPORT Dialog_Eject(i32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, char* arg6);

    // ??1Dialog_Eject@@UAE@XZ
    ARTS_IMPORT virtual ~Dialog_Eject();

    // ?AddName@Dialog_Eject@@QAEXPBDKH@Z
    ARTS_IMPORT void AddName(const char* arg1, ulong arg2, i32 arg3);

    // ?BootButtonCB@Dialog_Eject@@QAEXXZ
    ARTS_IMPORT void BootButtonCB();

    // ?ClearNames@Dialog_Eject@@QAEXXZ
    ARTS_IMPORT void ClearNames();

    // ?FindRosterName@Dialog_Eject@@QAEHK@Z
    ARTS_IMPORT i32 FindRosterName(ulong arg1);

    // ?FindRosterName@Dialog_Eject@@QAEHPBD@Z
    ARTS_IMPORT i32 FindRosterName(const char* arg1);

    // ?PostNames@Dialog_Eject@@QAEXXZ
    ARTS_IMPORT void PostNames();

    // ?PostSetup@Dialog_Eject@@UAEXXZ
    ARTS_IMPORT virtual void PostSetup();

    // ?PreSetup@Dialog_Eject@@UAEXXZ
    ARTS_IMPORT virtual void PreSetup();

    // ?RemoveName@Dialog_Eject@@QAEXK@Z
    ARTS_IMPORT void RemoveName(ulong arg1);

    // ?RemoveName@Dialog_Eject@@QAEXPBDK@Z
    ARTS_IMPORT void RemoveName(const char* arg1, ulong arg2);

    // ?SetBootCB@Dialog_Eject@@QAEXVdatCallback@@@Z
    ARTS_IMPORT void SetBootCB(datCallback arg1);
};

// ??_7Dialog_Eject@@6B@
// vtable at 0x005B3DF8
// check_size(Dialog_Eject, 0x2F8); // size known, members are not - cannot verify
