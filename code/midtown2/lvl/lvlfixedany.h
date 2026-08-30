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

#include "lvlinstance.h"

class Matrix34;

class lvlFixedAny : public lvlInstance
{
public:
    // ??0lvlFixedAny@@QAE@XZ
    ARTS_IMPORT lvlFixedAny();

    // ?Draw@lvlFixedAny@@UAEXH@Z
    ARTS_IMPORT virtual void Draw(i32 arg1);

    // ?DrawReflectedParts@lvlFixedAny@@UAEXH@Z
    ARTS_IMPORT virtual void DrawReflectedParts(i32 arg1);

    // ?DrawShadow@lvlFixedAny@@UAEXXZ
    ARTS_IMPORT virtual void DrawShadow();

    // ?DrawShadowMap@lvlFixedAny@@UAEXXZ
    ARTS_IMPORT virtual void DrawShadowMap();

    // ?Init@lvlFixedAny@@UAEHPBDABVMatrix34@@H@Z
    ARTS_IMPORT virtual i32 Init(const char* arg1, const Matrix34& arg2, i32 arg3);

    // ?SetVariant@lvlFixedAny@@UAEXH@Z
    ARTS_IMPORT virtual void SetVariant(i32 arg1);
};

// ??_7lvlFixedAny@@6B@
// vtable at 0x005B1B10
// check_size(lvlFixedAny, 0x14); // size known, members are not - cannot verify
