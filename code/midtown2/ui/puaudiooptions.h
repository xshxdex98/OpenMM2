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

#include "pumenubase.h"

class PUAudioOptions : public PUMenuBase
{
public:
    // ??0PUAudioOptions@@QAE@HMMMM@Z
    ARTS_IMPORT PUAudioOptions(i32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5);

    // ??1PUAudioOptions@@UAE@XZ
    ARTS_IMPORT virtual ~PUAudioOptions();

    // ?CancelAction@PUAudioOptions@@QAEXXZ
    ARTS_IMPORT void CancelAction();

    // ?PreSetup@PUAudioOptions@@UAEXXZ
    ARTS_IMPORT virtual void PreSetup();

    // ?SetBalance@PUAudioOptions@@QAEXXZ
    ARTS_IMPORT void SetBalance();

    // ?SetCDVolume@PUAudioOptions@@QAEXXZ
    ARTS_IMPORT void SetCDVolume();

    // ?SetWaveVolume@PUAudioOptions@@QAEXXZ
    ARTS_IMPORT void SetWaveVolume();
};

// ??_7PUAudioOptions@@6B@
// vtable at 0x005B49FC
// check_size(PUAudioOptions, 0x0); // TODO: no layout in the IDB type library
