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

union eqEvent;
class mmTextNode;

class mmCompBase : public asNode
{
public:
    // ??1mmCompBase@@UAE@XZ
    ARTS_IMPORT virtual ~mmCompBase();

    // ?Action@mmCompBase@@UAEXTeqEvent@@@Z
    ARTS_IMPORT virtual void Action(eqEvent arg1);

    // ?Action@mmCompBase@@UAEXTeqEvent@@MMH@Z
    ARTS_IMPORT virtual void Action(eqEvent arg1, f32 arg2, f32 arg3, i32 arg4);

    // ?Box@mmCompBase@@UAEXHPAVmmTextNode@@H@Z
    ARTS_IMPORT virtual void Box(i32 arg1, mmTextNode* arg2, i32 arg3);

    // ?CaptureAction@mmCompBase@@UAEXTeqEvent@@@Z
    ARTS_IMPORT virtual void CaptureAction(eqEvent arg1);

    // ?DisableBlt@mmCompBase@@UAEXXZ
    ARTS_IMPORT virtual void DisableBlt();

    // ?EvalMouseXY@mmCompBase@@UAEXMM@Z
    ARTS_IMPORT virtual void EvalMouseXY(f32 arg1, f32 arg2);

    // ?Highlight@mmCompBase@@UAEXPAVmmTextNode@@H@Z
    ARTS_IMPORT virtual void Highlight(mmTextNode* arg1, i32 arg2);

    // ?Reset@mmCompBase@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?SetBltXY@mmCompBase@@UAEXMM@Z
    ARTS_IMPORT virtual void SetBltXY(f32 arg1, f32 arg2);

    // ?SetGeometry@mmCompBase@@UAEXMMMM@Z
    ARTS_IMPORT virtual void SetGeometry(f32 arg1, f32 arg2, f32 arg3, f32 arg4);

    // ?Switch@mmCompBase@@UAEXH@Z
    ARTS_IMPORT virtual void Switch(i32 arg1);

    // ?Update@mmCompBase@@UAEXXZ
    ARTS_IMPORT virtual void Update();
};

// ??_7mmCompBase@@6B@
// vtable at 0x005B8834
// check_size(mmCompBase, 0x0); // TODO: no layout in the IDB type library
