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

struct LocString;
class UIMenu;
class mmTextNode;

class mmToolTip : public asNode
{
public:
    // ??0mmToolTip@@QAE@XZ
    ARTS_IMPORT mmToolTip();

    // ??1mmToolTip@@UAE@XZ
    ARTS_IMPORT virtual ~mmToolTip();

    // ?Update@mmToolTip@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Init@mmToolTip@@QAEXPAVUIMenu@@PAULocString@@@Z
    ARTS_IMPORT void Init(UIMenu* arg1, LocString* arg2);

    // ?SetText@mmToolTip@@QAEXPAULocString@@@Z
    ARTS_IMPORT void SetText(LocString* arg1);

    // ?Switch@mmToolTip@@QAEXH@Z
    ARTS_IMPORT void Switch(i32 arg1);

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    UIMenu* pParent; // 0x018
    mmTextNode* pText; // 0x01C
    u32 field_20; // 0x020
    Vector2 Size; // 0x024
};

// ??_7mmToolTip@@6B@
// vtable at 0x005B3B64
check_size(mmToolTip, 0x2C);
