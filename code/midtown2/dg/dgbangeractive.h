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

#include "dgphysentity.h"

class dgBangerInstance;
class lvlInstance;
class phInertialCS;

class dgBangerActive : public dgPhysEntity
{
public:
    // ??0dgBangerActive@@QAE@XZ
    ARTS_IMPORT dgBangerActive();

    // ??1dgBangerActive@@UAE@XZ
    ARTS_IMPORT virtual ~dgBangerActive();

    // ?Attach@dgBangerActive@@QAEXPAVdgBangerInstance@@@Z
    ARTS_IMPORT void Attach(dgBangerInstance* arg1);

    // ?Detach@dgBangerActive@@QAEXXZ
    ARTS_IMPORT void Detach();

    // ?DetachMe@dgBangerActive@@UAEXXZ
    ARTS_IMPORT virtual void DetachMe();

    // ?GetICS@dgBangerActive@@UAEPAVphInertialCS@@XZ
    ARTS_IMPORT virtual phInertialCS* GetICS();

    // ?GetInst@dgBangerActive@@UAEPAVlvlInstance@@XZ
    ARTS_IMPORT virtual lvlInstance* GetInst();

    // ?PostUpdate@dgBangerActive@@UAEXXZ
    ARTS_IMPORT virtual void PostUpdate();

    // ?Update@dgBangerActive@@UAEXXZ
    ARTS_IMPORT virtual void Update();
};

// ??_7dgBangerActive@@6B@
// vtable at 0x005B1398
// check_size(dgBangerActive, 0x318); // size known, members are not - cannot verify
