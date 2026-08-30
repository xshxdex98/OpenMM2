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

class AudSoundBase;

class Aud3DSampleWrapper
{
public:
    // ??0Aud3DSampleWrapper@@QAE@HHK@Z
    ARTS_IMPORT Aud3DSampleWrapper(i32 arg1, i32 arg2, ulong arg3);

    // ??0Aud3DSampleWrapper@@QAE@PADHK@Z
    ARTS_IMPORT Aud3DSampleWrapper(char* arg1, i32 arg2, ulong arg3);

    // ??1Aud3DSampleWrapper@@QAE@XZ
    ARTS_IMPORT ~Aud3DSampleWrapper();

    // ?GetSoundPtr@Aud3DSampleWrapper@@QAEPAVAudSoundBase@@F@Z
    ARTS_IMPORT AudSoundBase* GetSoundPtr(i16 arg1);

    // ?Load@Aud3DSampleWrapper@@QAEXPADH@Z
    ARTS_IMPORT void Load(char* arg1, i32 arg2);

    // ?SetSampleName@Aud3DSampleWrapper@@QAEXPAD@Z
    ARTS_IMPORT void SetSampleName(char* arg1);

    // ?Update@Aud3DSampleWrapper@@QAEXXZ
    ARTS_IMPORT void Update();
};

// check_size(Aud3DSampleWrapper, 0x0); // TODO: no layout in the IDB type library
