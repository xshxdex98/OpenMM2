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

#include "misc/timer.h"
#include "stream/string.h"

#include "mmgame.h"
#include "mmnetobject.h"

class Vector3;
class vehCar;

class mmGameMulti : public mmGame
{
public:
    // ??0mmGameMulti@@QAE@XZ
    ARTS_IMPORT mmGameMulti();

    // ??1mmGameMulti@@UAE@XZ
    ARTS_IMPORT virtual ~mmGameMulti();

    // ?Update@mmGameMulti@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@mmGameMulti@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Init@mmGameMulti@@UAEHXZ
    ARTS_IMPORT virtual i32 Init();

    // ?InitGameStrings@mmGameMulti@@UAEXXZ
    ARTS_IMPORT virtual void InitGameStrings();

    // ?InitOtherPlayers@mmGameMulti@@UAEXXZ
    ARTS_IMPORT virtual void InitOtherPlayers();

    // ?UpdateDebugKeyInput@mmGameMulti@@UAEXH@Z
    ARTS_IMPORT virtual void UpdateDebugKeyInput(i32 arg1);

    // ?UpdateGame@mmGameMulti@@UAEXXZ
    ARTS_IMPORT virtual void UpdateGame();

    // ?NextRace@mmGameMulti@@UAEXXZ
    ARTS_IMPORT virtual void NextRace();

    // ?HitWaterHandler@mmGameMulti@@UAEXXZ
    ARTS_IMPORT virtual void HitWaterHandler();

    // ?DropThruCityHandler@mmGameMulti@@UAEXXZ
    ARTS_IMPORT virtual void DropThruCityHandler();

    // ?SendChatMessage@mmGameMulti@@UAEXPAD@Z
    ARTS_IMPORT virtual void SendChatMessage(char* arg1);

    // ?BeDone@mmGameMulti@@UAEXH@Z
    ARTS_IMPORT virtual void BeDone(i32 arg1);

    // ?InitRoster@mmGameMulti@@UAEXXZ
    ARTS_IMPORT virtual void InitRoster();

    // ?ActivateMapNetObject@mmGameMulti@@QAEXH@Z
    ARTS_IMPORT void ActivateMapNetObject(i32 arg1);

    // ?BootPlayerCB@mmGameMulti@@QAEXPAX0@Z
    ARTS_IMPORT void BootPlayerCB(void* arg1, void* arg2);

    // ?BootStrapCars@mmGameMulti@@QAEXXZ
    ARTS_IMPORT void BootStrapCars();

    // ?BroadCastCarTuning@mmGameMulti@@QAEXH@Z
    ARTS_IMPORT void BroadCastCarTuning(i32 arg1);

    // ?ClearNetObjects@mmGameMulti@@QAEXXZ
    ARTS_IMPORT void ClearNetObjects();

    // ?ClearRank@mmGameMulti@@QAEXXZ
    ARTS_IMPORT void ClearRank();

    // ?DeactivateMapNetObject@mmGameMulti@@QAEXH@Z
    ARTS_IMPORT void DeactivateMapNetObject(i32 arg1);

    // ?FinishMessage@mmGameMulti@@QAEXH@Z
    ARTS_IMPORT void FinishMessage(i32 arg1);

    // ?GameMessageCB@mmGameMulti@@QAEXPAX0@Z
    ARTS_IMPORT void GameMessageCB(void* arg1, void* arg2);

    // ?GetInactiveNetObject@mmGameMulti@@QAEPAVmmNetObject@@XZ
    ARTS_IMPORT mmNetObject* GetInactiveNetObject();

    // ?GetInactiveNetObjectIndex@mmGameMulti@@QAEHXZ
    ARTS_IMPORT i32 GetInactiveNetObjectIndex();

    // ?GetNetObject@mmGameMulti@@QAEPAVmmNetObject@@KH@Z
    ARTS_IMPORT mmNetObject* GetNetObject(ulong arg1, i32 arg2);

    // ?GetNetObjectIndex@mmGameMulti@@QAEHKH@Z
    ARTS_IMPORT i32 GetNetObjectIndex(ulong arg1, i32 arg2);

    // ?ParseChatMessage@mmGameMulti@@QAEHPADH@Z
    ARTS_IMPORT i32 ParseChatMessage(char* arg1, i32 arg2);

    // ?PlayerClearLoaded@mmGameMulti@@QAEXXZ
    ARTS_IMPORT void PlayerClearLoaded();

    // ?PlayerFinishedLoading@mmGameMulti@@QAEXXZ
    ARTS_IMPORT void PlayerFinishedLoading();

    // ?QuitNetwork@mmGameMulti@@QAEXXZ
    ARTS_IMPORT void QuitNetwork();

