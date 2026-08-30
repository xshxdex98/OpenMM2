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

class Matrix34;
class lvlSDL;

class cityPropulator
{
public:
    // ??0cityPropulator@@QAE@ABVlvlSDL@@@Z
    ARTS_IMPORT cityPropulator(const lvlSDL& arg1);

    // ??1cityPropulator@@QAE@XZ
    ARTS_IMPORT ~cityPropulator();

    // ?Load@cityPropulator@@QAE_NPBD00@Z
    ARTS_IMPORT bool Load(const char* p2, const char* p3, const char* p4);

    // ?LookupRule@cityPropulator@@QAEHPBD@Z
    ARTS_IMPORT i32 LookupRule(const char* arg1);

    // ?Propulate@cityPropulator@@QAEXHHHP6AXHPBDABVMatrix34@@@Z@Z
    ARTS_IMPORT void Propulate(i32 a, i32 b, i32 rounding, void (__cdecl* rounding_corners)(i32,const char*,const Matrix34&));

public:
    static lvlSDL*& Access_sm_SDL() { return sm_SDL; }

private:
    // ?sm_SDL@cityPropulator@@0PAVlvlSDL@@A
    ARTS_IMPORT static lvlSDL* sm_SDL;
};

// check_size(cityPropulator, 0x0); // TODO: no layout in the IDB type library
