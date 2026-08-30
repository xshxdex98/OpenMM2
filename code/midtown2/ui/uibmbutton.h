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
#include "vector7/vector3.h"

#include "uiwidget.h"

struct LocString;
union eqEvent;

class UIBMButton : public uiWidget
{
public:
    // ??0UIBMButton@@QAE@XZ
    ARTS_IMPORT UIBMButton();

    // ??1UIBMButton@@UAE@XZ
    ARTS_IMPORT virtual ~UIBMButton();

    // ?Cull@UIBMButton@@UAEXXZ
    ARTS_IMPORT virtual void Cull();

    // ?Update@UIBMButton@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Disable@UIBMButton@@UAEXXZ
    ARTS_IMPORT virtual void Disable();

    // ?Enable@UIBMButton@@UAEXXZ
    ARTS_IMPORT virtual void Enable();

    // ?Action@UIBMButton@@UAEXTeqEvent@@@Z
    ARTS_IMPORT virtual void Action(eqEvent arg1);

    // ?Switch@UIBMButton@@UAEXH@Z
    ARTS_IMPORT virtual void Switch(i32 arg1);

    // ?ReturnDescription@UIBMButton@@UAEPADXZ
    ARTS_IMPORT virtual char* ReturnDescription();

    // ?SetPosition@UIBMButton@@UAEXMM@Z
    ARTS_IMPORT virtual void SetPosition(f32 arg1, f32 arg2);

    // ?GetScreenHeight@UIBMButton@@UAEMXZ
    ARTS_IMPORT virtual f32 GetScreenHeight();

    // ?DoToggle@UIBMButton@@QAEXXZ
    ARTS_IMPORT void DoToggle();

    // ?GetDiv@UIBMButton@@QAEHXZ
    ARTS_IMPORT i32 GetDiv();

    // ?GetHitArea@UIBMButton@@QAEXAAM0@Z
    ARTS_IMPORT void GetHitArea(f32& arg1, f32& arg2);

    // ?Init@UIBMButton@@QAEXPADMMHHPAHHHPAULocString@@VdatCallback@@3@Z
    ARTS_IMPORT void Init(char* arg1, f32 arg2, f32 arg3, i32 arg4, i32 arg5, i32* arg6, i32 arg7, i32 arg8, LocString* arg9, datCallback arg10, datCallback arg11);

    // ?Kill@UIBMButton@@QAEXXZ
    ARTS_IMPORT void Kill();

    // ?LoadBitmap@UIBMButton@@QAEXPAD@Z
    ARTS_IMPORT void LoadBitmap(char* arg1);

    // ?MexOff@UIBMButton@@QAEXXZ
    ARTS_IMPORT void MexOff();

    // ?MexOn@UIBMButton@@QAEXXZ
    ARTS_IMPORT void MexOn();

    // ?MexToggle@UIBMButton@@QAEXXZ
    ARTS_IMPORT void MexToggle();

    // ?Unkill@UIBMButton@@QAEXXZ
    ARTS_IMPORT void Unkill();

private:
    // ?AllocateSounds@UIBMButton@@CAXXZ
    ARTS_IMPORT static void AllocateSounds();

    // ?GetSize@UIBMButton@@AAEXXZ
    ARTS_IMPORT void GetSize();

    // ?PlaySound@UIBMButton@@AAEXXZ
    ARTS_IMPORT void PlaySound();

public:
    // Members from 0x06C; everything below that belongs to uiWidget.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u32 field_6C; // 0x06C
    u32 field_70; // 0x070
    Vector3 Pos; // 0x074
    u32 field_80; // 0x080
    u32 field_84; // 0x084
    u32 field_88; // 0x088
    u32 field_8C; // 0x08C
    u32 field_90; // 0x090
    u8 field_94[20]; // 0x094
    u32 field_A8; // 0x0A8
    u32 field_AC; // 0x0AC
    u32 field_B0; // 0x0B0
    u32 field_B4; // 0x0B4
    u32 field_B8; // 0x0B8
    u32 field_BC; // 0x0BC
    datCallback Callback1; // 0x0C0
    datCallback Callback2; // 0x0CC
};

// ??_7UIBMButton@@6B@
// vtable at 0x005B39FC
check_size(UIBMButton, 0xD8);
