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

class audObject;

class audStream
{
public:
    // ?CloseStream@audStream@@QAE_NXZ
    ARTS_IMPORT bool CloseStream();

    // ?Create@audStream@@QAEPAV1@PAVaudObject@@@Z
    ARTS_IMPORT audStream* Create(audObject* arg1);

    // ?Destroy@audStream@@QAEXXZ
    ARTS_IMPORT void Destroy();

    // ?GetBufferSize@audStream@@QAEHXZ
    ARTS_IMPORT i32 GetBufferSize();

    // ?GetCurrentCursor@audStream@@QAEHXZ
    ARTS_IMPORT i32 GetCurrentCursor();

    // ?Init@audStream@@QAEXXZ
    ARTS_IMPORT void Init();

    // ?IsFinished@audStream@@QAE_NXZ
    ARTS_IMPORT bool IsFinished();

    // ?SetBufferSize@audStream@@QAEXH@Z
    ARTS_IMPORT void SetBufferSize(i32 arg1);

    // ?SetFileInfo@audStream@@QAEXPAX@Z
    ARTS_IMPORT void SetFileInfo(void* arg1);

    // ?SetLooping@audStream@@QAEX_N@Z
    ARTS_IMPORT void SetLooping(bool arg1);

    // ?SetVolume@audStream@@QAEXM@Z
    ARTS_IMPORT void SetVolume(f32 arg1);

    // ?Update@audStream@@QAE_NH@Z
    ARTS_IMPORT bool Update(i32 arg1);
};

// check_size(audStream, 0x0); // TODO: no layout in the IDB type library
