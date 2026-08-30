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

struct IDirectSoundBuffer;

class EchoEffect
{
public:
    // ??0EchoEffect@@QAE@XZ
    ARTS_IMPORT EchoEffect();

    // ??1EchoEffect@@QAE@XZ
    ARTS_IMPORT ~EchoEffect();

    // ?CalculatePan@EchoEffect@@QAEXM@Z
    ARTS_IMPORT void CalculatePan(f32 arg1);

    // ?Disable@EchoEffect@@QAEXXZ
    ARTS_IMPORT void Disable();

    // ?Enable@EchoEffect@@QAEHKPAUIDirectSoundBuffer@@@Z
    ARTS_IMPORT i32 Enable(ulong arg1, IDirectSoundBuffer* arg2);

    // ?QueueFrequency@EchoEffect@@QAEXK@Z
    ARTS_IMPORT void QueueFrequency(ulong arg1);

    // ?QueuePlay@EchoEffect@@QAEXK@Z
    ARTS_IMPORT void QueuePlay(ulong arg1);

    // ?QueueStop@EchoEffect@@QAEXXZ
    ARTS_IMPORT void QueueStop();

    // ?QueueVolume@EchoEffect@@QAEXM@Z
    ARTS_IMPORT void QueueVolume(f32 arg1);

    // ?SetDelayTime@EchoEffect@@QAEXM@Z
    ARTS_IMPORT void SetDelayTime(f32 player);

    // ?SetFrequency@EchoEffect@@QAEXM@Z
    ARTS_IMPORT void SetFrequency(f32 arg1);

    // ?SetVolume@EchoEffect@@QAEXM@Z
    ARTS_IMPORT void SetVolume(f32 arg1);

    // ?Stop@EchoEffect@@QAEXXZ
    ARTS_IMPORT void Stop();

    // ?Update@EchoEffect@@QAEXXZ
    ARTS_IMPORT void Update();

private:
    // ?UpdatePitch@EchoEffect@@AAEXXZ
    ARTS_IMPORT void UpdatePitch();

    // ?UpdatePlay@EchoEffect@@AAEXXZ
    ARTS_IMPORT void UpdatePlay();

    // ?UpdateStop@EchoEffect@@AAEXXZ
    ARTS_IMPORT void UpdateStop();

    // ?UpdateVolume@EchoEffect@@AAEXXZ
    ARTS_IMPORT void UpdateVolume();
};

// check_size(EchoEffect, 0x0); // TODO: no layout in the IDB type library
