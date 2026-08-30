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
struct gfxMaterial;
class gfxTexture;

class modShader
{
public:
    // ?AddStaticMaterial@modShader@@SAPAUgfxMaterial@@ABU2@@Z
    ARTS_IMPORT static gfxMaterial* AddStaticMaterial(const gfxMaterial& arg1);

    // ?BeginEnvMap@modShader@@SAXPAVgfxTexture@@ABVMatrix34@@@Z
    ARTS_IMPORT static void BeginEnvMap(gfxTexture* location, const Matrix34& orientation);

    // ?EndEnvMap@modShader@@SAXXZ
    ARTS_IMPORT static void EndEnvMap();

    // ?KillAll@modShader@@SAXXZ
    ARTS_IMPORT static void KillAll();

    // ?Load@modShader@@QAEXPAVStream@@_N@Z
    ARTS_IMPORT void Load(Stream* arg1, bool arg2);

    // ?LoadShaderSet@modShader@@SAPAPAV1@PAVStream@@PAH1_N@Z
    ARTS_IMPORT static modShader** LoadShaderSet(Stream* stream, i32* shaderSets, i32* shadersPerSet, bool arg4);

    // ?PreLoad@modShader@@QAEXXZ
    ARTS_IMPORT void PreLoad();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    gfxTexture* Texture; // 0x000
    gfxMaterial* pMaterial; // 0x004
};

check_size(modShader, 0x8);
