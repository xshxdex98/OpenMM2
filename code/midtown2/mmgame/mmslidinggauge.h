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

#include "mmlineargauge.h"

class mmExternalView;

class mmSlidingGauge : public mmLinearGauge
{
public:
    // ??1mmSlidingGauge@@QAE@XZ
    ARTS_IMPORT ~mmSlidingGauge();

    // ?Draw@mmSlidingGauge@@QAEXXZ
    ARTS_IMPORT void Draw();

    // ?Init@mmSlidingGauge@@QAEXPADPAM1HPAVmmExternalView@@H@Z
    ARTS_IMPORT void Init(char* arg1, f32* arg2, f32* arg3, i32 arg4, mmExternalView* arg5, i32 arg6);
};

// check_size(mmSlidingGauge, 0x24); // size known, members are not - cannot verify
