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

class mmCompBase;

class UICompositeScroll : public uiWidget
{
public:
    // ??0UICompositeScroll@@QAE@XZ
    ARTS_IMPORT UICompositeScroll();

    // ??1UICompositeScroll@@UAE@XZ
    ARTS_IMPORT virtual ~UICompositeScroll();

    // ?Update@UICompositeScroll@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@UICompositeScroll@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Action@UICompositeScroll@@UAEXTeqEvent@@@Z
    ARTS_IMPORT virtual void Action(eqEvent arg1);

    // ?CaptureAction@UICompositeScroll@@UAEXTeqEvent@@@Z
    ARTS_IMPORT virtual void CaptureAction(eqEvent arg1);

    // ?Switch@UICompositeScroll@@UAEXH@Z
    ARTS_IMPORT virtual void Switch(i32 arg1);

    // ?AddComponent@UICompositeScroll@@QAEXPAVmmCompBase@@@Z
    ARTS_IMPORT void AddComponent(mmCompBase* arg1);

    // ?AddTitle@UICompositeScroll@@QAEXPAVmmCompBase@@@Z
    ARTS_IMPORT void AddTitle(mmCompBase* child);

    // ?Clear@UICompositeScroll@@QAEXXZ
    ARTS_IMPORT void Clear();

    // ?GetHeight@UICompositeScroll@@QAEMXZ
    ARTS_IMPORT f32 GetHeight();

    // ?GetHit@UICompositeScroll@@QAEHMM@Z
    ARTS_IMPORT i32 GetHit(f32 transformer, f32 DrawList);

    // ?GetSelectedCount@UICompositeScroll@@QAEHXZ
    ARTS_IMPORT i32 GetSelectedCount();

    // ?Init@UICompositeScroll@@QAEXMMMMMHPAHHHVdatCallback@@@Z
    ARTS_IMPORT void Init(f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, i32 arg6, i32* arg7, i32 arg8, i32 arg9, datCallback arg10);

    // ?InitVScroll@UICompositeScroll@@QAEXMMM@Z
    ARTS_IMPORT void InitVScroll(f32 arg1, f32 arg2, f32 arg3);

    // ?Redraw@UICompositeScroll@@QAEXXZ
    ARTS_IMPORT void Redraw();

    // ?RemoveAllComponentChildren@UICompositeScroll@@QAEXXZ
    ARTS_IMPORT void RemoveAllComponentChildren();

    // ?SetHighlight@UICompositeScroll@@QAEXH@Z
    ARTS_IMPORT void SetHighlight(i32 arg1);

    // ?SetHighlightComp@UICompositeScroll@@QAEXXZ
    ARTS_IMPORT void SetHighlightComp();

    // ?SetPosition@UICompositeScroll@@QAEXH@Z
    ARTS_IMPORT void SetPosition(i32 arg1);

    // ?SetVScrollPos@UICompositeScroll@@QAEXXZ
    ARTS_IMPORT void SetVScrollPos();

    // ?SetVScrollVals@UICompositeScroll@@QAEXXZ
    ARTS_IMPORT void SetVScrollVals();

    // ?VScrollCB@UICompositeScroll@@QAEXXZ
    ARTS_IMPORT void VScrollCB();
};

// ??_7UICompositeScroll@@6B@
// vtable at 0x005B357C
// check_size(UICompositeScroll, 0xD4); // size known, members are not - cannot verify
