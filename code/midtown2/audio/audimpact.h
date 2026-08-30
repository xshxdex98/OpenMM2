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

class AudImpactData;
class Stream;

class AudImpact
{
public:
    // ??0AudImpact@@QAE@PAV0@@Z
    ARTS_IMPORT AudImpact(AudImpact* arg1);

    // ??0AudImpact@@QAE@XZ
    ARTS_IMPORT AudImpact();

    // ??1AudImpact@@UAE@XZ
    ARTS_IMPORT virtual ~AudImpact();

    // ?AddToHash@AudImpact@@SAXPADH@Z
    ARTS_IMPORT static void AddToHash(char* arg1, i32 entry);

    // ?AssignSounds@AudImpact@@QAEXH@Z
    ARTS_IMPORT void AssignSounds(i32 arg1);

    // ?GetAudImpactDataPtr@AudImpact@@QAEPAVAudImpactData@@H@Z
    ARTS_IMPORT AudImpactData* GetAudImpactDataPtr(i32 arg1);

    // ?GetSecondsElapsed@AudImpact@@SAMXZ
    ARTS_IMPORT static f32 GetSecondsElapsed();

    // ?Load@AudImpact@@SAHPAPAV1@PAD1PAH@Z
    ARTS_IMPORT static i32 Load(AudImpact** arg1, char* arg2, char* arg3, i32* arg4);

    // ?Play@AudImpact@@QAEXHM@Z
    ARTS_IMPORT void Play(i32 arg1, f32 arg2);

    // ?Play@AudImpact@@QAEXMH@Z
    ARTS_IMPORT void Play(f32 arg1, i32 arg2);

    // ?Set3D@AudImpact@@QAEX_NM@Z
    ARTS_IMPORT void Set3D(bool arg1, f32 arg2);

    // ?SetPan@AudImpact@@QAEXM@Z
    ARTS_IMPORT void SetPan(f32 arg1);

    // ?SetSecondsElapsed@AudImpact@@SAXM@Z
    ARTS_IMPORT static void SetSecondsElapsed(f32 arg1);

    // ?UnAssignSounds@AudImpact@@QAEXXZ
    ARTS_IMPORT void UnAssignSounds();

    // ?Update@AudImpact@@QAEXXZ
    ARTS_IMPORT void Update();

    // ?UpdateAttenuation@AudImpact@@QAEXMM@Z
    ARTS_IMPORT void UpdateAttenuation(f32 arg1, f32 arg2);

private:
    // ?ReadCSV@AudImpact@@AAEHPAVStream@@@Z
    ARTS_IMPORT i32 ReadCSV(Stream* arg1);
};

// check_size(AudImpact, 0x1); // size known, members are not - cannot verify
