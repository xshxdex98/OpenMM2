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

#include "aivehicle.h"

class Matrix34;
class Vector3;
class aiObstacle;
class aiPath;
class vehCar;

class aiVehiclePhysics : public aiVehicle
{
public:
    // ??0aiVehiclePhysics@@QAE@XZ
    ARTS_IMPORT aiVehiclePhysics();

    // ??1aiVehiclePhysics@@QAE@XZ
    ARTS_IMPORT ~aiVehiclePhysics();

    // ?Position@aiVehiclePhysics@@UAEXAAVVector3@@@Z
    ARTS_IMPORT virtual void Position(Vector3& arg1);

    // ?Speed@aiVehiclePhysics@@UAEMXZ
    ARTS_IMPORT virtual f32 Speed();

    // ?CurrentRoadIdx@aiVehiclePhysics@@UAEHPAPAVaiPath@@QA_NPAH@Z
    ARTS_IMPORT virtual i32 CurrentRoadIdx(aiPath** arg1, bool*const arg2, i32* arg3);

    // ?CurrentRdVert@aiVehiclePhysics@@UAEHXZ
    ARTS_IMPORT virtual i32 CurrentRdVert();

    // ?Reset@aiVehiclePhysics@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Type@aiVehiclePhysics@@UAEHXZ
    ARTS_IMPORT virtual i32 Type();

    // ?GetMatrix@aiVehiclePhysics@@UAEAAVMatrix34@@XZ
    ARTS_IMPORT virtual Matrix34& GetMatrix();

    // ?FrontBumperDistance@aiVehiclePhysics@@UAEMXZ
    ARTS_IMPORT virtual f32 FrontBumperDistance();

    // ?BackBumperDistance@aiVehiclePhysics@@UAEMXZ
    ARTS_IMPORT virtual f32 BackBumperDistance();

    // ?LSideDistance@aiVehiclePhysics@@UAEMXZ
    ARTS_IMPORT virtual f32 LSideDistance();

    // ?RSideDistance@aiVehiclePhysics@@UAEMXZ
    ARTS_IMPORT virtual f32 RSideDistance();

    // ?CurrentLane@aiVehiclePhysics@@UAEHXZ
    ARTS_IMPORT virtual i32 CurrentLane();

    // ?CurrentRoadId@aiVehiclePhysics@@UAEHXZ
    ARTS_IMPORT virtual i32 CurrentRoadId();

    // ?DrawId@aiVehiclePhysics@@UAEXXZ
    ARTS_IMPORT virtual void DrawId();

    // ?ReplayDebug@aiVehiclePhysics@@UAEXXZ
    ARTS_IMPORT virtual void ReplayDebug();

    // ?DrawRouteThroughTraffic@aiVehiclePhysics@@QAEXXZ
    ARTS_IMPORT void DrawRouteThroughTraffic();

    // ?DrawTurn@aiVehiclePhysics@@QAEXH@Z
    ARTS_IMPORT void DrawTurn(i32 arg1);

    // ?DriveRoute@aiVehiclePhysics@@QAEXH@Z
    ARTS_IMPORT void DriveRoute(i32 arg1);

    // ?Dump@aiVehiclePhysics@@QAEXXZ
    ARTS_IMPORT void Dump();

    // ?Init@aiVehiclePhysics@@QAEXHPADFH@Z
    ARTS_IMPORT void Init(i32 arg1, char* arg2, i16 arg3, i32 arg4);

    // ?Mirror@aiVehiclePhysics@@QAEXPAVvehCar@@@Z
    ARTS_IMPORT void Mirror(vehCar* arg1);

    // ?RegisterRoute@aiVehiclePhysics@@QAEXPAFFABVVector3@@1HMM_N22222MMMM@Z
    ARTS_IMPORT void RegisterRoute(i16* arg1, i16 arg2, const Vector3& arg3, const Vector3& arg4, i32 arg5, f32 arg6, f32 arg7, bool arg8, bool arg9, bool arg10, bool arg11, bool arg12, bool arg13, f32 arg14, f32 arg15, f32 arg16, f32 arg17);

