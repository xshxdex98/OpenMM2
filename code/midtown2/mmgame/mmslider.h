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
#include "vector7/vector2.h"

class gfxBitmap;
class uiWidget;

class mmSlider : public asNode
{
public:
    // ??0mmSlider@@QAE@XZ
    ARTS_IMPORT mmSlider();

    // ??1mmSlider@@UAE@XZ
    ARTS_IMPORT virtual ~mmSlider();

    // ?Cull@mmSlider@@UAEXXZ
    ARTS_IMPORT virtual void Cull();

    // ?Update@mmSlider@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Dec@mmSlider@@QAEMXZ
    ARTS_IMPORT f32 Dec();

    // ?FudgeWidth@mmSlider@@QAEMXZ
    ARTS_IMPORT f32 FudgeWidth();

    // ?GetScreenHeight@mmSlider@@QAEMXZ
    ARTS_IMPORT f32 GetScreenHeight();

    // ?GetSliderHotSpots@mmSlider@@QAEXAAM00000@Z
    ARTS_IMPORT void GetSliderHotSpots(f32& arg1, f32& arg2, f32& arg3, f32& arg4, f32& arg5, f32& arg6);

    // ?Inc@mmSlider@@QAEMXZ
    ARTS_IMPORT f32 Inc();

    // ?Init@mmSlider@@QAEXPADPAVuiWidget@@MMMMHMMHH@Z
    ARTS_IMPORT void Init(char* arg1, uiWidget* arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, i32 arg7, f32 arg8, f32 arg9, i32 arg10, i32 arg11);

    // ?IsReadWrite@mmSlider@@QAEHXZ
    ARTS_IMPORT i32 IsReadWrite();

    // ?LoadBitmap@mmSlider@@QAEXPAD@Z
    ARTS_IMPORT void LoadBitmap(char* arg1);

    // ?SetPosition@mmSlider@@QAEXMM@Z
    ARTS_IMPORT void SetPosition(f32 transformer, f32 DrawList);

    // ?SetRange@mmSlider@@QAEXMM@Z
    ARTS_IMPORT void SetRange(f32 arg1, f32 arg2);

    // ?SetReadWrite@mmSlider@@QAEXH@Z
    ARTS_IMPORT void SetReadWrite(i32 arg1);

    // ?SetStep@mmSlider@@QAEXM@Z
    ARTS_IMPORT void SetStep(f32 arg1);

    // ?SetValue@mmSlider@@QAEMM@Z
    ARTS_IMPORT f32 SetValue(f32 arg1);

    // ?UpdatePosition@mmSlider@@QAEXXZ
    ARTS_IMPORT void UpdatePosition();

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    gfxBitmap* BitmapInactive; // 0x020
    u8 field_24[4]; // 0x024
    gfxBitmap* BitmapLeft; // 0x028
    gfxBitmap* BitmapRight; // 0x02C
    gfxBitmap* BitmapActive; // 0x030
    f32 fMin; // 0x034
    f32 fMax; // 0x038
    i32 EdgeType; // 0x03C
    u32 field_40; // 0x040
    Vector2 Position; // 0x044
    f32 CurrentValue; // 0x04C
    f32 StepAmount; // 0x050
    u32 Step; // 0x054
    u32 CurrentPosition; // 0x058
    u32 field_5C; // 0x05C
    u32 bIsReadWrite; // 0x060
    u32 Step2; // 0x064
    u8 field_68[16]; // 0x068
    i32 SelectionAmount; // 0x078
    u32 ActiveHeight; // 0x07C
    u32 Width; // 0x080
    u32 Height; // 0x084
    u32 HasDecreased; // 0x088
    u32 HasIncreased; // 0x08C
    u32 DestLineY; // 0x090
    u32 HotspotY; // 0x094
    u32 HotspotX; // 0x098
};

// ??_7mmSlider@@6B@
// vtable at 0x005B3C38
check_size(mmSlider, 0x9C);
