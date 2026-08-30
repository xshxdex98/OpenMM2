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

#include "stream/string.h"

#include "uimenu.h"

class UITextDropdown;
class mmTextNode;

class MainMenu : public UIMenu
{
public:
    // ??0MainMenu@@QAE@H@Z
    ARTS_IMPORT MainMenu(i32 arg1);

    // ??1MainMenu@@UAE@XZ
    ARTS_IMPORT virtual ~MainMenu();

    // ?AddPlayer@MainMenu@@QAEXPAD@Z
    ARTS_IMPORT void AddPlayer(char* arg1);

    // ?DecPlayer@MainMenu@@QAEXXZ
    ARTS_IMPORT void DecPlayer();

    // ?DeleteCB@MainMenu@@QAEXXZ
    ARTS_IMPORT void DeleteCB();

    // ?DisplayDriverInfo@MainMenu@@QAEXPAD0000H@Z
    ARTS_IMPORT void DisplayDriverInfo(char* arg1, char* arg2, char* arg3, char* arg4, char* arg5, i32 arg6);

    // ?EnableReplay@MainMenu@@QAEXH@Z
    ARTS_IMPORT void EnableReplay(i32 arg1);

    // ?EnterNewPlayer@MainMenu@@QAEXXZ
    ARTS_IMPORT void EnterNewPlayer();

    // ?FocusDescription@MainMenu@@QAEXHH@Z
    ARTS_IMPORT void FocusDescription(i32 arg1, i32 arg2);

    // ?IncPlayer@MainMenu@@QAEXXZ
    ARTS_IMPORT void IncPlayer();

    // ?InitDriver@MainMenu@@QAEXXZ
    ARTS_IMPORT void InitDriver();

    // ?IsReplayReadOnly@MainMenu@@QAEHXZ
    ARTS_IMPORT i32 IsReplayReadOnly();

    // ?PreSetup@MainMenu@@UAEXXZ
    ARTS_IMPORT virtual void PreSetup();

    // ?RemoveAllPlayers@MainMenu@@QAEXXZ
    ARTS_IMPORT void RemoveAllPlayers();

    // ?RemovePlayer@MainMenu@@QAEXPAD@Z
    ARTS_IMPORT void RemovePlayer(char* arg1);

    // ?SetController@MainMenu@@QAEXPAD@Z
    ARTS_IMPORT void SetController(char* arg1);

    // ?SetNetName@MainMenu@@QAEXPAD@Z
    ARTS_IMPORT void SetNetName(char* arg1);

    // ?SetPlayerPick@MainMenu@@QAEXH@Z
    ARTS_IMPORT void SetPlayerPick(i32 arg1);

    // ?TDPickCB@MainMenu@@QAEXXZ
    ARTS_IMPORT void TDPickCB();

public:
    // Members from 0x08C; everything below that belongs to UIMenu.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 field_8C[40]; // 0x08C
    i32 field_B4; // 0x0B4
    string field_B8; // 0x0B8
    string field_C0; // 0x0C0
    mmTextNode* field_C8; // 0x0C8
    i8 field_CC[40]; // 0x0CC
    UITextDropdown* field_F4; // 0x0F4
    i32 field_F8; // 0x0F8
    i32 field_FC; // 0x0FC
};

// ??_7MainMenu@@6B@
// vtable at 0x005B45E0
check_size(MainMenu, 0x100);
