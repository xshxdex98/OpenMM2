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

class mmRainAudio
{
public:
    // ??0mmRainAudio@@QAE@XZ
    ARTS_IMPORT mmRainAudio();

    // ??1mmRainAudio@@QAE@XZ
    ARTS_IMPORT ~mmRainAudio();

    // ?SetInterior@mmRainAudio@@QAEXE@Z
    ARTS_IMPORT void SetInterior(u8 ctx);

    // ?ShelterOff@mmRainAudio@@QAEXXZ
    ARTS_IMPORT void ShelterOff();

    // ?ShelterOn@mmRainAudio@@QAEXXZ
    ARTS_IMPORT void ShelterOn();

    // ?Update@mmRainAudio@@QAEXXZ
    ARTS_IMPORT void Update();
};

// check_size(mmRainAudio, 0x0); // TODO: no layout in the IDB type library