    // ?StopRoadTraffic@aiVehiclePhysics@@QAEX_N@Z
    ARTS_IMPORT void StopRoadTraffic(bool arg1);

private:
    // ?Backup@aiVehiclePhysics@@AAEXXZ
    ARTS_IMPORT void Backup();

    // ?CalcCurrentMaxWidthAdjustment@aiVehiclePhysics@@AAEMH@Z
    ARTS_IMPORT f32 CalcCurrentMaxWidthAdjustment(i32 arg1);

    // ?CalcCurrentRdOffset@aiVehiclePhysics@@AAEMH@Z
    ARTS_IMPORT f32 CalcCurrentRdOffset(i32 arg1);

    // ?CalcDestinationTarget@aiVehiclePhysics@@AAEXHAAVVector3@@@Z
    ARTS_IMPORT void CalcDestinationTarget(i32 arg1, Vector3& arg2);

    // ?CalcNextMaxWidthAdjustment@aiVehiclePhysics@@AAEMH@Z
    ARTS_IMPORT f32 CalcNextMaxWidthAdjustment(i32 arg1);

    // ?CalcNextRdOffset@aiVehiclePhysics@@AAEMH@Z
    ARTS_IMPORT f32 CalcNextRdOffset(i32 arg1);

    // ?CalcObstacleAvoidPoints@aiVehiclePhysics@@AAEHPAVaiObstacle@@HHPAVVector3@@PAPAV2@PAF3@Z
    ARTS_IMPORT i32 CalcObstacleAvoidPoints(aiObstacle* arg1, i32 arg2, i32 arg3, Vector3* arg4, aiObstacle** arg5, i16* arg6, i16* arg7);

    // ?CalcRoadSpeed@aiVehiclePhysics@@AAEXXZ
    ARTS_IMPORT void CalcRoadSpeed();

    // ?CalcRoadTarget@aiVehiclePhysics@@AAEXHAAVVector3@@@Z
    ARTS_IMPORT void CalcRoadTarget(i32 arg1, Vector3& arg2);

    // ?CalcRoadTurns@aiVehiclePhysics@@AAEXXZ
    ARTS_IMPORT void CalcRoadTurns();

    // ?CalcRoute@aiVehiclePhysics@@AAEXXZ
    ARTS_IMPORT void CalcRoute();

    // ?CalcSharpTurnTarget@aiVehiclePhysics@@AAEHAAHHAAVVector3@@@Z
    ARTS_IMPORT i32 CalcSharpTurnTarget(i32& arg1, i32 arg2, Vector3& arg3);

    // ?CalcSpeed@aiVehiclePhysics@@AAEXXZ
    ARTS_IMPORT void CalcSpeed();

    // ?CalcTurnIntersection@aiVehiclePhysics@@AAEMH@Z
    ARTS_IMPORT f32 CalcTurnIntersection(i32 arg1);

    // ?CheckDistance@aiVehiclePhysics@@AAEMH@Z
    ARTS_IMPORT f32 CheckDistance(i32 arg1);

    // ?CheckForShortcut@aiVehiclePhysics@@AAEXXZ
    ARTS_IMPORT void CheckForShortcut();

    // ?ContinueCheck@aiVehiclePhysics@@AAEXH@Z
    ARTS_IMPORT void ContinueCheck(i32 arg1);

    // ?DestMapComponent@aiVehiclePhysics@@AAEXABVVector3@@PAF1@Z
    ARTS_IMPORT void DestMapComponent(const Vector3& arg1, i16* arg2, i16* arg3);

    // ?DetermineBestRoute@aiVehiclePhysics@@AAEXXZ
    ARTS_IMPORT void DetermineBestRoute();

