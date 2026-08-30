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

struct adpcm_state;

class audSoundBuffer
{
public:
    enum SOUNDBUFFERTYPE : i32;

    // ?BytesAdded@audSoundBuffer@@QAE_NH@Z
    ARTS_IMPORT bool BytesAdded(i32 arg1);

    // ?BytesRemoved@audSoundBuffer@@QAE_NH@Z
    ARTS_IMPORT bool BytesRemoved(i32 arg1);

    // ?Create@audSoundBuffer@@QAEPAV1@W4SOUNDBUFFERTYPE@1@H@Z
    ARTS_IMPORT audSoundBuffer* Create(audSoundBuffer::SOUNDBUFFERTYPE arg1, i32 arg2);

    // ?Destroy@audSoundBuffer@@QAEXXZ
    ARTS_IMPORT void Destroy();

    // ?GetAdpcmState@audSoundBuffer@@QAEPAUadpcm_state@@XZ
    ARTS_IMPORT adpcm_state* GetAdpcmState();

    // ?GetDataPtr@audSoundBuffer@@QAEPAXXZ
    ARTS_IMPORT void* GetDataPtr();

    // ?GetExtra@audSoundBuffer@@QAEHXZ
    ARTS_IMPORT i32 GetExtra();

    // ?GetIndex1@audSoundBuffer@@QAEHXZ
    ARTS_IMPORT i32 GetIndex1();

    // ?GetIndex2@audSoundBuffer@@QAEHXZ
    ARTS_IMPORT i32 GetIndex2();

    // ?GetSize@audSoundBuffer@@QAEHXZ
    ARTS_IMPORT i32 GetSize();

    // ?GetType@audSoundBuffer@@QAE?AW4SOUNDBUFFERTYPE@1@XZ
    ARTS_IMPORT audSoundBuffer::SOUNDBUFFERTYPE GetType();

    // ?Init@audSoundBuffer@@QAEXW4SOUNDBUFFERTYPE@1@HPAX@Z
    ARTS_IMPORT void Init(audSoundBuffer::SOUNDBUFFERTYPE arg1, i32 arg2, void* arg3);

    // ?IsEmpty@audSoundBuffer@@QAE_NXZ
    ARTS_IMPORT bool IsEmpty();

    // ?IsFull@audSoundBuffer@@QAE_NXZ
    ARTS_IMPORT bool IsFull();

    // ?SetAdpcmState@audSoundBuffer@@QAEXPAUadpcm_state@@@Z
    ARTS_IMPORT void SetAdpcmState(adpcm_state* arg1);

    // ?SetDataPtr@audSoundBuffer@@QAEXPAX@Z
    ARTS_IMPORT void SetDataPtr(void* arg1);

    // ?SetExtra@audSoundBuffer@@QAEXH@Z
    ARTS_IMPORT void SetExtra(i32 arg1);

    // ?SetIndex1@audSoundBuffer@@QAEXH@Z
    ARTS_IMPORT void SetIndex1(i32 arg1);

    // ?SetIndex2@audSoundBuffer@@QAEXH@Z
    ARTS_IMPORT void SetIndex2(i32 arg1);

    // ?SetIndexes@audSoundBuffer@@QAEXHH@Z
    ARTS_IMPORT void SetIndexes(i32 arg1, i32 arg2);

    // ?SetSize@audSoundBuffer@@QAEXH@Z
    ARTS_IMPORT void SetSize(i32 arg1);

    // ?SetType@audSoundBuffer@@QAEXW4SOUNDBUFFERTYPE@1@@Z
    ARTS_IMPORT void SetType(audSoundBuffer::SOUNDBUFFERTYPE arg1);

    // ?Unused@audSoundBuffer@@QAEHXZ
    ARTS_IMPORT i32 Unused();

    // ?Used@audSoundBuffer@@QAEHXZ
    ARTS_IMPORT i32 Used();
};

// check_size(audSoundBuffer, 0x0); // TODO: no layout in the IDB type library
