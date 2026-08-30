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

class audSoundBuffer;

class audSoundHeap
{
public:
    // ?Create@audSoundHeap@@QAEPAV1@PAV1@H@Z
    ARTS_IMPORT audSoundHeap* Create(audSoundHeap* arg1, i32 arg2);

    // ?CreateNodeList@audSoundHeap@@QAEPAV1@PAV1@HH@Z
    ARTS_IMPORT audSoundHeap* CreateNodeList(audSoundHeap* arg1, i32 arg2, i32 arg3);

    // ?Destroy@audSoundHeap@@QAEXXZ
    ARTS_IMPORT void Destroy();

    // ?GetBottom@audSoundHeap@@QAEPAXXZ
    ARTS_IMPORT void* GetBottom();

    // ?GetBotttomOfTopStack@audSoundHeap@@QAEPAXXZ
    ARTS_IMPORT void* GetBotttomOfTopStack();

    // ?GetSoundBufferList@audSoundHeap@@QAEPAVaudSoundBuffer@@XZ
    ARTS_IMPORT audSoundBuffer* GetSoundBufferList();

    // ?GetTop@audSoundHeap@@QAEPAXXZ
    ARTS_IMPORT void* GetTop();

    // ?GetTopOfBottomStack@audSoundHeap@@QAEPAXXZ
    ARTS_IMPORT void* GetTopOfBottomStack();

    // ?ReleaseBuffer@audSoundHeap@@QAE_NPAVaudSoundBuffer@@@Z
    ARTS_IMPORT bool ReleaseBuffer(audSoundBuffer* arg1);

    // ?ReserveBuffer@audSoundHeap@@QAEPAVaudSoundBuffer@@H@Z
    ARTS_IMPORT audSoundBuffer* ReserveBuffer(i32 arg1);

    // ?SetBottomOfTopStack@audSoundHeap@@QAE_NPAX@Z
    ARTS_IMPORT bool SetBottomOfTopStack(void* arg1);

    // ?SetTopOfBottomStack@audSoundHeap@@QAE_NPAX@Z
    ARTS_IMPORT bool SetTopOfBottomStack(void* arg1);
};

// check_size(audSoundHeap, 0x0); // TODO: no layout in the IDB type library
