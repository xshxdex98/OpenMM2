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

#include "mmgamemulti.h"

struct NETSYS_MSG;
struct NET_RCXHEAD;
class mmWaypoints;

class mmMultiRace : public mmGameMulti
{
public:
    // ??0mmMultiRace@@QAE@XZ
    ARTS_IMPORT mmMultiRace();

    // ??1mmMultiRace@@UAE@XZ
    ARTS_IMPORT virtual ~mmMultiRace();

    // ?Reset@mmMultiRace@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Init@mmMultiRace@@UAEHXZ
    ARTS_IMPORT virtual i32 Init();

    // ?InitMyPlayer@mmMultiRace@@UAEXXZ
    ARTS_IMPORT virtual void InitMyPlayer();

    // ?InitGameObjects@mmMultiRace@@UAEXXZ
    ARTS_IMPORT virtual void InitGameObjects();

    // ?InitHUD@mmMultiRace@@UAEXXZ
    ARTS_IMPORT virtual void InitHUD();

    // ?UpdateGameInput@mmMultiRace@@UAEXH@Z
    ARTS_IMPORT virtual void UpdateGameInput(i32 arg1);

    // ?UpdateGame@mmMultiRace@@UAEXXZ
    ARTS_IMPORT virtual void UpdateGame();

    // ?SwitchState@mmMultiRace@@UAEXH@Z
    ARTS_IMPORT virtual void SwitchState(i32 arg1);

    // ?GetWaypoints@mmMultiRace@@UAEPAVmmWaypoints@@XZ
    ARTS_IMPORT virtual mmWaypoints* GetWaypoints();

    // ?InitNetworkPlayers@mmMultiRace@@UAEXXZ
    ARTS_IMPORT virtual void InitNetworkPlayers();

    // ?SystemMessage@mmMultiRace@@UAEXPAUNETSYS_MSG@@@Z
    ARTS_IMPORT virtual void SystemMessage(NETSYS_MSG* arg1);

    // ?GameMessage@mmMultiRace@@UAEXPAUNET_RCXHEAD@@@Z
    ARTS_IMPORT virtual void GameMessage(NET_RCXHEAD* window);

    // ?SetTimeoutOff@mmMultiRace@@QAEXXZ
    ARTS_IMPORT void SetTimeoutOff();

    // ?SetTimeoutOn@mmMultiRace@@QAEXXZ
    ARTS_IMPORT void SetTimeoutOn();

public:
    // Members from 0xB1FC; everything below that belongs to mmGameMulti.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 field_B1FC[8]; // 0xB1FC
    i32 field_B204; // 0xB204
};

// ??_7mmMultiRace@@6B@
// vtable at 0x005B0AB0
check_size(mmMultiRace, 0xB208);
