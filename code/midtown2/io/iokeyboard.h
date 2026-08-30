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

class ioKeyboard
{
public:
    // ?Begin@ioKeyboard@@SAX_N@Z
    ARTS_IMPORT static void Begin(bool arg1);

    // ?End@ioKeyboard@@SAXXZ
    ARTS_IMPORT static void End();

    // ?GetBufferedInput@ioKeyboard@@SAHPADH@Z
    ARTS_IMPORT static i32 GetBufferedInput(char* arg1, i32 arg2);

    // ?Update@ioKeyboard@@SAXXZ
    ARTS_IMPORT static void Update();
};

// check_size(ioKeyboard, 0x1); // size known, members are not - cannot verify
