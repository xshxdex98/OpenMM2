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

#include "audobject.h"

class audCD
{
public:
    // ?GetStatus@audCD@@QAE?AW4AUD_OBJECTSTATUS@audObject@@XZ
    ARTS_IMPORT audObject::AUD_OBJECTSTATUS GetStatus();

    // ?PauseResume@audCD@@QAE_N_N@Z
    ARTS_IMPORT bool PauseResume(bool arg1);

    // ?Play@audCD@@QAE_NXZ
    ARTS_IMPORT bool Play();

    // ?SetPan@audCD@@QAE_NM@Z
    ARTS_IMPORT bool SetPan(f32 arg1);

    // ?SetPitch@audCD@@QAE_NM@Z
    ARTS_IMPORT bool SetPitch(f32 arg1);

    // ?SetVolume@audCD@@QAE_NM@Z
    ARTS_IMPORT bool SetVolume(f32 arg1);

    // ?Stop@audCD@@QAE_NXZ
    ARTS_IMPORT bool Stop();

    // ?Update@audCD@@QAE_NH@Z
    ARTS_IMPORT bool Update(i32 arg1);
};

// check_size(audCD, 0x0); // TODO: no layout in the IDB type library
