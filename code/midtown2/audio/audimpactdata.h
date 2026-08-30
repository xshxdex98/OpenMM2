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

class Stream;
struct tagImpactSampleWrapper;

class AudImpactData
{
public:
    // ??0AudImpactData@@QAE@PAV0@@Z
    ARTS_IMPORT AudImpactData(AudImpactData* arg1);

    // ??0AudImpactData@@QAE@XZ
    ARTS_IMPORT AudImpactData();

    // ??1AudImpactData@@UAE@XZ
    ARTS_IMPORT virtual ~AudImpactData();

    // ?AssignSounds@AudImpactData@@QAEXH@Z
    ARTS_IMPORT void AssignSounds(i32 arg1);

    // ?Play@AudImpactData@@QAEXM@Z
    ARTS_IMPORT void Play(f32 arg1);

    // ?Play@AudImpactData@@QAEXMMM@Z
    ARTS_IMPORT void Play(f32 arg1, f32 arg2, f32 arg3);

    // ?ReadCSV@AudImpactData@@QAE_NPAVStream@@H@Z
    ARTS_IMPORT bool ReadCSV(Stream* arg1, i32 arg2);

    // ?SetPan@AudImpactData@@QAEXM@Z
    ARTS_IMPORT void SetPan(f32 arg1);

    // ?UnAssignSounds@AudImpactData@@QAEXXZ
    ARTS_IMPORT void UnAssignSounds();

    // ?UpdateAttenuation@AudImpactData@@QAEXMM@Z
    ARTS_IMPORT void UpdateAttenuation(f32 arg1, f32 arg2);

private:
    // ?PlaySample@AudImpactData@@AAEXPAUtagImpactSampleWrapper@@M@Z
    ARTS_IMPORT void PlaySample(tagImpactSampleWrapper* arg1, f32 arg2);

    // ?PlaySample@AudImpactData@@AAEXPAUtagImpactSampleWrapper@@MMM@Z
    ARTS_IMPORT void PlaySample(tagImpactSampleWrapper* arg1, f32 arg2, f32 arg3, f32 arg4);

    // ?UpdateAttenuation@AudImpactData@@AAEXPAUtagImpactSampleWrapper@@MM@Z
    ARTS_IMPORT void UpdateAttenuation(tagImpactSampleWrapper* arg1, f32 arg2, f32 arg3);
};

// check_size(AudImpactData, 0x0); // TODO: no layout in the IDB type library
