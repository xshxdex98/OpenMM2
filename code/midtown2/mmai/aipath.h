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

class Matrix34;
class Stream;
class Vector3;
class aiIntersection;
class aiObstacle;
class aiPedestrian;
class aiVehicleSpline;

class aiPath
{
public:
    // ??0aiPath@@QAE@H@Z
    ARTS_IMPORT aiPath(i32 DrawList);

    // ??1aiPath@@QAE@XZ
    ARTS_IMPORT ~aiPath();

    // ?AddAmbPlayer@aiPath@@QAEDH@Z
    ARTS_IMPORT char AddAmbPlayer(i32 arg1);

    // ?AddAmbVehicle@aiPath@@QAEXPAVaiVehicleSpline@@HMH@Z
    ARTS_IMPORT void AddAmbVehicle(aiVehicleSpline* DrawList, i32 __formal, f32 uv, i32 prim);

    // ?AddBanger@aiPath@@QAEXPAVaiObstacle@@HH@Z
    ARTS_IMPORT void AddBanger(aiObstacle* arg1, i32 arg2, i32 arg3);

    // ?AddBangersToObsMap@aiPath@@QAEXXZ
    ARTS_IMPORT void AddBangersToObsMap();

    // ?AddPedestrian@aiPath@@QAEXPAVaiObstacle@@HH@Z
    ARTS_IMPORT void AddPedestrian(aiObstacle* arg1, i32 arg2, i32 arg3);

    // ?AddPedestrian@aiPath@@QAEXPAVaiPedestrian@@@Z
    ARTS_IMPORT void AddPedestrian(aiPedestrian* arg1);

    // ?AddPedPlayer@aiPath@@QAEDH@Z
    ARTS_IMPORT char AddPedPlayer(i32 arg1);

    // ?AddVehicle@aiPath@@QAEXPAVaiObstacle@@HH@Z
    ARTS_IMPORT void AddVehicle(aiObstacle* arg1, i32 arg2, i32 arg3);

    // ?AllwaysGo@aiPath@@QAEXH@Z
    ARTS_IMPORT void AllwaysGo(i32 arg1);

    // ?AllwaysStop@aiPath@@QAEXH@Z
    ARTS_IMPORT void AllwaysStop(i32 arg1);

    // ?CalcRoadTurns@aiPath@@QAEXAAVVector3@@_N@Z
    ARTS_IMPORT void CalcRoadTurns(Vector3& DrawList, bool __formal);

    // ?CenterDist@aiPath@@QAEMABVVector3@@@Z
    ARTS_IMPORT f32 CenterDist(const Vector3& arg1);

    // ?CenterDistance@aiPath@@QAEHABVVector3@@PAFPAM2@Z
    ARTS_IMPORT i32 CenterDistance(const Vector3& arg1, i16* arg2, f32* arg3, f32* arg4);

    // ?CenterIndex@aiPath@@QAEHM@Z
    ARTS_IMPORT i32 CenterIndex(f32 arg1);

    // ?CenterLength@aiPath@@QAEMHH@Z
    ARTS_IMPORT f32 CenterLength(i32 arg1, i32 arg2);

    // ?CenterPosition@aiPath@@QAEXMAAVVector3@@@Z
    ARTS_IMPORT void CenterPosition(f32 arg1, Vector3& arg2);

    // ?ClearAmbients@aiPath@@QAEXXZ
    ARTS_IMPORT void ClearAmbients();

    // ?ClearPeds@aiPath@@QAEXXZ
    ARTS_IMPORT void ClearPeds();

    // ?DetermineRoadPosInfo@aiPath@@QAEXABVMatrix34@@PAFPAM121@Z
    ARTS_IMPORT void DetermineRoadPosInfo(const Matrix34& arg1, i16* arg2, f32* arg3, i16* arg4, f32* arg5, i16* arg6);

    // ?Direction@aiPath@@QAE_NABVMatrix34@@@Z
    ARTS_IMPORT bool Direction(const Matrix34& arg1);

    // ?GetHeading@aiPath@@QAEMMHH@Z
    ARTS_IMPORT f32 GetHeading(f32 arg1, i32 arg2, i32 arg3);

    // ?HasCableCarLine@aiPath@@QAE_NH@Z
    ARTS_IMPORT bool HasCableCarLine(i32 arg1);

