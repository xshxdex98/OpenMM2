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

struct _controlptrs_;
class audControl;
class audFX;
class audFileSystem;
class audHead;
class audSoundHeap;
class audStream;
class audTimeNode;

class audManager
{
public:
    enum AUDTYPE : i32;
    enum SOUNDFLAG : i32;

    enum AUDTYPE : i32;
    enum SOUNDFLAG : i32;

    // ?AddControl@audManager@@QAEHW4AUDTYPE@1@PAVaudControl@@@Z
    ARTS_IMPORT i32 AddControl(audManager::AUDTYPE arg1, audControl* arg2);

    // ?AddHead@audManager@@QAEHPAVaudHead@@@Z
    ARTS_IMPORT i32 AddHead(audHead* arg1);

    // ?AllocControl@audManager@@QAEPAVaudControl@@W4AUDTYPE@1@@Z
    ARTS_IMPORT audControl* AllocControl(audManager::AUDTYPE arg1);

    // ?AllocFileNode@audManager@@QAEPAVaudFileSystem@@XZ
    ARTS_IMPORT audFileSystem* AllocFileNode();

    // ?AllocFXNode@audManager@@QAEPAVaudFX@@XZ
    ARTS_IMPORT audFX* AllocFXNode();

    // ?AllocHeadNode@audManager@@QAEPAVaudHead@@XZ
    ARTS_IMPORT audHead* AllocHeadNode();

    // ?AllocStreamNode@audManager@@QAEPAVaudStream@@XZ
    ARTS_IMPORT audStream* AllocStreamNode();

    // ?AllocTimeNode@audManager@@QAEPAVaudTimeNode@@XZ
    ARTS_IMPORT audTimeNode* AllocTimeNode();

    // ?CountActiveSounds@audManager@@QAEHW4AUDTYPE@1@@Z
    ARTS_IMPORT i32 CountActiveSounds(audManager::AUDTYPE arg1);

    // ?CountSounds@audManager@@QAEHW4AUDTYPE@1@@Z
    ARTS_IMPORT i32 CountSounds(audManager::AUDTYPE arg1);

    // ?Create@audManager@@QAEPAV1@HHHHHHHHHHHW4SOUNDFLAG@1@@Z
    ARTS_IMPORT audManager* Create(i32 arg1, i32 arg2, i32 arg3, i32 arg4, i32 arg5, i32 arg6, i32 arg7, i32 arg8, i32 arg9, i32 arg10, i32 arg11, audManager::SOUNDFLAG arg12);

    // ?Destroy@audManager@@QAEXXZ
    ARTS_IMPORT void Destroy();

    // ?Enable@audManager@@QAEX_N@Z
    ARTS_IMPORT void Enable(bool arg1);

    // ?FreeAndDestroyAllSounds@audManager@@QAEXXZ
    ARTS_IMPORT void FreeAndDestroyAllSounds();

    // ?FreeControl@audManager@@QAEXW4AUDTYPE@1@PAVaudControl@@@Z
    ARTS_IMPORT void FreeControl(audManager::AUDTYPE arg1, audControl* arg2);

    // ?FreeFileNode@audManager@@QAEXPAVaudFileSystem@@@Z
    ARTS_IMPORT void FreeFileNode(audFileSystem* arg1);

    // ?FreeFXNode@audManager@@QAEXPAVaudFX@@@Z
    ARTS_IMPORT void FreeFXNode(audFX* arg1);

    // ?FreeHeadNode@audManager@@QAEXPAVaudHead@@@Z
    ARTS_IMPORT void FreeHeadNode(audHead* lod);

    // ?FreeStreamNode@audManager@@QAEXPAVaudStream@@@Z
    ARTS_IMPORT void FreeStreamNode(audStream* arg1);

    // ?FreeTimeNode@audManager@@QAEXPAVaudTimeNode@@@Z
    ARTS_IMPORT void FreeTimeNode(audTimeNode* lod);

