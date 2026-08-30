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

#include "aud3dobject.h"

class Vector3;

class AudCreatureContainer : public Aud3DObject
{
public:
    // ??0AudCreatureContainer@@QAE@XZ
    ARTS_IMPORT AudCreatureContainer();

    // ??1AudCreatureContainer@@UAE@XZ
    ARTS_IMPORT virtual ~AudCreatureContainer();

    // ?AssignSounds@AudCreatureContainer@@UAEXXZ
    ARTS_IMPORT virtual void AssignSounds();

    // ?UnAssignSounds@AudCreatureContainer@@UAEXH@Z
    ARTS_IMPORT virtual void UnAssignSounds(i32 arg1);

    // ?UpdateAudio@AudCreatureContainer@@UAEXXZ
    ARTS_IMPORT virtual void UpdateAudio();

    // ?Update@AudCreatureContainer@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?DeallocateStatics@AudCreatureContainer@@SAXXZ
    ARTS_IMPORT static void DeallocateStatics();

    // ?EchoOff@AudCreatureContainer@@QAEXXZ
    ARTS_IMPORT void EchoOff();

    // ?EchoOn@AudCreatureContainer@@QAEXXZ
    ARTS_IMPORT void EchoOn();

    // ?Init@AudCreatureContainer@@QAEXPADPAVVector3@@@Z
    ARTS_IMPORT void Init(char* renderer, Vector3* DrawList);

    // ?InitStatics@AudCreatureContainer@@SAXXZ
    ARTS_IMPORT static void InitStatics();

    // ?IsPlaying@AudCreatureContainer@@QAE_NXZ
    ARTS_IMPORT bool IsPlaying();

    // ?LoadNumFileChoices@AudCreatureContainer@@SAXPADPAH@Z
    ARTS_IMPORT static void LoadNumFileChoices(char* path, i32* arg2);

    // ?LoadVoices@AudCreatureContainer@@QAE_NPAD0@Z
    ARTS_IMPORT bool LoadVoices(char* renderer, char* DrawList);

    // ?PlayAvoidanceReaction@AudCreatureContainer@@QAEXM@Z
    ARTS_IMPORT void PlayAvoidanceReaction(f32 arg1);

    // ?PlayImpactReaction@AudCreatureContainer@@QAEXM@Z
    ARTS_IMPORT void PlayImpactReaction(f32 arg1);

    // ?Reset@AudCreatureContainer@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?UpdateAudio@AudCreatureContainer@@QAEHM@Z
    ARTS_IMPORT i32 UpdateAudio(f32 arg1);

    // ?UpdateStatics@AudCreatureContainer@@SAXM@Z
    ARTS_IMPORT static void UpdateStatics(f32 renderer);

private:
    // ?UpdateEcho@AudCreatureContainer@@AAEXXZ
    ARTS_IMPORT void UpdateEcho();

    // ?UpdateVoices@AudCreatureContainer@@CAXHM@Z
    ARTS_IMPORT static void UpdateVoices(i32 arg1, f32 arg2);
};

// ??_7AudCreatureContainer@@6B@
// vtable at 0x005B4C9C
// check_size(AudCreatureContainer, 0x60); // size known, members are not - cannot verify
