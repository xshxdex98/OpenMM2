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

#include "ph/phjoint.h"
#include "vector7/matrix34.h"
#include "vector7/vector3.h"

class datParser;
class phInertialCS;
class phJoint_vtbl;

class dgTrailerJoint : public phJoint
{
public:
    // ??0dgTrailerJoint@@QAE@XZ
    ARTS_IMPORT dgTrailerJoint();

    // ??1dgTrailerJoint@@UAE@XZ
    ARTS_IMPORT virtual ~dgTrailerJoint();

    // ?IsBroken@dgTrailerJoint@@UBE_NXZ
    ARTS_IMPORT virtual bool IsBroken() const;

    // ?ComputeInvMassMatrix@dgTrailerJoint@@UBEXPAVphInertialCS@@AAVMatrix34@@ABVVector3@@@Z
    ARTS_IMPORT virtual void ComputeInvMassMatrix(phInertialCS* arg1, Matrix34& arg2, const Vector3& arg3) const;

    // ?BreakJoint@dgTrailerJoint@@QAEXXZ
    ARTS_IMPORT void BreakJoint();

    // ?ComputeInvMassMatrix@dgTrailerJoint@@QAEXPAVphInertialCS@@0AAVMatrix34@@ABVVector3@@@Z
    ARTS_IMPORT void ComputeInvMassMatrix(phInertialCS* arg1, phInertialCS* arg2, Matrix34& arg3, const Vector3& arg4);

    // ?DoJointLimits@dgTrailerJoint@@QAEXMABVVector3@@M0AAV2@ABVMatrix34@@2222@Z
    ARTS_IMPORT void DoJointLimits(f32 arg1, const Vector3& arg2, f32 arg3, const Vector3& arg4, Vector3& arg5, const Matrix34& arg6, const Matrix34& arg7, const Matrix34& arg8, const Matrix34& arg9, const Matrix34& arg10);

    // ?DoJointTorque@dgTrailerJoint@@QAEXABVMatrix34@@000AAM1AAVVector3@@11@Z
    ARTS_IMPORT void DoJointTorque(const Matrix34& arg1, const Matrix34& arg2, const Matrix34& arg3, const Matrix34& arg4, f32& arg5, f32& arg6, Vector3& arg7, f32& arg8, f32& arg9);

    // ?FileIO@dgTrailerJoint@@UAEXAAVdatParser@@@Z
    ARTS_IMPORT virtual void FileIO(datParser& arg1);

    // ?GetClassName@dgTrailerJoint@@UAEPBDXZ
    ARTS_IMPORT virtual const char* GetClassName();

    // ?GetDirName@dgTrailerJoint@@UAEPBDXZ
    ARTS_IMPORT virtual const char* GetDirName();

    // ?Init@dgTrailerJoint@@QAEXPBDPAVphInertialCS@@1ABVVector3@@2@Z
    ARTS_IMPORT void Init(const char* arg1, phInertialCS* arg2, phInertialCS* arg3, const Vector3& arg4, const Vector3& arg5);

    // ?Load@dgTrailerJoint@@UAE_NXZ
    ARTS_IMPORT virtual bool Load();

    // ?MoveICS@dgTrailerJoint@@QAEXXZ
    ARTS_IMPORT void MoveICS();

    // ?Reset@dgTrailerJoint@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?Save@dgTrailerJoint@@UAE_NXZ
    ARTS_IMPORT virtual bool Save();

    // ?SetCosFreeLean@dgTrailerJoint@@QAEXXZ
    ARTS_IMPORT void SetCosFreeLean();

    // ?SetForceLimit@dgTrailerJoint@@QAEXM@Z
    ARTS_IMPORT void SetForceLimit(f32 arg1);

    // ?SetFrictionLean@dgTrailerJoint@@QAEXMMM@Z
    ARTS_IMPORT void SetFrictionLean(f32 arg1, f32 arg2, f32 arg3);

    // ?SetFrictionRoll@dgTrailerJoint@@QAEXMMM@Z
    ARTS_IMPORT void SetFrictionRoll(f32 arg1, f32 arg2, f32 arg3);

    // ?SetJointForceFlag@dgTrailerJoint@@QAEXXZ
    ARTS_IMPORT void SetJointForceFlag();

    // ?SetLeanLimit@dgTrailerJoint@@QAEXMM@Z
    ARTS_IMPORT void SetLeanLimit(f32 arg1, f32 arg2);

    // ?SetPosition@dgTrailerJoint@@QAEXABVVector3@@@Z
    ARTS_IMPORT void SetPosition(const Vector3& arg1);

    // ?SetRestOrientation@dgTrailerJoint@@QAEXXZ
    ARTS_IMPORT void SetRestOrientation();

    // ?SetRestOrientMat@dgTrailerJoint@@QAEXABVMatrix34@@0@Z
    ARTS_IMPORT void SetRestOrientMat(const Matrix34& arg1, const Matrix34& arg2);

    // ?SetRestOrientMat@dgTrailerJoint@@QAEXABVMatrix34@@@Z
    ARTS_IMPORT void SetRestOrientMat(const Matrix34& arg1);

    // ?SetRollLimit@dgTrailerJoint@@QAEXMM@Z
    ARTS_IMPORT void SetRollLimit(f32 arg1, f32 arg2);

    // ?SetRollLimit@dgTrailerJoint@@QAEXMMM@Z
    ARTS_IMPORT void SetRollLimit(f32 arg1, f32 arg2, f32 arg3);

    // ?SetRotate1@dgTrailerJoint@@QAEXABVMatrix34@@@Z
    ARTS_IMPORT void SetRotate1(const Matrix34& arg1);

    // ?SetRotate2@dgTrailerJoint@@QAEXABVMatrix34@@@Z
    ARTS_IMPORT void SetRotate2(const Matrix34& arg1);

    // ?UnbreakJoint@dgTrailerJoint@@QAEXXZ
    ARTS_IMPORT void UnbreakJoint();

    // ?Update@dgTrailerJoint@@QAEXXZ
    ARTS_IMPORT void Update();

public:
    // Members from 0x06C; everything below that belongs to phJoint.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i8 gap[176]; // 0x06C
};

// ??_7dgTrailerJoint@@6BphJoint@@@
// vtable at 0x005B8638
check_size(dgTrailerJoint, 0x11C);
