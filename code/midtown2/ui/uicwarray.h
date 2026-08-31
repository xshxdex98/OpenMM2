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
#include "eventq7/eqevent.h"

#include "uiwidget.h"

class Card2D;
class VSWidget;

class UICWArray : public uiWidget
{
public:
    // ??0UICWArray@@QAE@XZ
    ARTS_IMPORT UICWArray();

    // ??1UICWArray@@UAE@XZ
    ARTS_IMPORT virtual ~UICWArray();

    // ?Update@UICWArray@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@UICWArray@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Action@UICWArray@@UAEXTeqEvent@@@Z
    ARTS_IMPORT virtual void Action(eqEvent arg1);

    // ?CaptureAction@UICWArray@@UAEXTeqEvent@@@Z
    ARTS_IMPORT virtual void CaptureAction(eqEvent arg1);

    // ?Switch@UICWArray@@UAEXH@Z
    ARTS_IMPORT virtual void Switch(i32 arg1);

    // ?AcceptCapture@UICWArray@@QAEXXZ
    ARTS_IMPORT void AcceptCapture();

    // ?CheckCapture@UICWArray@@QAEXXZ
    ARTS_IMPORT void CheckCapture();

    // ?DebugForceSetting@UICWArray@@QAEXXZ
    ARTS_IMPORT void DebugForceSetting();

    // ?DefaultCFG@UICWArray@@QAEXXZ
    ARTS_IMPORT void DefaultCFG();

    // ?EnterCapture@UICWArray@@QAEXH@Z
    ARTS_IMPORT void EnterCapture(i32 arg1);

    // ?ForceCapture@UICWArray@@QAEXXZ
    ARTS_IMPORT void ForceCapture();

    // ?Init@UICWArray@@QAEXMMMMHVdatCallback@@@Z
    ARTS_IMPORT void Init(f32 arg1, f32 arg2, f32 arg3, f32 argC, i32 arg5, datCallback arg6);

    // ?LoadCFG@UICWArray@@QAEXXZ
    ARTS_IMPORT void LoadCFG();

    // ?Redraw@UICWArray@@QAEXXZ
    ARTS_IMPORT void Redraw();

    // ?ResetCapture@UICWArray@@QAEXXZ
    ARTS_IMPORT void ResetCapture();

    // ?SaveCFG@UICWArray@@QAEXXZ
    ARTS_IMPORT void SaveCFG();

    // ?SetStartOffset@UICWArray@@QAEXH@Z
    ARTS_IMPORT void SetStartOffset(i32 arg1);

    // ?SetVScrollPos@UICWArray@@QAEXXZ
    ARTS_IMPORT void SetVScrollPos();

    // ?SetVScrollVals@UICWArray@@QAEXXZ
    ARTS_IMPORT void SetVScrollVals();

    // ?VScrollCB@UICWArray@@QAEXXZ
    ARTS_IMPORT void VScrollCB();

public:
    // Members from 0x06C; everything below that belongs to uiWidget.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    Card2D* field_6C; // 0x06C
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
    VSWidget* field_B4; // 0x0B4
    u32 field_B8; // 0x0B8
    u32 field_BC; // 0x0BC
    datCallback datcallbackC0; // 0x0C0
};

// ??_7UICWArray@@6B@
// vtable at 0x005B3A60
check_size(UICWArray, 0xCC);
