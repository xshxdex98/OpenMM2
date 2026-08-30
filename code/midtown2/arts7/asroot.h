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

#include "vector7/matrix34.h"

#include "asnode.h"

class asRoot : public asNode
{
public:
    // ??0asRoot@@QAE@XZ
    ARTS_IMPORT asRoot();

    // ??1asRoot@@UAE@XZ
    ARTS_IMPORT virtual ~asRoot();

    // ?Update@asRoot@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@asRoot@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?GetClassName@asRoot@@UAEPADXZ
    ARTS_IMPORT virtual char* GetClassName();

    // ?Init@asRoot@@QAEX_N@Z
    ARTS_IMPORT void Init(bool nanSignal);

    // ?IsPaused@asRoot@@QAE_NXZ
    ARTS_IMPORT bool IsPaused();

    // ?Quit@asRoot@@QAEXXZ
    ARTS_IMPORT void Quit();

    // ?SetPause@asRoot@@QAEX_N@Z
    ARTS_IMPORT void SetPause(bool arg1);

    // ?TogglePause@asRoot@@QAEXXZ
    ARTS_IMPORT void TogglePause();

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    Matrix34 Matrix; // 0x018
    i8 Paused; // 0x048
    i8 ShouldPause; // 0x049
    i8 EnableNan; // 0x04A
};

// ??_7asRoot@@6B@
// vtable at 0x005B28B4
check_size(asRoot, 0x4C);
