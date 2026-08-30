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

class mmRewardRecord;

class mmRewardList
{
public:
    // ??0mmRewardList@@QAE@XZ
    ARTS_IMPORT mmRewardList();

    // ??1mmRewardList@@QAE@XZ
    ARTS_IMPORT ~mmRewardList();

    // ?CheckReward@mmRewardList@@SA_NPADHPAVmmRewardRecord@@@Z
    ARTS_IMPORT static bool CheckReward(char* renderer, i32 DrawList, mmRewardRecord* cull_rect);

    // ?GetRecord@mmRewardList@@QAEPAVmmRewardRecord@@H@Z
    ARTS_IMPORT mmRewardRecord* GetRecord(i32 arg1);

    // ?Init@mmRewardList@@QAEXH@Z
    ARTS_IMPORT void Init(i32 arg1);

    // ?Load@mmRewardList@@QAE_NPAD00@Z
    ARTS_IMPORT bool Load(char* renderer, char* DrawList, char* cull_rect);

    // ?UnlockPlayerRewards@mmRewardList@@SAXPAD@Z
    ARTS_IMPORT static void UnlockPlayerRewards(char* renderer);
};

// check_size(mmRewardList, 0x0); // TODO: no layout in the IDB type library
