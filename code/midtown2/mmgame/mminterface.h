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
#include "stream/string.h"

#include "mmmiscdata.h"
#include "mmplayerconfig.h"
#include "mmplayerdirectory.h"

enum dgGameMode : i32;

class ControlCustom;
class CrashCourse;
class Dialog_City2;
class Dialog_DriverRec;
class Dialog_Eject;
class Dialog_HallOfFame;
class Dialog_Host;
class Dialog_Replay;
class Dialog_ReplayEdit;
class Dialog_Serial;
class Dialog_TCPIP;
class HostRaceMenu;
class MainMenu;
struct NETSESSION_DESC;
class NetArena;
class NetSelectMenu;
class RaceMenu;
class VehicleSelectBase;
struct mmPlayerInfo;

class mmInterface : public asNode
{
public:
    // ??0mmInterface@@QAE@XZ
    ARTS_IMPORT mmInterface();

    // ??1mmInterface@@UAE@XZ
    ARTS_IMPORT virtual ~mmInterface();

    // ?BeDone@mmInterface@@QAEXXZ
    ARTS_IMPORT void BeDone();

    // ?InitLobby@mmInterface@@QAEXXZ
    ARTS_IMPORT void InitLobby();

    // ?InitPlayerInfo@mmInterface@@QAEXXZ
    ARTS_IMPORT void InitPlayerInfo();

    // ?LobbySwitch@mmInterface@@QAEXHH@Z
    ARTS_IMPORT void LobbySwitch(i32 arg1, i32 arg2);

    // ?MessageCallback@mmInterface@@QAEXPAX0@Z
    ARTS_IMPORT void MessageCallback(void* arg1, void* arg2);

    // ?MessageCallback2@mmInterface@@QAEXPAX0@Z
    ARTS_IMPORT void MessageCallback2(void* arg1, void* arg2);

    // ?Reset@mmInterface@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?SetupArchiveTest@mmInterface@@QAEXAAH@Z
    ARTS_IMPORT void SetupArchiveTest(i32& arg1);

    // ?ShowLockedVehicleMessage@mmInterface@@QAEXXZ
    ARTS_IMPORT void ShowLockedVehicleMessage();

    // ?ShowMain@mmInterface@@QAEXH@Z
    ARTS_IMPORT void ShowMain(i32 arg1);

    // ?Switch@mmInterface@@QAEXHH@Z
    ARTS_IMPORT void Switch(i32 arg1, i32 arg2);

    // ?Update@mmInterface@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?UpdateLobby@mmInterface@@QAEXXZ
    ARTS_IMPORT void UpdateLobby();

private:
    // ?BootPlayerCB@mmInterface@@AAEXPAX0@Z
    ARTS_IMPORT void BootPlayerCB(void* arg1, void* arg2);

    // ?ChangePlayerData@mmInterface@@AAEXH@Z
    ARTS_IMPORT void ChangePlayerData(i32 arg1);

    // ?CitySetupCB@mmInterface@@AAEXXZ
    ARTS_IMPORT void CitySetupCB();

    // ?ClearSessions@mmInterface@@AAEXXZ
    ARTS_IMPORT void ClearSessions();

    // ?CreatePlayer@mmInterface@@AAEHH@Z
    ARTS_IMPORT i32 CreatePlayer(i32 arg1);

    // ?CreateSession@mmInterface@@AAEHXZ
    ARTS_IMPORT i32 CreateSession();

    // ?GetHostPlayerData@mmInterface@@AAEHAAUmmPlayerInfo@@@Z
    ARTS_IMPORT i32 GetHostPlayerData(mmPlayerInfo& arg1);

    // ?GetRaceName@mmInterface@@AAEPADW4dgGameMode@@HPAD@Z
    ARTS_IMPORT char* GetRaceName(dgGameMode arg1, i32 arg2, char* arg3);

    // ?GetRaceString@mmInterface@@AAEXHPAD@Z
    ARTS_IMPORT void GetRaceString(i32 arg1, char* arg2);

    // ?GetReplayDescCB@mmInterface@@AAEXXZ
    ARTS_IMPORT void GetReplayDescCB();

    // ?GetScoreString@mmInterface@@AAEXHPAD@Z
    ARTS_IMPORT void GetScoreString(i32 arg1, char* arg2);

    // ?GetSessionData@mmInterface@@AAEXUNETSESSION_DESC@@@Z
    ARTS_IMPORT void GetSessionData(NETSESSION_DESC arg1);

    // ?GetTimeString@mmInterface@@AAEXMPAD@Z
    ARTS_IMPORT void GetTimeString(f32 arg1, char* arg2);

    // ?GetUniquePlayerName@mmInterface@@AAEXXZ
    ARTS_IMPORT void GetUniquePlayerName();

    // ?GetUnlockedCar@mmInterface@@AAEXXZ
    ARTS_IMPORT void GetUnlockedCar();

