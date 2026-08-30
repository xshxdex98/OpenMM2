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

#include "audio/aud3dobject.h"

class Vector3;

class aiCableCarAudio : public Aud3DObject
{
public:
    // ??0aiCableCarAudio@@QAE@XZ
    ARTS_IMPORT aiCableCarAudio();

    // ??1aiCableCarAudio@@UAE@XZ
    ARTS_IMPORT virtual ~aiCableCarAudio();

    // ?AssignSounds@aiCableCarAudio@@UAEXXZ
    ARTS_IMPORT virtual void AssignSounds();

    // ?UnAssignSounds@aiCableCarAudio@@UAEXH@Z
    ARTS_IMPORT virtual void UnAssignSounds(i32 arg1);

    // ?UpdateAudio@aiCableCarAudio@@UAEXXZ
    ARTS_IMPORT virtual void UpdateAudio();

    // ?Init@aiCableCarAudio@@QAEXPAVVector3@@PAM@Z
    ARTS_IMPORT void Init(Vector3* arg1, f32* arg2);

    // ?Reset@aiCableCarAudio@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?UpdateAudio@aiCableCarAudio@@QAEHM@Z
    ARTS_IMPORT i32 UpdateAudio(f32 arg1);
};

// ??_7aiCableCarAudio@@6B@
// vtable at 0x005B8754
// check_size(aiCableCarAudio, 0x60); // size known, members are not - cannot verify
