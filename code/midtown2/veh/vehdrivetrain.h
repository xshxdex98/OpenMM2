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
class vehWheel;

class vehDrivetrain : public asNode
{
public:
    // ??0vehDrivetrain@@QAE@XZ
    ARTS_IMPORT vehDrivetrain();

    // ??1vehDrivetrain@@UAE@XZ
    ARTS_IMPORT virtual ~vehDrivetrain();

    // ?AddWheel@vehDrivetrain@@QAEHPAVvehWheel@@@Z
    ARTS_IMPORT i32 AddWheel(vehWheel* arg1);

    // ?Attach@vehDrivetrain@@QAEXXZ
    ARTS_IMPORT void Attach();

    // ?CopyVars@vehDrivetrain@@QAEXPAV1@@Z
    ARTS_IMPORT void CopyVars(vehDrivetrain* arg1);

    // ?Detach@vehDrivetrain@@QAEXXZ
    ARTS_IMPORT void Detach();

    // ?FileIO@vehDrivetrain@@UAEXAAVdatParser@@@Z
    ARTS_IMPORT virtual void FileIO(datParser& arg1);

    // ?GetClassName@vehDrivetrain@@UAEPADXZ
    ARTS_IMPORT virtual char* GetClassName();

    // ?Init@vehDrivetrain@@QAEXPAVvehCarSim@@@Z
    ARTS_IMPORT void Init(vehCarSim* arg1);

    // ?Reset@vehDrivetrain@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Update@vehDrivetrain@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?diffRatioMax@vehDrivetrain@@2MA
    ARTS_IMPORT static f32 diffRatioMax;

    // ?diffRatioMaxHighSpeed@vehDrivetrain@@2MA
    ARTS_IMPORT static f32 diffRatioMaxHighSpeed;

    // ?diffRatioHighSpeedLevel@vehDrivetrain@@2MA
    ARTS_IMPORT static f32 diffRatioHighSpeedLevel;

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i32 field_18; // 0x018
    i32 field_1C; // 0x01C
    i32 field_20; // 0x020
    i32 field_24; // 0x024
    u8 field_28[24]; // 0x028
    f32 AngInertia; // 0x040
    f32 BrakeDynamicCoef; // 0x044
    f32 BrakeStaticCoef; // 0x048
};

// ??_7vehDrivetrain@@6B@
// vtable at 0x005B30D0
check_size(vehDrivetrain, 0x4C);
