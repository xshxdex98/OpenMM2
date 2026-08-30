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

class datTimeManager
{
public:
    enum EnumTimeMode : i32;

    enum EnumTimeMode : i32;

    // ?FixedFrame@datTimeManager@@SAXMI@Z
    ARTS_IMPORT static void FixedFrame(f32 arg1, u32 arg2);

    // ?RealTime@datTimeManager@@SAXM@Z
    ARTS_IMPORT static void RealTime(f32 arg1);

    // ?Reset@datTimeManager@@SAXXZ
    ARTS_IMPORT static void Reset();

    // ?SetTempOverSampling@datTimeManager@@SAX_NH@Z
    ARTS_IMPORT static void SetTempOverSampling(bool arg1, i32 arg2);

    // ?Update@datTimeManager@@SAXXZ
    ARTS_IMPORT static void Update();

public:
    static f32& Access_Seconds() { return Seconds; }
    static f32& Access_ActualSeconds() { return ActualSeconds; }
    static f32& Access_InvSeconds() { return InvSeconds; }
    static f32& Access_FPS() { return FPS; }
    static f32& Access_SampleStep() { return SampleStep; }
    static f32& Access_ClampMax() { return ClampMax; }
    static f32& Access_ClampMin() { return ClampMin; }
    static u32& Access_OverSamples() { return OverSamples; }
    static bool& Access_FirstFrame() { return FirstFrame; }
    static f32& Access_PrevElapsedTime() { return PrevElapsedTime; }
    static f32& Access_ElapsedTime() { return ElapsedTime; }
    static i32& Access_TempOverSampleAmount() { return TempOverSampleAmount; }
    static Timer& Access_Time() { return Time; }
    static f32& Access_TempSeconds() { return TempSeconds; }
    static u32& Access_FrameCount() { return FrameCount; }
    static bool& Access_ShowFrame() { return ShowFrame; }
    static bool& Access_FrameStep() { return FrameStep; }
    static datTimeManager::EnumTimeMode& Access_Mode() { return Mode; }
    static bool& Access_TempOverSampling() { return TempOverSampling; }

private:
    // ?Seconds@datTimeManager@@0MA
    ARTS_IMPORT static f32 Seconds;

    // ?ActualSeconds@datTimeManager@@0MA
    ARTS_IMPORT static f32 ActualSeconds;

    // ?InvSeconds@datTimeManager@@0MA
    ARTS_IMPORT static f32 InvSeconds;

    // ?FPS@datTimeManager@@0MA
    ARTS_IMPORT static f32 FPS;

    // ?SampleStep@datTimeManager@@0MA
    ARTS_IMPORT static f32 SampleStep;

    // ?ClampMax@datTimeManager@@0MA
    ARTS_IMPORT static f32 ClampMax;

    // ?ClampMin@datTimeManager@@0MA
    ARTS_IMPORT static f32 ClampMin;

    // ?OverSamples@datTimeManager@@0IA
    ARTS_IMPORT static u32 OverSamples;

    // ?FirstFrame@datTimeManager@@0_NA
    ARTS_IMPORT static bool FirstFrame;

    // ?PrevElapsedTime@datTimeManager@@0MA
    ARTS_IMPORT static f32 PrevElapsedTime;

    // ?ElapsedTime@datTimeManager@@0MA
    ARTS_IMPORT static f32 ElapsedTime;

    // ?TempOverSampleAmount@datTimeManager@@0HA
    ARTS_IMPORT static i32 TempOverSampleAmount;

    // ?Time@datTimeManager@@0VTimer@@A
    ARTS_IMPORT static Timer Time;

    // ?TempSeconds@datTimeManager@@0MA
    ARTS_IMPORT static f32 TempSeconds;

    // ?FrameCount@datTimeManager@@0IA
    ARTS_IMPORT static u32 FrameCount;

    // ?ShowFrame@datTimeManager@@0_NA
    ARTS_IMPORT static bool ShowFrame;

    // ?FrameStep@datTimeManager@@0_NA
    ARTS_IMPORT static bool FrameStep;

    // ?Mode@datTimeManager@@0W4EnumTimeMode@1@A
    ARTS_IMPORT static datTimeManager::EnumTimeMode Mode;

    // ?TempOverSampling@datTimeManager@@0_NA
    ARTS_IMPORT static bool TempOverSampling;
};

// check_size(datTimeManager, 0x1); // size known, members are not - cannot verify
