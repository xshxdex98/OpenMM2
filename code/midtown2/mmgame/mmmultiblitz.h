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

class mmMultiBlitz : public mmGameMulti
{
public:
    // ??0mmMultiBlitz@@QAE@XZ
    ARTS_IMPORT mmMultiBlitz();

    // ??1mmMultiBlitz@@UAE@XZ
    ARTS_IMPORT virtual ~mmMultiBlitz();

    // ?GameMessage@mmMultiBlitz@@UAEXPAUNET_RCXHEAD@@@Z
    ARTS_IMPORT virtual void GameMessage(NET_RCXHEAD* arg1);

    // ?GetWaypoints@mmMultiBlitz@@UAEPAVmmWaypoints@@XZ
    ARTS_IMPORT virtual mmWaypoints* GetWaypoints();

    // ?Init@mmMultiBlitz@@UAEHXZ
    ARTS_IMPORT virtual i32 Init();

    // ?InitGameObjects@mmMultiBlitz@@UAEXXZ
    ARTS_IMPORT virtual void InitGameObjects();

    // ?InitHUD@mmMultiBlitz@@UAEXXZ
    ARTS_IMPORT virtual void InitHUD();

    // ?InitMyPlayer@mmMultiBlitz@@UAEXXZ
    ARTS_IMPORT virtual void InitMyPlayer();

    // ?InitNetworkPlayers@mmMultiBlitz@@UAEXXZ
    ARTS_IMPORT virtual void InitNetworkPlayers();

    // ?PlayTimerWarning@mmMultiBlitz@@QAEXM@Z
    ARTS_IMPORT void PlayTimerWarning(f32 arg1);

    // ?Reset@mmMultiBlitz@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?SwitchState@mmMultiBlitz@@UAEXH@Z
    ARTS_IMPORT virtual void SwitchState(i32 arg1);

    // ?SystemMessage@mmMultiBlitz@@UAEXPAUNETSYS_MSG@@@Z
    ARTS_IMPORT virtual void SystemMessage(NETSYS_MSG* arg1);

    // ?UpdateGame@mmMultiBlitz@@UAEXXZ
    ARTS_IMPORT virtual void UpdateGame();

    // ?UpdateGameInput@mmMultiBlitz@@UAEXH@Z
    ARTS_IMPORT virtual void UpdateGameInput(i32 arg1);
};

// ??_7mmMultiBlitz@@6B@
// vtable at 0x005B0850
// check_size(mmMultiBlitz, 0xB218); // size known, members are not - cannot verify