    // ?GetActiveControlListPtr@audManager@@QAEPAVaudControl@@W4AUDTYPE@1@@Z
    ARTS_IMPORT audControl* GetActiveControlListPtr(audManager::AUDTYPE arg1);

    // ?GetActiveHead@audManager@@QAEPAVaudHead@@H@Z
    ARTS_IMPORT audHead* GetActiveHead(i32 arg1);

    // ?GetControlListPtr@audManager@@QAEPAVaudControl@@W4AUDTYPE@1@@Z
    ARTS_IMPORT audControl* GetControlListPtr(audManager::AUDTYPE arg1);

    // ?GetMasterVolume@audManager@@QAEMW4AUDTYPE@1@@Z
    ARTS_IMPORT f32 GetMasterVolume(audManager::AUDTYPE arg1);

    // ?IsEnabled@audManager@@QAE_NXZ
    ARTS_IMPORT bool IsEnabled();

    // ?IsSoundFlags@audManager@@QAE_NW4SOUNDFLAG@1@@Z
    ARTS_IMPORT bool IsSoundFlags(audManager::SOUNDFLAG arg1);

    // ?MoveActiveToBottom@audManager@@QAE_NW4AUDTYPE@1@PAVaudControl@@@Z
    ARTS_IMPORT bool MoveActiveToBottom(audManager::AUDTYPE arg1, audControl* arg2);

    // ?MoveActiveToTop@audManager@@QAE_NW4AUDTYPE@1@PAVaudControl@@@Z
    ARTS_IMPORT bool MoveActiveToTop(audManager::AUDTYPE arg1, audControl* arg2);

    // ?MoveFromActive@audManager@@QAE_NW4AUDTYPE@1@PAVaudControl@@@Z
    ARTS_IMPORT bool MoveFromActive(audManager::AUDTYPE arg1, audControl* arg2);

    // ?MoveToActive@audManager@@QAE_NW4AUDTYPE@1@PAVaudControl@@@Z
    ARTS_IMPORT bool MoveToActive(audManager::AUDTYPE arg1, audControl* arg2);

    // ?RemoveControl@audManager@@QAE_NW4AUDTYPE@1@PAVaudControl@@@Z
    ARTS_IMPORT bool RemoveControl(audManager::AUDTYPE arg1, audControl* arg2);

    // ?RemoveHead@audManager@@QAE_NPAVaudHead@@@Z
    ARTS_IMPORT bool RemoveHead(audHead* arg1);

    // ?RestartAudio@audManager@@QAEXHHH@Z
    ARTS_IMPORT void RestartAudio(i32 arg1, i32 arg2, i32 arg3);

    // ?SetMaxConcurrent@audManager@@QAEXW4AUDTYPE@1@H@Z
    ARTS_IMPORT void SetMaxConcurrent(audManager::AUDTYPE arg1, i32 arg2);

    // ?SetSoundFlags@audManager@@QAEXW4SOUNDFLAG@1@0@Z
    ARTS_IMPORT void SetSoundFlags(audManager::SOUNDFLAG arg1, audManager::SOUNDFLAG arg2);

    // ?SetVolAllSounds@audManager@@QAEXW4AUDTYPE@1@M@Z
    ARTS_IMPORT void SetVolAllSounds(audManager::AUDTYPE arg1, f32 arg2);

    // ?ShutDownAudio@audManager@@QAEXXZ
    ARTS_IMPORT void ShutDownAudio();

    // ?StopAllSounds@audManager@@QAE_NW4AUDTYPE@1@@Z
    ARTS_IMPORT bool StopAllSounds(audManager::AUDTYPE arg1);

    // ?Update@audManager@@QAEXH@Z
    ARTS_IMPORT void Update(i32 arg1);

private:
    // ?InitControlStruct@audManager@@AAE_NPAVaudSoundHeap@@PAU_controlptrs_@@H@Z
    ARTS_IMPORT bool InitControlStruct(audSoundHeap* arg1, _controlptrs_* arg2, i32 arg3);
};

// check_size(audManager, 0x0); // TODO: no layout in the IDB type library
