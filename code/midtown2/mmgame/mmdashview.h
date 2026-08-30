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

#include "arts7/aslinearcs.h"
#include "arts7/asnode.h"
#include "misc/radialgauge.h"

class datParser;
class mmPlayer;

class mmDashView : public asNode
{
public:
    // ??0mmDashView@@QAE@XZ
    ARTS_IMPORT mmDashView();

    // ??1mmDashView@@UAE@XZ
    ARTS_IMPORT virtual ~mmDashView();

    // ?Cull@mmDashView@@UAEXXZ
    ARTS_IMPORT virtual void Cull();

    // ?Update@mmDashView@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@mmDashView@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?AfterLoad@mmDashView@@UAEXXZ
    ARTS_IMPORT virtual void AfterLoad();

    // ?BeforeSave@mmDashView@@UAEXXZ
    ARTS_IMPORT virtual void BeforeSave();

    // ?GetClassNameA@mmDashView@@UAEPADXZ
    ARTS_IMPORT virtual char* GetClassNameA();

    // ?Activate@mmDashView@@QAEXXZ
    ARTS_IMPORT void Activate();

    // ?ActivateUntilTransitionIsOver@mmDashView@@QAEXXZ
    ARTS_IMPORT void ActivateUntilTransitionIsOver();

    // ?Deactivate@mmDashView@@QAEXXZ
    ARTS_IMPORT void Deactivate();

    // ?Init@mmDashView@@QAEXPADPAVmmPlayer@@@Z
    ARTS_IMPORT void Init(char* node, mmPlayer* window);

    // ?TempDeactivate@mmDashView@@QAEXXZ
    ARTS_IMPORT void TempDeactivate();

private:
    // ?FileIO@mmDashView@@EAEXAAVdatParser@@@Z
    ARTS_IMPORT virtual void FileIO(datParser& arg1);

    // ?LoadPivotInfo@mmDashView@@AAEXPAD@Z
    ARTS_IMPORT void LoadPivotInfo(char* node);

    // ?LoadPkg@mmDashView@@AAEXPAD@Z
    ARTS_IMPORT void LoadPkg(char* shaderSets);

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 field_80[16]; // 0x080
    RadialGauge field_90; // 0x090
    RadialGauge field_1A4; // 0x1A4
    RadialGauge field_2B8; // 0x2B8
    u8 field_3CC[36]; // 0x3CC
    i32 field_3F0; // 0x3F0
    i32 field_3F4; // 0x3F4
    i32 field_3F8; // 0x3F8
    i32 field_3FC; // 0x3FC
    f32 WheelFact; // 0x400
    f32 field_404; // 0x404
    asLinearCS field_408; // 0x408
    asLinearCS field_488; // 0x488
    asLinearCS field_508; // 0x508
    i32 field_588; // 0x588
    i32 field_58C; // 0x58C
    i32 field_590; // 0x590
    i32 DashPos; // 0x594
    f32 field_598; // 0x598
    f32 field_59C; // 0x59C
    i32 RoofPos; // 0x5A0
    f32 field_5A4; // 0x5A4
    i32 field_5A8; // 0x5A8
    i32 field_5AC; // 0x5AC
    i32 field_5B0; // 0x5B0
    i32 field_5B4; // 0x5B4
    u8 field_5B8[16]; // 0x5B8
    i32 field_5C8; // 0x5C8
    i32 field_5CC; // 0x5CC
    f32 field_5D0; // 0x5D0
    f32 field_5D4; // 0x5D4
    i32 field_5D8; // 0x5D8
    u16 field_5DC; // 0x5DC
    u16 field_5DE; // 0x5DE
    u8 field_5E0[36]; // 0x5E0
    u8 field_604; // 0x604
    u8 field_605[207]; // 0x605
    i32 field_6D4; // 0x6D4
    i32 field_6D8; // 0x6D8
};

// ??_7mmDashView@@6B@
// vtable at 0x005B0D78
check_size(mmDashView, 0x6DC);
