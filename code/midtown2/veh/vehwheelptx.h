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
#include "arts7/asparticles.h"

class asBirthRule;
class vehCarSim;
class vehWheel;

class vehWheelPtx : public asNode
{
public:
    // ??0vehWheelPtx@@QAE@XZ
    ARTS_IMPORT vehWheelPtx();

    // ??1vehWheelPtx@@UAE@XZ
    ARTS_IMPORT virtual ~vehWheelPtx();

    // ?Blast@vehWheelPtx@@QAEXPAVvehWheel@@MHH@Z
    ARTS_IMPORT void Blast(vehWheel* arg1, f32 arg2, i32 arg3, i32 arg4);

    // ?ConstructClass@vehWheelPtx@@SAXPBD@Z
    ARTS_IMPORT static void ConstructClass(const char* arg1);

    // ?DestroyClass@vehWheelPtx@@SAXXZ
    ARTS_IMPORT static void DestroyClass();

    // ?Init@vehWheelPtx@@QAEXPAVvehCarSim@@@Z
    ARTS_IMPORT void Init(vehCarSim* arg1);

    // ?Reset@vehWheelPtx@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Update@vehWheelPtx@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?UpdateWheel@vehWheelPtx@@QAEXPAVvehWheel@@@Z
    ARTS_IMPORT void UpdateWheel(vehWheel* arg1);

    // ?TexName@vehWheelPtx@@2PADA
    ARTS_IMPORT static char* TexName;

    // ?PtxName@vehWheelPtx@@2PAPADA
    ARTS_IMPORT static char** PtxName;

    // ?Rule@vehWheelPtx@@2PAPAVasBirthRule@@A
    ARTS_IMPORT static asBirthRule** Rule;

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u32 field_18; // 0x018
    asParticles Particles; // 0x01C
    u32 field_70; // 0x070
    u32 field_74; // 0x074
};

// ??_7vehWheelPtx@@6B@
// vtable at 0x005B2D98
check_size(vehWheelPtx, 0x78);
