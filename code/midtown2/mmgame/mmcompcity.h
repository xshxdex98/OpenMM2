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

#include "mmcompbase.h"

struct LocString;
class mmTextNode;

class mmCompCity : public mmCompBase
{
public:
    // ??0mmCompCity@@QAE@XZ
    ARTS_IMPORT mmCompCity();

    // ??1mmCompCity@@UAE@XZ
    ARTS_IMPORT virtual ~mmCompCity();

    // ?Cull@mmCompCity@@UAEXXZ
    ARTS_IMPORT virtual void Cull();

    // ?Update@mmCompCity@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@mmCompCity@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Highlight@mmCompCity@@UAEXPAVmmTextNode@@H@Z
    ARTS_IMPORT virtual void Highlight(mmTextNode* arg1, i32 index);

    // ?Box@mmCompCity@@UAEXHPAVmmTextNode@@H@Z
    ARTS_IMPORT virtual void Box(i32 arg1, mmTextNode* arg2, i32 index);

    // ?SetGeometry@mmCompCity@@UAEXMMMM@Z
    ARTS_IMPORT virtual void SetGeometry(f32 arg1, f32 arg2, f32 arg3, f32 arg4);

    // ?SetPosition@mmCompCity@@UAEXPAVmmTextNode@@HM@Z
    ARTS_IMPORT virtual void SetPosition(mmTextNode* arg1, i32 arg2, f32 arg3);

    // ?Init@mmCompCity@@QAEXPADHHH@Z
    ARTS_IMPORT void Init(char* arg1, i32 arg2, i32 arg3, i32 arg4);

    // ?InitTitle@mmCompCity@@QAEXMMMMPAULocString@@000@Z
    ARTS_IMPORT void InitTitle(f32 arg1, f32 arg2, f32 arg3, f32 arg4, LocString* arg5, LocString* arg6, LocString* arg7, LocString* arg8);

    // ?SetSubwidgetGeometry@mmCompCity@@QAEXXZ
    ARTS_IMPORT void SetSubwidgetGeometry();

    // ?SetTitleGeometry@mmCompCity@@QAEXXZ
    ARTS_IMPORT void SetTitleGeometry();
};

// ??_7mmCompCity@@6B@
// vtable at 0x005B87D4
// check_size(mmCompCity, 0x0); // TODO: no layout in the IDB type library
