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
#include "data7/datcallback.h"

class Matrix34;
class camCarCS;
class datParser;
class mmGame;
class phInertialCS;

class mmPlayer : public asNode
{
public:
    // ??0mmPlayer@@QAE@XZ
    ARTS_IMPORT mmPlayer();

    // ??1mmPlayer@@UAE@XZ
    ARTS_IMPORT virtual ~mmPlayer();

    // ?Update@mmPlayer@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@mmPlayer@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?FileIO@mmPlayer@@UAEXAAVdatParser@@@Z
    ARTS_IMPORT virtual void FileIO(datParser& arg1);

    // ?AfterLoad@mmPlayer@@UAEXXZ
    ARTS_IMPORT virtual void AfterLoad();

    // ?BeforeSave@mmPlayer@@UAEXXZ
    ARTS_IMPORT virtual void BeforeSave();

    // ?EnableRegen@mmPlayer@@QAEXH@Z
    ARTS_IMPORT void EnableRegen(i32 arg1);

    // ?FFImpactCallback@mmPlayer@@QAEXM@Z
    ARTS_IMPORT void FFImpactCallback(f32 arg1);

    // ?FilterSteering@mmPlayer@@QAEMM@Z
    ARTS_IMPORT f32 FilterSteering(f32 arg1);

    // ?ImpactCallback@mmPlayer@@QAEXPAX0@Z
    ARTS_IMPORT void ImpactCallback(void* arg1, void* arg2);

    // ?Init@mmPlayer@@QAEXPAD0PAVmmGame@@@Z
    ARTS_IMPORT void Init(char* arg1, char* arg2, mmGame* arg3);

    // ?IsMaxDamaged@mmPlayer@@QAEHXZ
    ARTS_IMPORT i32 IsMaxDamaged();

    // ?IsPOV@mmPlayer@@QAEHXZ
    ARTS_IMPORT i32 IsPOV();

    // ?ReInit@mmPlayer@@QAEXPAD@Z
    ARTS_IMPORT void ReInit(char* arg1);

    // ?ResetDamage@mmPlayer@@QAEXXZ
    ARTS_IMPORT void ResetDamage();

    // ?ResetFF@mmPlayer@@QAEXXZ
    ARTS_IMPORT void ResetFF();

    // ?SetCamInterest@mmPlayer@@QAEXPAVphInertialCS@@@Z
    ARTS_IMPORT void SetCamInterest(phInertialCS* arg1);

    // ?SetGameCallback@mmPlayer@@QAEXVdatCallback@@@Z
    ARTS_IMPORT void SetGameCallback(datCallback arg1);

    // ?SetMPPostCam@mmPlayer@@QAEXPAVMatrix34@@M@Z
    ARTS_IMPORT void SetMPPostCam(Matrix34* arg1, f32 arg2);

    // ?SetPostRaceCam@mmPlayer@@QAEXXZ
    ARTS_IMPORT void SetPostRaceCam();

    // ?SetPreRaceCam@mmPlayer@@QAEXXZ
    ARTS_IMPORT void SetPreRaceCam();

    // ?SetSteering@mmPlayer@@QAEXM@Z
    ARTS_IMPORT void SetSteering(f32 arg1);

    // ?SetWideFOV@mmPlayer@@QAEXH@Z
    ARTS_IMPORT void SetWideFOV(i32 arg1);

    // ?UpdateFF@mmPlayer@@QAEXXZ
    ARTS_IMPORT void UpdateFF();

    // ?UpdateHOG@mmPlayer@@QAEXXZ
    ARTS_IMPORT void UpdateHOG();

    // ?UpdateRegen@mmPlayer@@QAEXXZ
    ARTS_IMPORT void UpdateRegen();

protected:
    // ?GetCamera@mmPlayer@@IAEXAAH0@Z
    ARTS_IMPORT void GetCamera(i32& arg1, i32& arg2);

    // ?GetCurrentGameCamIndex@mmPlayer@@IAEHXZ
    ARTS_IMPORT i32 GetCurrentGameCamIndex();

    // ?GetCurrentXCamIndex@mmPlayer@@IAEHXZ
    ARTS_IMPORT i32 GetCurrentXCamIndex();

    // ?GetNextCycleCamIndex@mmPlayer@@IAEHXZ
    ARTS_IMPORT i32 GetNextCycleCamIndex();

    // ?GetNextCycleXCamIndex@mmPlayer@@IAEHXZ
    ARTS_IMPORT i32 GetNextCycleXCamIndex();

    // ?SetCamera@mmPlayer@@IAEXHH@Z
    ARTS_IMPORT void SetCamera(i32 arg1, i32 arg2);

private:
    // ?GetCurrentCameraPtr@mmPlayer@@AAEPAVcamCarCS@@XZ
    ARTS_IMPORT camCarCS* GetCurrentCameraPtr();

    // ?InitSpeechAudio@mmPlayer@@AAEXPAD@Z
    ARTS_IMPORT void InitSpeechAudio(char* arg1);
};

// ??_7mmPlayer@@6B@
// vtable at 0x005B03B4
// check_size(mmPlayer, 0x23CC); // size known, members are not - cannot verify
