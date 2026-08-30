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

#include "pumenubase.h"

class Dialog_HallOfFame : public PUMenuBase
{
public:
    // ??0Dialog_HallOfFame@@QAE@HMMMMPAD@Z
    ARTS_IMPORT Dialog_HallOfFame(i32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, char* arg6);

    // ??1Dialog_HallOfFame@@UAE@XZ
    ARTS_IMPORT virtual ~Dialog_HallOfFame();

    // ?PreSetup@Dialog_HallOfFame@@UAEXXZ
    ARTS_IMPORT virtual void PreSetup();

    // ?AddRaceRecord@Dialog_HallOfFame@@QAEXHPAD00H00@Z
    ARTS_IMPORT void AddRaceRecord(i32 arg1, char* arg2, char* arg3, char* arg4, i32 arg5, char* arg6, char* arg7);

    // ?InitRaceRecord@Dialog_HallOfFame@@QAEXXZ
    ARTS_IMPORT void InitRaceRecord();

    // ?ResetRaceRecord@Dialog_HallOfFame@@QAEXXZ
    ARTS_IMPORT void ResetRaceRecord();

    // ?SetSortState@Dialog_HallOfFame@@QAEXXZ
    ARTS_IMPORT void SetSortState();

    // ?SortByCity@Dialog_HallOfFame@@QAEXXZ
    ARTS_IMPORT void SortByCity();
};

// ??_7Dialog_HallOfFame@@6B@
// vtable at 0x005B4000
// check_size(Dialog_HallOfFame, 0xF0); // size known, members are not - cannot verify
