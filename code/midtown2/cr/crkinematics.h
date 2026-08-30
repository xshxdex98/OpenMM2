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

#include "crkinematicsbase.h"

class crKinematics : public crKinematicsBase
{
public:
    // ??0crKinematics@@QAE@XZ
    ARTS_IMPORT crKinematics();

    // ??1crKinematics@@UAE@XZ
    ARTS_IMPORT virtual ~crKinematics();

    // ?MatchPose@crKinematics@@UAEXXZ
    ARTS_IMPORT virtual void MatchPose();

    // ?Reset@crKinematics@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?SetIKBlend@crKinematics@@UAEXM@Z
    ARTS_IMPORT virtual void SetIKBlend(f32 arg1);

    // ?SetLimp@crKinematics@@UAEX_N@Z
    ARTS_IMPORT virtual void SetLimp(bool arg1);

    // ?TestPose@crKinematics@@QAEXXZ
    ARTS_IMPORT void TestPose();

    // ?Update@crKinematics@@UAEXXZ
    ARTS_IMPORT virtual void Update();
};

// ??_7crKinematics@@6B@
// vtable at 0x005B63F0
// check_size(crKinematics, 0x0); // TODO: no layout in the IDB type library
