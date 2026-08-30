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

class Aud3DObject;
class Stream;

class AudCreatureImpact
{
public:
    // ??0AudCreatureImpact@@QAE@PAV0@@Z
    ARTS_IMPORT AudCreatureImpact(AudCreatureImpact* arg1);

    // ??0AudCreatureImpact@@QAE@XZ
    ARTS_IMPORT AudCreatureImpact();

    // ??1AudCreatureImpact@@QAE@XZ
    ARTS_IMPORT ~AudCreatureImpact();

    // ?AssignSounds@AudCreatureImpact@@QAEXH@Z
    ARTS_IMPORT void AssignSounds(i32 arg1);

    // ?EchoOff@AudCreatureImpact@@QAEXXZ
    ARTS_IMPORT void EchoOff();

    // ?EchoOn@AudCreatureImpact@@QAEXM@Z
    ARTS_IMPORT void EchoOn(f32 arg1);

    // ?ParseCSVBuffer@AudCreatureImpact@@QAEHPAVStream@@@Z
    ARTS_IMPORT i32 ParseCSVBuffer(Stream* renderer);

    // ?QueuePlay@AudCreatureImpact@@QAEXM@Z
    ARTS_IMPORT void QueuePlay(f32 arg1);

    // ?SamplePlaying@AudCreatureImpact@@QAE_NXZ
    ARTS_IMPORT bool SamplePlaying();

    // ?SetAud3DObjectPtr@AudCreatureImpact@@QAEXPAVAud3DObject@@@Z
    ARTS_IMPORT void SetAud3DObjectPtr(Aud3DObject* arg1);

    // ?UnAssignSounds@AudCreatureImpact@@QAEXXZ
    ARTS_IMPORT void UnAssignSounds();

    // ?Update@AudCreatureImpact@@QAEXXZ
    ARTS_IMPORT void Update();

    // ?UpdateAttenuation@AudCreatureImpact@@QAEXMM@Z
    ARTS_IMPORT void UpdateAttenuation(f32 arg1, f32 arg2);

    // ?UpdateEcho@AudCreatureImpact@@QAEXXZ
    ARTS_IMPORT void UpdateEcho();

    // ?UpdateStatics@AudCreatureImpact@@SAXXZ
    ARTS_IMPORT static void UpdateStatics();

private:
    // ?Play@AudCreatureImpact@@AAEXXZ
    ARTS_IMPORT void Play();
};

// check_size(AudCreatureImpact, 0x0); // TODO: no layout in the IDB type library
