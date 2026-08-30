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

#include "arts7/asparticles.h"
#include "ph/phinertialcs.h"
#include "ph/phsleep.h"

#include "dgphysentity.h"

class Base_vtbl;
class dgBangerInstance;
class lvlInstance;

class dgBangerActive : public dgPhysEntity
{
public:
    // ??0dgBangerActive@@QAE@XZ
    ARTS_IMPORT dgBangerActive();

    // ??1dgBangerActive@@UAE@XZ
    ARTS_IMPORT virtual ~dgBangerActive();

    // ?Update@dgBangerActive@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?PostUpdate@dgBangerActive@@UAEXXZ
    ARTS_IMPORT virtual void PostUpdate();

    // ?GetICS@dgBangerActive@@UAEPAVphInertialCS@@XZ
    ARTS_IMPORT virtual phInertialCS* GetICS();

    // ?GetInst@dgBangerActive@@UAEPAVlvlInstance@@XZ
    ARTS_IMPORT virtual lvlInstance* GetInst();

    // ?DetachMe@dgBangerActive@@UAEXXZ
    ARTS_IMPORT virtual void DetachMe();

    // ?Attach@dgBangerActive@@QAEXPAVdgBangerInstance@@@Z
    ARTS_IMPORT void Attach(dgBangerInstance* arg1);

    // ?Detach@dgBangerActive@@QAEXXZ
    ARTS_IMPORT void Detach();

public:
    // Members from 0x258; everything below that belongs to dgPhysEntity.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 pad_258[20]; // 0x258
    phSleep Sleep; // 0x26C
    lvlInstance* Instance; // 0x2BC
    asParticles Particles; // 0x2C0
    f32 Timer; // 0x314
};

// ??_7dgBangerActive@@6B@
// vtable at 0x005B1398
check_size(dgBangerActive, 0x318);
