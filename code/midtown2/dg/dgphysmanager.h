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

class Vector3;
class lvlInstance;
class lvlIntersection;
class lvlSegment;
class phColliderBase;
class phImpact;
class phInstance;
class phIntersection;
class phSegment;

class dgPhysManager : public phPhysicsManager
{
public:
    // ??0dgPhysManager@@QAE@XZ
    ARTS_IMPORT dgPhysManager();

    // ??1dgPhysManager@@UAE@XZ
    ARTS_IMPORT virtual ~dgPhysManager();

    // ?TestProbe@dgPhysManager@@UBE_NABVphSegment@@PAVphIntersection@@IH@Z
    ARTS_IMPORT virtual bool TestProbe(const phSegment& arg1, phIntersection* arg2, u32 arg3, i32 arg4) const;

    // ?TestSphere@dgPhysManager@@UBE_NABVVector3@@MPAVphImpact@@IH@Z
    ARTS_IMPORT virtual bool TestSphere(const Vector3& arg1, f32 arg2, phImpact* arg3, u32 arg4, i32 arg5) const;

    // ?PromoteInstance@dgPhysManager@@UAEPAVphColliderBase@@PAVphInstance@@@Z
    ARTS_IMPORT virtual phColliderBase* PromoteInstance(phInstance* arg1);

    // ?DemoteInstance@dgPhysManager@@UAEXPAVphInstance@@@Z
    ARTS_IMPORT virtual void DemoteInstance(phInstance* arg1);

    // ?KillInstance@dgPhysManager@@UAEXPAVphInstance@@@Z
    ARTS_IMPORT virtual void KillInstance(phInstance* arg1);

    // ?DisableInstance@dgPhysManager@@UAEXPAVphInstance@@@Z
    ARTS_IMPORT virtual void DisableInstance(phInstance* arg1);

    // ?EnableInstance@dgPhysManager@@UAEXPAVphInstance@@H@Z
    ARTS_IMPORT virtual void EnableInstance(phInstance* arg1, i32 arg2);

    // ?GetCollider@dgPhysManager@@UBEPAVphColliderBase@@PBVphInstance@@@Z
    ARTS_IMPORT virtual phColliderBase* GetCollider(const phInstance* arg1) const;

    // ?CreateInstance@dgPhysManager@@UAEPAVphInstance@@XZ
    ARTS_IMPORT virtual phInstance* CreateInstance();

    // ?Collide@dgPhysManager@@QAE_NAAVlvlSegment@@PAVlvlIntersection@@HPAVlvlInstance@@HH@Z
    ARTS_IMPORT bool Collide(lvlSegment& arg1, lvlIntersection* arg2, i32 arg3, lvlInstance* arg4, i32 arg5, i32 arg6);

    // ?CollideInstances@dgPhysManager@@QAE_NPAVlvlInstance@@0@Z
    ARTS_IMPORT bool CollideInstances(lvlInstance* arg1, lvlInstance* arg2);

    // ?CollideProbe@dgPhysManager@@QAE_NAAVlvlSegment@@PAVlvlIntersection@@PAVlvlInstance@@@Z
    ARTS_IMPORT bool CollideProbe(lvlSegment& arg1, lvlIntersection* arg2, lvlInstance* arg3);

    // ?CollideTerrain@dgPhysManager@@QAE_NPAUCollisionTableEntry@1@@Z
    ARTS_IMPORT bool CollideTerrain(dgPhysManager::CollisionTableEntry* arg1);

    // ?DeclareMover@dgPhysManager@@QAEXPAVlvlInstance@@HH@Z
    ARTS_IMPORT void DeclareMover(lvlInstance* arg1, i32 arg2, i32 arg3);

    // ?Draw@dgPhysManager@@QAEXXZ
    ARTS_IMPORT void Draw();

    // ?GatherCollidables@dgPhysManager@@QAEXPAUCollisionTableEntry@1@@Z
    ARTS_IMPORT void GatherCollidables(dgPhysManager::CollisionTableEntry* arg1);

    // ?IgnoreMover@dgPhysManager@@QAEXPAVlvlInstance@@@Z
    ARTS_IMPORT void IgnoreMover(lvlInstance* arg1);

    // ?NewMover@dgPhysManager@@QAEXPAVlvlInstance@@00@Z
    ARTS_IMPORT void NewMover(lvlInstance* arg1, lvlInstance* arg2, lvlInstance* arg3);

    // ?NewMover@dgPhysManager@@QAEXPAVlvlInstance@@0@Z
    ARTS_IMPORT void NewMover(lvlInstance* arg1, lvlInstance* arg2);

    // ?NewMover@dgPhysManager@@QAEXPAVlvlInstance@@@Z
    ARTS_IMPORT void NewMover(lvlInstance* arg1);

    // ?Reset@dgPhysManager@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?ResetTable@dgPhysManager@@QAEXXZ
    ARTS_IMPORT void ResetTable();

    // ?TrivialCollideInstances@dgPhysManager@@QAE_NPAVlvlInstance@@0@Z
    ARTS_IMPORT bool TrivialCollideInstances(lvlInstance* arg1, lvlInstance* arg2);

    // ?Update@dgPhysManager@@QAEXXZ
    ARTS_IMPORT void Update();

    // ?m_pInst@dgPhysManager@@2PAV1@A
    ARTS_IMPORT static dgPhysManager* m_pInst;

    // ?PlayerInst@dgPhysManager@@2PAVlvlInstance@@A
    ARTS_IMPORT static lvlInstance* PlayerInst;

public:
    // Members from 0x004; everything below that belongs to phPhysicsManager.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i32 field_4; // 0x004
    u8 field_8[148]; // 0x008
    i32 field_9C; // 0x09C
    i32 field_A0; // 0x0A0
    u8 field_A4[4612]; // 0x0A4
    i32 field_12A8; // 0x12A8
    f32 field_12AC; // 0x12AC
};

// ??_7dgPhysManager@@6BphPhysicsManager@@@
// vtable at 0x005B1CE8
check_size(dgPhysManager, 0x12B0);
