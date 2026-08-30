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

class mmCompDRecord : public mmCompBase
{
public:
    // ??0mmCompDRecord@@QAE@XZ
    ARTS_IMPORT mmCompDRecord();

    // ??1mmCompDRecord@@UAE@XZ
    ARTS_IMPORT virtual ~mmCompDRecord();

    // ?Cull@mmCompDRecord@@UAEXXZ
    ARTS_IMPORT virtual void Cull();

    // ?Update@mmCompDRecord@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@mmCompDRecord@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?DisableBlt@mmCompDRecord@@UAEXXZ
    ARTS_IMPORT virtual void DisableBlt();

    // ?SetBltXY@mmCompDRecord@@UAEXMM@Z
    ARTS_IMPORT virtual void SetBltXY(f32 arg1, f32 arg2);

    // ?SetGeometry@mmCompDRecord@@UAEXMMMM@Z
    ARTS_IMPORT virtual void SetGeometry(f32 arg1, f32 arg2, f32 arg3, f32 arg4);

    // ?SetPosition@mmCompDRecord@@UAEXPAVmmTextNode@@HM@Z
    ARTS_IMPORT virtual void SetPosition(mmTextNode* arg0, i32 arg2, f32 arg3);

    // ?Init@mmCompDRecord@@QAEXHPADM0HHH@Z
    ARTS_IMPORT void Init(i32 arg1, char* arg2, f32 arg3, char* arg4, i32 arg5, i32 arg6, i32 arg7);

    // ?InitTitle@mmCompDRecord@@QAEXMMMMPAULocString@@0000H@Z
    ARTS_IMPORT void InitTitle(f32 arg1, f32 arg2, f32 arg3, f32 arg4, LocString* arg5, LocString* arg6, LocString* arg7, LocString* arg8, LocString* arg9, i32 arg10);

    // ?LoadBitmap@mmCompDRecord@@QAEXPAD@Z
    ARTS_IMPORT void LoadBitmap(char* name);

    // ?SetSubwidgetGeometry@mmCompDRecord@@QAEXXZ
    ARTS_IMPORT void SetSubwidgetGeometry();
};

// ??_7mmCompDRecord@@6B@
// vtable at 0x005B895C
// check_size(mmCompDRecord, 0x0); // TODO: no layout in the IDB type library
