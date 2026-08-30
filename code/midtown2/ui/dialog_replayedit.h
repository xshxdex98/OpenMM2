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

#include "pumenubase.h"

class Dialog_ReplayEdit : public PUMenuBase
{
public:
    // ??0Dialog_ReplayEdit@@QAE@HMMMMPAD@Z
    ARTS_IMPORT Dialog_ReplayEdit(i32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, char* arg6);

    // ??1Dialog_ReplayEdit@@UAE@XZ
    ARTS_IMPORT virtual ~Dialog_ReplayEdit();

    // ?PreSetup@Dialog_ReplayEdit@@UAEXXZ
    ARTS_IMPORT virtual void PreSetup();

    // ?ReplayDescCallback@Dialog_ReplayEdit@@QAEXXZ
    ARTS_IMPORT void ReplayDescCallback();

    // ?SetDesc@Dialog_ReplayEdit@@QAEXPAD@Z
    ARTS_IMPORT void SetDesc(char* arg1);
};

// ??_7Dialog_ReplayEdit@@6B@
// vtable at 0x005B3E88
// check_size(Dialog_ReplayEdit, 0x114); // size known, members are not - cannot verify
