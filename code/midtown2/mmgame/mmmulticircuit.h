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

class mmMultiCircuit : public mmGameMulti
{
public:
    // ??0mmMultiCircuit@@QAE@XZ
    ARTS_IMPORT mmMultiCircuit();

    // ??1mmMultiCircuit@@UAE@XZ
    ARTS_IMPORT virtual ~mmMultiCircuit();

    // ?GameMessage@mmMultiCircuit@@UAEXPAUNET_RCXHEAD@@@Z
    ARTS_IMPORT virtual void GameMessage(NET_RCXHEAD* arg1);

    // ?GetWaypoints@mmMultiCircuit@@UAEPAVmmWaypoints@@XZ
    ARTS_IMPORT virtual mmWaypoints* GetWaypoints();

    // ?Init@mmMultiCircuit@@UAEHXZ
    ARTS_IMPORT virtual i32 Init();

    // ?InitGameObjects@mmMultiCircuit@@UAEXXZ
    ARTS_IMPORT virtual void InitGameObjects();

    // ?InitHUD@mmMultiCircuit@@UAEXXZ
    ARTS_IMPORT virtual void InitHUD();

    // ?InitMyPlayer@mmMultiCircuit@@UAEXXZ
    ARTS_IMPORT virtual void InitMyPlayer();

    // ?InitNetworkPlayers@mmMultiCircuit@@UAEXXZ
    ARTS_IMPORT virtual void InitNetworkPlayers();

    // ?Reset@mmMultiCircuit@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?SwitchState@mmMultiCircuit@@UAEXH@Z
    ARTS_IMPORT virtual void SwitchState(i32 arg1);

    // ?SystemMessage@mmMultiCircuit@@UAEXPAUNETSYS_MSG@@@Z
    ARTS_IMPORT virtual void SystemMessage(NETSYS_MSG* arg1);

    // ?UpdateDebugKeyInput@mmMultiCircuit@@UAEXH@Z
    ARTS_IMPORT virtual void UpdateDebugKeyInput(i32 arg1);

    // ?UpdateGame@mmMultiCircuit@@UAEXXZ
    ARTS_IMPORT virtual void UpdateGame();

    // ?UpdateGameInput@mmMultiCircuit@@UAEXH@Z
    ARTS_IMPORT virtual void UpdateGameInput(i32 arg1);

public:
    // Members from 0xB1FC; everything below that belongs to mmGameMulti.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 field_B1FC[8]; // 0xB1FC
    i32 field_B204; // 0xB204
};

// ??_7mmMultiCircuit@@6B@
// vtable at 0x005B08EC
check_size(mmMultiCircuit, 0xB208);