    // ?HasSubwayLine@aiPath@@QAE_NH@Z
    ARTS_IMPORT bool HasSubwayLine(i32 arg1);

    // ?Index@aiPath@@QAEHABVVector3@@@Z
    ARTS_IMPORT i32 Index(const Vector3& arg1);

    // ?Index@aiPath@@QAEHMHH@Z
    ARTS_IMPORT i32 Index(f32 arg1, i32 arg2, i32 arg3);

    // ?InitRoadTurns@aiPath@@QAEXXZ
    ARTS_IMPORT void InitRoadTurns();

    // ?IntersectionEntryPt@aiPath@@QAEXAAVVector3@@HMH@Z
    ARTS_IMPORT void IntersectionEntryPt(Vector3& arg1, i32 arg2, f32 arg3, i32 arg4);

    // ?IntersectionEntryVector@aiPath@@QAEXAAVVector3@@MH@Z
    ARTS_IMPORT void IntersectionEntryVector(Vector3& DrawList, f32 __formal, i32 uv);

    // ?IntersectionExitVector@aiPath@@QAEXAAVVector3@@MH@Z
    ARTS_IMPORT void IntersectionExitVector(Vector3& arg1, f32 arg2, i32 arg3);

    // ?IsPosOnRoad@aiPath@@QAEHABVVector3@@MPAM@Z
    ARTS_IMPORT i32 IsPosOnRoad(const Vector3& DrawList, f32 __formal, f32* uv);

    // ?IsSharpTurn@aiPath@@QAEHH_N@Z
    ARTS_IMPORT i32 IsSharpTurn(i32 arg1, bool arg2);

    // ?Lane@aiPath@@QAEHAAVVector3@@H@Z
    ARTS_IMPORT i32 Lane(Vector3& arg1, i32 arg2);

    // ?NumVehiclesAfterDist@aiPath@@QAEHHMH@Z
    ARTS_IMPORT i32 NumVehiclesAfterDist(i32 arg1, f32 arg2, i32 arg3);

    // ?PopAmbVehicle@aiPath@@QAEXPAVaiVehicleSpline@@HH@Z
    ARTS_IMPORT void PopAmbVehicle(aiVehicleSpline* arg1, i32 arg2, i32 arg3);

    // ?Position@aiPath@@QAEXMAAVVector3@@HH@Z
    ARTS_IMPORT void Position(f32 arg1, Vector3& arg2, i32 arg3, i32 arg4);

    // ?PushAmbVehicle@aiPath@@QAEXPAVaiVehicleSpline@@HH@Z
    ARTS_IMPORT void PushAmbVehicle(aiVehicleSpline* DrawList, i32 __formal, i32 uv);

    // ?ReadBinary@aiPath@@QAEXPAVStream@@@Z
    ARTS_IMPORT void ReadBinary(Stream* DrawList);

    // ?ReadShortcut@aiPath@@QAEXPAVStream@@H@Z
    ARTS_IMPORT void ReadShortcut(Stream* arg1, i32 arg2);

    // ?RemAmbPlayer@aiPath@@QAEDH@Z
    ARTS_IMPORT char RemAmbPlayer(i32 arg1);

    // ?RemoveAmbVehicle@aiPath@@QAEHPAVaiVehicleSpline@@HH@Z
    ARTS_IMPORT i32 RemoveAmbVehicle(aiVehicleSpline* arg1, i32 arg2, i32 arg3);

    // ?RemoveBanger@aiPath@@QAEXPAVaiObstacle@@HH@Z
    ARTS_IMPORT void RemoveBanger(aiObstacle* arg1, i32 arg2, i32 arg3);

    // ?RemovePedestrian@aiPath@@QAEXPAVaiObstacle@@HH@Z
    ARTS_IMPORT void RemovePedestrian(aiObstacle* arg1, i32 arg2, i32 arg3);

    // ?RemovePedestrian@aiPath@@QAEXPAVaiPedestrian@@@Z
    ARTS_IMPORT void RemovePedestrian(aiPedestrian* arg1);

    // ?RemoveVehicle@aiPath@@QAEXPAVaiObstacle@@HH@Z
    ARTS_IMPORT void RemoveVehicle(aiObstacle* arg1, i32 arg2, i32 arg3);

