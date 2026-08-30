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

class mmSingleRoam : public mmGameSingle
{
public:
    // ??0mmSingleRoam@@QAE@XZ
    ARTS_IMPORT mmSingleRoam();

    // ??1mmSingleRoam@@UAE@XZ
    ARTS_IMPORT virtual ~mmSingleRoam();

    // ?Update@mmSingleRoam@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@mmSingleRoam@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Init@mmSingleRoam@@UAEHXZ
    ARTS_IMPORT virtual i32 Init();

    // ?InitMyPlayer@mmSingleRoam@@UAEXXZ
    ARTS_IMPORT virtual void InitMyPlayer();

    // ?InitOtherPlayers@mmSingleRoam@@UAEXXZ
    ARTS_IMPORT virtual void InitOtherPlayers();

    // ?InitGameObjects@mmSingleRoam@@UAEXXZ
    ARTS_IMPORT virtual void InitGameObjects();

    // ?InitHUD@mmSingleRoam@@UAEXXZ
    ARTS_IMPORT virtual void InitHUD();

    // ?UpdateGameInput@mmSingleRoam@@UAEXH@Z
    ARTS_IMPORT virtual void UpdateGameInput(i32 arg1);

    // ?UpdateDebugKeyInput@mmSingleRoam@@UAEXH@Z
    ARTS_IMPORT virtual void UpdateDebugKeyInput(i32 arg1);

    // ?UpdateGame@mmSingleRoam@@UAEXXZ
    ARTS_IMPORT virtual void UpdateGame();

    // ?NextRace@mmSingleRoam@@UAEXXZ
    ARTS_IMPORT virtual void NextRace();

    // ?HitWaterHandler@mmSingleRoam@@UAEXXZ
    ARTS_IMPORT virtual void HitWaterHandler();

    // ?SwitchState@mmSingleRoam@@UAEXH@Z
    ARTS_IMPORT virtual void SwitchState(i32 arg1);
};

// ??_7mmSingleRoam@@6B@
// vtable at 0x005B07CC
// check_size(mmSingleRoam, 0x76F0); // size known, members are not - cannot verify
