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

class Vector3;
class datParser;
class lvlInstance;
class phColliderJointed;

class vehCarSim : public asNode
{
public:
    // ??0vehCarSim@@QAE@XZ
    ARTS_IMPORT vehCarSim();

    // ??1vehCarSim@@UAE@XZ
    ARTS_IMPORT virtual ~vehCarSim();

    // ?Update@vehCarSim@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@vehCarSim@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?FileIO@vehCarSim@@UAEXAAVdatParser@@@Z
    ARTS_IMPORT virtual void FileIO(datParser& arg1);

    // ?GetClassName@vehCarSim@@UAEPADXZ
    ARTS_IMPORT virtual char* GetClassName();

    // ?GetDirName@vehCarSim@@UAEPBDXZ
    ARTS_IMPORT virtual const char* GetDirName();

    // ?BottomedOut@vehCarSim@@QAEHXZ
    ARTS_IMPORT i32 BottomedOut();

    // ?ClearDamage@vehCarSim@@QAEXXZ
    ARTS_EXPORT void ClearDamage();

    // ?ConfigureDrivetrain@vehCarSim@@QAEXXZ
    ARTS_IMPORT void ConfigureDrivetrain();

    // ?GetSSSFactor@vehCarSim@@QAEMM@Z
    ARTS_IMPORT f32 GetSSSFactor(f32 arg1);

    // ?Init@vehCarSim@@QAEXPBDPAVphColliderJointed@@PAVlvlInstance@@@Z
    ARTS_IMPORT void Init(const char* arg1, phColliderJointed* arg2, lvlInstance* arg3);

    // ?OnGround@vehCarSim@@QAEHXZ
    ARTS_IMPORT i32 OnGround();

    // ?PlaybackReplay@vehCarSim@@QAEXXZ
    ARTS_IMPORT void PlaybackReplay();

    // ?ReconfigureDrivetrain@vehCarSim@@QAEXXZ
    ARTS_IMPORT void ReconfigureDrivetrain();

    // ?RecordReplay@vehCarSim@@QAEXXZ
    ARTS_IMPORT void RecordReplay();

    // ?RestoreImpactParams@vehCarSim@@QAEXXZ
    ARTS_IMPORT void RestoreImpactParams();

    // ?SetHackedImpactParams@vehCarSim@@QAEXXZ
    ARTS_IMPORT void SetHackedImpactParams();

    // ?SetResetPos@vehCarSim@@QAEXABVVector3@@@Z
    ARTS_IMPORT void SetResetPos(const Vector3& arg1);

    // ?SetWorldMatrix@vehCarSim@@QAEXXZ
    ARTS_IMPORT void SetWorldMatrix();

    // ?UnconfigureDrivetrain@vehCarSim@@QAEXXZ
    ARTS_IMPORT void UnconfigureDrivetrain();

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 field_18[436]; // 0x018
    phColliderJointed* PhColliderJointed; // 0x1CC
    u8 field_1D0[5006]; // 0x1D0
};

// ??_7vehCarSim@@6B@
// vtable at 0x005B2C74
check_size(vehCarSim, 0x1560);
