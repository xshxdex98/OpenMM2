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

enum dgSkillLevel : i32;

class mmRaceData
{
public:
    // ??0mmRaceData@@QAE@XZ
    ARTS_IMPORT mmRaceData();

    // ??1mmRaceData@@QAE@XZ
    ARTS_IMPORT ~mmRaceData();

    // ?GetAmbientDensity@mmRaceData@@QAEMHW4dgSkillLevel@@@Z
    ARTS_IMPORT f32 GetAmbientDensity(i32 arg1, dgSkillLevel arg2);

    // ?GetCarType@mmRaceData@@QAEHHW4dgSkillLevel@@@Z
    ARTS_IMPORT i32 GetCarType(i32 arg1, dgSkillLevel arg2);

    // ?GetDifficulty@mmRaceData@@QAEMHW4dgSkillLevel@@@Z
    ARTS_IMPORT f32 GetDifficulty(i32 arg1, dgSkillLevel arg2);

    // ?GetNumCops@mmRaceData@@QAEHHW4dgSkillLevel@@@Z
    ARTS_IMPORT i32 GetNumCops(i32 arg1, dgSkillLevel arg2);

    // ?GetNumLaps@mmRaceData@@QAEHHW4dgSkillLevel@@@Z
    ARTS_IMPORT i32 GetNumLaps(i32 arg1, dgSkillLevel arg2);

    // ?GetNumOpponents@mmRaceData@@QAEHHW4dgSkillLevel@@@Z
    ARTS_IMPORT i32 GetNumOpponents(i32 arg1, dgSkillLevel arg2);

    // ?GetNumRaces@mmRaceData@@QAEHXZ
    ARTS_IMPORT i32 GetNumRaces();

    // ?GetPedDensity@mmRaceData@@QAEMHW4dgSkillLevel@@@Z
    ARTS_IMPORT f32 GetPedDensity(i32 arg1, dgSkillLevel arg2);

    // ?GetRaceName@mmRaceData@@QAEPADH@Z
    ARTS_IMPORT char* GetRaceName(i32 arg1);

    // ?GetTimeLimit@mmRaceData@@QAEMHW4dgSkillLevel@@@Z
    ARTS_IMPORT f32 GetTimeLimit(i32 arg1, dgSkillLevel arg2);

    // ?GetTimeOfDay@mmRaceData@@QAEHHW4dgSkillLevel@@@Z
    ARTS_IMPORT i32 GetTimeOfDay(i32 arg1, dgSkillLevel arg2);

    // ?GetWeather@mmRaceData@@QAEHHW4dgSkillLevel@@@Z
    ARTS_IMPORT i32 GetWeather(i32 arg1, dgSkillLevel arg2);

    // ?Load@mmRaceData@@QAEHPAD0@Z
    ARTS_IMPORT i32 Load(char* getter, char* transformer);
};

// check_size(mmRaceData, 0x0); // TODO: no layout in the IDB type library
