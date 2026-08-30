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

#include "uimenu.h"

class string;

class VehicleSelectBase : public UIMenu
{
public:
    // ??0VehicleSelectBase@@QAE@H@Z
    ARTS_IMPORT VehicleSelectBase(i32 arg1);

    // ??1VehicleSelectBase@@UAE@XZ
    ARTS_IMPORT virtual ~VehicleSelectBase();

    // ?Update@VehicleSelectBase@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@VehicleSelectBase@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?PreSetup@VehicleSelectBase@@UAEXXZ
    ARTS_IMPORT virtual void PreSetup();

    // ?PostSetup@VehicleSelectBase@@UAEXXZ
    ARTS_IMPORT virtual void PostSetup();

    // ?AllSetCar@VehicleSelectBase@@QAEXPADH@Z
    ARTS_IMPORT void AllSetCar(char* arg1, i32 arg2);

    // ?AssignVehicleStats@VehicleSelectBase@@QAEXHMMMM@Z
    ARTS_IMPORT void AssignVehicleStats(i32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5);

    // ?CarMod@VehicleSelectBase@@QAEXAAH@Z
    ARTS_IMPORT void CarMod(i32& arg1);

    // ?ColorCB@VehicleSelectBase@@QAEXXZ
    ARTS_IMPORT void ColorCB();

    // ?CurrentVehicleIsLocked@VehicleSelectBase@@QAEHXZ
    ARTS_IMPORT i32 CurrentVehicleIsLocked();

    // ?DecCar@VehicleSelectBase@@QAEXXZ
    ARTS_IMPORT void DecCar();

    // ?DecColor@VehicleSelectBase@@QAEXXZ
    ARTS_IMPORT void DecColor();

    // ?DecTrans@VehicleSelectBase@@QAEXXZ
    ARTS_IMPORT void DecTrans();

    // ?FillStats@VehicleSelectBase@@QAEXXZ
    ARTS_IMPORT void FillStats();

    // ?FocusDescription@VehicleSelectBase@@QAEXHH@Z
    ARTS_IMPORT void FocusDescription(i32 arg1, i32 arg2);

    // ?GetCarTitle@VehicleSelectBase@@QAEPADHPADFPAVstring@@@Z
    ARTS_IMPORT char* GetCarTitle(i32 arg1, char* arg2, i16 arg3, string* arg4);

    // ?IncCar@VehicleSelectBase@@QAEXXZ
    ARTS_IMPORT void IncCar();

    // ?IncColor@VehicleSelectBase@@QAEXXZ
    ARTS_IMPORT void IncColor();

    // ?IncTrans@VehicleSelectBase@@QAEXXZ
    ARTS_IMPORT void IncTrans();

    // ?InitCarSelection@VehicleSelectBase@@QAEXHMMMM@Z
    ARTS_IMPORT void InitCarSelection(i32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5);

    // ?LoadStats@VehicleSelectBase@@QAEHPAD@Z
    ARTS_IMPORT i32 LoadStats(char* arg1);

    // ?LockColor@VehicleSelectBase@@QAEXXZ
    ARTS_IMPORT void LockColor();

    // ?SetLastUnlockedVehicle@VehicleSelectBase@@QAEXXZ
    ARTS_IMPORT void SetLastUnlockedVehicle();

    // ?SetLockedLabel@VehicleSelectBase@@QAEXXZ
    ARTS_IMPORT void SetLockedLabel();

    // ?SetPick@VehicleSelectBase@@QAEXHF@Z
    ARTS_IMPORT void SetPick(i32 arg1, i16 arg2);

    // ?SetShowcaseFlag@VehicleSelectBase@@QAEXXZ
    ARTS_IMPORT void SetShowcaseFlag();

    // ?ShowCarDesc@VehicleSelectBase@@QAEXXZ
    ARTS_IMPORT void ShowCarDesc();

    // ?TDPickCB@VehicleSelectBase@@QAEXXZ
    ARTS_IMPORT void TDPickCB();
};

// ??_7VehicleSelectBase@@6B@
// vtable at 0x005B3D38
// check_size(VehicleSelectBase, 0x8C); // size known, members are not - cannot verify
