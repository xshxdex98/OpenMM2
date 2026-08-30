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

class phPolygon;

class phInertialCS
{
public:
    // ??0phInertialCS@@QAE@XZ
    ARTS_IMPORT phInertialCS();

    // ?AddInertia@phInertialCS@@QAE?AVMatrix34@@MABVVector3@@0@Z
    ARTS_IMPORT Matrix34 AddInertia(f32 arg1, const Vector3& arg2, const Vector3& arg3);

    // ?AddInertia@phInertialCS@@QAE?AVMatrix34@@MABVVector3@@ABV2@@Z
    ARTS_IMPORT Matrix34 AddInertia(f32 arg1, const Vector3& arg2, const Matrix34& arg3);

    // ?ApplyContactForce@phInertialCS@@QAEXABVVector3@@0ABVMatrix34@@@Z
    ARTS_IMPORT void ApplyContactForce(const Vector3& ptr_id, const Vector3& flags, const Matrix34& fmt);

    // ?CalcNetPush@phInertialCS@@QAEXABVVector3@@@Z
    ARTS_IMPORT void CalcNetPush(const Vector3& str_id);

    // ?CalcNetTurn@phInertialCS@@QAEXABVVector3@@@Z
    ARTS_IMPORT void CalcNetTurn(const Vector3& arg1);

    // ?ClearInertialValues@phInertialCS@@QAEXXZ
    ARTS_IMPORT void ClearInertialValues();

    // ?FindPrincipalAxes@phInertialCS@@QAE?AVMatrix34@@MMM@Z
    ARTS_IMPORT Matrix34 FindPrincipalAxes(f32 arg1, f32 arg2, f32 arg3);

    // ?Freeze@phInertialCS@@QAEXXZ
    ARTS_IMPORT void Freeze();

    // ?GetCMDisp@phInertialCS@@QBEXAAVVector3@@@Z
    ARTS_IMPORT void GetCMDisp(Vector3& arg1) const;

    // ?GetCMFilteredVelocity@phInertialCS@@QBEXAAVVector3@@@Z
    ARTS_IMPORT void GetCMFilteredVelocity(Vector3& arg1) const;

    // ?GetForce@phInertialCS@@QAEXMPAVVector3@@@Z
    ARTS_IMPORT void GetForce(f32 arg1, Vector3* arg2);

    // ?GetInertiaMatrix@phInertialCS@@QBEXABVVector3@@PAVMatrix34@@@Z
    ARTS_IMPORT void GetInertiaMatrix(const Vector3& label, Matrix34* flags) const;

    // ?GetInertiaMatrix@phInertialCS@@QBEXPAVMatrix34@@@Z
    ARTS_IMPORT void GetInertiaMatrix(Matrix34* arg1) const;

    // ?GetInverseInertiaMatrix@phInertialCS@@QBEXPAVMatrix34@@@Z
    ARTS_IMPORT void GetInverseInertiaMatrix(Matrix34* label) const;

    // ?GetInvMassMatrix@phInertialCS@@QBEXABVVector3@@0AAVMatrix34@@@Z
    ARTS_IMPORT void GetInvMassMatrix(const Vector3& arg1, const Vector3& arg2, Matrix34& arg3) const;

    // ?GetInvMassMatrix@phInertialCS@@QBEXABVVector3@@AAVMatrix34@@@Z
    ARTS_IMPORT void GetInvMassMatrix(const Vector3& arg1, Matrix34& arg2) const;

    // ?GetLocalAcceleration@phInertialCS@@QBEXABVVector3@@AAV2@@Z
    ARTS_IMPORT void GetLocalAcceleration(const Vector3& label, Vector3& current_item) const;

    // ?GetLocalDisp@phInertialCS@@QBEXABVVector3@@AAV2@@Z
    ARTS_IMPORT void GetLocalDisp(const Vector3& arg1, Vector3& arg2) const;

