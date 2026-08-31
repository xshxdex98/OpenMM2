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

#include "vector7/matrix34.h"

#include "asnode.h"

class Base_vtbl;
class Vector3;
class modShader;
class modStatic;

class asMeshSetForm : public asNode
{
public:
    // ??0asMeshSetForm@@QAE@XZ
    ARTS_IMPORT asMeshSetForm();

    // ??1asMeshSetForm@@UAE@XZ
    ARTS_IMPORT virtual ~asMeshSetForm();

    // ?Cull@asMeshSetForm@@UAEXXZ
    ARTS_IMPORT virtual void Cull();

    // ?Update@asMeshSetForm@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?EnableAlpha@asMeshSetForm@@QAEX_N@Z
    ARTS_IMPORT void EnableAlpha(bool arg1);

    // ?EnableLighting@asMeshSetForm@@QAEX_N@Z
    ARTS_IMPORT void EnableLighting(bool arg1);

    // ?SetShape@asMeshSetForm@@QAEXPAD0_NPAVVector3@@@Z
    ARTS_IMPORT void SetShape(char* arg1, char* arg2, bool arg3, Vector3* arg4);

    // ?SetZRead@asMeshSetForm@@QAEX_N@Z
    ARTS_IMPORT void SetZRead(bool arg1);

    // ?SetZWrite@asMeshSetForm@@QAEX_N@Z
    ARTS_IMPORT void SetZWrite(bool arg1);

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    modStatic* ModStatic; // 0x018
    i32 VariantCount; // 0x01C
    modShader** Shaders; // 0x020
    modShader* ChosenShaderSet; // 0x024
    i32 dword_28; // 0x028
    i32 Flags; // 0x02C
    Matrix34 Matrix; // 0x030
};

// ??_7asMeshSetForm@@6B@
// vtable at 0x005B5358
check_size(asMeshSetForm, 0x60);