    // ?GetUnlockedColor@mmInterface@@AAEXXZ
    ARTS_IMPORT void GetUnlockedColor();

    // ?HOFCB@mmInterface@@AAEXXZ
    ARTS_IMPORT void HOFCB();

    // ?HOFFillRecords@mmInterface@@AAEXPAD@Z
    ARTS_IMPORT void HOFFillRecords(char* arg1);

    // ?HOFInitRecords@mmInterface@@AAEXHPAD@
    ARTS_IMPORT void HOFInitRecords(i32 arg1, char* arg2);

    // ?InitNetwork@mmInterface@@AAEXXZ
    ARTS_IMPORT void InitNetwork();

    // ?InitProtocols@mmInterface@@AAEXXZ
    ARTS_IMPORT void InitProtocols();

    // ?JoinGame@mmInterface@@AAEXXZ
    ARTS_IMPORT void JoinGame();

    // ?JoinLAN@mmInterface@@AAEXXZ
    ARTS_IMPORT void JoinLAN();

    // ?JoinLobbyGame@mmInterface@@AAEXXZ
    ARTS_IMPORT void JoinLobbyGame();

    // ?JoinModem@mmInterface@@AAEHXZ
    ARTS_IMPORT i32 JoinModem();

    // ?JoinPasswordSession@mmInterface@@AAEHH@Z
    ARTS_IMPORT i32 JoinPasswordSession(i32 arg1);

    // ?JoinSerial@mmInterface@@AAEHXZ
    ARTS_IMPORT i32 JoinSerial();

    // ?JoinSession@mmInterface@@AAEHH@Z
    ARTS_IMPORT i32 JoinSession(i32 arg1);

    // ?LimitToHostCars@mmInterface@@AAEXVstring@@@Z
    ARTS_IMPORT void LimitToHostCars(string arg1);

    // ?LobbyCreate@mmInterface@@AAEHXZ
    ARTS_IMPORT i32 LobbyCreate();

    // ?MultiAllReady@mmInterface@@AAE_NXZ
    ARTS_IMPORT bool MultiAllReady();

    // ?MultiFillRoster@mmInterface@@AAEXXZ
    ARTS_IMPORT void MultiFillRoster();

    // ?MultiStartGame@mmInterface@@AAEXXZ
    ARTS_IMPORT void MultiStartGame();

    // ?NetJoinCB@mmInterface@@AAEXXZ
    ARTS_IMPORT void NetJoinCB();

    // ?NetNameCB@mmInterface@@AAEXXZ
    ARTS_IMPORT void NetNameCB();

    // ?PlayerCreate@mmInterface@@AAEHPAD@Z
    ARTS_IMPORT i32 PlayerCreate(char* arg1);

    // ?PlayerCreateCB@mmInterface@@AAEXXZ
    ARTS_IMPORT void PlayerCreateCB();

    // ?PlayerFillCrashRecords@mmInterface@@AAEXPAD@Z
    ARTS_IMPORT void PlayerFillCrashRecords(char* arg1);

    // ?PlayerFillRecords@mmInterface@@AAEXPAD@Z
    ARTS_IMPORT void PlayerFillRecords(char* arg1);

    // ?PlayerFillStats@mmInterface@@AAEXXZ
    ARTS_IMPORT void PlayerFillStats();

    // ?PlayerGraphicsCB@mmInterface@@AAEXXZ
    ARTS_IMPORT void PlayerGraphicsCB();

    // ?PlayerInitStats@mmInterface@@AAEXPAD@Z
    ARTS_IMPORT void PlayerInitStats(char* arg1);

    // ?PlayerLoad@mmInterface@@AAEXPAD@Z
    ARTS_IMPORT void PlayerLoad(char* arg1);

    // ?PlayerLoadCB@mmInterface@@AAEXXZ
    ARTS_IMPORT void PlayerLoadCB();

    // ?PlayerReadState@mmInterface@@AAEXXZ
    ARTS_IMPORT void PlayerReadState();

    // ?PlayerRemove@mmInterface@@AAEXPAD@Z
    ARTS_IMPORT void PlayerRemove(char* arg1);

    // ?PlayerRemoveCB@mmInterface@@AAEXXZ
    ARTS_IMPORT void PlayerRemoveCB();

    // ?PlayerResolveCars@mmInterface@@AAEXXZ
    ARTS_IMPORT void PlayerResolveCars();

    // ?PlayerResolveScore@mmInterface@@AAEXXZ
    ARTS_IMPORT void PlayerResolveScore();

    // ?PlayerSetState@mmInterface@@AAEXXZ
    ARTS_IMPORT void PlayerSetState();

    // ?PlayerSwitchCityCB@mmInterface@@AAEXXZ
    ARTS_IMPORT void PlayerSwitchCityCB();

    // ?PlayUIMusic@mmInterface@@AAEXXZ
    ARTS_IMPORT void PlayUIMusic();