    // ?GetLocalFilteredDisp@phInertialCS@@QBEXABVVector3@@AAV2@@Z
    ARTS_IMPORT void GetLocalFilteredDisp(const Vector3& arg1, Vector3& arg2) const;

    // ?GetLocalFilteredVelocity@phInertialCS@@QBEXABVVector3@@AAV2@@Z
    ARTS_IMPORT void GetLocalFilteredVelocity(const Vector3& arg1, Vector3& arg2) const;

    // ?GetLocalFilteredVelocity2@phInertialCS@@QBEXABVVector3@@AAV2@@Z
    ARTS_IMPORT void GetLocalFilteredVelocity2(const Vector3& arg1, Vector3& arg2) const;

    // ?GetLocalVelocity@phInertialCS@@QBEXABVVector3@@AAV2@@Z
    ARTS_IMPORT void GetLocalVelocity(const Vector3& arg1, Vector3& arg2) const;

    // ?GetOffsetVelocity@phInertialCS@@QBEXABVVector3@@AAV2@@Z
    ARTS_IMPORT void GetOffsetVelocity(const Vector3& arg1, Vector3& arg2) const;

    // ?GetTorque@phInertialCS@@QAEXMPAVVector3@@@Z
    ARTS_IMPORT void GetTorque(f32 arg1, Vector3* arg2);

    // ?Init@phInertialCS@@QAEXMMMM@Z
    ARTS_IMPORT void Init(f32 arg1, f32 arg2, f32 arg3, f32 arg4);

    // ?InitArray@phInertialCS@@SAXQAV1@H@Z
    ARTS_IMPORT static void InitArray(phInertialCS*const arg1, i32 arg2);

    // ?InitBoxDensity@phInertialCS@@QAEXMMMM@Z
    ARTS_IMPORT void InitBoxDensity(f32 arg1, f32 arg2, f32 arg3, f32 arg4);

    // ?InitBoxMass@phInertialCS@@QAEXMMMM@Z
    ARTS_IMPORT void InitBoxMass(f32 label, f32 col, f32 flags, f32 argC);

    // ?InitCylinderDensity@phInertialCS@@QAEXMMMD@Z
    ARTS_IMPORT void InitCylinderDensity(f32 arg1, f32 arg2, f32 arg3, char arg4);

    // ?InitCylinderMass@phInertialCS@@QAEXMMMD@Z
    ARTS_IMPORT void InitCylinderMass(f32 arg1, f32 arg2, f32 arg3, char argC);

    // ?InitFromGeometry@phInertialCS@@QAEXMPBVVector3@@PBVphPolygon@@HPAVMatrix34@@@Z
    ARTS_IMPORT void InitFromGeometry(f32 arg1, const Vector3* arg2, const phPolygon* arg3, i32 arg4, Matrix34* arg5);

    // ?InitFromValues@phInertialCS@@QAEXMMABVVector3@@0@Z
    ARTS_IMPORT void InitFromValues(f32 arg1, f32 arg2, const Vector3& arg3, const Vector3& arg4);

    // ?InitHotdogDensity@phInertialCS@@QAEXMMMD@Z
    ARTS_IMPORT void InitHotdogDensity(f32 arg1, f32 arg2, f32 arg3, char arg4);

    // ?InitHotdogMass@phInertialCS@@QAEXMMMD@Z
    ARTS_IMPORT void InitHotdogMass(f32 arg1, f32 arg2, f32 arg3, char argC);

    // ?InitSphereDensity@phInertialCS@@QAEXMM@Z
    ARTS_IMPORT void InitSphereDensity(f32 label, f32 col);

    // ?InitSphereMass@phInertialCS@@QAEXMM@Z
    ARTS_IMPORT void InitSphereMass(f32 arg1, f32 arg2);

    // ?MoveICS@phInertialCS@@QAEXXZ
    ARTS_IMPORT void MoveICS();

    // ??0TerrainContact@phInertialCS@@QAE@XZ
    ARTS_IMPORT void phInertialCS();

