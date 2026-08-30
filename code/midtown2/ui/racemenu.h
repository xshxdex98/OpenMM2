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

#include "racemenubase.h"

enum dgGameMode : i32;

class RaceMenu : public RaceMenuBase
{
public:
    // ??0RaceMenu@@QAE@H@Z
    ARTS_IMPORT RaceMenu(i32 arg1);

    // ??1RaceMenu@@UAE@XZ
    ARTS_IMPORT virtual ~RaceMenu();

    // ?GetRaceID@RaceMenu@@QAEHXZ
    ARTS_IMPORT i32 GetRaceID();

    // ?GetRaceName@RaceMenu@@QAEPADW4dgGameMode@@H@Z
    ARTS_IMPORT char* GetRaceName(dgGameMode arg1, i32 arg2);

    // ?SetBlitzMask@RaceMenu@@QAEXH@Z
    ARTS_IMPORT void SetBlitzMask(i32 arg1);

    // ?SetCheckpointMask@RaceMenu@@QAEXH@Z
    ARTS_IMPORT void SetCheckpointMask(i32 arg1);

    // ?SetCircuitMask@RaceMenu@@QAEXH@Z
    ARTS_IMPORT void SetCircuitMask(i32 arg1);

    // ?SetProgressMask@RaceMenu@@QAEXH@Z
    ARTS_IMPORT void SetProgressMask(i32 arg1);
};

// ??_7RaceMenu@@6B@
// vtable at 0x005B44A4
// check_size(RaceMenu, 0x138); // size known, members are not - cannot verify
