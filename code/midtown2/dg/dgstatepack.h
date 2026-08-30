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

class dgStatePack
{
public:
    // ??0dgStatePack@@QAE@XZ
    ARTS_IMPORT dgStatePack();

    // ??1dgStatePack@@QAE@XZ
    ARTS_IMPORT ~dgStatePack();

    // ?InitFromArgs@dgStatePack@@QAEXXZ
    ARTS_IMPORT void InitFromArgs();

    // ?Instance@dgStatePack@@2PAU1@A
    ARTS_IMPORT static dgStatePack* Instance;

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    dgGameMode GameMode; // 0x000
    i32 RaceId; // 0x004
    f32 TrafficDensity; // 0x008
    f32 PedestrianDensity; // 0x00C
    f32 CopDensity; // 0x010
    f32 OpponentDensity; // 0x014
    f32 field_18; // 0x018
    f32 field_1C; // 0x01C
    f32 field_20; // 0x020
    u8 field_24; // 0x024
    i32 NumberOfCTFRacers; // 0x028
    i32 DisableAI; // 0x02C
    i32 field_30; // 0x030
    i32 MaxAmbientVehicles; // 0x034
    i32 MaxAmbientPedestrians; // 0x038
    i32 field_3C; // 0x03C
    i32 EnableCableCars; // 0x040
    i32 EnableSubways; // 0x044
    i32 NumLaps; // 0x048
    i32 TextureQuality; // 0x04C
    i32 field_50; // 0x050
    i32 TimeOfDay; // 0x054
    i32 WeatherType; // 0x058
    dgSkillLevel SkillLevel; // 0x05C
    i32 AudioFlags; // 0x060
    i32 EnablePedestrians; // 0x064
    void* field_68; // 0x068
};

check_size(dgStatePack, 0x6C);
