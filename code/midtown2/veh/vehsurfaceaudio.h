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

class Vector3;
class vehCarDamage;
class vehCarSim;
class vehWheel;

class vehSurfaceAudio
{
public:
    // ??0vehSurfaceAudio@@QAE@XZ
    ARTS_IMPORT vehSurfaceAudio();

    // ??1vehSurfaceAudio@@UAE@XZ
    ARTS_IMPORT virtual ~vehSurfaceAudio();

    // ?AssignSounds@vehSurfaceAudio@@QAEXH@Z
    ARTS_IMPORT void AssignSounds(i32 arg1);

    // ?EchoOff@vehSurfaceAudio@@QAEXXZ
    ARTS_IMPORT void EchoOff();

    // ?EchoOn@vehSurfaceAudio@@QAEXM@Z
    ARTS_IMPORT void EchoOn(f32 arg1);

    // ?IsBrakeing@vehSurfaceAudio@@QAE_NXZ
    ARTS_IMPORT bool IsBrakeing();

    // ?LoadCSV@vehSurfaceAudio@@QAE_NPAD0@Z
    ARTS_IMPORT bool LoadCSV(char* arg1, char* arg2);

    // ?LoadSuspension@vehSurfaceAudio@@QAEHPAD0@Z
    ARTS_IMPORT i32 LoadSuspension(char* arg1, char* arg2);

    // ?LoadTireWobble@vehSurfaceAudio@@QAEHPAD0@Z
    ARTS_IMPORT i32 LoadTireWobble(char* arg1, char* arg2);

    // ?SetDamagePtr@vehSurfaceAudio@@QAEXPAVvehCarDamage@@@Z
    ARTS_IMPORT void SetDamagePtr(vehCarDamage* arg1);

    // ?SetPan@vehSurfaceAudio@@QAEXM@Z
    ARTS_IMPORT void SetPan(f32 arg1);

    // ?SetPositionPtr@vehSurfaceAudio@@QAEXPAVVector3@@@Z
    ARTS_IMPORT void SetPositionPtr(Vector3* arg1);

    // ?SetWheelPointers@vehSurfaceAudio@@QAEXPAVvehWheel@@000@Z
    ARTS_IMPORT void SetWheelPointers(vehWheel* arg1, vehWheel* arg2, vehWheel* arg3, vehWheel* arg4);

    // ?StopSkid@vehSurfaceAudio@@QAEXXZ
    ARTS_IMPORT void StopSkid();

    // ?StopSurface@vehSurfaceAudio@@QAEXXZ
    ARTS_IMPORT void StopSurface();

    // ?SurfaceChanged@vehSurfaceAudio@@QAEHXZ
    ARTS_IMPORT i32 SurfaceChanged();

    // ?UnAssignSounds@vehSurfaceAudio@@QAEXXZ
    ARTS_IMPORT void UnAssignSounds();

    // ?Update@vehSurfaceAudio@@QAEXPAVvehCarSim@@@Z
    ARTS_IMPORT void Update(vehCarSim* arg1);

    // ?Update@vehSurfaceAudio@@QAEXPAVvehCarSim@@MM@Z
    ARTS_IMPORT void Update(vehCarSim* arg1, f32 arg2, f32 arg3);

    // ?UpdateAir@vehSurfaceAudio@@QAEXXZ
    ARTS_IMPORT void UpdateAir();

    // ?UpdateEcho@vehSurfaceAudio@@QAEXXZ
    ARTS_IMPORT void UpdateEcho();

    // ?UpdateSkid@vehSurfaceAudio@@QAEXM@Z
    ARTS_IMPORT void UpdateSkid(f32 arg1);

    // ?UpdateSkid@vehSurfaceAudio@@QAEXMMM@Z
    ARTS_IMPORT void UpdateSkid(f32 arg1, f32 arg2, f32 arg3);

    // ?UpdateSurface@vehSurfaceAudio@@QAEXM@Z
    ARTS_IMPORT void UpdateSurface(f32 arg1);

    // ?UpdateSurface@vehSurfaceAudio@@QAEXMMM@Z
    ARTS_IMPORT void UpdateSurface(f32 arg1, f32 arg2, f32 arg3);

    // ?UpdateSuspension@vehSurfaceAudio@@QAEXMM@Z
    ARTS_IMPORT void UpdateSuspension(f32 arg1, f32 arg2);

    // ?UpdateSuspension@vehSurfaceAudio@@QAEXXZ
    ARTS_IMPORT void UpdateSuspension();

    // ?UpdateTireWobble@vehSurfaceAudio@@QAEXPAVvehCarSim@@@Z
    ARTS_IMPORT void UpdateTireWobble(vehCarSim* arg1);

    // ?UpdateTireWobble@vehSurfaceAudio@@QAEXPAVvehCarSim@@MM@Z
    ARTS_IMPORT void UpdateTireWobble(vehCarSim* arg1, f32 arg2, f32 arg3);
};

// check_size(vehSurfaceAudio, 0x84); // size known, members are not - cannot verify
