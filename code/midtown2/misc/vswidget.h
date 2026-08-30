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

#include "arts7/asnode.h"
#include "data7/datcallback.h"

union eqEvent;
class gfxBitmap;

class VSWidget : public asNode
{
public:
    // ??0VSWidget@@QAE@XZ
    ARTS_IMPORT VSWidget();

    // ??1VSWidget@@UAE@XZ
    ARTS_IMPORT virtual ~VSWidget();

    // ?Action@VSWidget@@QAEHTeqEvent@@MMH@Z
    ARTS_IMPORT i32 Action(eqEvent arg1, f32 arg2, f32 arg3, i32 arg4);

    // ?CalcTroughRatio@VSWidget@@QAEXM@Z
    ARTS_IMPORT void CalcTroughRatio(f32 arg1);

    // ?Cull@VSWidget@@UAEXXZ
    ARTS_IMPORT virtual void Cull();

    // ?Dec@VSWidget@@QAEMXZ
    ARTS_IMPORT f32 Dec();

    // ?EvalMouseXY@VSWidget@@QAEXMMH@Z
    ARTS_IMPORT void EvalMouseXY(f32 arg1, f32 arg2, i32 arg3);

    // ?Inc@VSWidget@@QAEMXZ
    ARTS_IMPORT f32 Inc();

    // ?Init@VSWidget@@QAEXMMMMPAHVdatCallback@@H@Z
    ARTS_IMPORT void Init(f32 arg1, f32 arg2, f32 arg3, f32 arg4, i32* arg5, datCallback arg6, i32 arg7);

    // ?LoadBitmap@VSWidget@@QAEXXZ
    ARTS_IMPORT void LoadBitmap();

    // ?SetHotSpots@VSWidget@@QAEXXZ
    ARTS_IMPORT void SetHotSpots();

    // ?SetStep@VSWidget@@QAEXHM@Z
    ARTS_IMPORT void SetStep(i32 arg1, f32 arg2);

    // ?SetTrough@VSWidget@@QAEXM@Z
    ARTS_IMPORT void SetTrough(f32 arg1);

    // ?Switch@VSWidget@@QAEXHMM@Z
    ARTS_IMPORT void Switch(i32 arg1, f32 arg2, f32 arg3);

    // ?SyncData@VSWidget@@QAEXXZ
    ARTS_IMPORT void SyncData();

    // ?Update@VSWidget@@UAEXXZ
    ARTS_IMPORT virtual void Update();

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    gfxBitmap* ScrollInact; // 0x018
    gfxBitmap* ScrollUarr; // 0x01C
    gfxBitmap* ScrollDarr; // 0x020
    gfxBitmap* ScrollAct; // 0x024
    f32 field_28; // 0x028
    f32 field_2C; // 0x02C
    f32 field_30; // 0x030
    u32 field_34; // 0x034
    f32 TroughRatio; // 0x038
    f32 MinY; // 0x03C
    f32 field_40; // 0x040
    f32 field_44; // 0x044
    f32 MaxY; // 0x048
    f32 MinX; // 0x04C
    f32 MaxX; // 0x050
    u32 field_54; // 0x054
    u32 field_58; // 0x058
    u32 field_5C; // 0x05C
    u32 field_60; // 0x060
    u32 field_64; // 0x064
    u32 field_68; // 0x068
    u32* pdword6C; // 0x06C
    u32 field_70; // 0x070
    u32 field_74; // 0x074
    u32 field_78; // 0x078
    u32 field_7C; // 0x07C
    u32 field_80; // 0x080
    u32 field_84; // 0x084
    u32 field_88; // 0x088
    u32 field_8C; // 0x08C
    u32 field_90; // 0x090
    u32 field_94; // 0x094
    u32 field_98; // 0x098
    u32 field_9C; // 0x09C
    u32 field_A0; // 0x0A0
    u32 field_A4; // 0x0A4
    u32 field_A8; // 0x0A8
    u32 field_AC; // 0x0AC
    u32 field_B0; // 0x0B0
    datCallback datcallbackB4; // 0x0B4
};

// ??_7VSWidget@@6B@
// vtable at 0x005B3C04
check_size(VSWidget, 0xC0);
