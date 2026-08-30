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

#include "arts7/asnode.h"

class AudManagerBase;

class AudStreamingMusic : public asNode
{
public:
    // ??0AudStreamingMusic@@QAE@AAVAudManagerBase@@@Z
    ARTS_IMPORT AudStreamingMusic(AudManagerBase& arg1);

    // ??1AudStreamingMusic@@UAE@XZ
    ARTS_IMPORT virtual ~AudStreamingMusic();

    // ?StreamingMusicInit@AudStreamingMusic@@QAEHPBD@Z
    ARTS_IMPORT i32 StreamingMusicInit(const char* renderer);

    // ?StreamingMusicPlay@AudStreamingMusic@@QAEHHH_N@Z
    ARTS_IMPORT i32 StreamingMusicPlay(i32 arg1, i32 arg2, bool arg3);

    // ?StreamingMusicReleaseControl@AudStreamingMusic@@QAEXXZ
    ARTS_IMPORT void StreamingMusicReleaseControl();

    // ?StreamingMusicSetVolume@AudStreamingMusic@@QAEXM@Z
    ARTS_IMPORT void StreamingMusicSetVolume(f32 arg1);

    // ?StreamingMusicStop@AudStreamingMusic@@QAEXXZ
    ARTS_IMPORT void StreamingMusicStop();

    // ?StreamingMusicUpdate@AudStreamingMusic@@QAEXXZ
    ARTS_IMPORT void StreamingMusicUpdate();
};

// ??_7AudStreamingMusic@@6B@
// vtable at 0x005B4BE4
// check_size(AudStreamingMusic, 0x0); // TODO: no layout in the IDB type library
