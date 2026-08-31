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

#include "dg/dgphysentity.h"
#include "lvl/lvltrackmanager.h"

class Base_vtbl;
class gfxTexture;
class lvlInstance;
class phInertialCS;
class vehCarAudioContainer;
class vehCarDamage;
class vehCarModel;
class vehCarSim;
class vehDriver;
class vehGyro;
class vehInput;
class vehSiren;
class vehSplash;
class vehStuck;
class vehTrailer;
class vehWheel;
class vehWheelPtx;

class vehCar : public dgPhysEntity
{
public:
    // ??0vehCar@@QAE@H@Z
    ARTS_IMPORT vehCar(i32 arg1);

    // ??1vehCar@@UAE@XZ
    ARTS_IMPORT virtual ~vehCar();

    // ?PreUpdate@vehCar@@UAEXXZ
    ARTS_IMPORT virtual void PreUpdate();

    // ?Update@vehCar@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?PostUpdate@vehCar@@UAEXXZ
    ARTS_IMPORT virtual void PostUpdate();

    // ?GetICS@vehCar@@UAEPAVphInertialCS@@XZ
    ARTS_IMPORT virtual phInertialCS* GetICS();

    // ?GetInst@vehCar@@UAEPAVlvlInstance@@XZ
    ARTS_IMPORT virtual lvlInstance* GetInst();

    // ?RequiresTerrainCollision@vehCar@@UAE_NXZ
    ARTS_IMPORT virtual bool RequiresTerrainCollision();

    // ?ClearDamage@vehCar@@QAEXXZ
    ARTS_IMPORT void ClearDamage();

    // ?DrawTracks@vehCar@@QAEXXZ
    ARTS_IMPORT void DrawTracks();

    // ?GetInput@vehCar@@QAEPAVvehInput@@XZ
    ARTS_IMPORT vehInput* GetInput();

    // ?Init@vehCar@@QAEXPBDHH_N@Z
    ARTS_IMPORT void Init(const char* arg1, i32 arg2, i32 arg3, bool arg4);

    // ?InitAudio@vehCar@@QAEXPBDH@Z
    ARTS_IMPORT void InitAudio(const char* arg1, i32 arg2);

    // ?IsPlayer@vehCar@@QAEHXZ
    ARTS_IMPORT i32 IsPlayer();

    // ?Reset@vehCar@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?SetAudio@vehCar@@QAEXPAVvehCarAudioContainer@@@Z
    ARTS_IMPORT void SetAudio(vehCarAudioContainer* arg1);

    // ?SetColliderID@vehCar@@QAEXH@Z
    ARTS_IMPORT void SetColliderID(i32 arg1);

    // ?SetDrivable@vehCar@@QAEXHH@Z
    ARTS_IMPORT void SetDrivable(i32 arg1, i32 arg2);

    // ?SetDriver@vehCar@@QAEXPAVvehDriver@@@Z
    ARTS_IMPORT void SetDriver(vehDriver* arg1);

    // ?SetInput@vehCar@@QAEXPAVvehInput@@@Z
    ARTS_IMPORT void SetInput(vehInput* arg1);

private:
    // ?UpdateTrack@vehCar@@AAEXAAVlvlTrackManager@@ABVvehWheel@@@Z
    ARTS_IMPORT void UpdateTrack(lvlTrackManager& arg1, const vehWheel& arg2);

public:
    // Members from 0x0B4; everything below that belongs to dgPhysEntity.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    void* Input; // 0x0B4
    vehCarSim* CarSim; // 0x0B8
    vehCarModel* CarModel; // 0x0BC
    vehCarDamage* CarDamage; // 0x0C0
    vehWheelPtx* WheelPtx; // 0x0C4
    vehSiren* Siren; // 0x0C8
    vehStuck* Stuck; // 0x0CC
    vehGyro* Gyro; // 0x0D0
    i32 field_D4; // 0x0D4
    vehTrailer* Trailer; // 0x0D8
    void* Driver; // 0x0DC
    vehSplash* Splash; // 0x0E0
    void* Feedback; // 0x0E4
    i32 Flags; // 0x0E8
    i32 DriveDisableMode; // 0x0EC
    gfxTexture* TireTrackTexture; // 0x0F0
    lvlTrackManager WHL0TrackManager; // 0x0F4
    lvlTrackManager WHL1TrackManager; // 0x14C
    lvlTrackManager WHL2TrackManager; // 0x1A4
    lvlTrackManager WHL3TrackManager; // 0x1FC
    vehCarAudioContainer* CarAudioContainer; // 0x254
};

// ??_7vehCar@@6B@
// vtable at 0x005B0BB0
check_size(vehCar, 0x258);
