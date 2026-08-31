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

#include "vector7/vector3.h"

#include "mmgamemulti.h"
#include "mmpositions.h"
#include "mmtimer.h"

struct NETSYS_MSG;
struct NET_RCXHEAD;
class mmWaypoints;
class vehCar;

class mmMultiCR : public mmGameMulti
{
public:
    // ??0mmMultiCR@@QAE@XZ
    ARTS_IMPORT mmMultiCR();

    // ??1mmMultiCR@@UAE@XZ
    ARTS_IMPORT virtual ~mmMultiCR();

    // ?Reset@mmMultiCR@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Init@mmMultiCR@@UAEHXZ
    ARTS_IMPORT virtual i32 Init();

    // ?InitMyPlayer@mmMultiCR@@UAEXXZ
    ARTS_IMPORT virtual void InitMyPlayer();

    // ?InitGameObjects@mmMultiCR@@UAEXXZ
    ARTS_IMPORT virtual void InitGameObjects();

    // ?InitHUD@mmMultiCR@@UAEXXZ
    ARTS_IMPORT virtual void InitHUD();

    // ?UpdateGameInput@mmMultiCR@@UAEXH@Z
    ARTS_EXPORT virtual void UpdateGameInput(i32 arg1);

    // ?UpdateDebugKeyInput@mmMultiCR@@UAEXH@Z
    ARTS_IMPORT virtual void UpdateDebugKeyInput(i32 arg1);

    // ?UpdateGame@mmMultiCR@@UAEXXZ
    ARTS_IMPORT virtual void UpdateGame();

    // ?HitWaterHandler@mmMultiCR@@UAEXXZ
    ARTS_IMPORT virtual void HitWaterHandler();

    // ?DropThruCityHandler@mmMultiCR@@UAEXXZ
    ARTS_IMPORT virtual void DropThruCityHandler();

    // ?SwitchState@mmMultiCR@@UAEXH@Z
    ARTS_IMPORT virtual void SwitchState(i32 arg1);

    // ?GetWaypoints@mmMultiCR@@UAEPAVmmWaypoints@@XZ
    ARTS_EXPORT virtual mmWaypoints* GetWaypoints();

    // ?InitNetworkPlayers@mmMultiCR@@UAEXXZ
    ARTS_IMPORT virtual void InitNetworkPlayers();

    // ?SystemMessage@mmMultiCR@@UAEXPAUNETSYS_MSG@@@Z
    ARTS_IMPORT virtual void SystemMessage(NETSYS_MSG* arg1);

    // ?GameMessage@mmMultiCR@@UAEXPAUNET_RCXHEAD@@@Z
    ARTS_IMPORT virtual void GameMessage(NET_RCXHEAD* arg1);

    // ?FondleCarMass@mmMultiCR@@QAEXPAVvehCar@@M@Z
    ARTS_IMPORT void FondleCarMass(vehCar* arg1, f32 arg2);

    // ?ImpactCallback@mmMultiCR@@QAEXPAX0@Z
    ARTS_IMPORT void ImpactCallback(void* arg1, void* arg2);

    // ?LoadCSV@mmMultiCR@@QAEHPAD@Z
    ARTS_IMPORT i32 LoadCSV(char* arg1);

    // ?LoadSets@mmMultiCR@@QAEHPAD@Z
    ARTS_IMPORT i32 LoadSets(char* __formal);

    // ?ResetPositions@mmMultiCR@@QAEXH@Z
    ARTS_IMPORT void ResetPositions(i32 arg1);

    // ?SendLobbyResults@mmMultiCR@@QAEXKHH@Z
    ARTS_IMPORT void SendLobbyResults(ulong arg1, i32 arg2, i32 arg3);

private:
    // ?CycleInterest@mmMultiCR@@AAEXH@Z
    ARTS_IMPORT void CycleInterest(i32 arg1);

    // ?DisplayTimeWarning@mmMultiCR@@AAEXH@Z
    ARTS_IMPORT void DisplayTimeWarning(i32 arg1);

    // ?DropGold@mmMultiCR@@AAEXVVector3@@HH@Z
    ARTS_IMPORT void DropGold(Vector3 arg1, i32 arg2, i32 arg3);

    // ?FillResults@mmMultiCR@@AAEXXZ
    ARTS_IMPORT void FillResults();

