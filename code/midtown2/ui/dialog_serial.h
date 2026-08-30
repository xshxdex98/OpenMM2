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

struct NETCOMMPACK;

class Dialog_Serial : public PUMenuBase
{
public:
    // ??0Dialog_Serial@@QAE@HMMMMPAD@Z
    ARTS_IMPORT Dialog_Serial(i32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, char* arg6);

    // ??1Dialog_Serial@@UAE@XZ
    ARTS_IMPORT virtual ~Dialog_Serial();

    // ?PreSetup@Dialog_Serial@@UAEXXZ
    ARTS_IMPORT virtual void PreSetup();

    // ?BuildComs@Dialog_Serial@@QAEXXZ
    ARTS_IMPORT void BuildComs();

    // ?GetCommPack@Dialog_Serial@@QAEPAUNETCOMMPACK@@XZ
    ARTS_IMPORT NETCOMMPACK* GetCommPack();

    // ?IPAddressCallback@Dialog_Serial@@QAEXXZ
    ARTS_IMPORT void IPAddressCallback();
};

// ??_7Dialog_Serial@@6B@
// vtable at 0x005B4124
// check_size(Dialog_Serial, 0x144); // size known, members are not - cannot verify
