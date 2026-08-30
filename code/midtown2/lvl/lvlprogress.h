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

class lvlProgress
{
public:
    // ?BeginTask@lvlProgress@@SAXPBD@Z
    ARTS_IMPORT static void BeginTask(const char* arg1);

    // ?EndTask@lvlProgress@@SAXXZ
    ARTS_IMPORT static void EndTask();

    // ?GetPercent@lvlProgress@@SAMXZ
    ARTS_IMPORT static f32 GetPercent();

    // ?PopSubTask@lvlProgress@@SAMXZ
    ARTS_IMPORT static f32 PopSubTask();

    // ?PushSubTask@lvlProgress@@SAXMMPAD@Z
    ARTS_IMPORT static void PushSubTask(f32 arg1, f32 arg2, char* arg3);

    // ?PushSubTask@lvlProgress@@SAXMPAD@Z
    ARTS_IMPORT static void PushSubTask(f32 arg1, char* arg2);

    // ?SetCallback@lvlProgress@@SAXP6AXPBDH@Z@Z
    ARTS_IMPORT static void SetCallback(void (__cdecl* arg1)(const char*,i32));

    // ?UpdateTask@lvlProgress@@SAXM_N@Z
    ARTS_IMPORT static void UpdateTask(f32 arg1, bool arg2);

public:
    static char**& Access_sm_SubTaskName() { return sm_SubTaskName; }

private:
    // ?sm_SubTaskName@lvlProgress@@0PAPADA
    ARTS_IMPORT static char** sm_SubTaskName;
};

// check_size(lvlProgress, 0x0); // TODO: no layout in the IDB type library
