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

#include "arts7/asnode.h"

class Aud3DObject;
class Aud3DSampleWrapper;
class AudSoundBase;
class Matrix34;

class Aud3DObjectManager : public asNode
{
public:
    // ??0Aud3DObjectManager@@QAE@H@Z
    ARTS_IMPORT Aud3DObjectManager(i32 arg1);

    // ??1Aud3DObjectManager@@UAE@XZ
    ARTS_IMPORT virtual ~Aud3DObjectManager();

    // ?Add@Aud3DObjectManager@@QAEHPAVAud3DObject@@@Z
    ARTS_IMPORT i32 Add(Aud3DObject* arg1);

    // ?AllocateSample@Aud3DObjectManager@@QAEHPADHK@Z
    ARTS_IMPORT i32 AllocateSample(char* arg1, i32 arg2, ulong arg3);

    // ?AllocateSample@Aud3DObjectManager@@QAEHPADK@Z
    ARTS_IMPORT i32 AllocateSample(char* renderer, ulong DrawList);

    // ?EchoOff@Aud3DObjectManager@@QAEXXZ
    ARTS_IMPORT void EchoOff();

    // ?EchoOn@Aud3DObjectManager@@QAEXM@Z
    ARTS_IMPORT void EchoOn(f32 arg1);

    // ?FindGreatestDistance@Aud3DObjectManager@@QAEHPAVAud3DObject@@@Z
    ARTS_IMPORT i32 FindGreatestDistance(Aud3DObject* arg1);

    // ?FindUnusedSlot@Aud3DObjectManager@@QAEHXZ
    ARTS_IMPORT i32 FindUnusedSlot();

    // ?GetClosestCop@Aud3DObjectManager@@QAEHXZ
    ARTS_IMPORT i32 GetClosestCop();

    // ?GetLeftHeadPtrPtr@Aud3DObjectManager@@QAEPAPAVMatrix34@@XZ
    ARTS_IMPORT Matrix34** GetLeftHeadPtrPtr();

    // ?GetRightHeadPtrPtr@Aud3DObjectManager@@QAEPAPAVMatrix34@@XZ
    ARTS_IMPORT Matrix34** GetRightHeadPtrPtr();

    // ?GetSample@Aud3DObjectManager@@QAEPAVAudSoundBase@@HH@Z
    ARTS_IMPORT AudSoundBase* GetSample(i32 arg1, i32 arg2);

    // ?GetSampleWrapper@Aud3DObjectManager@@QAEPAVAud3DSampleWrapper@@H@Z
    ARTS_IMPORT Aud3DSampleWrapper* GetSampleWrapper(i32 arg1);

    // ?InitAmbObjContainer@Aud3DObjectManager@@QAEXPAD@Z
    ARTS_IMPORT void InitAmbObjContainer(char* arg1);

    // ?PlayCopVoice@Aud3DObjectManager@@QAEHXZ
    ARTS_IMPORT i32 PlayCopVoice();

    // ?Process3D@Aud3DObjectManager@@QAEX_N@Z
    ARTS_IMPORT void Process3D(bool arg1);

    // ?QueueInCopVoice@Aud3DObjectManager@@QAEHM@Z
    ARTS_IMPORT i32 QueueInCopVoice(f32 arg1);

    // ?Remove@Aud3DObjectManager@@QAEXPAVAud3DObject@@H@Z
    ARTS_IMPORT void Remove(Aud3DObject* arg1, i32 arg2);

    // ?SetLeftHeadPtr@Aud3DObjectManager@@QAEXPAVMatrix34@@H@Z
    ARTS_IMPORT void SetLeftHeadPtr(Matrix34* arg1, i32 arg2);

    // ?SetNumScreenSplits@Aud3DObjectManager@@QAEXH@Z
    ARTS_IMPORT void SetNumScreenSplits(i32 arg1);

    // ?SetRightHeadPtr@Aud3DObjectManager@@QAEXPAVMatrix34@@H@Z
    ARTS_IMPORT void SetRightHeadPtr(Matrix34* arg1, i32 arg2);

    // ?Update@Aud3DObjectManager@@QAEXM@Z
    ARTS_IMPORT void Update(f32 arg1);

private:
    // ?CatName@Aud3DObjectManager@@AAEPADPADHH@Z
    ARTS_IMPORT char* CatName(char* arg1, i32 arg2, i32 arg3);
};

// ??_7Aud3DObjectManager@@6B@
// vtable at 0x005B4C58
// check_size(Aud3DObjectManager, 0x18); // size known, members are not - cannot verify
