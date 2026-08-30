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

struct LocString;
class gfxBitmap;

class mmText
{
public:
    // ??0mmText@@QAE@XZ
    ARTS_IMPORT mmText();

    // ??1mmText@@QAE@XZ
    ARTS_IMPORT ~mmText();

    // ?CreateFitBitmap@mmText@@SAPAVgfxBitmap@@PBDPBXHH@Z
    ARTS_IMPORT static gfxBitmap* CreateFitBitmap(const char* arg1, const void* arg2, i32 arg3, i32 arg4);

    // ?CreateFont@mmText@@SAPAXPBDH@Z
    ARTS_IMPORT static void* CreateFont(const char* arg1, i32 arg2);

    // ?CreateLocFont@mmText@@SAPAXPBULocString@@H@Z
    ARTS_EXPORT static void* CreateLocFont(const LocString* arg1, i32 arg2);

    // ?DeleteFont@mmText@@SAXPAX@Z
    ARTS_IMPORT static void DeleteFont(void* arg1);

    // ?Draw@mmText@@QAEXPAVgfxBitmap@@MMPBDPBX@Z
    ARTS_IMPORT void Draw(gfxBitmap* arg1, f32 arg2, f32 arg3, const char* arg4, const void* arg5);

    // ?Draw2@mmText@@QAEXPAVgfxBitmap@@MMPBDPBXI@Z
    ARTS_IMPORT void Draw2(gfxBitmap* arg1, f32 arg2, f32 arg3, const char* arg4, const void* arg5, u32 arg6);

    // ?GetDC@mmText@@SAPAXPAVgfxBitmap@@@Z
    ARTS_IMPORT static void* GetDC(gfxBitmap* arg1);

    // ?ReleaseDC@mmText@@SAXXZ
    ARTS_IMPORT static void ReleaseDC();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 field_0; // 0x000
    u8 field_1; // 0x001
};

check_size(mmText, 0x2);