    // ?RefreshDriverList@mmInterface@@AAEXXZ
    ARTS_IMPORT void RefreshDriverList();

    // ?RefreshMe@mmInterface@@AAEXXZ
    ARTS_IMPORT void RefreshMe();

    // ?RefreshPlayers@mmInterface@@AAEXXZ
    ARTS_IMPORT void RefreshPlayers();

    // ?RefreshSessions@mmInterface@@AAEXXZ
    ARTS_IMPORT void RefreshSessions();

    // ?RequestProverb@mmInterface@@AAEXXZ
    ARTS_IMPORT void RequestProverb();

    // ?SendBootMsg@mmInterface@@AAEXK@Z
    ARTS_IMPORT void SendBootMsg(ulong arg1);

    // ?SendChatMessage@mmInterface@@AAEXPAX@Z
    ARTS_IMPORT void SendChatMessage(void* arg1);

    // ?SendHostCars@mmInterface@@AAEXK@Z
    ARTS_IMPORT void SendHostCars(ulong arg1);

    // ?SendMsg@mmInterface@@AAEXHH@Z
    ARTS_IMPORT void SendMsg(i32 arg1, i32 arg2);

    // ?SendReadyReq@mmInterface@@AAEXXZ
    ARTS_IMPORT void SendReadyReq();

    // ?SendReadyStatus@mmInterface@@AAEXXZ
    ARTS_IMPORT void SendReadyStatus();

    // ?SendStartMsg@mmInterface@@AAEXXZ
    ARTS_IMPORT void SendStartMsg();

    // ?SetCRStateData@mmInterface@@AAEXXZ
    ARTS_IMPORT void SetCRStateData();

    // ?SetHostProtocol@mmInterface@@AAEXXZ
    ARTS_IMPORT void SetHostProtocol();

    // ?SetProtocol@mmInterface@@AAEXXZ
    ARTS_IMPORT void SetProtocol();

    // ?SetProtocol2@mmInterface@@AAEHXZ
    ARTS_IMPORT i32 SetProtocol2();

    // ?SetSessionData@mmInterface@@AAEXPAUNETSESSION_DESC@@@Z
    ARTS_IMPORT void SetSessionData(NETSESSION_DESC* arg1);

    // ?SetStateDefaults@mmInterface@@AAEXXZ
    ARTS_IMPORT void SetStateDefaults();

    // ?SetStateRace@mmInterface@@AAEXH@Z
    ARTS_IMPORT void SetStateRace(i32 arg1);

    // ?ShowSessions@mmInterface@@AAEXXZ
    ARTS_IMPORT void ShowSessions();

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    MainMenu* field_18; // 0x018
    RaceMenu* field_1C; // 0x01C
    Dialog_City2* field_20; // 0x020
    VehicleSelectBase* field_24; // 0x024
    u8 field_28[4]; // 0x028
    NetSelectMenu* field_2C; // 0x02C
    u8 field_30[4]; // 0x030
    CrashCourse* field_34; // 0x034
    u8 field_38[4]; // 0x038
    u8 field_3C[4]; // 0x03C
    u8 field_40[4]; // 0x040
    u8 field_44[4]; // 0x044
    u8 field_48[4]; // 0x048
    ControlCustom* field_4C; // 0x04C
    HostRaceMenu* field_50; // 0x050
    NetArena* field_54; // 0x054
    u8 field_58[32]; // 0x058
    i32 field_78; // 0x078
    i32 field_7C; // 0x07C
    u8 field_80[4]; // 0x080
    Dialog_TCPIP* field_84; // 0x084
    u8 field_88[4]; // 0x088
    Dialog_Serial* field_8C; // 0x08C
    void* field_90; // 0x090
    Dialog_DriverRec* field_94; // 0x094
    Dialog_HallOfFame* field_98; // 0x098
    u8 field_9C[8]; // 0x09C
    Dialog_Host* DialogHost; // 0x0A4
    Dialog_Replay* field_A8; // 0x0A8
    Dialog_ReplayEdit* field_AC; // 0x0AC
    Dialog_Eject* field_B0; // 0x0B0
    u8 field_B4[48]; // 0x0B4
    mmPlayerDirectory field_E4; // 0x0E4
    mmPlayerConfig field_1C8; // 0x1C8
    mmMiscData field_7340; // 0x7340
    mmMiscData field_74E4; // 0x74E4
    u8 field_7688[28]; // 0x7688
    u8 field_76A4[4]; // 0x76A4
    u32 field_76A8; // 0x76A8
    i32 field_76AC; // 0x76AC
    u8 field_76B0[16]; // 0x76B0
    i8 field_76C0[128]; // 0x76C0
    string field_7740; // 0x7740
    string field_7748; // 0x7748
};

// ??_7mmInterface@@6B@
// vtable at 0x005B04B0
check_size(mmInterface, 0x7750);
