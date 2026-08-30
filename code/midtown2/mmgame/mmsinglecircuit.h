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

class mmSingleCircuit : public mmGameSingle
{
public:
    // ??0mmSingleCircuit@@QAE@XZ
    ARTS_IMPORT mmSingleCircuit();

    // ??1mmSingleCircuit@@UAE@XZ
    ARTS_IMPORT virtual ~mmSingleCircuit();

    // ?Update@mmSingleCircuit@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@mmSingleCircuit@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Init@mmSingleCircuit@@UAEHXZ
    ARTS_IMPORT virtual i32 Init();

    // ?InitMyPlayer@mmSingleCircuit@@UAEXXZ
    ARTS_IMPORT virtual void InitMyPlayer();

    // ?InitOtherPlayers@mmSingleCircuit@@UAEXXZ
    ARTS_IMPORT virtual void InitOtherPlayers();

    // ?InitGameObjects@mmSingleCircuit@@UAEXXZ
    ARTS_IMPORT virtual void InitGameObjects();

    // ?InitHUD@mmSingleCircuit@@UAEXXZ
    ARTS_IMPORT virtual void InitHUD();

    // ?UpdateGameInput@mmSingleCircuit@@UAEXH@Z
    ARTS_IMPORT virtual void UpdateGameInput(i32 arg1);

    // ?UpdateDebugKeyInput@mmSingleCircuit@@UAEXH@Z
    ARTS_IMPORT virtual void UpdateDebugKeyInput(i32 arg1);

    // ?UpdateGame@mmSingleCircuit@@UAEXXZ
    ARTS_IMPORT virtual void UpdateGame();

    // ?NextRace@mmSingleCircuit@@UAEXXZ
    ARTS_IMPORT virtual void NextRace();

    // ?HitWaterHandler@mmSingleCircuit@@UAEXXZ
    ARTS_IMPORT virtual void HitWaterHandler();

    // ?SwitchState@mmSingleCircuit@@UAEXH@Z
    ARTS_IMPORT virtual void SwitchState(i32 arg1);

    // ?GetWaypoints@mmSingleCircuit@@UAEPAVmmWaypoints@@XZ
    ARTS_IMPORT virtual mmWaypoints* GetWaypoints();

    // ?UpdateOpponentStatus@mmSingleCircuit@@QAEXXZ
    ARTS_IMPORT void UpdateOpponentStatus();

    // ?UpdateScore@mmSingleCircuit@@QAEXXZ
    ARTS_IMPORT void UpdateScore();

private:
    // ?FinishMessage@mmSingleCircuit@@AAEXHH@Z
    ARTS_IMPORT void FinishMessage(i32 arg1, i32 arg2);

    // ?ProgressCheck@mmSingleCircuit@@AAEHHH@Z
    ARTS_IMPORT i32 ProgressCheck(i32 arg1, i32 arg2);

    // ?RegisterFinish@mmSingleCircuit@@AAEHXZ
    ARTS_IMPORT i32 RegisterFinish();

    // ?RegisterLap@mmSingleCircuit@@AAEHXZ
    ARTS_IMPORT i32 RegisterLap();
};

// ??_7mmSingleCircuit@@6B@
// vtable at 0x005B06CC
// check_size(mmSingleCircuit, 0x7720); // size known, members are not - cannot verify
