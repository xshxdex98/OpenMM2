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
class Vector3;
class modShader;
class modStatic;
class vehBreakable;

class vehBreakableMgr
{
public:
    // ??0vehBreakableMgr@@QAE@XZ
    ARTS_IMPORT vehBreakableMgr();

    // ??1vehBreakableMgr@@QAE@XZ
    ARTS_IMPORT ~vehBreakableMgr();

    // ?Add@vehBreakableMgr@@QAEXPBD0PAPAVmodStatic@@HH@Z
    ARTS_IMPORT void Add(const char* arg1, const char* arg2, modStatic** arg3, i32 arg4, i32 arg5);

    // ?Create@vehBreakableMgr@@QAEPAVvehBreakable@@PBD0PAPAVmodStatic@@HH@Z
    ARTS_IMPORT vehBreakable* Create(const char* arg1, const char* arg2, modStatic** arg3, i32 arg4, i32 arg5);

    // ?Draw@vehBreakableMgr@@QAEXABVMatrix34@@PAVmodShader@@H@Z
    ARTS_IMPORT void Draw(const Matrix34& arg1, modShader* arg2, i32 arg3);

    // ?DrawCityLit@vehBreakableMgr@@QAEXABVMatrix34@@PAVmodShader@@H@Z
    ARTS_IMPORT void DrawCityLit(const Matrix34& arg1, modShader* arg2, i32 arg3);

    // ?Eject@vehBreakableMgr@@QAEXPAVvehBreakable@@H@Z
    ARTS_IMPORT void Eject(vehBreakable* arg1, i32 arg2);

    // ?EjectAll@vehBreakableMgr@@QAEXH@Z
    ARTS_IMPORT void EjectAll(i32 arg1);

    // ?Get@vehBreakableMgr@@QAEPAVvehBreakable@@H@Z
    ARTS_IMPORT vehBreakable* Get(i32 arg1);

    // ?Impact@vehBreakableMgr@@QAEXMABVVector3@@H@Z
    ARTS_IMPORT void Impact(f32 arg1, const Vector3& arg2, i32 arg3);

    // ?Init@vehBreakableMgr@@QAE_NPAVMatrix34@@@Z
    ARTS_IMPORT bool Init(Matrix34* arg1);

    // ?Reset@vehBreakableMgr@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?Update@vehBreakableMgr@@QAEXXZ
    ARTS_IMPORT void Update();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u32 field_0; // 0x000
    f32 field_4; // 0x004
    Matrix34* pMatrix; // 0x008
    u32 ShaderSet; // 0x00C
    f32 field_10; // 0x010
    f32 field_14; // 0x014
    f32 field_18; // 0x018
    f32 field_1C; // 0x01C
};

check_size(vehBreakableMgr, 0x20);
