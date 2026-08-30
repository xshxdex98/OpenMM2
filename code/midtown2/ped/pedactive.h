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

#include "dg/dgphysentity.h"

class aiPedestrianInstance;
class lvlInstance;
class pedActiveData;
class phInertialCS;

class pedActive : public dgPhysEntity
{
public:
    // ??0pedActive@@QAE@XZ
    ARTS_IMPORT pedActive();

    // ??1pedActive@@UAE@XZ
    ARTS_IMPORT virtual ~pedActive();

    // ?Activate@pedActive@@UAEXPAVaiPedestrianInstance@@PAVpedActiveData@@@Z
    ARTS_IMPORT virtual void Activate(aiPedestrianInstance* arg1, pedActiveData* arg2);

    // ?Deactivate@pedActive@@UAEXXZ
    ARTS_IMPORT virtual void Deactivate();

    // ?FirstImpactCallback@pedActive@@UAEXXZ
    ARTS_IMPORT virtual void FirstImpactCallback();

    // ?GetICS@pedActive@@UAEPAVphInertialCS@@XZ
    ARTS_IMPORT virtual phInertialCS* GetICS();

    // ?GetInst@pedActive@@UAEPAVlvlInstance@@XZ
    ARTS_IMPORT virtual lvlInstance* GetInst();

    // ?IsAsleep@pedActive@@QAEHXZ
    ARTS_IMPORT i32 IsAsleep();

    // ?PostUpdate@pedActive@@UAEXXZ
    ARTS_IMPORT virtual void PostUpdate();

    // ?Reset@pedActive@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?SetRagdollBlend@pedActive@@QAEXM@Z
    ARTS_IMPORT void SetRagdollBlend(f32 arg1);

    // ?StartRagdoll@pedActive@@QAEXXZ
    ARTS_IMPORT void StartRagdoll();

    // ?StopRagdoll@pedActive@@QAEXXZ
    ARTS_IMPORT void StopRagdoll();

    // ?Update@pedActive@@UAEXXZ
    ARTS_IMPORT virtual void Update();
};

// ??_7pedActive@@6B@
// vtable at 0x005B639C
// check_size(pedActive, 0xB8); // size known, members are not - cannot verify
