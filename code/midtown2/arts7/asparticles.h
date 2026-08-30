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

#include "asmeshcardinfo.h"

class asBirthRule;
struct asMeshCardVertex;
struct asSparkPos;
class gfxTexture;

class asParticles
{
public:
    // ??0asParticles@@QAE@XZ
    ARTS_IMPORT asParticles();

    // ??1asParticles@@UAE@XZ
    ARTS_IMPORT virtual ~asParticles();

    // ?Blast@asParticles@@QAEXHPAVasBirthRule@@@Z
    ARTS_IMPORT void Blast(i32 arg1, asBirthRule* arg2);

    // ?Cull@asParticles@@UAEXXZ
    ARTS_IMPORT virtual void Cull();

    // ?Init@asParticles@@QAEXHHHHPAUasMeshCardVertex@@@Z
    ARTS_IMPORT void Init(i32 arg1, i32 arg2, i32 arg3, i32 arg4, asMeshCardVertex* arg5);

    // ?Reset@asParticles@@QAEXXZ
    ARTS_IMPORT void Reset();

    // ?SetTexture@asParticles@@QAEXPAVgfxTexture@@@Z
    ARTS_IMPORT void SetTexture(gfxTexture* arg1);

    // ?SetTexture@asParticles@@QAEXPBD@Z
    ARTS_IMPORT void SetTexture(const char* arg1);

    // ?Update@asParticles@@QAEXXZ
    ARTS_IMPORT void Update();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u32 field_4; // 0x004
    i32 SparkCount; // 0x008
    u32 field_C; // 0x00C
    asSparkPos* pSparks; // 0x010
    gfxTexture* Texture; // 0x014
    asMeshCardInfo MeshCardInfo; // 0x018
    u32 field_28; // 0x028
    u32 field_2C; // 0x02C
    f32 field_30; // 0x030
    u32 field_34; // 0x034
    u32 field_38; // 0x038
    asBirthRule* pBirthRule; // 0x03C
    u32 field_40; // 0x040
    u32 field_44; // 0x044
    u32 field_48; // 0x048
    u32 field_4C; // 0x04C
    f32 field_50; // 0x050
};

// ??_7asParticles@@6B@
// vtable at 0x005B1974
check_size(asParticles, 0x54);
