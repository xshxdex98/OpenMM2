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

class Dialog_TCPIP : public PUMenuBase
{
public:
    // ??0Dialog_TCPIP@@QAE@HMMMMPAD@Z
    ARTS_IMPORT Dialog_TCPIP(i32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, char* arg6);

    // ??1Dialog_TCPIP@@UAE@XZ
    ARTS_IMPORT virtual ~Dialog_TCPIP();

    // ?PreSetup@Dialog_TCPIP@@UAEXXZ
    ARTS_IMPORT virtual void PreSetup();

    // ?IPAddressCallback@Dialog_TCPIP@@QAEXXZ
    ARTS_IMPORT void IPAddressCallback();

    // ?SetIPAddress@Dialog_TCPIP@@QAEXPAD@Z
    ARTS_IMPORT void SetIPAddress(char* arg1);
};

// ??_7Dialog_TCPIP@@6B@
// vtable at 0x005B41B4
// check_size(Dialog_TCPIP, 0xEC); // size known, members are not - cannot verify
