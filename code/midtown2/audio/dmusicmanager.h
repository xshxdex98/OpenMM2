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

class DMusicObject;
struct IDirectSound;

class DMusicManager
{
public:
    // ??0DMusicManager@@QAE@XZ
    ARTS_IMPORT DMusicManager();

    // ??1DMusicManager@@QAE@XZ
    ARTS_IMPORT ~DMusicManager();

    // ?Activate@DMusicManager@@QAEXH@Z
    ARTS_IMPORT void Activate(i32 arg1);

    // ?GetDMusicObjectPtr@DMusicManager@@QAEPAVDMusicObject@@XZ
    ARTS_IMPORT DMusicObject* GetDMusicObjectPtr();

    // ?GetVolume@DMusicManager@@QAEMXZ
    ARTS_IMPORT f32 GetVolume();

    // ?Init@DMusicManager@@QAEHPAUIDirectSound@@HKHKK@Z
    ARTS_IMPORT i32 Init(IDirectSound* arg1, i32 arg2, ulong arg3, i32 arg4, ulong arg5, ulong arg6);

    // ?SetPan@DMusicManager@@QAEXM@Z
    ARTS_IMPORT void SetPan(f32 arg1);

    // ?SetVolume@DMusicManager@@QAEXM@Z
    ARTS_IMPORT void SetVolume(f32 arg1);

    // ?Update@DMusicManager@@QAEXXZ
    ARTS_IMPORT void Update();
};

// check_size(DMusicManager, 0x1); // size known, members are not - cannot verify
