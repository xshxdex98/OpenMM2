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

#include "vector7/matrix34.h"

class Vector3;
class phInertialCS;

class phJoint
{
public:
    // ??0phJoint@@QAE@PAVphInertialCS@@0ABVVector3@@1@Z
    ARTS_IMPORT phJoint(phInertialCS* arg1, phInertialCS* arg2, const Vector3& arg3, const Vector3& arg4);

    // ??0phJoint@@QAE@PAVphInertialCS@@0ABVVector3@@@Z
    ARTS_IMPORT phJoint(phInertialCS* arg1, phInertialCS* arg2, const Vector3& arg3);

    // ??0phJoint@@QAE@XZ
    ARTS_IMPORT phJoint();

    // ?ComputeInvMassMatrix@phJoint@@UAEXXZ
    ARTS_IMPORT virtual void ComputeInvMassMatrix();

    // ?ComputeInvMassMatrix@phJoint@@UBEXPAVphInertialCS@@AAVMatrix34@@ABVVector3@@@Z
    ARTS_IMPORT virtual void ComputeInvMassMatrix(phInertialCS* arg1, Matrix34& arg2, const Vector3& arg3) const;

    // ?ComputeJointForce@phJoint@@UAEXXZ
    ARTS_IMPORT virtual void ComputeJointForce();

    // ?ComputeJointPush@phJoint@@UAEXXZ
    ARTS_IMPORT virtual void ComputeJointPush();

    // ?GetInvMassMatrix@phJoint@@UAEAAVMatrix34@@XZ
    ARTS_IMPORT virtual Matrix34& GetInvMassMatrix();

    // ?Init@phJoint@@QAEXPAVphInertialCS@@0ABVVector3@@1@Z
    ARTS_IMPORT void Init(phInertialCS* arg1, phInertialCS* arg2, const Vector3& arg3, const Vector3& arg4);

    // ?Init@phJoint@@QAEXPAVphInertialCS@@0ABVVector3@@@Z
    ARTS_IMPORT void Init(phInertialCS* arg1, phInertialCS* arg2, const Vector3& arg3);

    // ?IsBroken@phJoint@@UBE_NXZ
    ARTS_IMPORT virtual bool IsBroken() const;

    // ?Reset@phJoint@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?Update@phJoint@@QAEXXZ
    ARTS_IMPORT void Update();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i32 field_4; // 0x004
    i32 field_8; // 0x008
    i32 field_C; // 0x00C
    i32 field_10; // 0x010
    i32 field_14; // 0x014
    i32 field_18; // 0x018
    i32 field_1C; // 0x01C
    i32 field_20; // 0x020
    i32 field_24; // 0x024
    i32 field_28; // 0x028
    i32 field_2C; // 0x02C
    i32 field_30; // 0x030
    i32 field_34; // 0x034
    i32 field_38; // 0x038
    Matrix34 field_3C; // 0x03C
};

// ??_7phJoint@@6B@
// vtable at 0x005B8690
check_size(phJoint, 0x6C);
