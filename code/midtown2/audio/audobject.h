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
struct _soundattribs_;
class audControl;
class audFX;
class audFileSystem;
class audSoundBankHdr;
class audSoundBuffer;
class audStream;

class audObject
{
public:
    enum AUD_OBJECTFLAGS : i32;
    enum AUD_OBJECTSTATUS : i32;

    // ?CreateDuplicate@audObject@@QAEPAV1@PAV1@@Z
    ARTS_IMPORT audObject* CreateDuplicate(audObject* arg1);

    // ?CreateEmptyObject@audObject@@QAEPAV1@W4AUDTYPE@audManager@@PAV3@@Z
    ARTS_IMPORT audObject* CreateEmptyObject(audManager::AUDTYPE arg1, audManager* arg2);

    // ?CreateFromFile@audObject@@QAEPAV1@W4AUDTYPE@audManager@@PAV3@PAD2K@Z
    ARTS_IMPORT audObject* CreateFromFile(audManager::AUDTYPE arg1, audManager* arg2, char* arg3, char* arg4, ulong arg5);

    // ?CreateFromSoundBank@audObject@@QAEPAV1@W4AUDTYPE@audManager@@PAVaudSoundBankHdr@@HHHPAV3@@Z
    ARTS_IMPORT audObject* CreateFromSoundBank(audManager::AUDTYPE arg1, audSoundBankHdr* arg2, i32 arg3, i32 arg4, i32 arg5, audManager* arg6);

    // ?CreateFXControl@audObject@@QAEPAVaudFX@@XZ
    ARTS_IMPORT audFX* CreateFXControl();

    // ?CreateStreamControl@audObject@@QAEPAVaudStream@@H@Z
    ARTS_IMPORT audStream* CreateStreamControl(i32 arg1);

    // ?CreateStreamFromFile@audObject@@QAEPAV1@W4AUDTYPE@audManager@@PAV3@HPAD2@Z
    ARTS_IMPORT audObject* CreateStreamFromFile(audManager::AUDTYPE arg1, audManager* arg2, i32 arg3, char* arg4, char* arg5);

    // ?CreateVoiceOverFromFile@audObject@@QAEPAV1@W4AUDTYPE@audManager@@PAV3@PAD2K@Z
    ARTS_IMPORT audObject* CreateVoiceOverFromFile(audManager::AUDTYPE arg1, audManager* arg2, char* arg3, char* arg4, ulong arg5);

    // ?Destroy@audObject@@QAEXXZ
    ARTS_IMPORT void Destroy();

    // ?Get3D@audObject@@QAEPAU_audvector3_@@XZ
    ARTS_IMPORT _audvector3_* Get3D();

    // ?Get3DPosition@audObject@@QAEPAU_audvector3_@@XZ
    ARTS_IMPORT _audvector3_* Get3DPosition();

    // ?Get3DVelocity@audObject@@QAEPAU_audvector3_@@XZ
    ARTS_IMPORT _audvector3_* Get3DVelocity();

    // ?GetBankHandle@audObject@@QAEHXZ
    ARTS_IMPORT i32 GetBankHandle();

    // ?GetBitsPerSample@audObject@@QAEHXZ
    ARTS_IMPORT i32 GetBitsPerSample();

    // ?GetBuffer@audObject@@QAEPAVaudSoundBuffer@@XZ
    ARTS_IMPORT audSoundBuffer* GetBuffer();

    // ?GetControl@audObject@@QAEPAVaudControl@@XZ
    ARTS_IMPORT audControl* GetControl();

    // ?GetCreatedBufferFlag@audObject@@QAE_NXZ
    ARTS_IMPORT bool GetCreatedBufferFlag();

    // ?GetDefaultPan@audObject@@QAEMXZ
    ARTS_IMPORT f32 GetDefaultPan();

    // ?GetDefaultPitch@audObject@@QAEMXZ
    ARTS_IMPORT f32 GetDefaultPitch();

    // ?GetDefaultVolume@audObject@@QAEMXZ
    ARTS_IMPORT f32 GetDefaultVolume();

    // ?GetFile@audObject@@QAEPAVaudFileSystem@@XZ
    ARTS_IMPORT audFileSystem* GetFile();

    // ?GetFlags@audObject@@QAE?AW4AUD_OBJECTFLAGS@1@XZ
    ARTS_IMPORT audObject::AUD_OBJECTFLAGS GetFlags();

    // ?GetFXControl@audObject@@QAEPAVaudFX@@XZ
    ARTS_IMPORT audFX* GetFXControl();

    // ?GetHandle@audObject@@QAEHXZ
    ARTS_IMPORT i32 GetHandle();

    // ?GetManager@audObject@@QAEPAVaudManager@@XZ
    ARTS_IMPORT audManager* GetManager();

    // ?GetNChannels@audObject@@QAEHXZ
    ARTS_IMPORT i32 GetNChannels();

    // ?GetPan@audObject@@QAEMXZ
    ARTS_IMPORT f32 GetPan();

    // ?GetPitch@audObject@@QAEMXZ
    ARTS_IMPORT f32 GetPitch();

    // ?GetPriority@audObject@@QAEHXZ
    ARTS_IMPORT i32 GetPriority();

