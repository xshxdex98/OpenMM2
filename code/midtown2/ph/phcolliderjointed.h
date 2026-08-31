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

#include "phcolliderbase.h"

class Matrix34;
class Vector3;
class phContact;
class phImpactBase;
class phJoint;

class phColliderJointed : public phColliderBase
{
public:
    // ??0phColliderJointed@@QAE@XZ
    ARTS_IMPORT phColliderJointed();

    // ?GetInvMassMatrix@phColliderJointed@@UBEXABVVector3@@0AAVMatrix34@@H@Z
    ARTS_IMPORT virtual void GetInvMassMatrix(const Vector3& arg1, const Vector3& arg2, Matrix34& arg3, i32 arg4) const;

    // ?GetInvMassMatrix@phColliderJointed@@UBEXABVVector3@@AAVMatrix34@@H@Z
    ARTS_IMPORT virtual void GetInvMassMatrix(const Vector3& arg1, Matrix34& arg2, i32 arg3) const;

    // ?Impact@phColliderJointed@@UAEXPBVphImpactBase@@ABVVector3@@@Z
    ARTS_IMPORT virtual void Impact(const phImpactBase* arg1, const Vector3& arg2);

    // ?Impact@phColliderJointed@@UAEXPBVphImpactBase@@ABVVector3@@1@Z
    ARTS_IMPORT virtual void Impact(const phImpactBase* arg1, const Vector3& rhs, const Vector3& arg3);

    // ?Contact@phColliderJointed@@UAEXABVVector3@@PBVphContact@@@Z
    ARTS_IMPORT virtual void Contact(const Vector3& arg1, const phContact* arg2);

    // ?Contact@phColliderJointed@@UAEXABVVector3@@PBVphContact@@ABVMatrix34@@@Z
    ARTS_IMPORT virtual void Contact(const Vector3& arg1, const phContact* arg2, const Matrix34& arg3);

    // ?Contact@phColliderJointed@@UAEXPBVphImpactBase@@ABVVector3@@11ABVMatrix34@@@Z
    ARTS_IMPORT virtual void Contact(const phImpactBase* arg1, const Vector3& arg2, const Vector3& arg3, const Vector3& arg4, const Matrix34& arg5);

    // ?Attach@phColliderJointed@@QAEXPBVphJoint@@@Z
    ARTS_IMPORT void Attach(const phJoint* arg1);

    // ?InitArray@phColliderJointed@@SAXQAV1@H@Z
    ARTS_IMPORT static void InitArray(phColliderJointed*const output, i32 i);

public:
    // Members from 0x074; everything below that belongs to phColliderBase.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 field_74[56]; // 0x074
    u32 field_AC; // 0x0AC
};

// ??_7phColliderJointed@@6B@
// vtable at 0x005B1E8C
check_size(phColliderJointed, 0xB0);