    // ?PrincipalAxis@phInertialCS@@QAE?AVVector3@@MMMM@Z
    ARTS_IMPORT Vector3 PrincipalAxis(f32 arg1, f32 arg2, f32 arg3, f32 arg4);

    // ?Reset@phInertialCS@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?Rotate@phInertialCS@@QAEXABVVector3@@@Z
    ARTS_IMPORT void Rotate(const Vector3& arg1);

    // ?SubtractInertia@phInertialCS@@QAE?AVMatrix34@@MABVVector3@@0@Z
    ARTS_IMPORT Matrix34 SubtractInertia(f32 arg1, const Vector3& arg2, const Vector3& arg3);

    // ?SubtractInertia@phInertialCS@@QAE?AVMatrix34@@MABVVector3@@0PAV1@@Z
    ARTS_IMPORT Matrix34 SubtractInertia(f32 arg1, const Vector3& arg2, const Vector3& arg3, phInertialCS* arg4);

    // ?SubtractInertia@phInertialCS@@QAE?AVMatrix34@@MABVVector3@@ABV2@@Z
    ARTS_IMPORT Matrix34 SubtractInertia(f32 arg1, const Vector3& arg2, const Matrix34& arg3);

    // ?SubtractInertia@phInertialCS@@QAE?AVMatrix34@@MABVVector3@@ABV2@PAV1@@Z
    ARTS_IMPORT Matrix34 SubtractInertia(f32 arg1, const Vector3& arg2, const Matrix34& arg3, phInertialCS* arg4);

    // ?TetrahedronAngInertia@phInertialCS@@QAEXPAVVector3@@PAM11111@Z
    ARTS_IMPORT void TetrahedronAngInertia(Vector3* arg1, f32* arg2, f32* arg3, f32* arg4, f32* arg5, f32* arg6, f32* arg7);

    // ?Update@phInertialCS@@QAEXXZ
    ARTS_IMPORT void Update();

    // ?UpdateOversample@phInertialCS@@QAEXM@Z
    ARTS_IMPORT void UpdateOversample(f32 arg1);

    // ?UpdateOversampleDone@phInertialCS@@QAEXXZ
    ARTS_IMPORT void UpdateOversampleDone();

    // ?Zero@phInertialCS@@QAEXXZ
    ARTS_IMPORT void Zero();

    // ?ZeroForces@phInertialCS@@QAEXXZ
    ARTS_IMPORT void ZeroForces();

private:
    // ?Update@phInertialCS@@AAEXM@Z
    ARTS_IMPORT void Update(f32 arg1);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u32 field_0; // 0x000
    u32 field_4; // 0x004
    u8 field_8; // 0x008
    f32 field_C; // 0x00C
    f32 field_10; // 0x010
    f32 field_14; // 0x014
    f32 field_18; // 0x018
    f32 field_1C; // 0x01C
    f32 field_20; // 0x020
    f32 field_24; // 0x024
    f32 field_28; // 0x028
    f32 field_2C; // 0x02C
    f32 field_30; // 0x030
    f32 field_34; // 0x034
    f32 field_38; // 0x038
    u8 field_3C[172]; // 0x03C
    u32 field_E8; // 0x0E8
    u32 field_EC; // 0x0EC
    u32 field_F0; // 0x0F0
    u32 field_F4; // 0x0F4
    u32 field_F8; // 0x0F8
    u32 field_FC; // 0x0FC
    u32 field_100; // 0x100
    u32 field_104; // 0x104
    u32 field_108; // 0x108
    u32 field_10C; // 0x10C
    u32 field_110; // 0x110
    u32 field_114; // 0x114
    u8 field_118; // 0x118
    u32 field_11C; // 0x11C
    Matrix34 field_120; // 0x120
    Matrix34 field_150; // 0x150
    Matrix34 field_180; // 0x180
    f32 field_1B0; // 0x1B0
};

check_size(phInertialCS, 0x1B4);
