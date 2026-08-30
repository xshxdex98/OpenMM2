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

#include "mmgame.h"

class mmWaypoints;

class mmGameSingle : public mmGame
{
public:
    // ??0mmGameSingle@@QAE@XZ
    ARTS_IMPORT mmGameSingle();

    // ??1mmGameSingle@@UAE@XZ
    ARTS_IMPORT virtual ~mmGameSingle();

    // ?Update@mmGameSingle@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@mmGameSingle@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Init@mmGameSingle@@UAEHXZ
    ARTS_IMPORT virtual i32 Init();

    // ?InitOtherPlayers@mmGameSingle@@UAEXXZ
    ARTS_IMPORT virtual void InitOtherPlayers();

    // ?UpdateGameInput@mmGameSingle@@UAEXH@Z
    ARTS_IMPORT virtual void UpdateGameInput(i32 arg1);

    // ?UpdateDebugKeyInput@mmGameSingle@@UAEXH@Z
    ARTS_IMPORT virtual void UpdateDebugKeyInput(i32 arg1);

    // ?UpdateGame@mmGameSingle@@UAEXXZ
    ARTS_IMPORT virtual void UpdateGame();

    // ?HitWaterHandler@mmGameSingle@@UAEXXZ
    ARTS_IMPORT virtual void HitWaterHandler();

    // ?GetWaypoints@mmGameSingle@@UAEPAVmmWaypoints@@XZ
    ARTS_IMPORT virtual mmWaypoints* GetWaypoints();

protected:
    // ?DisableRacers@mmGameSingle@@IAEXXZ
    ARTS_IMPORT void DisableRacers();

    // ?EnableRacers@mmGameSingle@@IAEXXZ
    ARTS_IMPORT void EnableRacers();

    // ?UpdateRewards@mmGameSingle@@IAE_NXZ
    ARTS_IMPORT bool UpdateRewards();
};

// ??_7mmGameSingle@@6B@
// vtable at 0x005B0EB0
// check_size(mmGameSingle, 0x278); // size known, members are not - cannot verify
