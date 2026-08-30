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

#include "uimenu.h"

class gfxBitmap;

class PUMenuBase : public UIMenu
{
public:
    // ??0PUMenuBase@@QAE@HMMMMPAD_N@Z
    ARTS_IMPORT PUMenuBase(i32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, char* arg6, bool arg7);

    // ??1PUMenuBase@@UAE@XZ
    ARTS_IMPORT virtual ~PUMenuBase();

    // ?Cull@PUMenuBase@@UAEXXZ
    ARTS_IMPORT virtual void Cull();

    // ?Update@PUMenuBase@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?AddExit@PUMenuBase@@QAEMMMMM@Z
    ARTS_IMPORT f32 AddExit(f32 arg1, f32 arg2, f32 arg3, f32 arg4);

    // ?AddOKCancel@PUMenuBase@@QAEXVdatCallback@@0@Z
    ARTS_IMPORT void AddOKCancel(datCallback arg1, datCallback arg2);

    // ?AddPrevious@PUMenuBase@@QAEMMMMM@Z
    ARTS_IMPORT f32 AddPrevious(f32 arg1, f32 arg2, f32 arg3, f32 arg4);

    // ?CreateDummyBitmap@PUMenuBase@@QAEPAVgfxBitmap@@XZ
    ARTS_IMPORT gfxBitmap* CreateDummyBitmap();

    // ?CreateTitle@PUMenuBase@@QAEMH@Z
    ARTS_IMPORT f32 CreateTitle(i32 arg1);

    // ?DisableExit@PUMenuBase@@QAEXXZ
    ARTS_IMPORT void DisableExit();

    // ?EnableExit@PUMenuBase@@QAEXXZ
    ARTS_IMPORT void EnableExit();

public:
    // Members from 0x08C; everything below that belongs to UIMenu.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 field_8C[4]; // 0x08C
    gfxBitmap* field_90; // 0x090
    u8 field_94[4]; // 0x094
    u8 field_94[4]; // 0x098
    u8 field_98[4]; // 0x09C
    u8 field_A0[4]; // 0x0A0
    u8 field_A4[4]; // 0x0A4
    u8 field_A8[4]; // 0x0A8
    u8 field_AC[4]; // 0x0AC
    u8 field_B0[4]; // 0x0B0
    u8 field_B4[4]; // 0x0B4
    u8 field_B8[4]; // 0x0B8
};

// ??_7PUMenuBase@@6B@
// vtable at 0x005B4A8C
check_size(PUMenuBase, 0xBC);
