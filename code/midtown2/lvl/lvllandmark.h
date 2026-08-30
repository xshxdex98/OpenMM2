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

#include "lvlfixedroty.h"

class Matrix34;
class gfxViewport;

class lvlLandmark : public lvlFixedRotY
{
public:
    // ?Init@lvlLandmark@@UAEHPBDABVMatrix34@@H@Z
    ARTS_IMPORT virtual i32 Init(const char* arg1, const Matrix34& arg2, i32 arg3);

    // ?IsCollidable@lvlLandmark@@UAE_NXZ
    ARTS_IMPORT virtual bool IsCollidable();

    // ?IsLandmark@lvlLandmark@@UAE_NXZ
    ARTS_IMPORT virtual bool IsLandmark();

    // ?IsVisible@lvlLandmark@@UAEHABVgfxViewport@@@Z
    ARTS_IMPORT virtual i32 IsVisible(const gfxViewport& arg1);

    // ?SizeOf@lvlLandmark@@UAEIXZ
    ARTS_IMPORT virtual u32 SizeOf();
};

// ??_7lvlLandmark@@6B@
// vtable at 0x005B1A88
// check_size(lvlLandmark, 0x28); // size known, members are not - cannot verify
