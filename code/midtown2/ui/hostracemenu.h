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

#include "stream/string.h"

#include "racemenubase.h"
#include "uimenu.h"

class UIBMButton;

class HostRaceMenu : public UIMenu
{
public:
    // ??0HostRaceMenu@@QAE@H@Z
    ARTS_IMPORT HostRaceMenu(i32 arg1);

    // ??1HostRaceMenu@@UAE@XZ
    ARTS_IMPORT virtual ~HostRaceMenu();

    // ?PreSetup@HostRaceMenu@@UAEXXZ
    ARTS_IMPORT virtual void PreSetup();

    // ?SetCRWidgets@HostRaceMenu@@UAEXH@Z
    ARTS_IMPORT virtual void SetCRWidgets(i32 getter);

    // ?InitCRWidgets@HostRaceMenu@@UAEXXZ
    ARTS_IMPORT virtual void InitCRWidgets();

    // ?DecodeCRData@HostRaceMenu@@QAEXH@Z
    ARTS_IMPORT void DecodeCRData(i32 arg1);

    // ?EncodeCRData@HostRaceMenu@@QAEHXZ
    ARTS_IMPORT i32 EncodeCRData();

    // ?GetGoldMass@HostRaceMenu@@QAEHXZ
    ARTS_IMPORT i32 GetGoldMass();

    // ?GetGoldMassVal@HostRaceMenu@@QAEHXZ
    ARTS_IMPORT i32 GetGoldMassVal();

    // ?GetLimit@HostRaceMenu@@QAEXAAH0@Z
    ARTS_IMPORT void GetLimit(i32& getter, i32& arg2);

    // ?GetLimitVal@HostRaceMenu@@QAEHAAH@Z
    ARTS_IMPORT i32 GetLimitVal(i32& arg1);

    // ?LimitDec@HostRaceMenu@@QAEXXZ
    ARTS_IMPORT void LimitDec();

    // ?LimitInc@HostRaceMenu@@QAEXXZ
    ARTS_IMPORT void LimitInc();

    // ?MassDec@HostRaceMenu@@QAEXXZ
    ARTS_IMPORT void MassDec();

    // ?MassInc@HostRaceMenu@@QAEXXZ
    ARTS_IMPORT void MassInc();

    // ?SetGameClassCallback@HostRaceMenu@@QAEXXZ
    ARTS_IMPORT void SetGameClassCallback();

    // ?SetGoldMass@HostRaceMenu@@QAEXH@Z
    ARTS_IMPORT void SetGoldMass(i32 arg1);

    // ?SetLimit@HostRaceMenu@@QAEXAAH0@Z
    ARTS_IMPORT void SetLimit(i32& getter, i32& arg2);

    // ?SetLimitControl@HostRaceMenu@@QAEXXZ
    ARTS_IMPORT void SetLimitControl();

public:
    // Members from 0x08C; everything below that belongs to UIMenu.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 pad_8C[152]; // 0x08C
    u8 field_8C[152]; // 0x08C
    u8 field_124[36]; // 0x124
    u8 field_148[4]; // 0x148
    u8 field_14C[4]; // 0x14C
    u8 field_150[4]; // 0x150
    UIBMButton* field_154; // 0x154
    u8 field_158[44]; // 0x158
    string field_184; // 0x184
    string field_18C; // 0x18C
    string field_194; // 0x194
    u8 field_19C[8]; // 0x19C
    u8 field_1A4[4]; // 0x1A4
    u8 field_1A8[12]; // 0x1A8
    i32 field_1B4; // 0x1B4
};

// ??_7HostRaceMenu@@6B@
// vtable at 0x005B424C
check_size(HostRaceMenu, 0x1B8);
