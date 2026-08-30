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

#include "arts7/asnode.h"

class Vector3;
class mmPopup;

class mmGame : public asNode
{
public:
    // ??0mmGame@@QAE@XZ
    ARTS_IMPORT mmGame();

    // ??1mmGame@@UAE@XZ
    ARTS_IMPORT virtual ~mmGame();

    // ?Update@mmGame@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@mmGame@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?UpdatePaused@mmGame@@UAEXXZ
    ARTS_IMPORT virtual void UpdatePaused();

    // ?Init@mmGame@@UAEHXZ
    ARTS_IMPORT virtual i32 Init();

    // ?InitGameStrings@mmGame@@UAEXXZ
    ARTS_IMPORT virtual void InitGameStrings();

    // ?InitOtherPlayers@mmGame@@UAEXXZ
    ARTS_IMPORT virtual void InitOtherPlayers();

    // ?HitWaterHandler@mmGame@@UAEXXZ
    ARTS_IMPORT virtual void HitWaterHandler();

    // ?DropThruCityHandler@mmGame@@UAEXXZ
    ARTS_IMPORT virtual void DropThruCityHandler();

    // ?SendChatMessage@mmGame@@UAEXPAD@Z
    ARTS_IMPORT virtual void SendChatMessage(char* arg1);

    // ?BeDone@mmGame@@UAEXH@Z
    ARTS_IMPORT virtual void BeDone(i32 arg1);

    // ?CollideAIOpponents@mmGame@@QAEXXZ
    ARTS_IMPORT void CollideAIOpponents();

    // ?CycleCam@mmGame@@QAEXXZ
    ARTS_IMPORT void CycleCam();

    // ?FarClipCB@mmGame@@QAEXXZ
    ARTS_IMPORT void FarClipCB();

    // ?FindGroundPos@mmGame@@QAEXAAVVector3@@0@Z
    ARTS_IMPORT void FindGroundPos(Vector3& arg1, Vector3& arg2);

    // ?IsPopupEnabled@mmGame@@QAEHXZ
    ARTS_IMPORT i32 IsPopupEnabled();

    // ?NetHost@mmGame@@QAEHXZ
    ARTS_IMPORT i32 NetHost();

    // ?PlayerSetState@mmGame@@QAEXXZ
    ARTS_IMPORT void PlayerSetState();

    // ?SetIconsState@mmGame@@QAEXXZ
    ARTS_IMPORT void SetIconsState();

    // ?SetLevelGraphics@mmGame@@QAEXXZ
    ARTS_IMPORT void SetLevelGraphics();

    // ?StartMusic@mmGame@@QAEXXZ
    ARTS_IMPORT void StartMusic();

    // ?UpdateDebugInput@mmGame@@QAE_NXZ
    ARTS_IMPORT bool UpdateDebugInput();

    // ?UpdateDMusic@mmGame@@QAEXXZ
    ARTS_IMPORT void UpdateDMusic();

    // ?UpdateGameInput@mmGame@@QAEXXZ
    ARTS_IMPORT void UpdateGameInput();

    // ?UpdateHorn@mmGame@@QAEX_N@Z
    ARTS_IMPORT void UpdateHorn(bool arg1);

    // ?UpdateSteeringBrakes@mmGame@@QAEXXZ
    ARTS_IMPORT void UpdateSteeringBrakes();

protected:
    // ?CalculateRaceScore@mmGame@@IAEHHH@Z
    ARTS_IMPORT i32 CalculateRaceScore(i32 arg1, i32 arg2);

    // ?InitGizmos@mmGame@@IAEXXZ
    ARTS_IMPORT void InitGizmos();

    // ?InitWeather@mmGame@@IAEXXZ
    ARTS_IMPORT void InitWeather();

    // ?RespawnXYZ@mmGame@@IAEXAAVVector3@@AAM_N22@Z
    ARTS_IMPORT void RespawnXYZ(Vector3& arg1, f32& arg2, bool arg3, bool arg4, bool arg5);

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 gap10[48]; // 0x018
    mmGame::mmGame::mmGame::mmGame::mmPlayer* pPlayer; // 0x048
    u8 field_4C[64]; // 0x04C
    void* field_8C; // 0x08C
    u8 field_90[4]; // 0x090
    mmPopup* popup; // 0x094
    u8 field_98[476]; // 0x098
    u8 field_274[4]; // 0x274
};

// ??_7mmGame@@6B@
// vtable at 0x005B0500
check_size(mmGame, 0x278);
