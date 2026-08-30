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

#include "arts7/ascullable.h"

class modShader;
class modStatic;

class lvlSky : public asCullable
{
public:
    // ??0lvlSky@@QAE@XZ
    ARTS_IMPORT lvlSky();

    // ??1lvlSky@@UAE@XZ
    ARTS_IMPORT virtual ~lvlSky();

    // ?Reset@lvlSky@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Update@lvlSky@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Draw@lvlSky@@UAEXXZ
    ARTS_IMPORT virtual void Draw();

    // ?DrawWithNoZState@lvlSky@@UAEXXZ
    ARTS_IMPORT virtual void DrawWithNoZState();

    // ?DrawHat@lvlSky@@UAEXXZ
    ARTS_IMPORT virtual void DrawHat();

    // ?AutoInit@lvlSky@@QAEXPBD0H@Z
    ARTS_IMPORT void AutoInit(const char* arg1, const char* arg2, i32 arg3);

    // ?Init@lvlSky@@QAEXPBDMMMH@Z
    ARTS_IMPORT void Init(const char* arg1, f32 arg4, f32 arg3, f32 arg4, i32 arg5);

    // ?SetupFog@lvlSky@@QAEXM@Z
    ARTS_IMPORT void SetupFog(f32 arg1);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    modStatic* HighModel; // 0x004
    modShader** pShaders; // 0x008
    f32 field_C; // 0x00C
    f32 field_10; // 0x010
    f32 flaot14; // 0x014
    f32 field_18; // 0x018
    u32 ShaderIndex; // 0x01C
    i32 ShdaderSets; // 0x020
    u8 useSoftware; // 0x024
};

// ??_7lvlSky@@6B@
// vtable at 0x005B19F4
check_size(lvlSky, 0x28);
