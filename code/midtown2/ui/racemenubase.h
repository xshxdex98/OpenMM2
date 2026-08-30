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

#include "data7/datcallback.h"
#include "mmgame/mmracedata.h"

#include "uimenu.h"

class uiWidget;

class RaceMenuBase : public UIMenu
{
public:
    // ??0RaceMenuBase@@QAE@H@Z
    ARTS_IMPORT RaceMenuBase(i32 arg1);

    // ??1RaceMenuBase@@UAE@XZ
    ARTS_IMPORT virtual ~RaceMenuBase();

    // ?AICallback@RaceMenuBase@@QAEXXZ
    ARTS_IMPORT void AICallback();

    // ?AnotherCityChangeCB@RaceMenuBase@@QAEXXZ
    ARTS_IMPORT void AnotherCityChangeCB();

    // ?ChangeLocalVals@RaceMenuBase@@QAEXXZ
    ARTS_IMPORT void ChangeLocalVals();

    // ?CheatCallback@RaceMenuBase@@QAEXXZ
    ARTS_IMPORT void CheatCallback();

    // ?CityChange@RaceMenuBase@@QAEXXZ
    ARTS_IMPORT void CityChange();

    // ?DecLocale@RaceMenuBase@@QAEXXZ
    ARTS_IMPORT void DecLocale();

    // ?DecRaceName@RaceMenuBase@@QAEXXZ
    ARTS_IMPORT void DecRaceName();

    // ?DecTime@RaceMenuBase@@QAEXXZ
    ARTS_IMPORT void DecTime();

    // ?DecWeather@RaceMenuBase@@QAEXXZ
    ARTS_IMPORT void DecWeather();

    // ?FocusDescription@RaceMenuBase@@QAEXHH@Z
    ARTS_IMPORT void FocusDescription(i32 arg1, i32 arg2);

    // ?GameCallback@RaceMenuBase@@QAEXXZ
    ARTS_IMPORT void GameCallback();

    // ?IncLocale@RaceMenuBase@@QAEXXZ
    ARTS_IMPORT void IncLocale();

    // ?IncRaceName@RaceMenuBase@@QAEXXZ
    ARTS_IMPORT void IncRaceName();

    // ?IncTime@RaceMenuBase@@QAEXXZ
    ARTS_IMPORT void IncTime();

    // ?IncWeather@RaceMenuBase@@QAEXXZ
    ARTS_IMPORT void IncWeather();

    // ?Init@RaceMenuBase@@QAEXH@Z
    ARTS_IMPORT void Init(i32 arg1);

    // ?InitCRWidgets@RaceMenuBase@@UAEXXZ
    ARTS_IMPORT virtual void InitCRWidgets();

    // ?LapsCallback@RaceMenuBase@@QAEXXZ
    ARTS_IMPORT void LapsCallback();

    // ?LoadRaceMap@RaceMenuBase@@QAEXXZ
    ARTS_IMPORT void LoadRaceMap();

    // ?PreSetup@RaceMenuBase@@UAEXXZ
    ARTS_IMPORT virtual void PreSetup();

    // ?SetCRWidgets@RaceMenuBase@@UAEXH@Z
    ARTS_IMPORT virtual void SetCRWidgets(i32 arg1);

    // ?SetRW@RaceMenuBase@@QAEXXZ
    ARTS_IMPORT void SetRW();

    // ?SetStateRace@RaceMenuBase@@QAEXXZ
    ARTS_IMPORT void SetStateRace();

    // ?SyncRaceState@RaceMenuBase@@QAEXXZ
    ARTS_IMPORT void SyncRaceState();

    // ?WidgetOnOff@RaceMenuBase@@QAEXHPAVuiWidget@@@Z
    ARTS_IMPORT void WidgetOnOff(i32 arg1, uiWidget* arg2);

    // ?CircuitData@RaceMenuBase@@2VmmRaceData@@A
    ARTS_IMPORT static mmRaceData CircuitData;

    // ?RaceData@RaceMenuBase@@2VmmRaceData@@A
    ARTS_IMPORT static mmRaceData RaceData;

    // ?BlitzData@RaceMenuBase@@2VmmRaceData@@A
    ARTS_IMPORT static mmRaceData BlitzData;

public:
    // Members from 0x08C; everything below that belongs to UIMenu.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 field_8C[28]; // 0x08C
    u8 field_A8[4]; // 0x0A8
    u8 field_AC[4]; // 0x0AC
    u8 field_B0[4]; // 0x0B0
    u8 field_B4[4]; // 0x0B4
    u8 field_B8[4]; // 0x0B8
    u8 field_BC[4]; // 0x0BC
    datCallback field_C0; // 0x0C0
    u8 field_CC[20]; // 0x0CC
    u8 field_E0[4]; // 0x0E0
    u8 field_E4[4]; // 0x0E4
    u8 field_E8[32]; // 0x0E8
    u8 field_108[4]; // 0x108
    u8 field_10C[4]; // 0x10C
    u8 field_110[4]; // 0x110
    u8 field_114[4]; // 0x114
    u8 field_118[4]; // 0x118
    u8 field_11C[4]; // 0x11C
    u8 field_120[4]; // 0x120
};

// ??_7RaceMenuBase@@6B@
// vtable at 0x005B4630
check_size(RaceMenuBase, 0x124);
