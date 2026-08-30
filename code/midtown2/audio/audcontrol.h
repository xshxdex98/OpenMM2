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

#include "audmanager.h"

struct _audvector3_;
class audObject;

class audControl
{
public:
    enum AUDCONTROLFLAGS : i32;

    // ?Add@audControl@@QAEHPAVaudObject@@H@Z
    ARTS_IMPORT i32 Add(audObject* arg1, i32 arg2);

    // ?Count@audControl@@QAEHXZ
    ARTS_IMPORT i32 Count();

    // ?Create@audControl@@QAEPAV1@W4AUDTYPE@audManager@@PAV3@@Z
    ARTS_IMPORT audControl* Create(audManager::AUDTYPE arg1, audManager* arg2);

    // ?Destroy@audControl@@QAEXXZ
    ARTS_IMPORT void Destroy();

    // ?Disable3DMode@audControl@@QAEXH@Z
    ARTS_IMPORT void Disable3DMode(i32 arg1);

    // ?Enable3DMode@audControl@@QAEXH@Z
    ARTS_IMPORT void Enable3DMode(i32 arg1);

    // ?FindActiveObjectByHandle@audControl@@QAEPAVaudObject@@H@Z
    ARTS_IMPORT audObject* FindActiveObjectByHandle(i32 arg1);

    // ?FindObjectByHandle@audControl@@QAEPAVaudObject@@H@Z
    ARTS_IMPORT audObject* FindObjectByHandle(i32 arg1);

    // ?FreeAndDestroy@audControl@@QAEXXZ
    ARTS_IMPORT void FreeAndDestroy();

    // ?GetAssetDbId@audControl@@QAEHXZ
    ARTS_IMPORT i32 GetAssetDbId();

    // ?GetControlFlags@audControl@@QAEHXZ
    ARTS_IMPORT i32 GetControlFlags();

    // ?GetHandle@audControl@@QAEHXZ
    ARTS_IMPORT i32 GetHandle();

    // ?GetManager@audControl@@QAEPAVaudManager@@XZ
    ARTS_IMPORT audManager* GetManager();

    // ?GetObjectHandle@audControl@@QAEHPAVaudObject@@@Z
    ARTS_IMPORT i32 GetObjectHandle(audObject* arg1);

    // ?GetPan@audControl@@QAEMH@Z
    ARTS_IMPORT f32 GetPan(i32 arg1);

    // ?GetPath@audControl@@QAEPADXZ
    ARTS_IMPORT char* GetPath();

    // ?GetPitch@audControl@@QAEMH@Z
    ARTS_IMPORT f32 GetPitch(i32 arg1);

    // ?GetPosition@audControl@@QAEPAU_audvector3_@@H@Z
    ARTS_IMPORT _audvector3_* GetPosition(i32 arg1);

    // ?GetPriority@audControl@@QAEHXZ
    ARTS_IMPORT i32 GetPriority();

    // ?GetVolume@audControl@@QAEMH@Z
    ARTS_IMPORT f32 GetVolume(i32 arg1);

    // ?Init@audControl@@QAEXW4AUDTYPE@audManager@@PAV3@@Z
    ARTS_IMPORT void Init(audManager::AUDTYPE arg1, audManager* arg2);

    // ?MoveFromActive@audControl@@QAE_NW4AUDTYPE@audManager@@PAVaudObject@@@Z
    ARTS_IMPORT bool MoveFromActive(audManager::AUDTYPE arg1, audObject* arg2);

    // ?MoveToActive@audControl@@QAE_NW4AUDTYPE@audManager@@PAVaudObject@@@Z
    ARTS_IMPORT bool MoveToActive(audManager::AUDTYPE arg1, audObject* arg2);

    // ?PauseResume@audControl@@QAE_N_NH@Z
    ARTS_IMPORT bool PauseResume(bool arg1, i32 arg2);

    // ?Play@audControl@@QAE_NH@Z
    ARTS_IMPORT bool Play(i32 arg1);

    // ?PlayRandom@audControl@@QAE_NH@Z
    ARTS_IMPORT bool PlayRandom(i32 arg1);

    // ?Remove@audControl@@QAE_NPAVaudObject@@H@Z
    ARTS_IMPORT bool Remove(audObject* arg1, i32 arg2);

    // ?ResetAutoHandle@audControl@@QAEXH@Z
    ARTS_IMPORT void ResetAutoHandle(i32 arg1);

    // ?SetAssetDbId@audControl@@QAEXH@Z
    ARTS_IMPORT void SetAssetDbId(i32 arg1);

    // ?SetControlFlags@audControl@@QAEXW4AUDCONTROLFLAGS@1@0@Z
    ARTS_IMPORT void SetControlFlags(audControl::AUDCONTROLFLAGS arg1, audControl::AUDCONTROLFLAGS arg2);

    // ?SetHandle@audControl@@QAEXH@Z
    ARTS_IMPORT void SetHandle(i32 arg1);

    // ?SetMaxConcurrent@audControl@@QAEXH@Z
    ARTS_IMPORT void SetMaxConcurrent(i32 arg1);

    // ?SetPan@audControl@@QAEXMH@Z
    ARTS_IMPORT void SetPan(f32 arg1, i32 arg2);

    // ?SetPath@audControl@@QAEXPBD@Z
    ARTS_IMPORT void SetPath(const char* arg1);

    // ?SetPitch@audControl@@QAEXMH@Z
    ARTS_IMPORT void SetPitch(f32 arg1, i32 arg2);

    // ?SetPosition@audControl@@QAEXPAU_audvector3_@@H@Z
    ARTS_IMPORT void SetPosition(_audvector3_* arg1, i32 arg2);

    // ?SetPriority@audControl@@QAEXH@Z
    ARTS_IMPORT void SetPriority(i32 arg1);

    // ?SetVolPCEchoBuffers@audControl@@QAEXM@Z
    ARTS_IMPORT void SetVolPCEchoBuffers(f32 arg1);

    // ?SetVolume@audControl@@QAEXMH@Z
    ARTS_IMPORT void SetVolume(f32 result, i32 arg2);

    // ?Stop@audControl@@QAE_NH@Z
    ARTS_IMPORT bool Stop(i32 arg1);

    // ?StopPCEchoBuffers@audControl@@QAEXXZ
    ARTS_IMPORT void StopPCEchoBuffers();

    // ?Update@audControl@@QAEXH@Z
    ARTS_IMPORT void Update(i32 arg1);
};

// check_size(audControl, 0x0); // TODO: no layout in the IDB type library
