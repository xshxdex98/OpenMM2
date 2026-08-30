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

class ControlCustom : public OptionsBase
{
public:
    // ??0ControlCustom@@QAE@H@Z
    ARTS_IMPORT ControlCustom(i32 arg1);

    // ??1ControlCustom@@UAE@XZ
    ARTS_IMPORT virtual ~ControlCustom();

    // ?BadAssignCB@ControlCustom@@QAEXXZ
    ARTS_IMPORT void BadAssignCB();

    // ?CancelAction@ControlCustom@@UAEXXZ
    ARTS_IMPORT virtual void CancelAction();

    // ?CancelBadAssignment@ControlCustom@@QAEXXZ
    ARTS_IMPORT void CancelBadAssignment();

    // ?ClearBadAssignment@ControlCustom@@QAEXXZ
    ARTS_IMPORT void ClearBadAssignment();

    // ?DoneAction@ControlCustom@@UAEXXZ
    ARTS_IMPORT virtual void DoneAction();

    // ?ResetDefaultAction@ControlCustom@@UAEXXZ
    ARTS_IMPORT virtual void ResetDefaultAction();

    // ?VerifyBadAssignment@ControlCustom@@QAEXXZ
    ARTS_IMPORT void VerifyBadAssignment();
};

// ??_7ControlCustom@@6B@
// vtable at 0x005B42F4
// check_size(ControlCustom, 0x7218); // size known, members are not - cannot verify
