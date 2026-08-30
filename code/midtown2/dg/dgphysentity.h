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

#include "lvl/lvltrackmanager.h"
#include "misc/base.h"
#include "ph/phcolliderjointed.h"

class phCollider;
class vehCarDamage;
class vehCarModel;
class vehCarSim;
class vehFeedback;
class vehWheelPtx;

class dgPhysEntity : public Base
{
public:
    // ??1dgPhysEntity@@UAE@XZ
    ARTS_IMPORT virtual ~dgPhysEntity();

    // ?DetachMe@dgPhysEntity@@UAEXXZ
    ARTS_IMPORT virtual void DetachMe();

    // ?FirstImpactCallback@dgPhysEntity@@UAEXXZ
    ARTS_IMPORT virtual void FirstImpactCallback();

    // ?GetCollider@dgPhysEntity@@UAEPAVphCollider@@XZ
    ARTS_IMPORT virtual phCollider* GetCollider();

    // ?PreUpdate@dgPhysEntity@@UAEXXZ
    ARTS_IMPORT virtual void PreUpdate();

    // ?RequiresTerrainCollision@dgPhysEntity@@UAE_NXZ
    ARTS_IMPORT virtual bool RequiresTerrainCollision();

    // ?Update@dgPhysEntity@@UAEXXZ
    ARTS_IMPORT virtual void Update();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    phColliderJointed JointedCollider; // 0x004
    u32 field_B4; // 0x0B4
    vehCarSim* pSim; // 0x0B8
    vehCarModel* pModel; // 0x0BC
    vehCarDamage* pCarDamage; // 0x0C0
    vehWheelPtx* pWheelPtx; // 0x0C4
    u32 field_C8; // 0x0C8
    u32 pVehStuck; // 0x0CC
    u32 pVehGyro; // 0x0D0
    u32 field_D4; // 0x0D4
    u32 field_D8; // 0x0D8
    u32 field_DC; // 0x0DC
    u32 field_E0; // 0x0E0
    vehFeedback* pFeedback; // 0x0E4
    u32 field_E8; // 0x0E8
    u32 field_EC; // 0x0EC
    u8 field_F0[4]; // 0x0F0
    lvlTrackManager LvlTrackManager_F4; // 0x0F4
    lvlTrackManager LvlTrackManager_14C; // 0x14C
    lvlTrackManager LvlTrackManager_1A4; // 0x1A4
    lvlTrackManager LvlTrackManager_1FC; // 0x1FC
    u32 field_254; // 0x254
};

// ??_7dgPhysEntity@@6B@
// vtable at 0x005B0BD8
check_size(dgPhysEntity, 0x258);
