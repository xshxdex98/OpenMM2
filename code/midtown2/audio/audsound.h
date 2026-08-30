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

#include "audobject.h"
#include "audsoundbuffer.h"

class audSound
{
public:
    // ?CloseSoundFile@audSound@@QAE_NXZ
    ARTS_IMPORT bool CloseSoundFile();

    // ?CreateDuplicateSoundBuffer@audSound@@QAEPAVaudSoundBuffer@@PAV1@@Z
    ARTS_IMPORT audSoundBuffer* CreateDuplicateSoundBuffer(audSound* arg1);

    // ?CreateSoundBufferFromFile@audSound@@QAEPAVaudSoundBuffer@@PAD0W4SOUNDBUFFERTYPE@2@HK@Z
    ARTS_IMPORT audSoundBuffer* CreateSoundBufferFromFile(char* arg1, char* arg2, audSoundBuffer::SOUNDBUFFERTYPE arg3, i32 arg4, ulong arg5);

    // ?CreateSoundBufferFromMemory@audSound@@QAEPAVaudSoundBuffer@@PAXW4SOUNDBUFFERTYPE@2@PAPAX@Z
    ARTS_IMPORT audSoundBuffer* CreateSoundBufferFromMemory(void* arg1, audSoundBuffer::SOUNDBUFFERTYPE arg2, void** arg3);

    // ?Destroy@audSound@@QAEXXZ
    ARTS_IMPORT void Destroy();

    // ?GetStatus@audSound@@QAE?AW4AUD_OBJECTSTATUS@audObject@@XZ
    ARTS_IMPORT audObject::AUD_OBJECTSTATUS GetStatus();

    // ?Init@audSound@@QAEXXZ
    ARTS_IMPORT void Init();

    // ?OpenSoundFile@audSound@@QAEHH@Z
    ARTS_IMPORT i32 OpenSoundFile(i32 arg1);

    // ?PauseResume@audSound@@QAE_N_N@Z
    ARTS_IMPORT bool PauseResume(bool arg1);

    // ?Play@audSound@@QAE_NXZ
    ARTS_IMPORT bool Play();

    // ?SetPan@audSound@@QAE_NM@Z
    ARTS_IMPORT bool SetPan(f32 arg1);

    // ?SetPitch@audSound@@QAE_NM@Z
    ARTS_IMPORT bool SetPitch(f32 arg1);

    // ?SetVolume@audSound@@QAE_NM@Z
    ARTS_IMPORT bool SetVolume(f32 arg1);

    // ?Stop@audSound@@QAE_NXZ
    ARTS_IMPORT bool Stop();

    // ?Update@audSound@@QAE_NH@Z
    ARTS_IMPORT bool Update(i32 arg1);

    // ?WriteWaveDataToBuffer@audSound@@QAEHHPAH@Z
    ARTS_IMPORT i32 WriteWaveDataToBuffer(i32 arg1, i32* arg2);
};

// check_size(audSound, 0x0); // TODO: no layout in the IDB type library
