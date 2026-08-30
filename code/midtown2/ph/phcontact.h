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
class phImpact;

class phContact
{
public:
    // ?CalcContactForce@phContact@@QAE_NPBVphImpact@@ABVVector3@@11PAV3@PAVMatrix34@@PAMMM@Z
    ARTS_IMPORT bool CalcContactForce(const phImpact* arg1, const Vector3& arg2, const Vector3& arg3, const Vector3& arg4, Vector3* arg5, Matrix34* arg6, f32* arg7, f32 arg8, f32 arg9);

    // ?DisableContacts@phContact@@SAXXZ
    ARTS_IMPORT static void DisableContacts();

    // ?Init@phContact@@QAEXABVphImpact@@@Z
    ARTS_IMPORT void Init(const phImpact& idx);

    // ?IsEqual@phContact@@QAE_NABVphImpact@@@Z
    ARTS_IMPORT bool IsEqual(const phImpact& arg1);

    // ?Set@phContact@@QAEXABV1@@Z
    ARTS_IMPORT void Set(const phContact& n);

    // ?SetContactForceLimit@phContact@@QAEXABVphImpact@@ABVVector3@@@Z
    ARTS_IMPORT void SetContactForceLimit(const phImpact& sz, const Vector3& arg2);

    // ?SetContactPenetration@phContact@@SAXXZ
    ARTS_IMPORT static void SetContactPenetration();

    // ?SetContactPenetrationScale@phContact@@SAXM@Z
    ARTS_IMPORT static void SetContactPenetrationScale(f32 arg1);

    // ?SwapAB@phContact@@QAEXXZ
    ARTS_IMPORT void SwapAB();
};

// check_size(phContact, 0x0); // TODO: no layout in the IDB type library
