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

enum aiMapCompTypes : i32;

class CArrayList;
class Matrix34;
class Vector3;
class Vector4;
class aiCTFRacer;
class aiCableCar;
class aiIntersection;
class aiMapEntry;
class aiPath;
class aiPedestrian;
class aiPoliceOfficer;
class aiRailSet;
class aiRouteRacer;
class aiSubway;
class aiVehicleAmbient;
class aiVehiclePlayer;
class aiVehicleSpline;
struct dgStatePack;
class mcHookman;
class vehCar;

class aiMap : public asNode
{
public:
    // ??0aiMap@@QAE@XZ
    ARTS_IMPORT aiMap();

    // ??1aiMap@@UAE@XZ
    ARTS_IMPORT virtual ~aiMap();

    // ?Cull@aiMap@@UAEXXZ
    ARTS_IMPORT virtual void Cull();

    // ?Update@aiMap@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@aiMap@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?UpdatePaused@aiMap@@UAEXXZ
    ARTS_IMPORT virtual void UpdatePaused();

    // ?AddAmbient@aiMap@@QAEXPAVaiVehicleSpline@@@Z
    ARTS_IMPORT void AddAmbient(aiVehicleSpline* arg1);

    // ?AddPedestrian@aiMap@@QAEXPAVaiPedestrian@@@Z
    ARTS_IMPORT void AddPedestrian(aiPedestrian* arg1);

    // ?AddPlayer@aiMap@@QAEHPAVvehCar@@@Z
    ARTS_IMPORT i32 AddPlayer(vehCar* arg1);

    // ?AddRoutingNode@aiMap@@QAEXH@Z
    ARTS_IMPORT void AddRoutingNode(i32 arg1);

    // ?AllwaysGreen@aiMap@@QAEXXZ
    ARTS_IMPORT void AllwaysGreen();

    // ?AllwaysRed@aiMap@@QAEXXZ
    ARTS_IMPORT void AllwaysRed();

    // ?CableCar@aiMap@@QAEPAVaiCableCar@@H@Z
    ARTS_IMPORT aiCableCar* CableCar(i32 arg1);

    // ?CalcRoute@aiMap@@QAEXABVMatrix34@@ABVVector3@@1PAF2FF_N@Z
    ARTS_IMPORT void CalcRoute(const Matrix34& arg1, const Vector3& arg2, const Vector3& arg3, i16* arg4, i16* arg5, i16 arg6, i16 arg7, bool arg8);

    // ?ChooseNextFreewayLink@aiMap@@QAEHPAVaiRailSet@@@Z
    ARTS_IMPORT i32 ChooseNextFreewayLink(aiRailSet* arg1);

    // ?ChooseNextLaneLink@aiMap@@QAEHPAVaiRailSet@@@Z
    ARTS_IMPORT i32 ChooseNextLaneLink(aiRailSet* arg1);

    // ?ChooseNextLeftStraightLink@aiMap@@QAEHPAVaiRailSet@@@Z
    ARTS_IMPORT i32 ChooseNextLeftStraightLink(aiRailSet* arg1);

    // ?ChooseNextRandomLink@aiMap@@QAEHPAVaiRailSet@@@Z
    ARTS_IMPORT i32 ChooseNextRandomLink(aiRailSet* arg1);

    // ?ChooseNextRightLink@aiMap@@QAEHPAVaiRailSet@@@Z
    ARTS_IMPORT i32 ChooseNextRightLink(aiRailSet* arg1);

    // ?ChooseNextRightStraightFreewayLink@aiMap@@QAEHPAVaiRailSet@@@Z
    ARTS_IMPORT i32 ChooseNextRightStraightFreewayLink(aiRailSet* arg1);

    // ?ChooseNextRightStraightLink@aiMap@@QAEHPAVaiRailSet@@@Z
    ARTS_IMPORT i32 ChooseNextRightStraightLink(aiRailSet* arg1);

    // ?ChooseNextStraightLink@aiMap@@QAEHPAVaiRailSet@@@Z
    ARTS_IMPORT i32 ChooseNextStraightLink(aiRailSet* arg1);

    // ?ChooseStraightLinkAt4Way@aiMap@@QAEHPAVaiRailSet@@@Z
    ARTS_IMPORT i32 ChooseStraightLinkAt4Way(aiRailSet* arg1);

    // ?Clean@aiMap@@QAEXXZ
    ARTS_IMPORT void Clean();

    // ?ClearPeds@aiMap@@QAEXPAVaiPath@@@Z
    ARTS_IMPORT void ClearPeds(aiPath* arg1);

    // ?CoreMapComponent@aiMap@@QAEHABVVector3@@PAF1FF@Z
    ARTS_IMPORT i32 CoreMapComponent(const Vector3& arg1, i16* arg2, i16* arg3, i16 arg4, i16 arg5);

