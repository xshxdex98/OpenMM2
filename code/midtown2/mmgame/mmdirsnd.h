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

#include "audio/dirsnd.h"

class mmDirSnd : public DirSnd
{
public:
    // ??0mmDirSnd@@QAE@XZ
    ARTS_IMPORT mmDirSnd();

    // ??1mmDirSnd@@QAE@XZ
    ARTS_IMPORT ~mmDirSnd();

    // ?DeInit@mmDirSnd@@QAEXFF@Z
    ARTS_IMPORT void DeInit(i16 arg1, i16 arg2);

    // ?DSound3DEnabled@mmDirSnd@@QAEIXZ
    ARTS_IMPORT u32 DSound3DEnabled();

    // ?EAXEnabled@mmDirSnd@@QAEIXZ
    ARTS_IMPORT u32 EAXEnabled();

    // ?Init@mmDirSnd@@SAPAV1@KEHHPADFF@Z
    ARTS_IMPORT static mmDirSnd* Init(ulong arg1, u8 arg2, i32 arg3, i32 arg4, char* arg5, i16 arg6, i16 arg7);

    // ?InitPrimarySoundBuffer@mmDirSnd@@UAEHKEPAD@Z
    ARTS_IMPORT virtual i32 InitPrimarySoundBuffer(ulong arg1, u8 arg2, char* arg3);

public:
    // Members from 0x03C; everything below that belongs to DirSnd.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i32 EAXEnabled; // 0x03C
    i32 DSound3DEnabled; // 0x040
};

// ??_7mmDirSnd@@6B@
// vtable at 0x005B4E74
check_size(mmDirSnd, 0x44);
