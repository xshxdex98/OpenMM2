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

class datCallback;
class phColliderBase;
class phInertialCS;

class phSleep
{
public:
    // ??0phSleep@@QAE@PAVphInertialCS@@PAVphColliderBase@@@Z
    ARTS_IMPORT phSleep(phInertialCS* arg1, phColliderBase* arg2);

    // ??0phSleep@@QAE@XZ
    ARTS_IMPORT phSleep();

    // ??1phSleep@@QAE@XZ
    ARTS_IMPORT ~phSleep();

    // ?Init@phSleep@@QAEXPAVphInertialCS@@PAVphColliderBase@@@Z
    ARTS_IMPORT void Init(phInertialCS* arg1, phColliderBase* arg2);

    // ?Reset@phSleep@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?SendToSleep@phSleep@@QAEXXZ
    ARTS_IMPORT void SendToSleep();

    // ?SetAsleepCB@phSleep@@QAEXPAVdatCallback@@@Z
    ARTS_IMPORT void SetAsleepCB(datCallback* arg1);

    // ?SmoothAngInertia@phSleep@@SAXAAVphInertialCS@@M@Z
    ARTS_IMPORT static void SmoothAngInertia(phInertialCS& arg1, f32 arg2);

    // ?Update@phSleep@@QAEXXZ
    ARTS_IMPORT void Update();

    // ?WakeUp@phSleep@@QAEXXZ
    ARTS_IMPORT void WakeUp();

    // ?WakeUpNextTime@phSleep@@QAEXXZ
    ARTS_IMPORT void WakeUpNextTime();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i32 field_0; // 0x000
    i32 field_4; // 0x004
    u8 field_8; // 0x008
    u8 field_9[67]; // 0x009
    i32 field_4C; // 0x04C
};

check_size(phSleep, 0x50);
