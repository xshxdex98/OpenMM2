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

#include "optionsbase.h"

class uiWidget;

class ControlSetup : public OptionsBase
{
public:
    // ??0ControlSetup@@QAE@H@Z
    ARTS_IMPORT ControlSetup(i32 getter);

    // ??1ControlSetup@@UAE@XZ
    ARTS_IMPORT virtual ~ControlSetup();

    // ?Update@ControlSetup@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?PreSetup@ControlSetup@@UAEXXZ
    ARTS_IMPORT virtual void PreSetup();

    // ?CancelAction@ControlSetup@@UAEXXZ
    ARTS_IMPORT virtual void CancelAction();

    // ?DoneAction@ControlSetup@@UAEXXZ
    ARTS_IMPORT virtual void DoneAction();

    // ?ResetDefaultAction@ControlSetup@@UAEXXZ
    ARTS_IMPORT virtual void ResetDefaultAction();

    // ?StoreCurrentSetup@ControlSetup@@UAEXXZ
    ARTS_IMPORT virtual void StoreCurrentSetup();

    // ?ActivateDeviceOptions@ControlSetup@@QAEXXZ
    ARTS_IMPORT void ActivateDeviceOptions();

    // ?ControlSelect@ControlSetup@@QAEXXZ
    ARTS_IMPORT void ControlSelect();

    // ?CreateDeviceOptions@ControlSetup@@QAEXXZ
    ARTS_IMPORT void CreateDeviceOptions();

    // ?DeactivateAllDeviceOptions@ControlSetup@@QAEXXZ
    ARTS_IMPORT void DeactivateAllDeviceOptions();

    // ?FocusDescription@ControlSetup@@QAEXPAX0@Z
    ARTS_IMPORT void FocusDescription(void* getter, void* arg2);

    // ?InitCustomControls@ControlSetup@@QAEXXZ
    ARTS_IMPORT void InitCustomControls();

    // ?LaunchJoyCpl@ControlSetup@@QAEXXZ
    ARTS_IMPORT void LaunchJoyCpl();

    // ?POVCB@ControlSetup@@QAEXXZ
    ARTS_IMPORT void POVCB();

    // ?SetControlPosition@ControlSetup@@QAEMPAVuiWidget@@MM@Z
    ARTS_IMPORT f32 SetControlPosition(uiWidget* arg1, f32 arg2, f32 arg3);

    // ?SetFFPermissions@ControlSetup@@QAEXXZ
    ARTS_IMPORT void SetFFPermissions();

    // ?SetSensitivityCB@ControlSetup@@QAEXXZ
    ARTS_IMPORT void SetSensitivityCB();
};

// ??_7ControlSetup@@6B@
// vtable at 0x005B429C
// check_size(ControlSetup, 0x8C); // size known, members are not - cannot verify
