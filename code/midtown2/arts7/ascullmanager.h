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

#include "misc/timer.h"

#include "asnode.h"

class Matrix34;
class asCamera;
class asCullable;
class gfxBitmap;

class asCullManager : public asNode
{
public:
    struct VTbl;

    // ??0asCullManager@@QAE@HH@Z
    ARTS_IMPORT asCullManager(i32 maxCullables, i32 maxCullables2D);

    // ??1asCullManager@@UAE@XZ
    ARTS_IMPORT virtual ~asCullManager();

    // ?Update@asCullManager@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@asCullManager@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?DeclareCamera@asCullManager@@UAEXPAVasCamera@@@Z
    ARTS_IMPORT virtual void DeclareCamera(asCamera* camera);

    // ?DeclareCullable@asCullManager@@UAEXPAVasCullable@@@Z
    ARTS_IMPORT virtual void DeclareCullable(asCullable* newCullable);

    // ?DeclareCullable2D@asCullManager@@UAEXPAVasCullable@@@Z
    ARTS_IMPORT virtual void DeclareCullable2D(asCullable* newCullable);

    // ?DeclareCullable2DFG@asCullManager@@UAEXPAVasCullable@@@Z
    ARTS_IMPORT virtual void DeclareCullable2DFG(asCullable* newCullable);

    // ?DeclareBitmap@asCullManager@@QAEXPAVasCullable@@PAVgfxBitmap@@@Z
    ARTS_IMPORT void DeclareBitmap(asCullable* arg1, gfxBitmap* arg2);

public:
    static asCullManager*& Access_Instance() { return Instance; }

protected:
    // ?DisplayVersionString@asCullManager@@IAEXXZ
    ARTS_IMPORT void DisplayVersionString();

    // ?Instance@asCullManager@@1PAV1@A
    ARTS_IMPORT static asCullManager* Instance;

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u32 field_18; // 0x018
    u32 CameraCount; // 0x01C
    asCamera* CameraArray[16]; // 0x020
    asCamera* CurrentCamera; // 0x060
    i32 CurrentCullables; // 0x064
    i32 MaxCullables; // 0x068
    i32 CurrentCullables2D; // 0x06C
    i32 MaxCullables2D; // 0x070
    i32 CurrentCullables2DFG; // 0x074
    i32 MaxCullables2DFG; // 0x078
    asNode** CullableArray; // 0x07C
    asNode** Cullables2D; // 0x080
    asCullable** Cullables2DFG; // 0x084
    Matrix34** CullablsMatrices; // 0x088
    i32 BaseColor; // 0x08C
    i32 field_90; // 0x090
    i32 field_94; // 0x094
    Timer Timer1; // 0x098
    Timer Timer2; // 0x09C
    i32 field_A0; // 0x0A0
    i8 ShouldReset; // 0x0A4
    i8 field_A5; // 0x0A5
    i8 field_A6; // 0x0A6
    i8 field_A7; // 0x0A7
};

// ??_7asCullManager@@6B@
// vtable at 0x005B291C
check_size(asCullManager, 0xA8);
