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

class Matrix34;
class Vector3;
class phBound;
class phColliderBase;

class phImpactBase
{
public:
    struct BoundCallbackData;

    // ?AddImpactShaftPlaneTest@phImpactBase@@SA_NPAV1@PAHABVVector3@@M2@Z
    ARTS_IMPORT static bool AddImpactShaftPlaneTest(phImpactBase* data_type, i32* v, const Vector3& arg3, f32 arg4, const Vector3& arg5);

    // ?AddImpactSpherePlaneTest@phImpactBase@@SA_NPAV1@PAHABVVector3@@M@Z
    ARTS_IMPORT static bool AddImpactSpherePlaneTest(phImpactBase* arg1, i32* arg2, const Vector3& arg3, f32 arg4);

    // ?CullImpactList@phImpactBase@@SAXPAV1@PAHABVVector3@@@Z
    ARTS_IMPORT static void CullImpactList(phImpactBase* arg1, i32* arg2, const Vector3& arg3);

    // ?FinishMakingNewImpact@phImpactBase@@QAEXHHPAVphColliderBase@@0PBVphBound@@1H@Z
    ARTS_IMPORT void FinishMakingNewImpact(i32 arg1, i32 arg2, phColliderBase* arg3, phColliderBase* arg4, const phBound* arg5, const phBound* arg6, i32 arg7);

    // ?ImpactIsInList@phImpactBase@@SA_NHHHPAV1@H@Z
    ARTS_IMPORT static bool ImpactIsInList(i32 arg1, i32 arg2, i32 arg3, phImpactBase* arg4, i32 arg5);

    // ?MakeNewImpact@phImpactBase@@QAEXPAVphColliderBase@@0ABVVector3@@1MPBVphBound@@2HHH@Z
    ARTS_IMPORT void MakeNewImpact(phColliderBase* arg1, phColliderBase* arg2, const Vector3& arg3, const Vector3& arg4, f32 arg5, const phBound* arg6, const phBound* arg7, i32 arg8, i32 arg9, i32 arg10);

    // ?Reset@phImpactBase@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?SetColliders@phImpactBase@@QAEXPAVphColliderBase@@0@Z
    ARTS_IMPORT void SetColliders(phColliderBase* arg1, phColliderBase* arg2);

    // ?SetColliders@phImpactBase@@QAEXPAVphColliderBase@@0HH@Z
    ARTS_IMPORT void SetColliders(phColliderBase* arg1, phColliderBase* arg2, i32 arg3, i32 arg4);

    // ?SetColliders@phImpactBase@@SAXPAVphColliderBase@@0PAV1@H@Z
    ARTS_IMPORT static void SetColliders(phColliderBase* arg1, phColliderBase* arg2, phImpactBase* arg3, i32 arg4);

    // ?SetComponentA@phImpactBase@@QAEXH@Z
    ARTS_IMPORT void SetComponentA(i32 arg1);

    // ?SetComponentB@phImpactBase@@QAEXH@Z
    ARTS_IMPORT void SetComponentB(i32 arg1);

    // ?StartMakingNewImpact@phImpactBase@@QAEXMABVVector3@@0PAVphColliderBase@@1PBVMatrix34@@HHH@Z
    ARTS_IMPORT void StartMakingNewImpact(f32 arg1, const Vector3& arg2, const Vector3& arg3, phColliderBase* arg4, phColliderBase* arg5, const Matrix34* arg6, i32 arg7, i32 arg8, i32 arg9);

    // ?SwapColliders@phImpactBase@@QAEXXZ
    ARTS_IMPORT void SwapColliders();
};

// check_size(phImpactBase, 0x1); // size known, members are not - cannot verify
