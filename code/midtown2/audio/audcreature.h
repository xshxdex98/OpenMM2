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

class AudCreature
{
public:
    // ??0AudCreature@@QAE@PAV0@@Z
    ARTS_IMPORT AudCreature(AudCreature* arg1);

    // ??0AudCreature@@QAE@XZ
    ARTS_IMPORT AudCreature();

    // ??1AudCreature@@QAE@XZ
    ARTS_IMPORT ~AudCreature();

    // ?AddToHash@AudCreature@@SAXPADH@Z
    ARTS_IMPORT static void AddToHash(char* argName, i32 entry);

    // ?AssignSounds@AudCreature@@QAEXH@Z
    ARTS_IMPORT void AssignSounds(i32 arg1);

    // ?EchoOff@AudCreature@@QAEXXZ
    ARTS_IMPORT void EchoOff();

    // ?EchoOn@AudCreature@@QAEXM@Z
    ARTS_IMPORT void EchoOn(f32 arg1);

    // ?IsPlaying@AudCreature@@QAE_NXZ
    ARTS_IMPORT bool IsPlaying();

    // ?Load@AudCreature@@SAHPAPAV1@PAD1PAH@Z
    ARTS_IMPORT static i32 Load(AudCreature** valueOut, char* name, char* asssetSection, i32* arg4);

    // ?PlayAvoidance@AudCreature@@QAEXM@Z
    ARTS_IMPORT void PlayAvoidance(f32 arg1);

    // ?PlayImpact@AudCreature@@QAEXM@Z
    ARTS_IMPORT void PlayImpact(f32 arg1);

    // ?SetAud3DObjectPtr@AudCreature@@QAEXPAVAud3DObject@@@Z
    ARTS_IMPORT void SetAud3DObjectPtr(Aud3DObject* arg1);

    // ?UnAssignSounds@AudCreature@@QAEXXZ
    ARTS_IMPORT void UnAssignSounds();

    // ?Update@AudCreature@@QAEXM@Z
    ARTS_IMPORT void Update(f32 arg1);

    // ?UpdateAttenuation@AudCreature@@QAEXMMM@Z
    ARTS_IMPORT void UpdateAttenuation(f32 arg1, f32 arg2, f32 arg3);

    // ?UpdateEcho@AudCreature@@QAEXXZ
    ARTS_IMPORT void UpdateEcho();

private:
    // ?ReadCSV@AudCreature@@AAEHPAVStream@@@Z
    ARTS_IMPORT i32 ReadCSV(Stream* renderer);
};

// check_size(AudCreature, 0x0); // TODO: no layout in the IDB type library
