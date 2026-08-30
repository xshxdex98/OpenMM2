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

class AudCreatureAvoid
{
public:
    // ??0AudCreatureAvoid@@QAE@PAV0@@Z
    ARTS_IMPORT AudCreatureAvoid(AudCreatureAvoid* arg1);

    // ??0AudCreatureAvoid@@QAE@XZ
    ARTS_IMPORT AudCreatureAvoid();

    // ??1AudCreatureAvoid@@QAE@XZ
    ARTS_IMPORT ~AudCreatureAvoid();

    // ?AssignSounds@AudCreatureAvoid@@QAEXH@Z
    ARTS_IMPORT void AssignSounds(i32 arg1);

    // ?EchoOff@AudCreatureAvoid@@QAEXXZ
    ARTS_IMPORT void EchoOff();

    // ?EchoOn@AudCreatureAvoid@@QAEXM@Z
    ARTS_IMPORT void EchoOn(f32 arg1);

    // ?IsEligible@AudCreatureAvoid@@QAE_NXZ
    ARTS_IMPORT bool IsEligible();

    // ?ParseCSVBuffer@AudCreatureAvoid@@QAEHPAVStream@@@Z
    ARTS_IMPORT i32 ParseCSVBuffer(Stream* arg1);

    // ?QueuePlay@AudCreatureAvoid@@QAEXXZ
    ARTS_IMPORT void QueuePlay();

    // ?SamplePlaying@AudCreatureAvoid@@QAE_NXZ
    ARTS_IMPORT bool SamplePlaying();

    // ?SetAud3DObjectPtr@AudCreatureAvoid@@QAEXPAVAud3DObject@@@Z
    ARTS_IMPORT void SetAud3DObjectPtr(Aud3DObject* arg1);

    // ?UnAssignSounds@AudCreatureAvoid@@QAEXXZ
    ARTS_IMPORT void UnAssignSounds();

    // ?Update@AudCreatureAvoid@@QAEXM@Z
    ARTS_IMPORT void Update(f32 arg1);

    // ?UpdateAttenuation@AudCreatureAvoid@@QAEXMM@Z
    ARTS_IMPORT void UpdateAttenuation(f32 arg1, f32 arg2);

    // ?UpdateEcho@AudCreatureAvoid@@QAEXXZ
    ARTS_IMPORT void UpdateEcho();

private:
    // ?InSpeedRange@AudCreatureAvoid@@AAE_NM@Z
    ARTS_IMPORT bool InSpeedRange(f32 arg1);

    // ?Play@AudCreatureAvoid@@AAEXXZ
    ARTS_IMPORT void Play();
};

// check_size(AudCreatureAvoid, 0x0); // TODO: no layout in the IDB type library
