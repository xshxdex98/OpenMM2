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
#include "dg/dgphysentity.h"
#include "dg/dgtrailerjoint.h"
#include "lvl/lvltrackmanager.h"
#include "ph/phcolliderjointed.h"

#include "vehdrivetrain.h"
#include "vehwheel.h"

class Vector3;
class datParser;
class lvlInstance;
class phInertialCS;
class vehCarSim;

class vehTrailer : public dgPhysEntity
{
public:
    // ??0vehTrailer@@QAE@XZ
    ARTS_IMPORT vehTrailer();

    // ??1vehTrailer@@UAE@XZ
    ARTS_IMPORT virtual ~vehTrailer();

    // ?Update@vehTrailer@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?PostUpdate@vehTrailer@@UAEXXZ
    ARTS_EXPORT virtual void PostUpdate();

    // ?GetICS@vehTrailer@@UAEPAVphInertialCS@@XZ
    ARTS_IMPORT virtual phInertialCS* GetICS();

    // ?GetInst@vehTrailer@@UAEPAVlvlInstance@@XZ
    ARTS_IMPORT virtual lvlInstance* GetInst();

    // ?RequiresTerrainCollision@vehTrailer@@UAE_NXZ
    ARTS_IMPORT virtual bool RequiresTerrainCollision();

    // ?BottomedOut@vehTrailer@@QAEHXZ
    ARTS_IMPORT i32 BottomedOut();

    // ?FileIO@vehTrailer@@UAEXAAVdatParser@@@Z
    ARTS_IMPORT virtual void FileIO(datParser& arg1);

    // ?GetClassName@vehTrailer@@UAEPADXZ
    ARTS_IMPORT virtual char* GetClassName();

    // ?GetDirName@vehTrailer@@UAEPBDXZ
    ARTS_IMPORT virtual const char* GetDirName();

    // ?Init@vehTrailer@@QAEXPBDABVVector3@@PAVvehCarSim@@H@Z
    ARTS_IMPORT void Init(const char* arg1, const Vector3& arg2, vehCarSim* arg3, i32 arg4);

    // ?Load@vehTrailer@@UAE_NXZ
    ARTS_IMPORT virtual bool Load();

    // ?Reset@vehTrailer@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Save@vehTrailer@@UAE_NXZ
    ARTS_IMPORT virtual bool Save();

    // ?SetCarHitchOffset@vehTrailer@@QAEXXZ
    ARTS_IMPORT void SetCarHitchOffset();

    // ?SetTrailerHitchOffset@vehTrailer@@QAEXXZ
    ARTS_IMPORT void SetTrailerHitchOffset();

public:
    // Members from 0x0B4; everything below that belongs to dgPhysEntity.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    asNode field_B4; // 0x0B4
    dgTrailerJoint field_CC; // 0x0CC
    u8 field_1E8[476]; // 0x1E8
    f32 field_3C4; // 0x3C4
    f32 field_3C8; // 0x3C8
    f32 field_3CC; // 0x3CC
    f32 field_3D0; // 0x3D0
    u8 field_3D4[12]; // 0x3D4
    i32 field_3E0; // 0x3E0
    i32 field_3E4; // 0x3E4
    i32 field_3E8; // 0x3E8
    i32 field_3EC; // 0x3EC
    i32 field_3F0; // 0x3F0
    i32 field_3F4; // 0x3F4
    vehDrivetrain field_3F8; // 0x3F8
    vehDrivetrain field_444; // 0x444
    vehDrivetrain field_490; // 0x490
    vehDrivetrain field_4DC; // 0x4DC
    vehWheel field_528; // 0x528
    vehWheel field_794; // 0x794
    vehWheel field_A00; // 0xA00
    vehWheel field_C6C; // 0xC6C
    lvlTrackManager field_ED8; // 0xED8
    lvlTrackManager field_F30; // 0xF30
    lvlTrackManager field_F88; // 0xF88
    lvlTrackManager field_FE0; // 0xFE0
};

// ??_7vehTrailer@@6BasNode@@@
// vtable at 0x005B2F28
check_size(vehTrailer, 0x1038);
