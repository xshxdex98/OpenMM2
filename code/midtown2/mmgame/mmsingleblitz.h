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

class mmSingleBlitz : public mmGameSingle
{
public:
    // ??0mmSingleBlitz@@QAE@XZ
    ARTS_IMPORT mmSingleBlitz();

    // ??1mmSingleBlitz@@UAE@XZ
    ARTS_IMPORT virtual ~mmSingleBlitz();

    // ?GetWaypoints@mmSingleBlitz@@UAEPAVmmWaypoints@@XZ
    ARTS_IMPORT virtual mmWaypoints* GetWaypoints();

    // ?HitWaterHandler@mmSingleBlitz@@UAEXXZ
    ARTS_IMPORT virtual void HitWaterHandler();

    // ?Init@mmSingleBlitz@@UAEHXZ
    ARTS_IMPORT virtual i32 Init();

    // ?InitGameObjects@mmSingleBlitz@@UAEXXZ
    ARTS_IMPORT virtual void InitGameObjects();

    // ?InitHUD@mmSingleBlitz@@UAEXXZ
    ARTS_IMPORT virtual void InitHUD();

    // ?InitMyPlayer@mmSingleBlitz@@UAEXXZ
    ARTS_IMPORT virtual void InitMyPlayer();

    // ?InitOtherPlayers@mmSingleBlitz@@UAEXXZ
    ARTS_IMPORT virtual void InitOtherPlayers();

    // ?NextRace@mmSingleBlitz@@UAEXXZ
    ARTS_IMPORT virtual void NextRace();

    // ?PlayTimerWarning@mmSingleBlitz@@QAEXM@Z
    ARTS_IMPORT void PlayTimerWarning(f32 arg1);

    // ?Reset@mmSingleBlitz@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?SwitchState@mmSingleBlitz@@UAEXH@Z
    ARTS_IMPORT virtual void SwitchState(i32 arg1);

    // ?Update@mmSingleBlitz@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?UpdateDebugKeyInput@mmSingleBlitz@@UAEXH@Z
    ARTS_IMPORT virtual void UpdateDebugKeyInput(i32 arg1);

    // ?UpdateGame@mmSingleBlitz@@UAEXXZ
    ARTS_IMPORT virtual void UpdateGame();

    // ?UpdateGameInput@mmSingleBlitz@@UAEXH@Z
    ARTS_IMPORT virtual void UpdateGameInput(i32 arg1);

private:
    // ?FinishMessage@mmSingleBlitz@@AAEXH@Z
    ARTS_IMPORT void FinishMessage(i32 arg1);

    // ?ProgressCheck@mmSingleBlitz@@AAEHHH@Z
    ARTS_IMPORT i32 ProgressCheck(i32 arg1, i32 arg2);

    // ?RegisterFinish@mmSingleBlitz@@AAEHXZ
    ARTS_IMPORT i32 RegisterFinish();
};

// ??_7mmSingleBlitz@@6B@
// vtable at 0x005B064C
// check_size(mmSingleBlitz, 0x7710); // size known, members are not - cannot verify
