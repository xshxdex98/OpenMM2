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

#include "arts7/asnode.h"
#include "vector7/vector3.h"

class Base_vtbl;
class datParser;
class vehCarSim;

class vehAero : public asNode
{
public:
    // ??0vehAero@@QAE@XZ
    ARTS_IMPORT vehAero();

    // ??1vehAero@@UAE@XZ
    ARTS_IMPORT virtual ~vehAero();

    // ?Update@vehAero@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?FileIO@vehAero@@UAEXAAVdatParser@@@Z
    ARTS_IMPORT virtual void FileIO(datParser& arg1);

    // ?GetClassName@vehAero@@UAEPADXZ
    ARTS_IMPORT virtual char* GetClassName();

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i32 EnableAero; // 0x018
    vehCarSim* m_CarSimPtr; // 0x01C
    Vector3 AngCDamp; // 0x020
    Vector3 AngVelDamp; // 0x02C
    Vector3 AngVel2Damp; // 0x038
    f32 Drag; // 0x044
    f32 Down; // 0x048
};

// ??_7vehAero@@6B@
// vtable at 0x005B3034
check_size(vehAero, 0x4C);