    // ?CTFOpponent@aiMap@@QAEPAVaiCTFRacer@@H@Z
    ARTS_IMPORT aiCTFRacer* CTFOpponent(i32 arg1);

    // ?DetermineRoadPosInfo@aiMap@@QAEHABVMatrix34@@PAVaiRailSet@@PAFPAM23HH@Z
    ARTS_IMPORT i32 DetermineRoadPosInfo(const Matrix34& arg1, aiRailSet* arg2, i16* arg3, f32* arg4, i16* arg5, f32* arg6, i32 arg7, i32 arg8);

    // ?DetRdSegBetweenInts@aiMap@@QAEPAVaiPath@@HH@Z
    ARTS_IMPORT aiPath* DetRdSegBetweenInts(i32 arg1, i32 arg2);

    // ?DetRdSegBetweenInts@aiMap@@QAEPAVaiPath@@PAVaiIntersection@@0PA_N@Z
    ARTS_IMPORT aiPath* DetRdSegBetweenInts(aiIntersection* arg1, aiIntersection* arg2, bool* arg3);

    // ?DrawAppRoads@aiMap@@QAEXXZ
    ARTS_IMPORT void DrawAppRoads();

    // ?DrawFinalCheckPoint@aiMap@@QAEXXZ
    ARTS_IMPORT void DrawFinalCheckPoint();

    // ?DrawMap@aiMap@@QAEXXZ
    ARTS_IMPORT void DrawMap();

    // ?DrawNextVisibleAmbient@aiMap@@QAEXXZ
    ARTS_IMPORT void DrawNextVisibleAmbient();

    // ?DrawPathIds@aiMap@@QAEXXZ
    ARTS_IMPORT void DrawPathIds();

    // ?DrawX@aiMap@@QAEXAAVVector3@@@Z
    ARTS_IMPORT void DrawX(Vector3& arg1);

    // ?Dump@aiMap@@QAEXXZ
    ARTS_IMPORT void Dump();

    // ?FindAmbAppRoad@aiMap@@QAEHH@Z
    ARTS_IMPORT i32 FindAmbAppRoad(i32 arg1);

    // ?FindPedAppRoad@aiMap@@QAEHH@Z
    ARTS_IMPORT i32 FindPedAppRoad(i32 arg1);

    // ?GetPathFromID@aiMap@@QAEPAVaiPath@@H@Z
    ARTS_IMPORT aiPath* GetPathFromID(i32 arg1);

    // ?GoodRoadForCopStartPos@aiMap@@QAEHH@Z
    ARTS_IMPORT i32 GoodRoadForCopStartPos(i32 arg1);

    // ?Hookman@aiMap@@QAEPAVmcHookman@@H@Z
    ARTS_IMPORT mcHookman* Hookman(i32 arg1);

    // ?Init@aiMap@@QAEXPAD00ABUdgStatePack@@HPAPAVvehCar@@_N@Z
    ARTS_IMPORT void Init(char* arg1, char* arg2, char* arg3, const dgStatePack& arg4, i32 arg5, vehCar** arg6, bool arg7);

    // ?InitRouting@aiMap@@QAEXXZ
    ARTS_IMPORT void InitRouting();

    // ?Intersection@aiMap@@QAEPAVaiIntersection@@H@Z
    ARTS_IMPORT aiIntersection* Intersection(i32 arg1);

    // ?MapComponent@aiMap@@QAEHABVVector3@@PAF1F@Z
    ARTS_IMPORT i32 MapComponent(const Vector3& arg1, i16* arg2, i16* arg3, i16 arg4);

    // ?MapComponent@aiMap@@QAEHABVVector3@@PAF1FF@Z
    ARTS_IMPORT i32 MapComponent(const Vector3& arg1, i16* arg2, i16* arg3, i16 arg4, i16 arg5);

    // ?MapComponentType@aiMap@@QAEHHPAH@Z
    ARTS_IMPORT i32 MapComponentType(i32 arg1, i32* arg2);

    // ?Opponent@aiMap@@QAEPAVaiRouteRacer@@H@Z
    ARTS_IMPORT aiRouteRacer* Opponent(i32 arg1);

    // ?Path@aiMap@@QAEPAVaiPath@@H@Z
    ARTS_IMPORT aiPath* Path(i32 arg1);

    // ?Pedestrian@aiMap@@QAEPAVaiPedestrian@@H@Z
    ARTS_IMPORT aiPedestrian* Pedestrian(i32 arg1);

    // ?Player@aiMap@@QAEPAVaiVehiclePlayer@@H@Z
    ARTS_IMPORT aiVehiclePlayer* Player(i32 arg1);

