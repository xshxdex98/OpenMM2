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

struct HWND__;

class CDMan
{
public:
    // ??0CDMan@@QAE@XZ
    ARTS_IMPORT CDMan();

    // ??1CDMan@@UAE@XZ
    ARTS_IMPORT virtual ~CDMan();

    // ?GetNumTracks@CDMan@@QAEFXZ
    ARTS_IMPORT i16 GetNumTracks();

    // ?GetPosition@CDMan@@QAEKPAE000@Z
    ARTS_IMPORT ulong GetPosition(u8* arg1, u8* arg2, u8* arg3, u8* arg4);

    // ?Init@CDMan@@QAEKF@Z
    ARTS_IMPORT ulong Init(i16 renderer);

    // ?PlayTrack@CDMan@@QAEKEE@Z
    ARTS_IMPORT ulong PlayTrack(u8 renderer, u8 dwParam2);

    // ?PlayTrack@CDMan@@QAEKEEEEE@Z
    ARTS_IMPORT ulong PlayTrack(u8 arg1, u8 arg2, u8 arg3, u8 arg4, u8 dwParam2);

    // ?ResumePlay@CDMan@@QAEKXZ
    ARTS_IMPORT ulong ResumePlay();

    // ?SeekTrack@CDMan@@QAEKE@Z
    ARTS_IMPORT ulong SeekTrack(u8 arg1);

    // ?Stop@CDMan@@QAEKXZ
    ARTS_IMPORT ulong Stop();

private:
    // ?WindowProc@CDMan@@EAEJPAUHWND__@@IIJ@Z
    ARTS_IMPORT virtual ilong WindowProc(HWND__* arg1, u32 arg2, u32 arg3, ilong arg4);
};

// ??_7CDMan@@6B@
// vtable at 0x005B4E78
// check_size(CDMan, 0x0); // TODO: no layout in the IDB type library
