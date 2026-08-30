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

#include "vector7/vector3.h"

class Stream;
class aiObstacle;
class aiPath;
class aiTrafficLightSet;
class aiVehicle;

class aiIntersection
{
public:
    // ??0aiIntersection@@QAE@H@Z
    ARTS_EXPORT aiIntersection(i32 arg1);

    // ??1aiIntersection@@QAE@XZ
    ARTS_IMPORT ~aiIntersection();

    // ?AddBanger@aiIntersection@@QAEXPAVaiObstacle@@@Z
    ARTS_EXPORT void AddBanger(aiObstacle* arg1);

    // ?AddBangersToObsMap@aiIntersection@@QAEXXZ
    ARTS_IMPORT void AddBangersToObsMap();

    // ?AddRoad@aiIntersection@@QAEXPAVaiPath@@@Z
    ARTS_EXPORT void AddRoad(aiPath* road);

    // ?AddToStopSignCntl@aiIntersection@@QAEXPAVaiVehicle@@@Z
    ARTS_EXPORT void AddToStopSignCntl(aiVehicle* arg1);

    // ?AddVehicle@aiIntersection@@QAEXPAVaiObstacle@@@Z
    ARTS_EXPORT void AddVehicle(aiObstacle* arg1);

    // ?CreateRoadMap@aiIntersection@@QAEXXZ
    ARTS_IMPORT void CreateRoadMap();

    // ?Dump@aiIntersection@@QAEXXZ
    ARTS_EXPORT void Dump();

    // ?Init@aiIntersection@@QAEXXZ
    ARTS_IMPORT void Init();

    // ?IsCableCarStart@aiIntersection@@QAE_NPAHPAF@Z
    ARTS_IMPORT bool IsCableCarStart(i32* arg1, i16* arg2);

    // ?IsSubwayEnd@aiIntersection@@QAE_NPAH@Z
    ARTS_IMPORT bool IsSubwayEnd(i32* arg1);

    // ?IsSubwayStart@aiIntersection@@QAE_NPAH00@Z
    ARTS_IMPORT bool IsSubwayStart(i32* arg1, i32* arg2, i32* arg3);

    // ?NumAvailSinks@aiIntersection@@QAEHPAVaiPath@@@Z
    ARTS_EXPORT i32 NumAvailSinks(aiPath* arg1);

    // ?NumOneWays@aiIntersection@@QAEHXZ
    ARTS_EXPORT i32 NumOneWays();

    // ?NumSinks@aiIntersection@@QAEHXZ
    ARTS_EXPORT i32 NumSinks();

    // ?NumSources@aiIntersection@@QAEHXZ
    ARTS_EXPORT i32 NumSources();

    // ?Path@aiIntersection@@QAEPAVaiPath@@H@Z
    ARTS_EXPORT aiPath* Path(i32 arg1);

    // ?ReadBinary@aiIntersection@@QAEXPAVStream@@@Z
    ARTS_IMPORT void ReadBinary(Stream* arg1);

    // ?RemoveBanger@aiIntersection@@QAEXPAVaiObstacle@@@Z
    ARTS_EXPORT void RemoveBanger(aiObstacle* arg1);

    // ?RemoveFromStopSignCntl@aiIntersection@@QAEXPAVaiVehicle@@@Z
    ARTS_EXPORT void RemoveFromStopSignCntl(aiVehicle* arg1);

    // ?RemoveTotalFromStopSignCntl@aiIntersection@@QAEXPAVaiVehicle@@@Z
    ARTS_EXPORT void RemoveTotalFromStopSignCntl(aiVehicle* arg1);

    // ?RemoveVehicle@aiIntersection@@QAEXPAVaiObstacle@@@Z
    ARTS_EXPORT void RemoveVehicle(aiObstacle* arg1);

    // ?Reset@aiIntersection@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?Road@aiIntersection@@QAEPAVaiPath@@H@Z
    ARTS_EXPORT aiPath* Road(i32 arg1);

    // ?SaveBinary@aiIntersection@@QAEXPAVStream@@@Z
    ARTS_IMPORT void SaveBinary(Stream* arg1);

    // ?SetFourWay@aiIntersection@@QAEXXZ
    ARTS_EXPORT void SetFourWay();

    // ?StopSignOkayToGo@aiIntersection@@QAE_NPAVaiVehicle@@@Z
    ARTS_IMPORT bool StopSignOkayToGo(aiVehicle* arg1);

    // ?StopSources@aiIntersection@@QAEXH@Z
    ARTS_EXPORT void StopSources(i32 arg1);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    aiPath** Roads; // 0x000
    i16 RoadCount; // 0x004
    aiVehicle* PrevVeh; // 0x008
    aiVehicle* NextVeh; // 0x00C
    u16 RoomId; // 0x010
    u16 IntersectionId; // 0x012
    Vector3 Center; // 0x014
    aiTrafficLightSet* TrafficLights; // 0x020
    aiObstacle* Vehicles; // 0x024
    aiObstacle* Bangers; // 0x028
};

check_size(aiIntersection, 0x2C);