    // ?RemPedPlayer@aiPath@@QAEDH@Z
    ARTS_IMPORT char RemPedPlayer(i32 DrawList);

    // ?Reset@aiPath@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?ResetObstacles@aiPath@@QAEXXZ
    ARTS_IMPORT void ResetObstacles();

    // ?ResetVehicleReactTicks@aiPath@@QAEXH@Z
    ARTS_IMPORT void ResetVehicleReactTicks(i32 arg1);

    // ?ReverseDirection@aiPath@@QAEXXZ
    ARTS_IMPORT void ReverseDirection();

    // ?RoadCapacity@aiPath@@QAEHPAVaiVehicleSpline@@HH@Z
    ARTS_IMPORT i32 RoadCapacity(aiVehicleSpline* arg1, i32 arg2, i32 arg3);

    // ?RoadDistance@aiPath@@QAEXABVVector3@@PAFPAMF2H@Z
    ARTS_IMPORT void RoadDistance(const Vector3& arg1, i16* arg2, f32* arg3, i16 arg4, f32* arg5, i32 arg6);

    // ?RoadVertice@aiPath@@QAEHABVVector3@@H@Z
    ARTS_IMPORT i32 RoadVertice(const Vector3& arg1, i32 arg2);

    // ?RoadVertice@aiPath@@QAEHABVVector3@@HH@Z
    ARTS_IMPORT i32 RoadVertice(const Vector3& DrawList, i32 __formal, i32 uv);

    // ?SaveBinary@aiPath@@QAEXPAVStream@@@Z
    ARTS_IMPORT void SaveBinary(Stream* arg1);

    // ?SetCullroom@aiPath@@QAEXXZ
    ARTS_IMPORT void SetCullroom();

    // ?SharpTurnAngle@aiPath@@QAEMH_N@Z
    ARTS_IMPORT f32 SharpTurnAngle(i32 arg1, bool arg2);

    // ?SharpTurnCenter@aiPath@@QAEAAVVector3@@H_N@Z
    ARTS_IMPORT Vector3& SharpTurnCenter(i32 arg1, bool arg2);

    // ?SharpTurnDir@aiPath@@QAEMH_N@Z
    ARTS_IMPORT f32 SharpTurnDir(i32 arg1, bool arg2);

    // ?SharpTurnEndDir@aiPath@@QAEAAVVector3@@H_N@Z
    ARTS_IMPORT Vector3& SharpTurnEndDir(i32 arg1, bool arg2);

    // ?SharpTurnIntersection@aiPath@@QAEAAVVector3@@H_N@Z
    ARTS_IMPORT Vector3& SharpTurnIntersection(i32 arg1, bool arg2);

    // ?SharpTurnRadius@aiPath@@QAEMH_N@Z
    ARTS_IMPORT f32 SharpTurnRadius(i32 arg1, bool arg2);

    // ?SharpTurnSetback@aiPath@@QAEMH_N@Z
    ARTS_IMPORT f32 SharpTurnSetback(i32 arg1, bool arg2);

    // ?SharpTurnStartDir@aiPath@@QAEAAVVector3@@H_N@Z
    ARTS_IMPORT Vector3& SharpTurnStartDir(i32 arg1, bool arg2);

    // ?SharpTurnVertIndex@aiPath@@QAEHH_N@Z
    ARTS_IMPORT i32 SharpTurnVertIndex(i32 arg1, bool arg2);

    // ?SidewalkSubSectionLength@aiPath@@QAEMHHF@Z
    ARTS_IMPORT f32 SidewalkSubSectionLength(i32 arg1, i32 arg2, i16 arg3);

    // ?SidewalkVertice@aiPath@@QAEAAVVector3@@HF@Z
    ARTS_IMPORT Vector3& SidewalkVertice(i32 arg1, i16 arg2);

    // ?SubSectionDir@aiPath@@QAEXAAVVector3@@HMH@Z
    ARTS_IMPORT void SubSectionDir(Vector3& arg1, i32 arg2, f32 arg3, i32 arg4);

    // ?SubSectionDist@aiPath@@QAEMMHH@Z
    ARTS_IMPORT f32 SubSectionDist(f32 arg1, i32 arg2, i32 arg3);

