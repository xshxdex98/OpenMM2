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

class mmCompRoster : public mmCompBase
{
public:
    // ??0mmCompRoster@@QAE@XZ
    ARTS_IMPORT mmCompRoster();

    // ??1mmCompRoster@@UAE@XZ
    ARTS_IMPORT virtual ~mmCompRoster();

    // ?Cull@mmCompRoster@@UAEXXZ
    ARTS_IMPORT virtual void Cull();

    // ?Update@mmCompRoster@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@mmCompRoster@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?SetBltXY@mmCompRoster@@UAEXMM@Z
    ARTS_IMPORT virtual void SetBltXY(f32 arg1, f32 arg2);

    // ?SetGeometry@mmCompRoster@@UAEXMMMM@Z
    ARTS_IMPORT virtual void SetGeometry(f32 arg1, f32 arg2, f32 arg3, f32 arg4);

    // ?SetPosition@mmCompRoster@@UAEXPAVmmTextNode@@HM@Z
    ARTS_IMPORT virtual void SetPosition(mmTextNode* arg0, i32 arg2, f32 arg3);

    // ?Init@mmCompRoster@@QAEXPAD00KHH@Z
    ARTS_IMPORT void Init(char* arg1, char* arg2, char* arg3, ulong arg4, i32 arg5, i32 arg6);

    // ?InitTitle@mmCompRoster@@QAEXMMMMPAULocString@@000H@Z
    ARTS_IMPORT void InitTitle(f32 arg1, f32 arg2, f32 arg3, f32 arg4, LocString* arg5, LocString* arg6, LocString* arg7, LocString* arg8, i32 arg9);

    // ?LoadBitmap@mmCompRoster@@QAEXPAD@Z
    ARTS_IMPORT void LoadBitmap(char* name);

    // ?LoadTeamBitmap@mmCompRoster@@QAEXPAD@Z
    ARTS_IMPORT void LoadTeamBitmap(char* name);

    // ?SetCar@mmCompRoster@@QAEXPAD@Z
    ARTS_IMPORT void SetCar(char* arg1);

    // ?SetColor@mmCompRoster@@QAEXPAD@Z
    ARTS_IMPORT void SetColor(char* arg1);

    // ?SetError@mmCompRoster@@QAEXXZ
    ARTS_IMPORT void SetError();

    // ?SetName@mmCompRoster@@QAEXPAD@Z
    ARTS_IMPORT void SetName(char* arg1);

    // ?SetReady@mmCompRoster@@QAEXH@Z
    ARTS_IMPORT void SetReady(i32 readyStatus);

    // ?SetSubwidgetGeometry@mmCompRoster@@QAEXXZ
    ARTS_IMPORT void SetSubwidgetGeometry();

    // ?SetTeam@mmCompRoster@@QAEXH@Z
    ARTS_IMPORT void SetTeam(i32 arg1);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 field_18[100]; // 0x018
    i32 field_7C; // 0x07C
    u8 field_80[12]; // 0x080
    i32 field_8C; // 0x08C
    u8 field_90[8]; // 0x090
};

// ??_7mmCompRoster@@6B@
// vtable at 0x005B89BC
check_size(mmCompRoster, 0x98);
