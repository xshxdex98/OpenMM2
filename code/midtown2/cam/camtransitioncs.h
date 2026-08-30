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

#include "camcarcs.h"

class Vector3;
class vehCar;

class camTransitionCS : public camCarCS
{
public:
    // ??0camTransitionCS@@QAE@XZ
    ARTS_IMPORT camTransitionCS();

    // ??1camTransitionCS@@UAE@XZ
    ARTS_IMPORT virtual ~camTransitionCS();

    // ?Update@camTransitionCS@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@camTransitionCS@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?ForceMatrixDelta@camTransitionCS@@UAEXABVVector3@@@Z
    ARTS_IMPORT virtual void ForceMatrixDelta(const Vector3& arg1);

    // ?Init@camTransitionCS@@QAEXPAVvehCar@@@Z
    ARTS_IMPORT void Init(vehCar* arg1);

    // ?NewTransition@camTransitionCS@@QAEXPAVcamCarCS@@0@Z
    ARTS_IMPORT void NewTransition(camCarCS* arg1, camCarCS* arg2);

    // ?NextTransition@camTransitionCS@@QAEXPAVcamCarCS@@@Z
    ARTS_IMPORT void NextTransition(camCarCS* arg1);

    // ?ReverseTransition@camTransitionCS@@QAEXXZ
    ARTS_IMPORT void ReverseTransition();

    // ?StartNextTransition@camTransitionCS@@QAEXXZ
    ARTS_IMPORT void StartNextTransition();

    // ?StartTransition@camTransitionCS@@QAEXXZ
    ARTS_IMPORT void StartTransition();
};

// ??_7camTransitionCS@@6B@
// vtable at 0x005B50EC
// check_size(camTransitionCS, 0x110); // size known, members are not - cannot verify