    // ?SubSectionLength@aiPath@@QAEMHHH@Z
    ARTS_IMPORT f32 SubSectionLength(i32 arg1, i32 arg2, i32 arg3);

    // ?SubSectionPt@aiPath@@QAEXAAVVector3@@HHMH@Z
    ARTS_IMPORT void SubSectionPt(Vector3& arg1, i32 arg2, i32 arg3, f32 arg4, i32 arg5);

    // ?UpdateAmbients@aiPath@@QAEXXZ
    ARTS_IMPORT void UpdateAmbients();

    // ?UpdatePedestrians@aiPath@@QAEXXZ
    ARTS_IMPORT void UpdatePedestrians();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u32 field_0; // 0x000
    u32 field_4; // 0x004
    u16 ObstacleCount; // 0x008
    u16 Id; // 0x00A
    u16 field_C; // 0x00C
    u16 CullRoomCount; // 0x00E
    u16* CullRooms; // 0x010
    f32 field_14; // 0x014
    f32 field_18; // 0x018
    u16 PedCount; // 0x01C
    u8 field_1E[2]; // 0x01E
    aiPedestrian* Peds; // 0x020
    u8 field_24[8]; // 0x024
    i8 AmbPlayerFlags; // 0x02C
    i8 field_2D; // 0x02D
    u32 field_30; // 0x030
    u32 field_34; // 0x034
    u16 SidewalkVerticesThing2; // 0x038
    u16 field_3A; // 0x03A
    u16 field_3C; // 0x03C
    u16 field_3E; // 0x03E
    u16 Flags; // 0x040
    f32* SubLengths2; // 0x044
    u32* field_48; // 0x048
    u8 field_4C[4]; // 0x04C
    f32 field_50; // 0x050
    u8 field_54[32]; // 0x054
    Vector3* SidewalkVertices2; // 0x074
    Vector3* CableCarLine2; // 0x078
    Vector3* SubwayLine2; // 0x07C
    void* field_80; // 0x080
    u16* field_84; // 0x084
    u32* field_88; // 0x088
    u32* field_8C; // 0x08C
    u32* Obstacles; // 0x090
    u32* field_94; // 0x094
    aiObstacle** field_98; // 0x098
    u16 SidewalkVerticesThing1; // 0x09C
    u16 field_9E; // 0x09E
    u16 field_A0; // 0x0A0
    i16 field_A2; // 0x0A2
    u16 Flags2; // 0x0A4
    f32* SubLengths1; // 0x0A8
    void* field_AC; // 0x0AC
    i8 field_B0; // 0x0B0
    f32 field_B4; // 0x0B4
    u8 field_B8[32]; // 0x0B8
    Vector3* SidewalkVertices1; // 0x0D8
    void* CableCarLine1; // 0x0DC
    void* SubwayLine1; // 0x0E0
    void* field_E4; // 0x0E4
    u16* field_E8; // 0x0E8
    u32* field_EC; // 0x0EC
    u32* field_F0; // 0x0F0
    u32* field_F4; // 0x0F4
    u32* field_F8; // 0x0F8
    aiObstacle** field_FC; // 0x0FC
    f32* Lengths; // 0x100
    Vector3* field_104; // 0x104
    Vector3* field_108; // 0x108
    void* field_10C; // 0x10C
    Vector3* field_110; // 0x110
    void* field_114; // 0x114
    aiIntersection* Sink; // 0x118
    i8 field_11C; // 0x11C
    u8 field_11D[1]; // 0x11D
    u16 field_11E; // 0x11E
    i8 field_120; // 0x120
    u8 field_121[3]; // 0x121
    i8 field_124; // 0x124
    u8 field_125[11]; // 0x125
    i8 field_130; // 0x130
    u8 field_131[11]; // 0x131
    aiIntersection* Source; // 0x13C
    i8 field_140; // 0x140
    u8 field_141[1]; // 0x141
    u16 field_142; // 0x142
    i8 field_144; // 0x144
    u8 field_145[3]; // 0x145
    i8 field_148; // 0x148
    u8 field_149[11]; // 0x149
    i8 field_154; // 0x154
    u32 field_158; // 0x158
    u32 field_15C; // 0x15C
    i16 AlwaysGo; // 0x160
    i16 AlwaysStop; // 0x162
};

check_size(aiPath, 0x164);