    // ?FindGround@mmMultiCR@@AAEXVVector3@@AAV2@@Z
    ARTS_IMPORT void FindGround(Vector3 arg1, Vector3& arg2);

    // ?GetNewSet@mmMultiCR@@AAEXXZ
    ARTS_IMPORT void GetNewSet();

    // ?GetRandomIndex@mmMultiCR@@AAEHXZ
    ARTS_IMPORT i32 GetRandomIndex();

    // ?GetRandomPoints@mmMultiCR@@AAEXAAVVector3@@00@Z
    ARTS_IMPORT void GetRandomPoints(Vector3& arg1, Vector3& arg2, Vector3& arg3);

    // ?OppStealGold@mmMultiCR@@AAEXH@Z
    ARTS_IMPORT void OppStealGold(i32 arg1);

    // ?Score@mmMultiCR@@AAEXH@Z
    ARTS_IMPORT void Score(i32 arg1);

    // ?SelectTeams@mmMultiCR@@AAEHXZ
    ARTS_EXPORT i32 SelectTeams();

    // ?SendChangeSet@mmMultiCR@@AAEXH@Z
    ARTS_IMPORT void SendChangeSet(i32 arg1);

    // ?SendGameState@mmMultiCR@@AAEXK@Z
    ARTS_IMPORT void SendGameState(ulong arg1);

    // ?SendGoldAck@mmMultiCR@@AAEXK@Z
    ARTS_IMPORT void SendGoldAck(ulong arg1);

    // ?SendGoldDeliver@mmMultiCR@@AAEXXZ
    ARTS_IMPORT void SendGoldDeliver();

    // ?SendGoldDrop@mmMultiCR@@AAEXVVector3@@@Z
    ARTS_IMPORT void SendGoldDrop(Vector3 arg1);

    // ?SendLimitReached@mmMultiCR@@AAEXH@Z
    ARTS_IMPORT void SendLimitReached(i32 arg1);

    // ?SendSetup@mmMultiCR@@AAEXK@Z
    ARTS_EXPORT void SendSetup(ulong arg1);

    // ?SendTimeWarning@mmMultiCR@@AAEXH@Z
    ARTS_IMPORT void SendTimeWarning(i32 result);

    // ?StealGold@mmMultiCR@@AAEXPAVvehCar@@@Z
    ARTS_IMPORT void StealGold(vehCar* arg1);

    // ?UpdateBank@mmMultiCR@@AAEXXZ
    ARTS_IMPORT void UpdateBank();

    // ?UpdateGold@mmMultiCR@@AAEXXZ
    ARTS_IMPORT void UpdateGold();

    // ?UpdateHideout@mmMultiCR@@AAEXXZ
    ARTS_IMPORT void UpdateHideout();

    // ?UpdateHUD@mmMultiCR@@AAEXXZ
    ARTS_IMPORT void UpdateHUD();

    // ?UpdateLimit@mmMultiCR@@AAEXXZ
    ARTS_IMPORT void UpdateLimit();

    // ?UpdateTimeWarning@mmMultiCR@@AAEXXZ
    ARTS_IMPORT void UpdateTimeWarning();

public:
    // Members from 0xB1FC; everything below that belongs to mmGameMulti.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 field_B1FC[16]; // 0xB1FC
    i32 field_B20C; // 0xB20C
    i32 field_B210; // 0xB210
    i32 field_B214; // 0xB214
    i32 field_B218; // 0xB218
    i32 field_B21C; // 0xB21C
    i32 field_B220; // 0xB220
    i32 field_B224; // 0xB224
    u8 field_B228[32]; // 0xB228
    i32 field_B248; // 0xB248
    i32 field_B24C; // 0xB24C
    i32 field_B250; // 0xB250
    i32 field_B254; // 0xB254
    i32 field_B258; // 0xB258
    i32 field_B25C; // 0xB25C
    mmTimer field_B260; // 0xB260
    f32 field_B290; // 0xB290
    i32 field_B294; // 0xB294
    mmPositions field_B298; // 0xB298
    i32 field_B2A4; // 0xB2A4
};

// ??_7mmMultiCR@@6B@
// vtable at 0x005B0980
check_size(mmMultiCR, 0xB2A8);
