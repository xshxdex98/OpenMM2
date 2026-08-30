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

class Stream;
class Vector3;
struct tagAud3DAmbientSoundData;

class Aud3DAmbientObject : public Aud3DObject
{
public:
    // ??0Aud3DAmbientObject@@QAE@XZ
    ARTS_IMPORT Aud3DAmbientObject();

    // ??1Aud3DAmbientObject@@UAE@XZ
    ARTS_IMPORT virtual ~Aud3DAmbientObject();

    // ?AssignSounds@Aud3DAmbientObject@@UAEXXZ
    ARTS_IMPORT virtual void AssignSounds();

    // ?UnAssignSounds@Aud3DAmbientObject@@UAEXH@Z
    ARTS_IMPORT virtual void UnAssignSounds(i32 arg1);

    // ?UpdateAudio@Aud3DAmbientObject@@UAEXXZ
    ARTS_IMPORT virtual void UpdateAudio();

    // ?ActivateSound@Aud3DAmbientObject@@QAEXH@Z
    ARTS_IMPORT void ActivateSound(i32 arg1);

    // ?DeactivateSound@Aud3DAmbientObject@@QAEXH@Z
    ARTS_IMPORT void DeactivateSound(i32 arg1);

    // ?GetSoundDataPtr@Aud3DAmbientObject@@QAEPAUtagAud3DAmbientSoundData@@H@Z
    ARTS_IMPORT tagAud3DAmbientSoundData* GetSoundDataPtr(i32 arg1);

    // ?GetSoundIndex@Aud3DAmbientObject@@QAEHPAD@Z
    ARTS_IMPORT i32 GetSoundIndex(char* renderer);

    // ?Init@Aud3DAmbientObject@@QAEXPAVVector3@@PAD@Z
    ARTS_IMPORT void Init(Vector3* arg1, char* arg2);

    // ?Load@Aud3DAmbientObject@@QAEXPAD@Z
    ARTS_IMPORT void Load(char* renderer);

    // ?PlayOneShot@Aud3DAmbientObject@@QAEXH@Z
    ARTS_IMPORT void PlayOneShot(i32 arg1);

    // ?Reset@Aud3DAmbientObject@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?Update@Aud3DAmbientObject@@QAEXM@Z
    ARTS_IMPORT void Update(f32 arg1);

    // ?UpdateAudio@Aud3DAmbientObject@@QAEHM@Z
    ARTS_IMPORT i32 UpdateAudio(f32 arg1);

    // ?UpdateDoppler@Aud3DAmbientObject@@QAEXPAUtagAud3DAmbientSoundData@@@Z
    ARTS_IMPORT void UpdateDoppler(tagAud3DAmbientSoundData* arg1);

private:
    // ?AssignSounds@Aud3DAmbientObject@@AAEXPAPAUtagAud3DAmbientSoundData@@H@Z
    ARTS_IMPORT void AssignSounds(tagAud3DAmbientSoundData** arg1, i32 arg2);

    // ?EchoOff@Aud3DAmbientObject@@AAEXPAPAUtagAud3DAmbientSoundData@@H@Z
    ARTS_IMPORT void EchoOff(tagAud3DAmbientSoundData** arg1, i32 arg2);

    // ?EchoOff@Aud3DAmbientObject@@AAEXXZ
    ARTS_IMPORT void EchoOff();

    // ?EchoOn@Aud3DAmbientObject@@AAEXPAPAUtagAud3DAmbientSoundData@@H@Z
    ARTS_IMPORT void EchoOn(tagAud3DAmbientSoundData** arg1, i32 arg2);

    // ?EchoOn@Aud3DAmbientObject@@AAEXXZ
    ARTS_IMPORT void EchoOn();

    // ?PendOneShot@Aud3DAmbientObject@@AAEXPAUtagAud3DAmbientSoundData@@@Z
    ARTS_IMPORT void PendOneShot(tagAud3DAmbientSoundData* arg1);

    // ?PlayOneShot@Aud3DAmbientObject@@AAEXPAUtagAud3DAmbientSoundData@@@Z
    ARTS_IMPORT void PlayOneShot(tagAud3DAmbientSoundData* renderer);

    // ?ReadSoundData@Aud3DAmbientObject@@AAE_NPAVStream@@@Z
    ARTS_IMPORT bool ReadSoundData(Stream* renderer);

    // ?SetSoundData@Aud3DAmbientObject@@AAEXPAUtagAud3DAmbientSoundData@@0PAD@Z
    ARTS_IMPORT void SetSoundData(tagAud3DAmbientSoundData* arg1, tagAud3DAmbientSoundData* arg2, char* arg3);

    // ?UnAssignSounds@Aud3DAmbientObject@@AAEXPAPAUtagAud3DAmbientSoundData@@H@Z
    ARTS_IMPORT void UnAssignSounds(tagAud3DAmbientSoundData** arg1, i32 arg2);

    // ?UpdateEcho@Aud3DAmbientObject@@AAEXPAPAUtagAud3DAmbientSoundData@@H@Z
    ARTS_IMPORT void UpdateEcho(tagAud3DAmbientSoundData** arg1, i32 arg2);

    // ?UpdateEcho@Aud3DAmbientObject@@AAEXXZ
    ARTS_IMPORT void UpdateEcho();

    // ?UpdateLoop@Aud3DAmbientObject@@AAEXPAUtagAud3DAmbientSoundData@@@Z
    ARTS_IMPORT void UpdateLoop(tagAud3DAmbientSoundData* arg1);

    // ?UpdateOneShot@Aud3DAmbientObject@@AAEXPAUtagAud3DAmbientSoundData@@@Z
    ARTS_IMPORT void UpdateOneShot(tagAud3DAmbientSoundData* arg1);

    // ?UpdateSoundData@Aud3DAmbientObject@@AAEXXZ
    ARTS_IMPORT void UpdateSoundData();
};

// ??_7Aud3DAmbientObject@@6B@
// vtable at 0x005B4D18
// check_size(Aud3DAmbientObject, 0x0); // TODO: no layout in the IDB type library
