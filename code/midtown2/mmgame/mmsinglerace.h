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

#include "mmgamesingle.h"

class mmWaypoints;

class mmSingleRace : public mmGameSingle
{
public:
    // ??0mmSingleRace@@QAE@XZ
    ARTS_IMPORT mmSingleRace();

    // ??1mmSingleRace@@UAE@XZ
    ARTS_IMPORT virtual ~mmSingleRace();

    // ?Update@mmSingleRace@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@mmSingleRace@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Init@mmSingleRace@@UAEHXZ
    ARTS_IMPORT virtual i32 Init();

    // ?InitMyPlayer@mmSingleRace@@UAEXXZ
    ARTS_IMPORT virtual void InitMyPlayer();

    // ?InitOtherPlayers@mmSingleRace@@UAEXXZ
    ARTS_IMPORT virtual void InitOtherPlayers();

    // ?InitGameObjects@mmSingleRace@@UAEXXZ
    ARTS_IMPORT virtual void InitGameObjects();

    // ?InitHUD@mmSingleRace@@UAEXXZ
    ARTS_IMPORT virtual void InitHUD();

    // ?UpdateGameInput@mmSingleRace@@UAEXH@Z
    ARTS_IMPORT virtual void UpdateGameInput(i32 arg1);

    // ?UpdateDebugKeyInput@mmSingleRace@@UAEXH@Z
    ARTS_IMPORT virtual void UpdateDebugKeyInput(i32 arg1);

    // ?UpdateGame@mmSingleRace@@UAEXXZ
    ARTS_IMPORT virtual void UpdateGame();

    // ?NextRace@mmSingleRace@@UAEXXZ
    ARTS_IMPORT virtual void NextRace();

    // ?HitWaterHandler@mmSingleRace@@UAEXXZ
    ARTS_IMPORT virtual void HitWaterHandler();

    // ?SwitchState@mmSingleRace@@UAEXH@Z
    ARTS_IMPORT virtual void SwitchState(i32 arg1);

    // ?GetWaypoints@mmSingleRace@@UAEPAVmmWaypoints@@XZ
    ARTS_IMPORT virtual mmWaypoints* GetWaypoints();

    // ?UpdateOpponentStatus@mmSingleRace@@QAEXXZ
    ARTS_IMPORT void UpdateOpponentStatus();

    // ?UpdateScore@mmSingleRace@@QAEXXZ
    ARTS_IMPORT void UpdateScore();

private:
    // ?FinishMessage@mmSingleRace@@AAEXHH@Z
    ARTS_IMPORT void FinishMessage(i32 arg1, i32 arg2);

    // ?NextRaceAvailable@mmSingleRace@@AAEHXZ
    ARTS_IMPORT i32 NextRaceAvailable();

    // ?ProgressCheck@mmSingleRace@@AAEHHH@Z
    ARTS_IMPORT i32 ProgressCheck(i32 arg1, i32 arg2);

    // ?RegisterFinish@mmSingleRace@@AAEHXZ
    ARTS_IMPORT i32 RegisterFinish();
};

// ??_7mmSingleRace@@6B@
// vtable at 0x005B074C
// check_size(mmSingleRace, 0x76F0); // size known, members are not - cannot verify
