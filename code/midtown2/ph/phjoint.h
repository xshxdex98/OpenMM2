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
#include "vector7/vector3.h"

class phInertialCS;
class phJoint_vtbl;

class phJoint
{
public:
    // ??0phJoint@@QAE@PAVphInertialCS@@0ABVVector3@@1@Z
    ARTS_IMPORT phJoint(phInertialCS* arg1, phInertialCS* arg2, const Vector3& arg3, const Vector3& arg4);

    // ??0phJoint@@QAE@PAVphInertialCS@@0ABVVector3@@@Z
    ARTS_IMPORT phJoint(phInertialCS* arg1, phInertialCS* arg2, const Vector3& arg3);

    // ??0phJoint@@QAE@XZ
    ARTS_IMPORT phJoint();

    // ?IsBroken@phJoint@@UBE_NXZ
    ARTS_IMPORT virtual bool IsBroken() const;

    // ?ComputeInvMassMatrix@phJoint@@UBEXPAVphInertialCS@@AAVMatrix34@@ABVVector3@@@Z
    ARTS_IMPORT virtual void ComputeInvMassMatrix(phInertialCS* arg1, Matrix34& arg2, const Vector3& arg3) const;

    // ?ComputeInvMassMatrix@phJoint@@UAEXXZ
    ARTS_IMPORT virtual void ComputeInvMassMatrix();

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

    // ?Reset@phJoint@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?Update@phJoint@@QAEXXZ
    ARTS_IMPORT void Update();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    phInertialCS* ICS1; // 0x004
    phInertialCS* ICS2; // 0x008
    Vector3 Offset1; // 0x00C
    Vector3 Offset2; // 0x018
    Vector3 PremultipliedOffset; // 0x024
    Vector3 PremultipliedOffset1; // 0x030
    Matrix34 InvMassMatrix; // 0x03C
};

// ??_7phJoint@@6B@
// vtable at 0x005B8690
check_size(phJoint, 0x6C);
