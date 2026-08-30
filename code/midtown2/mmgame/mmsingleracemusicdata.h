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

class Stream;

class mmSingleRaceMusicData
{
public:
    // ?LoadMusic@mmSingleRaceMusicData@@UAE_NPAD@Z
    ARTS_IMPORT virtual bool LoadMusic(char* host_window);

private:
    // ?LoadMusicSegments@mmSingleRaceMusicData@@AAE_NPAVStream@@F@Z
    ARTS_IMPORT bool LoadMusicSegments(Stream* stream, i16 arg2);
};

// ??_7mmSingleRaceMusicData@@6B@
// vtable at 0x005B0610
// check_size(mmSingleRaceMusicData, 0x0); // TODO: no layout in the IDB type library
