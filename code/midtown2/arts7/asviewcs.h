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

#include "aslinearcs.h"

class asViewCS : public asLinearCS
{
public:
    // ??0asViewCS@@QAE@XZ
    ARTS_IMPORT asViewCS();

    // ??1asViewCS@@UAE@XZ
    ARTS_IMPORT virtual ~asViewCS();

    // ?Update@asViewCS@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@asViewCS@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?SetAzimuth@asViewCS@@QAEXXZ
    ARTS_IMPORT void SetAzimuth();

    // ?UpdateLookAt@asViewCS@@QAEXXZ
    ARTS_IMPORT void UpdateLookAt();

    // ?UpdatePolar@asViewCS@@QAEXXZ
    ARTS_IMPORT void UpdatePolar();

    // ?UpdatePOV@asViewCS@@QAEXXZ
    ARTS_IMPORT void UpdatePOV();

    // ?UpdateRoam@asViewCS@@QAEXXZ
    ARTS_IMPORT void UpdateRoam();

    // ?UpdateStereo@asViewCS@@QAEXXZ
    ARTS_IMPORT void UpdateStereo();

    // ?UpdateTrack@asViewCS@@QAEXXZ
    ARTS_IMPORT void UpdateTrack();
};

// ??_7asViewCS@@6B@
// vtable at 0x005B86A8
// check_size(asViewCS, 0x0); // TODO: no layout in the IDB type library
