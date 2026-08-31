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
#include "vector7/vector3.h"

class Base_vtbl;
class Matrix34;
class fxShard;
class modShader;

class fxShardManager : public asNode
{
public:
    // ??0fxShardManager@@QAE@XZ
    ARTS_IMPORT fxShardManager();

    // ??1fxShardManager@@UAE@XZ
    ARTS_IMPORT virtual ~fxShardManager();

    // ?Update@fxShardManager@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Draw@fxShardManager@@QAEXXZ
    ARTS_IMPORT void Draw();

    // ?EmitAllShards@fxShardManager@@QAEXVVector3@@MABVMatrix34@@@Z
    ARTS_IMPORT void EmitAllShards(Vector3 arg1, f32 arg2, const Matrix34& arg3);

    // ?EmitShard@fxShardManager@@QAEXVVector3@@MABVMatrix34@@@Z
    ARTS_IMPORT void EmitShard(Vector3 arg1, f32 arg2, const Matrix34& arg3);

    // ?EmitShards@fxShardManager@@QAEXVVector3@@MMABVMatrix34@@@Z
    ARTS_IMPORT void EmitShards(Vector3 arg1, f32 arg2, f32 arg3, const Matrix34& arg4);

    // ?GetInstance@fxShardManager@@SAPAV1@H@Z
    ARTS_IMPORT static fxShardManager* GetInstance(i32 arg1);

    // ?Init@fxShardManager@@QAEXHPAVmodShader@@H@Z
    ARTS_IMPORT void Init(i32 arg1, modShader* arg2, i32 arg3);

    // ?SetShader@fxShardManager@@QAEXPAVmodShader@@H@Z
    ARTS_IMPORT void SetShader(modShader* arg1, i32 arg2);

public:
    static fxShardManager**& Access_Instances() { return Instances; }
    static i32& Access_NumInstances() { return NumInstances; }

protected:
    // ?Instances@fxShardManager@@1PAPAV1@A
    ARTS_IMPORT static fxShardManager** Instances;

    // ?NumInstances@fxShardManager@@1HA
    ARTS_IMPORT static i32 NumInstances;

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    fxShard* Shards; // 0x018
    i32 ShardCount; // 0x01C
    i32 LastShardIndex; // 0x020
    f32 ImpulseThreshold; // 0x024
    f32 ImpulseEmitRatio; // 0x028
    f32 XDamp; // 0x02C
    f32 YDamp; // 0x030
    f32 ZDamp; // 0x034
    f32 RotationSpeedBase; // 0x038
    f32 SpeedThreshold; // 0x03C
    i32 InstanceNumber; // 0x040
    modShader* ShaderSet; // 0x044
    i32 ShaderCount; // 0x048
};

// ??_7fxShardManager@@6B@
// vtable at 0x005B1930
check_size(fxShardManager, 0x4C);
