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

class asCamera;

class Card2D : public asNode
{
public:
    // ??0Card2D@@QAE@XZ
    ARTS_IMPORT Card2D();

    // ??1Card2D@@UAE@XZ
    ARTS_IMPORT virtual ~Card2D();

    // ?Init@Card2D@@QAEXPAVasCamera@@MMMMM@Z
    ARTS_IMPORT void Init(asCamera* arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6);

    // ?SetAlpha@Card2D@@QAEXM@Z
    ARTS_IMPORT void SetAlpha(f32 arg1);

    // ?SetDimensions@Card2D@@QAEXMMMM@Z
    ARTS_IMPORT void SetDimensions(f32 arg1, f32 arg2, f32 arg3, f32 arg4);

    // ?SetPosition@Card2D@@QAEXMM@Z
    ARTS_IMPORT void SetPosition(f32 arg1, f32 arg2);

    // ?Update@Card2D@@UAEXXZ
    ARTS_IMPORT virtual void Update();

private:
    // ?Cull@Card2D@@EAEXXZ
    ARTS_IMPORT virtual void Cull();

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    Vector2 Pos; // 0x018
    Vector2 Size; // 0x020
    u8 R; // 0x028
    u8 G; // 0x029
    u8 B; // 0x02A
    u8 A; // 0x02B
};

// ??_7Card2D@@6B@
// vtable at 0x005B8600
check_size(Card2D, 0x2C);
