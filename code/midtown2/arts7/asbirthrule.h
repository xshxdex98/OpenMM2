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

#include "vector7/vector3.h"

#include "asnode.h"

class asParticles;
struct asSparkInfo;
struct asSparkPos;
class datBaseTokenizer;
class datParser;

class asBirthRule : public asNode
{
public:
    // ??0asBirthRule@@QAE@XZ
    ARTS_IMPORT asBirthRule();

    // ??1asBirthRule@@UAE@XZ
    ARTS_IMPORT virtual ~asBirthRule();

    // ?FileIO@asBirthRule@@UAEXAAVdatParser@@@Z
    ARTS_IMPORT virtual void FileIO(datParser& arg1);

    // ?GetClassName@asBirthRule@@UAEPADXZ
    ARTS_IMPORT virtual char* GetClassName();

    // ?GetDirName@asBirthRule@@UAEPBDXZ
    ARTS_IMPORT virtual const char* GetDirName();

    // ?Copy@asBirthRule@@QAEXPAV1@@Z
    ARTS_IMPORT void Copy(asBirthRule* arg1);

    // ?Indent@asBirthRule@@QAEXPAVdatBaseTokenizer@@H@Z
    ARTS_IMPORT void Indent(datBaseTokenizer* arg1, i32 arg2);

    // ?InitSpark@asBirthRule@@QAEXPAUasSparkInfo@@AAVasParticles@@H@Z
    ARTS_IMPORT void InitSpark(asSparkInfo* arg1, asParticles& arg2, i32 arg3);

    // ?Load@asBirthRule@@QAEXPAVdatBaseTokenizer@@@Z
    ARTS_IMPORT void Load(datBaseTokenizer* arg1);

    // ?Save@asBirthRule@@QAEXPAVdatBaseTokenizer@@H@Z
    ARTS_IMPORT void Save(datBaseTokenizer* out_pixels, i32 out_width);

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    Vector3 Position; // 0x018
    Vector3 PositionVar; // 0x024
    Vector3 Velocity; // 0x030
    Vector3 VelocityVar; // 0x03C
    f32 Life; // 0x048
    f32 LifeVar; // 0x04C
    f32 Mass; // 0x050
    f32 MassVar; // 0x054
    f32 Radius; // 0x058
    f32 RadiusVar; // 0x05C
    f32 DRadius; // 0x060
    f32 DRadiusVar; // 0x064
    f32 Drag; // 0x068
    f32 DragVar; // 0x06C
    f32 Damp; // 0x070
    f32 DampVar; // 0x074
    f32 SpewRate; // 0x078
    f32 SpewRateLimit; // 0x07C
    f32 Gravity; // 0x080
    f32 Height; // 0x084
    f32 Intensity; // 0x088
    i32 Color; // 0x08C
    i32 DAlpha; // 0x090
    i32 DAlphaVar; // 0x094
    i32 DRotation; // 0x098
    i32 DRotationVar; // 0x09C
    i32 TexFrameStart; // 0x0A0
    i32 TexFrameEnd; // 0x0A4
    i32 InitialBlast; // 0x0A8
    i32 BirthFlags; // 0x0AC
    void (__cdecl*OnSparkAdded)(asSparkInfo*, asSparkPos*); // 0x0B0
};

// ??_7asBirthRule@@6B@
// vtable at 0x005B18F8
check_size(asBirthRule, 0xB4);
