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

class mmSingleStunt : public mmGameSingle
{
public:
    // ??0mmSingleStunt@@QAE@XZ
    ARTS_IMPORT mmSingleStunt();

    // ??1mmSingleStunt@@UAE@XZ
    ARTS_IMPORT virtual ~mmSingleStunt();

    // ?Update@mmSingleStunt@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@mmSingleStunt@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Init@mmSingleStunt@@UAEHXZ
    ARTS_IMPORT virtual i32 Init();

    // ?InitMyPlayer@mmSingleStunt@@UAEXXZ
    ARTS_IMPORT virtual void InitMyPlayer();

    // ?InitOtherPlayers@mmSingleStunt@@UAEXXZ
    ARTS_IMPORT virtual void InitOtherPlayers();

    // ?InitGameObjects@mmSingleStunt@@UAEXXZ
    ARTS_IMPORT virtual void InitGameObjects();

    // ?InitHUD@mmSingleStunt@@UAEXXZ
    ARTS_IMPORT virtual void InitHUD();

    // ?UpdateGameInput@mmSingleStunt@@UAEXH@Z
    ARTS_IMPORT virtual void UpdateGameInput(i32 arg1);

    // ?UpdateDebugKeyInput@mmSingleStunt@@UAEXH@Z
    ARTS_IMPORT virtual void UpdateDebugKeyInput(i32 arg1);

    // ?UpdateGame@mmSingleStunt@@UAEXXZ
    ARTS_IMPORT virtual void UpdateGame();

    // ?NextRace@mmSingleStunt@@UAEXXZ
    ARTS_IMPORT virtual void NextRace();

    // ?HitWaterHandler@mmSingleStunt@@UAEXXZ
    ARTS_IMPORT virtual void HitWaterHandler();

    // ?SwitchState@mmSingleStunt@@UAEXH@Z
    ARTS_IMPORT virtual void SwitchState(i32 arg1);

    // ?EnableRacers@mmSingleStunt@@QAEXXZ
    ARTS_IMPORT void EnableRacers();

    // ?EventSoundCtrl@mmSingleStunt@@QAEX_NH@Z
    ARTS_IMPORT void EventSoundCtrl(bool arg1, i32 arg2);

    // ?InitNewEvent@mmSingleStunt@@QAEXXZ
    ARTS_IMPORT void InitNewEvent();

    // ?RegisterFinish@mmSingleStunt@@QAEHH@Z
    ARTS_IMPORT i32 RegisterFinish(i32 arg1);

private:
    // ?CheckCopPursuit@mmSingleStunt@@AAE_NXZ
    ARTS_IMPORT bool CheckCopPursuit();

    // ?CheckTimeUp@mmSingleStunt@@AAE_NXZ
    ARTS_IMPORT bool CheckTimeUp();

    // ?GetOpponentIndex@mmSingleStunt@@AAEHXZ
    ARTS_IMPORT i32 GetOpponentIndex();

    // ?HUDMessage@mmSingleStunt@@AAEXH@Z
    ARTS_IMPORT void HUDMessage(i32 arg1);

    // ?LoadEventFile@mmSingleStunt@@AAE_NPAD@Z
    ARTS_IMPORT bool LoadEventFile(char* platform_render_arg);

    // ?NextRaceAvailable@mmSingleStunt@@AAE_NXZ
    ARTS_IMPORT bool NextRaceAvailable();

    // ?PlayTimerWarning@mmSingleStunt@@AAEXM@Z
    ARTS_IMPORT void PlayTimerWarning(f32 arg1);

    // ?UpdateAccel@mmSingleStunt@@AAEXXZ
    ARTS_IMPORT void UpdateAccel();

    // ?UpdateBlitz@mmSingleStunt@@AAEXXZ
    ARTS_IMPORT void UpdateBlitz();

    // ?UpdateChase@mmSingleStunt@@AAEXXZ
    ARTS_IMPORT void UpdateChase();

    // ?UpdateCollide@mmSingleStunt@@AAEXXZ
    ARTS_IMPORT void UpdateCollide();

    // ?UpdateCorner@mmSingleStunt@@AAEXXZ
    ARTS_IMPORT void UpdateCorner();

    // ?UpdateEvade@mmSingleStunt@@AAEXXZ
    ARTS_IMPORT void UpdateEvade();

    // ?UpdateFrogger@mmSingleStunt@@AAEXXZ
    ARTS_IMPORT void UpdateFrogger();

    // ?UpdateJump@mmSingleStunt@@AAEXXZ
    ARTS_IMPORT void UpdateJump();

    // ?UpdateStop@mmSingleStunt@@AAEXXZ
    ARTS_IMPORT void UpdateStop();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 field_4[24]; // 0x004
    u8 field_1C[112]; // 0x01C
    void* field_8C; // 0x08C
    u8 field_90[888]; // 0x090
    i32 field_408; // 0x408
    u8 field_40C[29308]; // 0x40C
    i32 field_7688; // 0x7688
    u8 field_768C[692]; // 0x768C
    i32 field_7940; // 0x7940
    i32 field_7944; // 0x7944
    u8 field_7948[8]; // 0x7948
    i32 field_7950; // 0x7950
    i32 field_7954; // 0x7954
    f32 field_7958; // 0x7958
    f32 field_795C; // 0x795C
    f32 field_7960; // 0x7960
    i32 field_7964; // 0x7964
    i32 field_7968; // 0x7968
    i32 field_796C; // 0x796C
};

// ??_7mmSingleStunt@@6B@
// vtable at 0x005B059C
check_size(mmSingleStunt, 0x7970);
