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

#include "uimenu.h"

struct LocString;
class UIBMButton;
class UICompositeScroll;
class mmTextNode;

class NetArena : public UIMenu
{
public:
    // ??0NetArena@@QAE@H@Z
    ARTS_IMPORT NetArena(i32 getter);

    // ??1NetArena@@UAE@XZ
    ARTS_IMPORT virtual ~NetArena();

    // ?PreSetup@NetArena@@UAEXXZ
    ARTS_IMPORT virtual void PreSetup();

    // ?AddGameChatLine@NetArena@@QAEXPAD@Z
    ARTS_IMPORT void AddGameChatLine(char* arg1);

    // ?AddRosterName@NetArena@@QAEXPAD@Z
    ARTS_IMPORT void AddRosterName(char* TestChar);

    // ?AddRosterName@NetArena@@QAEXPAULocString@@00KHH@Z
    ARTS_IMPORT void AddRosterName(LocString* arg1, LocString* arg2, LocString* arg3, ulong arg4, i32 arg5, i32 arg6);

    // ?ChangeRosterData@NetArena@@QAEXPAULocString@@KHH@Z
    ARTS_IMPORT void ChangeRosterData(LocString* arg1, ulong arg2, i32 arg3, i32 arg4);

    // ?ChatEntry@NetArena@@QAEXXZ
    ARTS_IMPORT void ChatEntry();

    // ?DisablePlayButton@NetArena@@QAEXXZ
    ARTS_IMPORT void DisablePlayButton();

    // ?EnablePlayButton@NetArena@@QAEXXZ
    ARTS_IMPORT void EnablePlayButton();

    // ?FindRosterName@NetArena@@QAEHPAD@Z
    ARTS_IMPORT i32 FindRosterName(char* arg1);

    // ?GetRaceName@NetArena@@QAEPADXZ
    ARTS_IMPORT char* GetRaceName();

    // ?GetStatus@NetArena@@QAE_NK@Z
    ARTS_IMPORT bool GetStatus(ulong arg1);

    // ?LateJoin@NetArena@@QAEXXZ
    ARTS_IMPORT void LateJoin();

    // ?LoadRaceMap@NetArena@@QAEXXZ
    ARTS_IMPORT void LoadRaceMap();

    // ?PostChatMessages@NetArena@@QAEXXZ
    ARTS_IMPORT void PostChatMessages();

    // ?PostHostSettings@NetArena@@QAEXH@Z
    ARTS_IMPORT void PostHostSettings(i32 arg1);

    // ?PostPlayerInfo@NetArena@@QAEXPAULocString@@00@Z
    ARTS_IMPORT void PostPlayerInfo(LocString* arg1, LocString* arg2, LocString* arg3);

    // ?RemoveRosterEntry@NetArena@@QAEXH@Z
    ARTS_IMPORT void RemoveRosterEntry(i32 arg1);

    // ?RemoveRosterName@NetArena@@QAEXPAD@Z
    ARTS_IMPORT void RemoveRosterName(char* arg1);

    // ?ResetGameChat@NetArena@@QAEXXZ
    ARTS_IMPORT void ResetGameChat();

    // ?ResetRoster@NetArena@@QAEXXZ
    ARTS_IMPORT void ResetRoster();

    // ?RetrieveChatLine@NetArena@@QAEHPAD@Z
    ARTS_IMPORT i32 RetrieveChatLine(char* arg1);

    // ?SetHost@NetArena@@QAEXH@Z
    ARTS_IMPORT void SetHost(i32 arg1);

    // ?SetMyStatus@NetArena@@QAEXH@Z
    ARTS_IMPORT void SetMyStatus(i32 arg1);

    // ?SetStatus@NetArena@@QAEXKHH@Z
    ARTS_IMPORT void SetStatus(ulong arg1, i32 readyStatus, i32 arg3);

    // ?SetTeamWidgets@NetArena@@QAEXXZ
    ARTS_IMPORT void SetTeamWidgets();

    // ?ShowRosterTeam@NetArena@@QAEXH@Z
    ARTS_IMPORT void ShowRosterTeam(i32 arg1);

    // ?SyncJoin@NetArena@@QAEXXZ
    ARTS_IMPORT void SyncJoin();

    // ?TeamCallback@NetArena@@QAEXXZ
    ARTS_IMPORT void TeamCallback();

public:
    // Members from 0x08C; everything below that belongs to UIMenu.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    void* field_8C; // 0x08C
    u8 field_90[4]; // 0x090
    mmTextNode* field_94; // 0x094
    mmTextNode* field_98; // 0x098
    mmTextNode* MmTextNode1; // 0x09C
    mmTextNode* MmTextNode3; // 0x0A0
    u8 field_A4[12]; // 0x0A4
    i32 field_B0; // 0x0B0
    u8 field_B4[8]; // 0x0B4
    u8 field_BC[4]; // 0x0BC
    i8 field_C0; // 0x0C0
    void* field_C4; // 0x0C4
    u8 field_C8[4]; // 0x0C8
    i8* field_CC; // 0x0CC
    i8* field_D0; // 0x0D0
    u8 field_D4[16]; // 0x0D4
    i8* field_E4; // 0x0E4
    u8 field_E8[4]; // 0x0E8
    datCallback datCallback1; // 0x0EC
    datCallback TeamDatCallback; // 0x0F8
    u8 field_104[16]; // 0x104
    u8 field_114[4]; // 0x114
    UIBMButton* UIBMButton1; // 0x118
    u8 field_11C[4]; // 0x11C
    UIBMButton* UIBMButton2; // 0x120
    UIBMButton* UIBMButton3; // 0x124
    u8 field_128[4]; // 0x128
    UICompositeScroll* uiCompositeScroll; // 0x12C
    u8 field_130[4]; // 0x130
    u8 field_134[12]; // 0x134
    mmTextNode* MmTextNode2; // 0x140
};

// ??_7NetArena@@6B@
// vtable at 0x005B4200
check_size(NetArena, 0x144);
