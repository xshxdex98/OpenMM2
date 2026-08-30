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

#include "misc/base.h"

class aiCTFRacer : public Base
{
public:
    // ??0aiCTFRacer@@QAE@XZ
    ARTS_IMPORT aiCTFRacer();

    // ??1aiCTFRacer@@UAE@XZ
    ARTS_IMPORT virtual ~aiCTFRacer();

    // ?DrawRouteThroughTraffic@aiCTFRacer@@QAEXXZ
    ARTS_IMPORT void DrawRouteThroughTraffic();

    // ?DropFlag@aiCTFRacer@@QAEXHPBM@Z
    ARTS_IMPORT void DropFlag(i32 arg1, const f32* arg2);

    // ?Init@aiCTFRacer@@QAEXHPAD@Z
    ARTS_IMPORT void Init(i32 arg1, char* arg2);

    // ?PickupFlag@aiCTFRacer@@QAEXXZ
    ARTS_IMPORT void PickupFlag();

    // ?ReleaseFlag@aiCTFRacer@@QAEXXZ
    ARTS_IMPORT void ReleaseFlag();

    // ?Reset@aiCTFRacer@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?Update@aiCTFRacer@@QAEXXZ
    ARTS_IMPORT void Update();

private:
    // ?AquireFlag@aiCTFRacer@@AAEXXZ
    ARTS_IMPORT void AquireFlag();

    // ?DeliverFlag@aiCTFRacer@@AAEXXZ
    ARTS_IMPORT void DeliverFlag();

    // ?PreAquireFlag@aiCTFRacer@@AAEXXZ
    ARTS_IMPORT void PreAquireFlag();

    // ?PreDeliverFlag@aiCTFRacer@@AAEXXZ
    ARTS_IMPORT void PreDeliverFlag();
};

// check_size(aiCTFRacer, 0x9878); // size known, members are not - cannot verify
