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

struct LocString;

class PUResults : public PUMenuBase
{
public:
    // ??0PUResults@@QAE@HMMMMPADHH@Z
    ARTS_IMPORT PUResults(i32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, char* arg6, i32 arg7, i32 arg8);

    // ??1PUResults@@UAE@XZ
    ARTS_IMPORT virtual ~PUResults();

    // ?AddLoser@PUResults@@QAEXHPBD@Z
    ARTS_IMPORT void AddLoser(i32 arg1, const char* arg2);

    // ?AddName@PUResults@@QAEXHPBD0@Z
    ARTS_IMPORT void AddName(i32 arg1, const char* arg2, const char* arg3);

    // ?AddName@PUResults@@QAEXHPBDH@Z
    ARTS_IMPORT void AddName(i32 arg1, const char* arg2, i32 arg3);

    // ?AddName@PUResults@@QAEXHPBDM@Z
    ARTS_IMPORT void AddName(i32 arg1, const char* arg2, f32 arg3);

    // ?AddTitle@PUResults@@QAEXPAULocString@@0@Z
    ARTS_IMPORT void AddTitle(LocString* arg1, LocString* arg2);

    // ?ClearNames@PUResults@@QAEXXZ
    ARTS_IMPORT void ClearNames();

    // ?DisableNextRace@PUResults@@QAEXXZ
    ARTS_IMPORT void DisableNextRace();

    // ?EnableNextRace@PUResults@@QAEXXZ
    ARTS_IMPORT void EnableNextRace();

    // ?Init320@PUResults@@QAEXXZ
    ARTS_IMPORT void Init320();

    // ?Init640@PUResults@@QAEXXZ
    ARTS_IMPORT void Init640();

    // ?IsRaceMenuReadOnly@PUResults@@QAEHXZ
    ARTS_IMPORT i32 IsRaceMenuReadOnly();

    // ?IsRosterReadOnly@PUResults@@QAEHXZ
    ARTS_IMPORT i32 IsRosterReadOnly();

    // ?RaceMenuRO@PUResults@@QAEXH@Z
    ARTS_IMPORT void RaceMenuRO(i32 arg1);

    // ?Reset@PUResults@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?RestartRO@PUResults@@QAEXH@Z
    ARTS_IMPORT void RestartRO(i32 arg1);

    // ?RosterRO@PUResults@@QAEXH@Z
    ARTS_IMPORT void RosterRO(i32 arg1);

    // ?SetMessage@PUResults@@QAEXPAULocString@@@Z
    ARTS_IMPORT void SetMessage(LocString* arg1);
};

// ??_7PUResults@@6B@
// vtable at 0x005B4680
// check_size(PUResults, 0x0); // TODO: no layout in the IDB type library
