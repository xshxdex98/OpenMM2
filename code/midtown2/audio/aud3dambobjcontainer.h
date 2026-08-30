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

class Aud3DAmbObjContainer
{
public:
    // ??0Aud3DAmbObjContainer@@QAE@XZ
    ARTS_IMPORT Aud3DAmbObjContainer();

    // ??1Aud3DAmbObjContainer@@QAE@XZ
    ARTS_IMPORT ~Aud3DAmbObjContainer();

    // ?Init@Aud3DAmbObjContainer@@QAE_NPAD@Z
    ARTS_IMPORT bool Init(char* renderer);

    // ?Update@Aud3DAmbObjContainer@@QAEXM@Z
    ARTS_IMPORT void Update(f32 arg1);

private:
    // ?CreateAmbientObject@Aud3DAmbObjContainer@@AAEXPAD@Z
    ARTS_IMPORT void CreateAmbientObject(char* renderer);

    // ?FileValid@Aud3DAmbObjContainer@@AAE_NPAD@Z
    ARTS_IMPORT bool FileValid(char* path);
};

// check_size(Aud3DAmbObjContainer, 0x0); // TODO: no layout in the IDB type library