    // ?EnumRoutes@aiVehiclePhysics@@AAEXH@Z
    ARTS_IMPORT void EnumRoutes(i32 arg1);

    // ?EnumTargets@aiVehiclePhysics@@AAEXAAVVector3@@PAVaiObstacle@@HHHHHHPAV2@PAPAV3@PAF4PAH@Z
    ARTS_IMPORT void EnumTargets(Vector3& arg1, aiObstacle* arg2, i32 arg3, i32 arg4, i32 arg5, i32 arg6, i32 arg7, i32 arg8, Vector3* arg9, aiObstacle** arg10, i16* arg11, i16* arg12, i32* arg13);

    // ?FinishedBackingUp@aiVehiclePhysics@@AAEXXZ
    ARTS_IMPORT void FinishedBackingUp();

    // ?Forward@aiVehiclePhysics@@AAEXXZ
    ARTS_IMPORT void Forward();

    // ?InitBackup@aiVehiclePhysics@@AAEXXZ
    ARTS_IMPORT void InitBackup();

    // ?InitForward@aiVehiclePhysics@@AAEXXZ
    ARTS_IMPORT void InitForward();

    // ?InitRoadTurns@aiVehiclePhysics@@AAEXXZ
    ARTS_IMPORT void InitRoadTurns();

    // ?InitShortcut@aiVehiclePhysics@@AAEXXZ
    ARTS_IMPORT void InitShortcut();

    // ?InSharpTurn@aiVehiclePhysics@@AAEHH@Z
    ARTS_IMPORT i32 InSharpTurn(i32 arg1);

    // ?IsTargetBlocked@aiVehiclePhysics@@AAEPAVaiObstacle@@AAVVector3@@0HHHHMPAH@Z
    ARTS_IMPORT aiObstacle* IsTargetBlocked(Vector3& arg1, Vector3& arg2, i32 arg3, i32 arg4, i32 arg5, i32 arg6, f32 arg7, i32* arg8);

    // ?LocateWayPtFromInt@aiVehiclePhysics@@AAEHH@Z
    ARTS_IMPORT i32 LocateWayPtFromInt(i32 arg1);

    // ?LocateWayPtFromRoad@aiVehiclePhysics@@AAEHPAVaiPath@@@Z
    ARTS_IMPORT i32 LocateWayPtFromRoad(aiPath* arg1);

    // ?PlanRoute@aiVehiclePhysics@@AAEHXZ
    ARTS_IMPORT i32 PlanRoute();

    // ?SaveTarget@aiVehiclePhysics@@AAEXHAAVVector3@@PAVaiObstacle@@AAFH2@Z
    ARTS_IMPORT void SaveTarget(i32 arg1, Vector3& arg2, aiObstacle* arg3, i16& arg4, i32 arg5, i16& arg6);

    // ?SaveTurnTarget@aiVehiclePhysics@@AAEXHH@Z
    ARTS_IMPORT void SaveTurnTarget(i32 arg1, i32 arg2);

    // ?SetTargetPtToDestination@aiVehiclePhysics@@AAEXH@Z
    ARTS_IMPORT void SetTargetPtToDestination(i32 arg1);

    // ?Shortcut@aiVehiclePhysics@@AAEXXZ
    ARTS_IMPORT void Shortcut();

    // ?SolveRoadTargetPoint@aiVehiclePhysics@@AAEXXZ
    ARTS_IMPORT void SolveRoadTargetPoint();

    // ?SolveShortcutTargetPoint@aiVehiclePhysics@@AAEXXZ
    ARTS_IMPORT void SolveShortcutTargetPoint();

    // ?Stop@aiVehiclePhysics@@AAEXXZ
    ARTS_IMPORT void Stop();
};

// ??_7aiVehiclePhysics@@6B@
// vtable at 0x005B5A7C
// check_size(aiVehiclePhysics, 0x9770); // size known, members are not - cannot verify
