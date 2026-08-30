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
#include "stream/string.h"

class mmDropDown : public asNode
{
public:
    // ??0mmDropDown@@QAE@XZ
    ARTS_IMPORT mmDropDown();

    // ??1mmDropDown@@UAE@XZ
    ARTS_IMPORT virtual ~mmDropDown();

    // ?Update@mmDropDown@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?FindFirstEnabled@mmDropDown@@QAEHXZ
    ARTS_IMPORT i32 FindFirstEnabled();

    // ?GetCurrentString@mmDropDown@@QAEXPADH@Z
    ARTS_IMPORT void GetCurrentString(char* arg1, i32 arg2);

    // ?GetH@mmDropDown@@QAEMXZ
    ARTS_IMPORT f32 GetH();

    // ?GetHit@mmDropDown@@QAEHMM@Z
    ARTS_IMPORT i32 GetHit(f32 transformer, f32 DrawList);

    // ?GetW@mmDropDown@@QAEMXZ
    ARTS_IMPORT f32 GetW();

    // ?GetXmin@mmDropDown@@QAEMXZ
    ARTS_IMPORT f32 GetXmin();

    // ?GetYmin@mmDropDown@@QAEMXZ
    ARTS_IMPORT f32 GetYmin();

    // ?Init@mmDropDown@@QAEXPAXMMMMVstring@@H@Z
    ARTS_IMPORT void Init(void* arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, string arg6, i32 arg7);

    // ?InitString@mmDropDown@@QAEXVstring@@@Z
    ARTS_IMPORT void InitString(string arg1);

    // ?SetDisabledColors@mmDropDown@@QAEXXZ
    ARTS_IMPORT void SetDisabledColors();

    // ?SetHighlight@mmDropDown@@QAEXH@Z
    ARTS_IMPORT void SetHighlight(i32 arg1);

private:
    // ?SetString@mmDropDown@@AAEXVstring@@@Z
    ARTS_IMPORT void SetString(string arg1);
};

// ??_7mmDropDown@@6B@
// vtable at 0x005B3BD0
// check_size(mmDropDown, 0x0); // TODO: no layout in the IDB type library
