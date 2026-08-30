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

class AudSoundBase;
class Stream;

class vehSurfaceAudioData
{
public:
    // ??0vehSurfaceAudioData@@QAE@XZ
    ARTS_IMPORT vehSurfaceAudioData();

    // ??1vehSurfaceAudioData@@UAE@XZ
    ARTS_IMPORT virtual ~vehSurfaceAudioData();

    // ?AllocateSkidSamples@vehSurfaceAudioData@@QAEXH@Z
    ARTS_IMPORT void AllocateSkidSamples(i32 arg1);

    // ?AssignSounds@vehSurfaceAudioData@@QAEXH@Z
    ARTS_IMPORT void AssignSounds(i32 arg1);

    // ?EchoOff@vehSurfaceAudioData@@QAEXXZ
    ARTS_IMPORT void EchoOff();

    // ?EchoOn@vehSurfaceAudioData@@QAEXM@Z
    ARTS_IMPORT void EchoOn(f32 arg1);

    // ?GetSurfaceSoundPtr@vehSurfaceAudioData@@QAEPAVAudSoundBase@@XZ
    ARTS_IMPORT AudSoundBase* GetSurfaceSoundPtr();

    // ?ParseCSVBuffer@vehSurfaceAudioData@@QAE_NPAVStream@@@Z
    ARTS_IMPORT bool ParseCSVBuffer(Stream* arg1);

    // ?SetPan@vehSurfaceAudioData@@QAEXM@Z
    ARTS_IMPORT void SetPan(f32 arg1);

    // ?SkidPlaying@vehSurfaceAudioData@@QAEHXZ
    ARTS_IMPORT i32 SkidPlaying();

    // ?StopSkid@vehSurfaceAudioData@@QAEXXZ
    ARTS_IMPORT void StopSkid();

    // ?StopSurface@vehSurfaceAudioData@@QAEXXZ
    ARTS_IMPORT void StopSurface();

    // ?SurfacePlaying@vehSurfaceAudioData@@QAEHXZ
    ARTS_IMPORT i32 SurfacePlaying();

    // ?UnAssignSounds@vehSurfaceAudioData@@QAEXXZ
    ARTS_IMPORT void UnAssignSounds();

    // ?UpdateSkid@vehSurfaceAudioData@@QAEXM@Z
    ARTS_IMPORT void UpdateSkid(f32 arg1);

    // ?UpdateSkid@vehSurfaceAudioData@@QAEXMMM@Z
    ARTS_IMPORT void UpdateSkid(f32 arg1, f32 arg2, f32 arg3);

    // ?UpdateSurface@vehSurfaceAudioData@@QAEXM@Z
    ARTS_IMPORT void UpdateSurface(f32 arg1);

    // ?UpdateSurface@vehSurfaceAudioData@@QAEXMMM@Z
    ARTS_IMPORT void UpdateSurface(f32 arg1, f32 arg2, f32 arg3);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i32 field_0; // 0x000
    i32 field_4; // 0x004
    u8 field_8[4]; // 0x008
    i32 field_C; // 0x00C
    u8 field_10[4]; // 0x010
    i32 field_14; // 0x014
    i32 field_18; // 0x018
    i32 field_1C; // 0x01C
    i32 field_20; // 0x020
    i32 field_24; // 0x024
    i32 field_28; // 0x028
    i32 field_2C; // 0x02C
    i32 field_30; // 0x030
    i32 field_34; // 0x034
};

check_size(vehSurfaceAudioData, 0x38);
