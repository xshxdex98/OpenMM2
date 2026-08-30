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

class UIBMButton;

class uiNavBar : public UIMenu
{
public:
    // ??0uiNavBar@@QAE@H@Z
    ARTS_IMPORT uiNavBar(i32 arg1);

    // ??1uiNavBar@@UAE@XZ
    ARTS_IMPORT virtual ~uiNavBar();

    // ?Update@uiNavBar@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?BackUp@uiNavBar@@UAEXXZ
    ARTS_IMPORT virtual void BackUp();

    // ?Help@uiNavBar@@QAEXXZ
    ARTS_IMPORT void Help();

    // ?Minimize@uiNavBar@@QAEXXZ
    ARTS_IMPORT void Minimize();

    // ?OptionActive@uiNavBar@@QAEXXZ
    ARTS_IMPORT void OptionActive();

    // ?OptionInActive@uiNavBar@@QAEXXZ
    ARTS_IMPORT void OptionInActive();

    // ?ResetState@uiNavBar@@QAEXXZ
    ARTS_IMPORT void ResetState();

    // ?SetPrevBitmap@uiNavBar@@QAEXPAD@Z
    ARTS_IMPORT void SetPrevBitmap(char* arg1);

    // ?SetPrevPos@uiNavBar@@QAEXMM@Z
    ARTS_IMPORT void SetPrevPos(f32 arg1, f32 arg2);

    // ?TurnOffPrev@uiNavBar@@QAEXXZ
    ARTS_IMPORT void TurnOffPrev();

    // ?TurnOnPrev@uiNavBar@@QAEXXZ
    ARTS_IMPORT void TurnOnPrev();

public:
    // Members from 0x08C; everything below that belongs to UIMenu.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 field_8C[24]; // 0x08C
    UIBMButton* field_A4; // 0x0A4
    UIBMButton* field_A8; // 0x0A8
    f32 field_AC; // 0x0AC
    f32 field_B0; // 0x0B0
    u32 field_B4; // 0x0B4
    u32 field_B8; // 0x0B8
};

// ??_7uiNavBar@@6B@
// vtable at 0x005B32C8
check_size(uiNavBar, 0xBC);