    // ?RegisterMapNetObjects@mmGameMulti@@QAEXH@Z
    ARTS_IMPORT void RegisterMapNetObjects(i32 arg1);

    // ?SendBootMsg@mmGameMulti@@QAEXK@Z
    ARTS_IMPORT void SendBootMsg(ulong arg1);

    // ?SendCarTuning@mmGameMulti@@QAEXPBDK@Z
    ARTS_IMPORT void SendCarTuning(const char* arg1, ulong arg2);

    // ?SendHostCars@mmGameMulti@@QAEXK@Z
    ARTS_IMPORT void SendHostCars(ulong arg1);

    // ?SendLobbyResults@mmGameMulti@@QAEXHIM@Z
    ARTS_IMPORT void SendLobbyResults(i32 arg1, u32 arg2, f32 arg3);

    // ?SendMsg@mmGameMulti@@QAEXH@Z
    ARTS_IMPORT void SendMsg(i32 arg1);

    // ?SendPosition@mmGameMulti@@QAEXXZ
    ARTS_IMPORT void SendPosition();

    // ?SendTimeMsg@mmGameMulti@@QAEXK@Z
    ARTS_IMPORT void SendTimeMsg(ulong arg1);

    // ?SendToChatMessage@mmGameMulti@@QAEXPADK@Z
    ARTS_IMPORT void SendToChatMessage(char* arg1, ulong arg2);

    // ?SetFinishCam@mmGameMulti@@QAEXXZ
    ARTS_IMPORT void SetFinishCam();

    // ?SystemMessageCB@mmGameMulti@@QAEXPAX0@Z
    ARTS_IMPORT void SystemMessageCB(void* arg1, void* arg2);

    // ?UpdateNetWorkAudio@mmGameMulti@@QAEXXZ
    ARTS_IMPORT void UpdateNetWorkAudio();

    // ?UpdateScore@mmGameMulti@@QAEXXZ
    ARTS_IMPORT void UpdateScore();

protected:
    // ?DisableRacers@mmGameMulti@@IAEXXZ
    ARTS_IMPORT void DisableRacers();

    // ?EnableRacers@mmGameMulti@@IAEXXZ
    ARTS_IMPORT void EnableRacers();

    // ?GetCarCRC@mmGameMulti@@IAEKPAD@Z
    ARTS_IMPORT ulong GetCarCRC(char* arg1);

    // ?SendCheater@mmGameMulti@@IAEXK@Z
    ARTS_IMPORT void SendCheater(ulong arg1);

    // ?SendFinishAck@mmGameMulti@@IAEXKMH@Z
    ARTS_IMPORT void SendFinishAck(ulong arg1, f32 arg2, i32 arg3);

    // ?SendFinishReq@mmGameMulti@@IAEXMH@Z
    ARTS_IMPORT void SendFinishReq(f32 arg1, i32 arg2);

    // ?SendGameSet@mmGameMulti@@IAEXXZ
    ARTS_IMPORT void SendGameSet();

    // ?SendHitWater@mmGameMulti@@IAEXXZ
    ARTS_IMPORT void SendHitWater();

    // ?SendRaceReady@mmGameMulti@@IAEXXZ
    ARTS_IMPORT void SendRaceReady();

    // ?SortResults@mmGameMulti@@IAE_NKMH@Z
    ARTS_IMPORT bool SortResults(ulong arg1, f32 arg2, i32 arg3);

    // ?StartXYZ@mmGameMulti@@IAEXHAAVVector3@@0MM@Z
    ARTS_IMPORT void StartXYZ(i32 arg1, Vector3& arg2, Vector3& arg3, f32 arg4, f32 arg5);

    // ?UpdateResults@mmGameMulti@@IAEXH@Z
    ARTS_IMPORT void UpdateResults(i32 arg1);

public:
    // Members from 0x278; everything below that belongs to mmGame.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 field_278[29720]; // 0x278
    void* field_7690; // 0x7690
    string field_7694; // 0x7694
    string field_769C; // 0x769C
    u8 field_76A4[16]; // 0x76A4
    Timer Timer_1; // 0x76B4
    u8 field_76B8[24]; // 0x76B8
    mmNetObject MMNetObject; // 0x76D0
    mmNetObject* MMNetObjectArray; // 0x7D4C
    u8 gapB12C[13308]; // 0x7D50
    vehCar* VehCar; // 0xB14C
    u8 field_B150[148]; // 0xB150
    void* field_B1E4; // 0xB1E4
    u8 field_B1E8[12]; // 0xB1E8
    string field_B1F4; // 0xB1F4
};

// ??_7mmGameMulti@@6B@
// vtable at 0x005B10A8
check_size(mmGameMulti, 0xB1FC);
