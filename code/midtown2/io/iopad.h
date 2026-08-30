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

class ioPad
{
public:
    // ??0ioPad@@QAE@XZ
    ARTS_IMPORT ioPad();

    // ??1ioPad@@QAE@XZ
    ARTS_IMPORT ~ioPad();

    // ?BeginAll@ioPad@@SAXXZ
    ARTS_IMPORT static void BeginAll();

    // ?EndAll@ioPad@@SAXXZ
    ARTS_IMPORT static void EndAll();

    // ?UpdateAll@ioPad@@SAXXZ
    ARTS_IMPORT static void UpdateAll();

public:
    static ioPad*& Access_PADS() { return PADS; }
    static bool& Access_sm_UseKeymap() { return sm_UseKeymap; }

private:
    // ?Begin@ioPad@@AAEXH@Z
    ARTS_IMPORT void Begin(i32 arg1);

    // ?End@ioPad@@AAEXXZ
    ARTS_IMPORT void End();

    // ?Update@ioPad@@AAEXXZ
    ARTS_IMPORT void Update();

    // ?PADS@ioPad@@0PAV1@A
    ARTS_IMPORT static ioPad* PADS;

    // ?sm_UseKeymap@ioPad@@0_NA
    ARTS_IMPORT static bool sm_UseKeymap;
};

// check_size(ioPad, 0x0); // TODO: no layout in the IDB type library
