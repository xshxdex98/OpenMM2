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
#include "vector7/vector4.h"

class Stream;

class mmReplayManager : public asNode
{
public:
    // ??0mmReplayManager@@QAE@XZ
    ARTS_IMPORT mmReplayManager();

    // ??1mmReplayManager@@UAE@XZ
    ARTS_IMPORT virtual ~mmReplayManager();

    // ?Cull@mmReplayManager@@UAEXXZ
    ARTS_IMPORT virtual void Cull();

    // ?Update@mmReplayManager@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@mmReplayManager@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?EndOfReplay@mmReplayManager@@QAE_NXZ
    ARTS_IMPORT bool EndOfReplay();

    // ?GetBrakes@mmReplayManager@@QAEMXZ
    ARTS_IMPORT f32 GetBrakes();

    // ?GetData@mmReplayManager@@QAEXPAHPA_N@Z
    ARTS_IMPORT void GetData(i32* arg1, bool* arg2);

    // ?GetHandBrakes@mmReplayManager@@QAEMXZ
    ARTS_IMPORT f32 GetHandBrakes();

    // ?GetSteering@mmReplayManager@@QAEMXZ
    ARTS_IMPORT f32 GetSteering();

    // ?GetThrottle@mmReplayManager@@QAEMXZ
    ARTS_IMPORT f32 GetThrottle();

    // ?LoadReplay@mmReplayManager@@QAEXPAD@Z
    ARTS_IMPORT void LoadReplay(char* path);

    // ?LoadReplayDesc@mmReplayManager@@QAEHPAD@Z
    ARTS_IMPORT i32 LoadReplayDesc(char* arg1);

    // ?SaveReplay@mmReplayManager@@QAEXPAD@Z
    ARTS_IMPORT void SaveReplay(char* arg1);

    // ?SaveReplayDesc@mmReplayManager@@QAEXPAD@Z
    ARTS_IMPORT void SaveReplayDesc(char* arg1);

    // ?SetData@mmReplayManager@@QAEXH_N@Z
    ARTS_IMPORT void SetData(i32 _Arg, bool arg2);

    // ?SetReplayDesc@mmReplayManager@@QAEXPAD@Z
    ARTS_IMPORT void SetReplayDesc(char* arg1);

public:
    static mmReplayManager*& Access_Instance() { return Instance; }

private:
    // ?GetReplayInfo@mmReplayManager@@AAEXXZ
    ARTS_IMPORT void GetReplayInfo();

    // ?ProcessCam@mmReplayManager@@AAEXXZ
    ARTS_IMPORT void ProcessCam();

    // ?ReadReplayInfo@mmReplayManager@@AAEXPAVStream@@@Z
    ARTS_IMPORT void ReadReplayInfo(Stream* stream);

    // ?SetReplayInfo@mmReplayManager@@AAEXXZ
    ARTS_IMPORT void SetReplayInfo();

    // ?StartReplay@mmReplayManager@@AAEXXZ
    ARTS_IMPORT void StartReplay();

    // ?WriteReplayInfo@mmReplayManager@@AAEXPAVStream@@@Z
    ARTS_IMPORT void WriteReplayInfo(Stream* arg1);

    // ?Instance@mmReplayManager@@0PAV1@A
    ARTS_IMPORT static mmReplayManager* Instance;

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i8 field_18; // 0x018
    i8 field_19; // 0x019
    i8 field_1A; // 0x01A
    i8 field_1B; // 0x01B
    i8 field_1C; // 0x01C
    i8 field_1D; // 0x01D
    i8 field_1E; // 0x01E
    i8 field_1F; // 0x01F
    u32 CurrentFrame; // 0x020
    i32 FrameCount; // 0x024
    i32 field_28; // 0x028
    i32 field_2C; // 0x02C
    i8 field_30; // 0x030
    i8 field_31; // 0x031
    i8 field_32; // 0x032
    i8 field_33; // 0x033
    i32 field_34; // 0x034
    f32 field_38; // 0x038
    f32 field_3C; // 0x03C
    i16 field_40; // 0x040
    i8 SomeBuffer[80]; // 0x042
    i16 field_92; // 0x092
    Vector4 CameraPoints[100]; // 0x094
    i32 field_6D4; // 0x6D4
    i32 RandSeed; // 0x6D8
    i8 CityName[40]; // 0x6DC
    i8 CityName2[40]; // 0x704
    u32 VehicleID; // 0x72C
    u32 field_730; // 0x730
    u32 field_734; // 0x734
    u32 RaceID; // 0x738
    f32 Difficulty; // 0x73C
    u32 TransmissionType; // 0x740
    f32 TrafficDensity; // 0x744
    f32 PedestrianDensity; // 0x748
    f32 CopDensity; // 0x74C
    f32 OpponentDensity; // 0x750
    f32 PhysicsRealism; // 0x754
    u32 WeatherType; // 0x758
    u32 TimeOfDay; // 0x75C
    i8 VehicleName[80]; // 0x760
    u32 VehiclePaintJob; // 0x7B0
    i8 NetName[80]; // 0x7B4
    f32 RaceTimeLimit; // 0x804
    i32 DisableRegen; // 0x808
    u32 DisableAI; // 0x80C
    u32 field_810; // 0x810
    u32 MaxAmbientVehicles; // 0x814
    u32 NumLaps; // 0x818
    u32 field_81C; // 0x81C
    u32 GameMode; // 0x820
    u32 DisableProfile; // 0x824
    u32 field_828; // 0x828
    i32 field_82C; // 0x82C
    i32 field_830; // 0x830
    u32 field_834; // 0x834
    u32 field_838; // 0x838
    u32 field_83C; // 0x83C
};

// ??_7mmReplayManager@@6B@
// vtable at 0x005B0444
check_size(mmReplayManager, 0x840);
