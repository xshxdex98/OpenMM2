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

#include "vector7/vector3.h"

class Matrix34;
class phColliderBase;
class phInertialCS;
class phMaterial;

class phImpact
{
public:
    // ?CalcCollision@phImpact@@QAEXABVVector3@@MPAV2@11@Z
    ARTS_IMPORT void CalcCollision(const Vector3& arg1, f32 arg2, Vector3* arg3, Vector3* arg4, Vector3* arg5);

    // ?CalcCollisionNoFriction@phImpact@@SAMABVphInertialCS@@ABVVector3@@M1@Z
    ARTS_IMPORT static f32 CalcCollisionNoFriction(const phInertialCS& arg1, const Vector3& arg2, f32 arg3, const Vector3& arg4);

    // ?Contact@phImpact@@QAEXABVVector3@@00ABVMatrix34@@@Z
    ARTS_IMPORT void Contact(const Vector3& arg1, const Vector3& arg2, const Vector3& arg3, const Matrix34& arg4);

    // ?EffectiveMass@phImpact@@QBEMXZ
    ARTS_IMPORT f32 EffectiveMass() const;

    // ?FindFrictionAndElasticity@phImpact@@QAEXXZ
    ARTS_IMPORT void FindFrictionAndElasticity();

    // ?GetLocalVelocities@phImpact@@QBEXAAVVector3@@0@Z
    ARTS_IMPORT void GetLocalVelocities(Vector3& arg1, Vector3& arg2) const;

    // ?GetMaterial@phImpact@@QAEPBVphMaterial@@PAVphColliderBase@@H@Z
    ARTS_IMPORT const phMaterial* GetMaterial(phColliderBase* arg1, i32 arg2);

    // ?GetRelDisplacement@phImpact@@QBEXPAVVector3@@@Z
    ARTS_IMPORT void GetRelDisplacement(Vector3* arg1) const;

    // ?Impact@phImpact@@QAEXABVVector3@@00@Z
    ARTS_IMPORT void Impact(const Vector3& arg1, const Vector3& arg2, const Vector3& arg3);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i32 field_0; // 0x000
    i32 field_4; // 0x004
    i32 field_8; // 0x008
    i32 ComponentA; // 0x00C
    i32 ComponentB; // 0x010
    Vector3 vector14; // 0x014
    Vector3 vector20; // 0x020
    f32 field_2C; // 0x02C
    f32 field_30; // 0x030
    f32 Friction; // 0x034
    f32 Elasticity; // 0x038
    phColliderBase* ColliderA; // 0x03C
    phColliderBase* ColliderB; // 0x040
};

check_size(phImpact, 0x44);
