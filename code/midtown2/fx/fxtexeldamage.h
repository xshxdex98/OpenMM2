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

class TexelDamageTri;
class Vector2;
class Vector3;
class gfxTexture;
class modShader;
class modStatic;

class fxTexelDamage
{
public:
    // ??0fxTexelDamage@@QAE@XZ
    ARTS_IMPORT fxTexelDamage();

    // ??1fxTexelDamage@@QAE@XZ
    ARTS_IMPORT ~fxTexelDamage();

    // ?ApplyDamage@fxTexelDamage@@QAEXABVVector3@@M@Z
    ARTS_IMPORT void ApplyDamage(const Vector3& arg1, f32 arg2);

    // ?Init@fxTexelDamage@@QAE_NPAVmodStatic@@PAVmodShader@@H@Z
    ARTS_IMPORT bool Init(modStatic* arg1, modShader* arg2, i32 arg3);

    // ?Reset@fxTexelDamage@@QAEXXZ
    ARTS_IMPORT void Reset();

private:
    // ?Kill@fxTexelDamage@@AAEXXZ
    ARTS_IMPORT void Kill();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i32 AdjunctCount; // 0x000
    i32 TriCount; // 0x004
    i32 TextureCount; // 0x008
    Vector3* Positions; // 0x00C
    Vector2* TexCoords; // 0x010
    TexelDamageTri* DamageTris; // 0x014
    modShader* CurrentShaders; // 0x018
    gfxTexture** CleanTextures; // 0x01C
    gfxTexture** DamageTextures; // 0x020
};

check_size(fxTexelDamage, 0x24);
