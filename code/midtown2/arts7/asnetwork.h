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

#include "data7/datcallback.h"
#include "misc/timer.h"
#include "net/netzonescore.h"

class DPLCONNECTION;
struct DPMSG_GENERIC;
class IDirectPlay4A;
class IDirectPlayLobby3A;
struct NETCOMMPACK;
struct NETSESSION_DESC;
struct NET_TXHEAD;
struct _GUID;

class asNetwork
{
public:
    // ??0asNetwork@@QAE@XZ
    ARTS_IMPORT asNetwork();

    // ??1asNetwork@@QAE@XZ
    ARTS_IMPORT ~asNetwork();

    // ?BootPlayer@asNetwork@@QAEXH@Z
    ARTS_IMPORT void BootPlayer(i32 arg1);

    // ?ChatMessage@asNetwork@@QAEXKPAD@Z
    ARTS_IMPORT void ChatMessage(ulong arg1, char* arg2);

    // ?CloseSession@asNetwork@@QAEXXZ
    ARTS_IMPORT void CloseSession();

    // ?CreateInterface@asNetwork@@QAEHXZ
    ARTS_IMPORT i32 CreateInterface();

    // ?CreatePlayer@asNetwork@@QAEHPADPAXH@Z
    ARTS_IMPORT i32 CreatePlayer(char* arg1, void* arg2, i32 arg3);

    // ?CreateSession@asNetwork@@QAEHPAD0HPAUNETSESSION_DESC@@@Z
    ARTS_IMPORT i32 CreateSession(char* arg1, char* arg2, i32 arg3, NETSESSION_DESC* arg4);

    // ?Deallocate@asNetwork@@QAEXXZ
    ARTS_IMPORT void Deallocate();

    // ?DestroyPlayer@asNetwork@@QAEXXZ
    ARTS_IMPORT void DestroyPlayer();

    // ?Disconnect@asNetwork@@QAEXXZ
    ARTS_IMPORT void Disconnect();

    // ?DumpRxQueueInfo@asNetwork@@QAEXXZ
    ARTS_IMPORT void DumpRxQueueInfo();

    // ?DumpTxQueueInfo@asNetwork@@QAEXXZ
    ARTS_IMPORT void DumpTxQueueInfo();

    // ?GetEnumModem@asNetwork@@QAEPADH@Z
    ARTS_IMPORT char* GetEnumModem(i32 arg1);

    // ?GetEnumPlayer@asNetwork@@QAEPADH@Z
    ARTS_IMPORT char* GetEnumPlayer(i32 arg1);

    // ?GetEnumPlayerData@asNetwork@@QAEHHPAXH@Z
    ARTS_IMPORT i32 GetEnumPlayerData(i32 key, void* value, i32 buf_len);

    // ?GetEnumProtocol@asNetwork@@QAEPADH@Z
    ARTS_IMPORT char* GetEnumProtocol(i32 arg1);

    // ?GetEnumSession@asNetwork@@QAEPADH@Z
    ARTS_IMPORT char* GetEnumSession(i32 sessionIndex);

    // ?GetEnumSessionID@asNetwork@@QAEHH@Z
    ARTS_IMPORT i32 GetEnumSessionID(i32 arg1);

    // ?GetEnumSessionLock@asNetwork@@QAEHH@Z
    ARTS_IMPORT i32 GetEnumSessionLock(i32 arg1);

    // ?GetEnumSessionStatus@asNetwork@@QAEHH@Z
    ARTS_IMPORT i32 GetEnumSessionStatus(i32 arg1);

    // ?GetGameVersion@asNetwork@@QAEHH@Z
    ARTS_IMPORT i32 GetGameVersion(i32 sessionIndex);

    // ?GetMessageQueue@asNetwork@@QAEXKKAAI0H@Z
    ARTS_IMPORT void GetMessageQueue(ulong arg1, ulong arg2, u32& arg3, u32& arg4, i32 arg5);

    // ?GetMyLogicalIndex@asNetwork@@QAEHXZ
    ARTS_IMPORT i32 GetMyLogicalIndex();

    // ?GetNetworkCaps@asNetwork@@QAEHXZ
    ARTS_IMPORT i32 GetNetworkCaps();

    // ?GetNumModems@asNetwork@@QAEHXZ
    ARTS_IMPORT i32 GetNumModems();

    // ?GetNumPlayers@asNetwork@@QAEHXZ
    ARTS_IMPORT i32 GetNumPlayers();

    // ?GetNumProtocols@asNetwork@@QAEHXZ
    ARTS_IMPORT i32 GetNumProtocols();

    // ?GetNumSessions@asNetwork@@QAEHXZ
    ARTS_IMPORT i32 GetNumSessions();

    // ?GetPlayerData@asNetwork@@QAEHKPAXH@Z
    ARTS_IMPORT i32 GetPlayerData(ulong arg1, void* arg2, i32 arg3);

    // ?GetPlayerID@asNetwork@@QAEKH@Z
    ARTS_IMPORT ulong GetPlayerID(i32 arg1);

    // ?GetPlayerName@asNetwork@@QAEHKPAD@Z
    ARTS_IMPORT i32 GetPlayerName(ulong arg1, char* arg2);

    // ?GetPlayerName@asNetwork@@QAEPADK@Z
    ARTS_IMPORT char* GetPlayerName(ulong arg1);

    // ?GetPlayers@asNetwork@@QAEHPAU_GUID@@@Z
    ARTS_IMPORT i32 GetPlayers(_GUID* arg1);

