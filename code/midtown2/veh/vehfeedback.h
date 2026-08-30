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

#include "misc/timer.h"

class ioPad;

class vehFeedback
{
public:
    // ??0vehFeedback@@QAE@XZ
    ARTS_IMPORT vehFeedback();

    // ??1vehFeedback@@UAE@XZ
    ARTS_IMPORT virtual ~vehFeedback();

    // ?ClearAllSamples@vehFeedback@@UAEH_N@Z
    ARTS_IMPORT virtual i32 ClearAllSamples(bool arg1);

    // ?GetNextUnit@vehFeedback@@UAEMH@Z
    ARTS_IMPORT virtual f32 GetNextUnit(i32 arg1);

    // ?GetNumActuators@vehFeedback@@UAEHXZ
    ARTS_IMPORT virtual i32 GetNumActuators();

    // ?GetPad@vehFeedback@@QAEPAVioPad@@XZ
    ARTS_IMPORT ioPad* GetPad();

    // ?GetPadID@vehFeedback@@QAEHXZ
    ARTS_IMPORT i32 GetPadID();

    // ?PlayFeedbackSample@vehFeedback@@UAEHHHPAMPAH@Z
    ARTS_IMPORT virtual i32 PlayFeedbackSample(i32 arg1, i32 arg2, f32* arg3, i32* arg4);

    // ?PlayFeedbackSampleID@vehFeedback@@UAEHH@Z
    ARTS_IMPORT virtual i32 PlayFeedbackSampleID(i32 arg1);

    // ?SetActuatorValue@vehFeedback@@UAEHHM@Z
    ARTS_IMPORT virtual i32 SetActuatorValue(i32 arg1, f32 arg2);

    // ?SetFeedback@vehFeedback@@UAEH_N@Z
    ARTS_IMPORT virtual i32 SetFeedback(bool arg1);

    // ?SetPad@vehFeedback@@QAEXPAVioPad@@@Z
    ARTS_IMPORT void SetPad(ioPad* arg1);

    // ?SetPadID@vehFeedback@@QAEXH@Z
    ARTS_IMPORT void SetPadID(i32 arg1);

    // ?SetTimingUnit@vehFeedback@@UAEHM@Z
    ARTS_IMPORT virtual i32 SetTimingUnit(f32 arg1);

    // ?Update@vehFeedback@@UAEHXZ
    ARTS_IMPORT virtual i32 Update();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u32 field_4; // 0x004
    u32 field_8; // 0x008
    f32 field_C; // 0x00C
    Timer Timers[2]; // 0x010
    u32 field_18; // 0x018
    u32 field_1C; // 0x01C
    f32 field_20; // 0x020
    u8 field_24[128]; // 0x024
    u32 field_A4; // 0x0A4
    u8 field_A8[128]; // 0x0A8
    u32 field_128; // 0x128
};

// ??_7vehFeedback@@6B@
// vtable at 0x005B2E5C
check_size(vehFeedback, 0x12C);
