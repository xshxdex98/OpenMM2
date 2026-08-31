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
#include "vector7/vector3.h"

class Base_vtbl;
class datParser;
class mmPlayer;
class modShader;
class modStatic;

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
    i32 field_18; // 0x018
    i32 field_1c; // 0x01C
    i32 field_20; // 0x020
    i32 field_24; // 0x024
    i32 field_28; // 0x028
    i32 field_2c; // 0x02C
    i32 field_30; // 0x030
    i32 field_34; // 0x034
    i32 field_38; // 0x038
    i32 field_3c; // 0x03C
    i32 field_40; // 0x040
    i32 field_44; // 0x044
    i32 field_48; // 0x048
    i32 field_4c; // 0x04C
    i32 field_50; // 0x050
    i32 field_54; // 0x054
    i32 field_58; // 0x058
    i32 field_5c; // 0x05C
    i32 field_60; // 0x060
    i32 field_64; // 0x064
    i32 field_68; // 0x068
    i32 field_6c; // 0x06C
    i32 field_70; // 0x070
    i32 field_74; // 0x074
    i32 field_78; // 0x078
    i32 field_7c; // 0x07C
    mmPlayer* Player; // 0x080
    i32 field_84; // 0x084
    i32 field_88; // 0x088
    i32 field_8c; // 0x08C
    RadialGauge RPMGauge; // 0x090
    RadialGauge SpeedGauge; // 0x1A4
    RadialGauge DamageGauge; // 0x2B8
    i32 field_3cc; // 0x3CC
    i32 field_3d0; // 0x3D0
    i32 field_3d4; // 0x3D4
    i32 field_3d8; // 0x3D8
    i32 field_3dc; // 0x3DC
    i32 field_3e0; // 0x3E0
    i32 field_3e4; // 0x3E4
    i32 field_3e8; // 0x3E8
    i32 field_3ec; // 0x3EC
    i32 field_3f0; // 0x3F0
    f32 field_3f4; // 0x3F4
    f32 field_3f8; // 0x3F8
    f32 field_3fc; // 0x3FC
    f32 WheelFact; // 0x400
    i32 field_404; // 0x404
    asLinearCS field_408; // 0x408
    asLinearCS field_488; // 0x488
    asLinearCS field_508; // 0x508
    f32 field_588; // 0x588
    f32 field_58c; // 0x58C
    f32 field_590; // 0x590
    Vector3 DashPos; // 0x594
    Vector3 RoofPos; // 0x5A0
    i32 field_5ac; // 0x5AC
    i32 field_5b0; // 0x5B0
    i32 field_5b4; // 0x5B4
    i32 field_5b8; // 0x5B8
    i32 field_5bc; // 0x5BC
    i32 field_5c0; // 0x5C0
    i32 field_5c4; // 0x5C4
    i32 field_5c8; // 0x5C8
    i32 field_5cc; // 0x5CC
    f32 field_5d0; // 0x5D0
    f32 field_5d4; // 0x5D4
    f32 field_5d8; // 0x5D8
    i32 field_5dc; // 0x5DC
    modStatic* DamageNeedleModStatic; // 0x5E0
    modStatic* DashModStatic; // 0x5E4
    modStatic* DashExtraModStatic; // 0x5E8
    modStatic* GearIndicatorModStatic; // 0x5EC
    modStatic* RoofModStatic; // 0x5F0
    modStatic* SpeedNeedleModStatic; // 0x5F4
    modStatic* TachNeedleModStatic; // 0x5F8
    modStatic* WheelModStatic; // 0x5FC
    modShader** ShaderSet; // 0x600
    u8 field_604; // 0x604
    u8 pad_605[3]; // 0x605
    Vector3 GearIndicatorPivot; // 0x608
    Vector3 DamageNeedlePivot; // 0x614
    Vector3 SpeedNeedlePivot; // 0x620
    Vector3 TachNeedlePivot; // 0x62C
    Vector3 DmgOffset; // 0x638
    Vector3 SpeedOffset; // 0x644
    Vector3 TachOffset; // 0x650
    Vector3 WheelPos; // 0x65C
    Vector3 DmgPivotOffset; // 0x668
    Vector3 SpeedPivotOffset; // 0x674
    Vector3 TachPivotOffset; // 0x680
    Vector3 WheelPivotOffset; // 0x68C
    Vector3 GearPivotOffset; // 0x698
    f32 field_6a4; // 0x6A4
    f32 field_6a8; // 0x6A8
    f32 field_6ac; // 0x6AC
    f32 field_6b0; // 0x6B0
    f32 field_6b4; // 0x6B4
    f32 field_6b8; // 0x6B8
    f32 field_6bc; // 0x6BC
    f32 field_6c0; // 0x6C0
    f32 field_6c4; // 0x6C4
    f32 field_6c8; // 0x6C8
    f32 field_6cc; // 0x6CC
    f32 field_6d0; // 0x6D0
    i32 PivotDebug; // 0x6D4
    i32 field_6d8; // 0x6D8
};

// ??_7mmDashView@@6B@
// vtable at 0x005B0D78
check_size(mmDashView, 0x6DC);