    // ?GetProtocols@asNetwork@@QAEXXZ
    ARTS_IMPORT void GetProtocols();

    // ?GetSessionData@asNetwork@@QAEXPAUNETSESSION_DESC@@@Z
    ARTS_IMPORT void GetSessionData(NETSESSION_DESC* _Al);

    // ?GetSessionsAsynch@asNetwork@@QAEXXZ
    ARTS_IMPORT void GetSessionsAsynch();

    // ?GetSessionsSynch@asNetwork@@QAEHH@Z
    ARTS_IMPORT i32 GetSessionsSynch(i32 arg1);

    // ?GetTicks@asNetwork@@QAEMXZ
    ARTS_IMPORT f32 GetTicks();

    // ?GetTime@asNetwork@@QAEMXZ
    ARTS_IMPORT f32 GetTime();

    // ?HandleAppMessage@asNetwork@@QAEXPAUNET_TXHEAD@@KK@Z
    ARTS_IMPORT void HandleAppMessage(NET_TXHEAD* arg1, ulong arg2, ulong arg3);

    // ?HandleSysMessage@asNetwork@@QAEXPAUDPMSG_GENERIC@@@Z
    ARTS_IMPORT void HandleSysMessage(DPMSG_GENERIC* arg1);

    // ?Initialize@asNetwork@@QAEHHHH@Z
    ARTS_IMPORT i32 Initialize(i32 maxPlayers, i32 arg2, i32 gameVersion);

    // ?InitializeLobby@asNetwork@@QAEHHH@Z
    ARTS_IMPORT i32 InitializeLobby(i32 maxPlayers, i32 arg2);

    // ?JoinLobbySession@asNetwork@@QAEHXZ
    ARTS_IMPORT i32 JoinLobbySession();

    // ?JoinSession@asNetwork@@QAEHHPAD@Z
    ARTS_IMPORT i32 JoinSession(i32 arg1, char* arg2);

    // ?JoinSession@asNetwork@@QAEHPADPAU_GUID@@0@Z
    ARTS_IMPORT i32 JoinSession(char* arg1, _GUID* arg2, char* arg3);

    // ?Logout@asNetwork@@QAEXXZ
    ARTS_IMPORT void Logout();

    // ?Ping@asNetwork@@QAEKK@Z
    ARTS_IMPORT ulong Ping(ulong arg1);

    // ?PollLobby@asNetwork@@QAEHXZ
    ARTS_IMPORT i32 PollLobby();

    // ?QueryModems@asNetwork@@QAEHXZ
    ARTS_IMPORT i32 QueryModems();

    // ?SealSession@asNetwork@@QAEXXZ
    ARTS_IMPORT void SealSession();

    // ?Send@asNetwork@@QAEHKHPAXKH@Z
    ARTS_IMPORT i32 Send(ulong arg1, i32 arg2, void* arg3, ulong arg4, i32 arg5);

    // ?SendLobbyProperty@asNetwork@@QAEXPAXH@Z
    ARTS_IMPORT void SendLobbyProperty(void* arg1, i32 arg2);

    // ?SetEnumSessionLock@asNetwork@@QAEXHH@Z
    ARTS_IMPORT void SetEnumSessionLock(i32 sessionIndex, i32 arg2);

    // ?SetPlayerData@asNetwork@@QAEXKPAXH@Z
    ARTS_IMPORT void SetPlayerData(ulong arg1, void* arg2, i32 arg3);

    // ?SetProtocol@asNetwork@@QAEHH@Z
    ARTS_IMPORT i32 SetProtocol(i32 arg1);

    // ?SetProtocol@asNetwork@@QAEHHPAUNETCOMMPACK@@@Z
    ARTS_IMPORT i32 SetProtocol(i32 arg1, NETCOMMPACK* arg2);

    // ?SetSessionData@asNetwork@@QAEXPAUNETSESSION_DESC@@PAD@Z
    ARTS_IMPORT void SetSessionData(NETSESSION_DESC* arg1, char* arg2);

    // ?SetTime@asNetwork@@QAEXM@Z
    ARTS_IMPORT void SetTime(f32 arg1);

    // ?StopSessionsAsynch@asNetwork@@QAEXXZ
    ARTS_IMPORT void StopSessionsAsynch();

    // ?UnSealSession@asNetwork@@QAEXXZ
    ARTS_IMPORT void UnSealSession();

    // ?Update@asNetwork@@QAEHXZ
    ARTS_IMPORT i32 Update();

    // ?WaitForLobbyConnection@asNetwork@@QAEXH@Z
    ARTS_IMPORT void WaitForLobbyConnection(i32 arg1);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    datCallback SysMessageCB; // 0x000
    datCallback AppMessageCB; // 0x00C
    IDirectPlay4A* pDPlay; // 0x018
    IDirectPlayLobby3A* pLobby; // 0x01C
    u32 PlayerID; // 0x020
    DPLCONNECTION* pConnection; // 0x024
    GUID* pGUID; // 0x028
    void* nRecvBufSize; // 0x02C
    u32 pRecvBuf; // 0x030
    f32 field_34; // 0x034
    u32 MaxPlayers; // 0x038
    u32 SessionOpen; // 0x03C
    u32 InLobby; // 0x040
    u32 field_44; // 0x044
    u32 field_48; // 0x048
    u8 IsHost; // 0x04C
    Timer Time; // 0x050
    f32 Elapsed; // 0x054
    netZoneScore NetScore; // 0x058
};

check_size(asNetwork, 0x70);
