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

class Dialog_City2 : public PUMenuBase
{
public:
    // ??0Dialog_City2@@QAE@HMMMMPAD@Z
    ARTS_IMPORT Dialog_City2(i32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, char* arg6);

    // ??1Dialog_City2@@UAE@XZ
    ARTS_IMPORT virtual ~Dialog_City2();

    // ?CancelCB@Dialog_City2@@QAEXXZ
    ARTS_IMPORT void CancelCB();

    // ?DoneCB@Dialog_City2@@QAEXXZ
    ARTS_IMPORT void DoneCB();

    // ?PostSetup@Dialog_City2@@UAEXXZ
    ARTS_IMPORT virtual void PostSetup();

    // ?PreSetup@Dialog_City2@@UAEXXZ
    ARTS_IMPORT virtual void PreSetup();

    // ?ScrollCB@Dialog_City2@@QAEXXZ
    ARTS_IMPORT void ScrollCB();

    // ?SetCurrentCity@Dialog_City2@@QAEXH@Z
    ARTS_IMPORT void SetCurrentCity(i32 arg1);
};

// ??_7Dialog_City2@@6B@
// vtable at 0x005B3E40
// check_size(Dialog_City2, 0xD0); // size known, members are not - cannot verify
