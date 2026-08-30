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

#include "uimenu.h"

class CrashCourse : public UIMenu
{
public:
    // ??0CrashCourse@@QAE@H@Z
    ARTS_IMPORT CrashCourse(i32 arg1);

    // ??1CrashCourse@@UAE@XZ
    ARTS_IMPORT virtual ~CrashCourse();

    // ?PreSetup@CrashCourse@@UAEXXZ
    ARTS_IMPORT virtual void PreSetup();

    // ?ChangeLocalVals@CrashCourse@@QAEXXZ
    ARTS_IMPORT void ChangeLocalVals();

    // ?DecRaceName@CrashCourse@@QAEXXZ
    ARTS_IMPORT void DecRaceName();

    // ?FocusDescription@CrashCourse@@QAEXHH@Z
    ARTS_IMPORT void FocusDescription(i32 arg1, i32 arg2);

    // ?GameCallback@CrashCourse@@QAEXXZ
    ARTS_IMPORT void GameCallback();

    // ?IncRaceName@CrashCourse@@QAEXXZ
    ARTS_IMPORT void IncRaceName();

    // ?SetBlitzMask@CrashCourse@@QAEXH@Z
    ARTS_IMPORT void SetBlitzMask(i32 arg1);

    // ?SetCheckpointMask@CrashCourse@@QAEXH@Z
    ARTS_IMPORT void SetCheckpointMask(i32 arg1);

    // ?SetEnvironment@CrashCourse@@QAEXXZ
    ARTS_IMPORT void SetEnvironment();

    // ?SetProgressMask@CrashCourse@@QAEXH@Z
    ARTS_IMPORT void SetProgressMask(i32 arg1);

    // ?SetRaceGrade@CrashCourse@@QAEXHH@Z
    ARTS_IMPORT void SetRaceGrade(i32 arg1, i32 arg2);

    // ?SetRaceState@CrashCourse@@QAEXXZ
    ARTS_IMPORT void SetRaceState();

    // ?SetVehicleNext@CrashCourse@@QAEX_N@Z
    ARTS_IMPORT void SetVehicleNext(bool arg1);
};

// ??_7CrashCourse@@6B@
// vtable at 0x005B4410
// check_size(CrashCourse, 0x850); // size known, members are not - cannot verify