    // ?GetSamplesPerSec@audObject@@QAEHXZ
    ARTS_IMPORT i32 GetSamplesPerSec();

    // ?GetSoundBank@audObject@@QAEPAVaudSoundBankHdr@@XZ
    ARTS_IMPORT audSoundBankHdr* GetSoundBank();

    // ?GetStatus@audObject@@QAE?AW4AUD_OBJECTSTATUS@1@XZ
    ARTS_IMPORT audObject::AUD_OBJECTSTATUS GetStatus();

    // ?GetStreamControl@audObject@@QAEPAVaudStream@@XZ
    ARTS_IMPORT audStream* GetStreamControl();

    // ?GetTotalSize@audObject@@QAEHXZ
    ARTS_IMPORT i32 GetTotalSize();

    // ?GetType@audObject@@QAE?AW4AUDTYPE@audManager@@XZ
    ARTS_IMPORT audManager::AUDTYPE GetType();

    // ?GetVolume@audObject@@QAEMXZ
    ARTS_IMPORT f32 GetVolume();

    // ?Init@audObject@@QAEXXZ
    ARTS_IMPORT void Init();

    // ?PauseResume@audObject@@QAE_N_N@Z
    ARTS_IMPORT bool PauseResume(bool arg1);

    // ?Play@audObject@@QAE_NXZ
    ARTS_IMPORT bool Play();

    // ?RewindFile@audObject@@QAEXXZ
    ARTS_IMPORT void RewindFile();

    // ?Set3DPosition@audObject@@QAEXPAU_audvector3_@@@Z
    ARTS_IMPORT void Set3DPosition(_audvector3_* arg1);

    // ?Set3DVelocity@audObject@@QAEXPAU_audvector3_@@@Z
    ARTS_IMPORT void Set3DVelocity(_audvector3_* arg1);

    // ?SetBitsPerSample@audObject@@QAEXH@Z
    ARTS_IMPORT void SetBitsPerSample(i32 arg1);

    // ?SetBuffer@audObject@@QAEXPAVaudSoundBuffer@@@Z
    ARTS_IMPORT void SetBuffer(audSoundBuffer* arg1);

    // ?SetControl@audObject@@QAEXPAVaudControl@@@Z
    ARTS_IMPORT void SetControl(audControl* arg1);

    // ?SetCreatedBufferFlag@audObject@@QAEX_N@Z
    ARTS_IMPORT void SetCreatedBufferFlag(bool arg1);

    // ?SetDefaultPan@audObject@@QAEXM@Z
    ARTS_IMPORT void SetDefaultPan(f32 arg1);

    // ?SetDefaultPitch@audObject@@QAEXM@Z
    ARTS_IMPORT void SetDefaultPitch(f32 arg1);

    // ?SetDefaults@audObject@@QAEXPAU_soundattribs_@@@Z
    ARTS_IMPORT void SetDefaults(_soundattribs_* arg1);

    // ?SetDefaultVolume@audObject@@QAEXM@Z
    ARTS_IMPORT void SetDefaultVolume(f32 arg1);

    // ?SetFlags@audObject@@QAEXW4AUD_OBJECTFLAGS@1@0@Z
    ARTS_IMPORT void SetFlags(audObject::AUD_OBJECTFLAGS arg1, audObject::AUD_OBJECTFLAGS arg2);

    // ?SetHandle@audObject@@QAEXH@Z
    ARTS_IMPORT void SetHandle(i32 arg1);

    // ?SetNbrChannels@audObject@@QAEXH@Z
    ARTS_IMPORT void SetNbrChannels(i32 arg1);

    // ?SetNbrSamplesPerSec@audObject@@QAEXH@Z
    ARTS_IMPORT void SetNbrSamplesPerSec(i32 arg1);

    // ?SetPan@audObject@@QAE_NM@Z
    ARTS_IMPORT bool SetPan(f32 arg1);

    // ?SetPitch@audObject@@QAE_NM@Z
    ARTS_IMPORT bool SetPitch(f32 arg1);

    // ?SetPriority@audObject@@QAEXH@Z
    ARTS_IMPORT void SetPriority(i32 arg1);

    // ?SetTotalSize@audObject@@QAEXH@Z
    ARTS_IMPORT void SetTotalSize(i32 arg1);

    // ?SetType@audObject@@QAEXW4AUDTYPE@audManager@@@Z
    ARTS_IMPORT void SetType(audManager::AUDTYPE arg1);

    // ?SetVolPCEchoBuffer@audObject@@QAEXM@Z
    ARTS_IMPORT void SetVolPCEchoBuffer(f32 arg1);

    // ?SetVolume@audObject@@QAE_NM@Z
    ARTS_IMPORT bool SetVolume(f32 arg1);

    // ?Stop@audObject@@QAE_NXZ
    ARTS_IMPORT bool Stop();

    // ?StopPCEchoBuffer@audObject@@QAEXXZ
    ARTS_IMPORT void StopPCEchoBuffer();

    // ?Update@audObject@@QAE_NH@Z
    ARTS_IMPORT bool Update(i32 arg1);
};

// check_size(audObject, 0x0); // TODO: no layout in the IDB type library
