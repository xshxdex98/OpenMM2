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

class Stream;
union eqEvent;

class mmIO
{
public:
    // ??0mmIO@@QAE@XZ
    ARTS_IMPORT mmIO();

    // ??1mmIO@@QAE@XZ
    ARTS_IMPORT ~mmIO();

    // ?Assign@mmIO@@QAEXHH@Z
    ARTS_IMPORT void Assign(i32 arg1, i32 arg2);

    // ?Clear@mmIO@@QAEXXZ
    ARTS_IMPORT void Clear();

    // ?CompareComponent@mmIO@@QAEHH@Z
    ARTS_IMPORT i32 CompareComponent(i32 getter1);

    // ?ForceSettingAcrossConfigs@mmIO@@QAEXH@Z
    ARTS_IMPORT void ForceSettingAcrossConfigs(i32 arg1);

    // ?GetDescription@mmIO@@QAEXPAD@Z
    ARTS_IMPORT void GetDescription(char* arg1);

    // ?Init@mmIO@@QAEXPADHJ@Z
    ARTS_IMPORT void Init(char* getter1, i32 getter2, ilong transformer);

    // ?InitDev@mmIO@@QAEXH_JHHH@Z
    ARTS_IMPORT void InitDev(i32 arg1, i64 arg2, i32 arg3, i32 arg4, i32 arg5);

    // ??8mmIO@@QAEHPATeqEvent@@@Z
    ARTS_IMPORT i32 operator==(eqEvent* arg1);

    // ?Read@mmIO@@QAEHPAVStream@@@Z
    ARTS_IMPORT i32 Read(Stream* getter1);

    // ?Write@mmIO@@QAEHPAVStream@@@Z
    ARTS_IMPORT i32 Write(Stream* getter1);
};

// check_size(mmIO, 0x0); // TODO: no layout in the IDB type library
