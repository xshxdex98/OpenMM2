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

class Base_vtbl;
class Matrix34;
class datParser;
class vehCarSim;
class vehTransmission;

class vehEngine : public asNode
{
public:
    // ??0vehEngine@@QAE@XZ
    ARTS_IMPORT vehEngine();

    // ??1vehEngine@@UAE@XZ
    ARTS_IMPORT virtual ~vehEngine();

    // ?Update@vehEngine@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@vehEngine@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?FileIO@vehEngine@@UAEXAAVdatParser@@@Z
    ARTS_IMPORT virtual void FileIO(datParser& arg1);

    // ?GetClassName@vehEngine@@UAEPADXZ
    ARTS_IMPORT virtual char* GetClassName();

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

    // ?Init@vehEngine@@QAEXPAVvehCarSim@@PBD1@Z
    ARTS_IMPORT void Init(vehCarSim* arg1, const char* arg2, const char* arg3);

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    f32 MaxHorsePower; // 0x018
    f32 IdleRPM; // 0x01C
    f32 OptRPM; // 0x020
    f32 MaxRPM; // 0x024
    f32 GCL; // 0x028
    f32 unknown648; // 0x02C
    i32 unknown652; // 0x030
    f32 AngInertia; // 0x034
    f32 MaxTorque; // 0x038
    f32 OptTorque; // 0x03C
    f32 IdleTorque; // 0x040
    f32 TorqueGap; // 0x044
    f32 unknown676; // 0x048
    f32 sq5add1; // 0x04C
    f32 sq5min1; // 0x050
    f32 GearChangedAtRPM; // 0x054
    f32 GCLTimer; // 0x058
    i32 WaitingOnGCL; // 0x05C
    f32 ThrottleInput; // 0x060
    f32 CurrentTorque; // 0x064
    f32 CurrentRPM; // 0x068
    f32 unknown712; // 0x06C
    f32 ThrottleTorque; // 0x070
    Matrix34* EngineVisualMatrixRef; // 0x074
    Matrix34* EngineVisualMatrixPtr; // 0x078
    vehCarSim* VehCarSimPtr; // 0x07C
    vehTransmission* VehTransmissionPtr; // 0x080
};

// ??_7vehEngine@@6B@
// vtable at 0x005B2FEC
check_size(vehEngine, 0x84);
