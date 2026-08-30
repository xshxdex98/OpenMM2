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

class mmCarRoadFF : public asNode
{
public:
    // ??0mmCarRoadFF@@QAE@XZ
    ARTS_IMPORT mmCarRoadFF();

    // ??1mmCarRoadFF@@UAE@XZ
    ARTS_IMPORT virtual ~mmCarRoadFF();

    // ?Update@mmCarRoadFF@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@mmCarRoadFF@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?AssignProperties@mmCarRoadFF@@QAEXMH@Z
    ARTS_IMPORT void AssignProperties(f32 arg1, i32 arg2);

    // ?IsPlaying@mmCarRoadFF@@QAEHXZ
    ARTS_IMPORT i32 IsPlaying();

    // ?SetFGVals@mmCarRoadFF@@QAEXMMM@Z
    ARTS_IMPORT void SetFGVals(f32 ctx, f32 window, f32 arg3);

    // ?Start@mmCarRoadFF@@QAEXXZ
    ARTS_IMPORT void Start();

    // ?Stop@mmCarRoadFF@@QAEXXZ
    ARTS_IMPORT void Stop();

    // ?UpdateVals@mmCarRoadFF@@QAEXXZ
    ARTS_IMPORT void UpdateVals();
};

// ??_7mmCarRoadFF@@6B@
// vtable at 0x005B0D38
// check_size(mmCarRoadFF, 0x18); // size known, members are not - cannot verify
