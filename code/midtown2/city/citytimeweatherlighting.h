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

#include "vector7/vector3.h"

class datParser;

class cityTimeWeatherLighting
{
public:
    // ??0cityTimeWeatherLighting@@QAE@XZ
    ARTS_IMPORT cityTimeWeatherLighting();

    // ?ComputeAmbientLightLevels@cityTimeWeatherLighting@@QAEXXZ
    ARTS_IMPORT void ComputeAmbientLightLevels();

    // ?FileIO@cityTimeWeatherLighting@@QAEXAAVdatParser@@@Z
    ARTS_IMPORT void FileIO(datParser& arg1);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    f32 KeyHeading; // 0x000
    f32 KeyPitch; // 0x004
    Vector3 KeyColor; // 0x008
    f32 Fill1Heading; // 0x014
    f32 Fill1Pitch; // 0x018
    Vector3 Fill1Color; // 0x01C
    f32 Fill2Heading; // 0x028
    f32 Fill2Pitch; // 0x02C
    Vector3 Fill2Color; // 0x030
    i32 AmbientLevel3; // 0x03C
    i32 AmbientLevel2; // 0x040
    i32 AmbientLevel1; // 0x044
    i32 Ambient; // 0x048
};

check_size(cityTimeWeatherLighting, 0x4C);