    // ?Police@aiMap@@QAEPAVaiPoliceOfficer@@H@Z
    ARTS_IMPORT aiPoliceOfficer* Police(i32 arg1);

    // ?PositionToAIMapComp@aiMap@@QAEHABVVector3@@PAF11F@Z
    ARTS_IMPORT i32 PositionToAIMapComp(const Vector3& arg1, i16* arg2, i16* arg3, i16* arg4, i16 arg5);

    // ?PredictAmbFreewayIntersectionPath@aiMap@@QAEPAVaiPath@@HABVMatrix34@@@Z
    ARTS_IMPORT aiPath* PredictAmbFreewayIntersectionPath(i32 arg1, const Matrix34& arg2);

    // ?PredictAmbIntersectionPath@aiMap@@QAEPAVaiPath@@HABVMatrix34@@@Z
    ARTS_IMPORT aiPath* PredictAmbIntersectionPath(i32 arg1, const Matrix34& arg2);

    // ?PredictIntersectionPath@aiMap@@QAEPAVaiPath@@HABVMatrix34@@HPA_N@Z
    ARTS_IMPORT aiPath* PredictIntersectionPath(i32 arg1, const Matrix34& arg2, i32 arg3, bool* arg4);

    // ?ReadBinary@aiMap@@QAEXPAD0@Z
    ARTS_IMPORT void ReadBinary(char* arg1, char* arg2);

    // ?RemoveAmbient@aiMap@@QAEXPAVaiVehicleSpline@@@Z
    ARTS_IMPORT void RemoveAmbient(aiVehicleSpline* arg1);

    // ?RemovePedestrian@aiMap@@QAEXPAVaiPedestrian@@@Z
    ARTS_IMPORT void RemovePedestrian(aiPedestrian* arg1);

    // ?RemovePlayer@aiMap@@QAEHPAVvehCar@@@Z
    ARTS_IMPORT i32 RemovePlayer(vehCar* arg1);

    // ?RemoveRoutingNode@aiMap@@QAEXH@Z
    ARTS_IMPORT void RemoveRoutingNode(i32 arg1);

    // ?SaveBinary@aiMap@@QAEXPAD0@Z
    ARTS_IMPORT void SaveBinary(char* arg1, char* arg2);

    // ?SetWaypoints@aiMap@@QAEXPAVVector4@@HH@Z
    ARTS_IMPORT void SetWaypoints(Vector4* arg1, i32 arg2, i32 arg3);

    // ?Subway@aiMap@@QAEPAVaiSubway@@H@Z
    ARTS_IMPORT aiSubway* Subway(i32 arg1);

    // ?TestProbes@aiMap@@QAEXH@Z
    ARTS_IMPORT void TestProbes(i32 arg1);

    // ?Vehicle@aiMap@@QAEPAVaiVehicleAmbient@@H@Z
    ARTS_IMPORT aiVehicleAmbient* Vehicle(i32 arg1);

private:
    // ?AdjustAmbients@aiMap@@AAEXHHH@Z
    ARTS_IMPORT void AdjustAmbients(i32 arg1, i32 arg2, i32 arg3);

    // ?AdjustPedestrians@aiMap@@AAEXHHH@Z
    ARTS_IMPORT void AdjustPedestrians(i32 arg1, i32 arg2, i32 arg3);

    // ?FindInt@aiMap@@AAEHPAFHH@Z
    ARTS_IMPORT i32 FindInt(i16* arg1, i32 arg2, i32 arg3);

    // ?FindRoad@aiMap@@AAEHPAVCArrayList@@H@Z
    ARTS_IMPORT i32 FindRoad(CArrayList* arg1, i32 arg2);

    // ?MapRoadToRooms@aiMap@@AAEXPAVaiPath@@W4aiMapCompTypes@@@Z
    ARTS_IMPORT void MapRoadToRooms(aiPath* arg1, aiMapCompTypes arg2);

    // ?NumCars@aiMap@@AAEHHMM@Z
    ARTS_IMPORT i32 NumCars(i32 arg1, f32 arg2, f32 arg3);

    // ?StopRoadTraffic@aiMap@@AAEX_N@Z
    ARTS_IMPORT void StopRoadTraffic(bool arg1);

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u32* field_24; // 0x024
    u16 NumIntersections; // 0x028
    aiPath** pPaths; // 0x02C
    i16 NumPaths; // 0x030
    u8 field_32[122]; // 0x032
    i16 ShortcutCount; // 0x0AC
    u8 field_AE[198]; // 0x0AE
    u32 field_174; // 0x174
    u32 field_178; // 0x178
    u8 field_17C[20]; // 0x17C
    aiMapEntry* field_190; // 0x190
};

// ??_7aiMap@@6B@
// vtable at 0x005B5460
check_size(aiMap, 0x194);
