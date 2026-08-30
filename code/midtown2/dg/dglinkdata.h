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

class dgLinkData
{
public:
    // ??0dgLinkData@@QAE@XZ
    ARTS_IMPORT dgLinkData();

    // ?FileIO@dgLinkData@@QAEXAAVdatParser@@@Z
    ARTS_IMPORT void FileIO(datParser& arg1);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    f32 Mass; // 0x000
    f32 MaxRadius; // 0x004
    f32 Elasticity; // 0x008
    f32 Friction; // 0x00C
    f32 GraspElasticity; // 0x010
    f32 Damping; // 0x014
    f32 TendonAngle; // 0x018
    f32 TendonLimit; // 0x01C
    f32 TendonSpring; // 0x020
    Vector3 TendonVector; // 0x024
};

check_size(dgLinkData, 0x30);
