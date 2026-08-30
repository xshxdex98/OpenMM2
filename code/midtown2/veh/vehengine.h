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

#include "arts7/asnode.h"

class datParser;
class vehCarSim;

class vehEngine : public asNode
{
public:
    // ??0vehEngine@@QAE@XZ
    ARTS_IMPORT vehEngine();

    // ??1vehEngine@@UAE@XZ
    ARTS_IMPORT virtual ~vehEngine();

    // ?CalcHPAtFullThrottle@vehEngine@@QAEMM@Z
    ARTS_IMPORT f32 CalcHPAtFullThrottle(f32 arg1);

    // ?CalcTorque@vehEngine@@QAEMM@Z
    ARTS_IMPORT f32 CalcTorque(f32 arg1);

    // ?CalcTorqueAtFullThrottle@vehEngine@@QAEMM@Z
    ARTS_IMPORT f32 CalcTorqueAtFullThrottle(f32 arg1);

    // ?CalcTorqueAtZeroThrottle@vehEngine@@QAEMXZ
    ARTS_IMPORT f32 CalcTorqueAtZeroThrottle();

    // ?ComputeConstants@vehEngine@@QAEXXZ
    ARTS_IMPORT void ComputeConstants();

    // ?FileIO@vehEngine@@UAEXAAVdatParser@@@Z
    ARTS_IMPORT virtual void FileIO(datParser& arg1);

    // ?GetClassName@vehEngine@@UAEPADXZ
    ARTS_IMPORT virtual char* GetClassName();

    // ?Init@vehEngine@@QAEXPAVvehCarSim@@PBD1@Z
    ARTS_IMPORT void Init(vehCarSim* arg1, const char* arg2, const char* arg3);

    // ?Reset@vehEngine@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Update@vehEngine@@UAEXXZ
    ARTS_IMPORT virtual void Update();
};

// ??_7vehEngine@@6B@
// vtable at 0x005B2FEC
// check_size(vehEngine, 0x84); // size known, members are not - cannot verify
