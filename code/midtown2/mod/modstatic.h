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

class gfxPacket;
class gfxPacketList;
class gfxTexture;
class modShader;

class modStatic
{
public:
    // ?Clone@modStatic@@QBEPAV1@XZ
    ARTS_IMPORT modStatic* Clone() const;

    // ?CopyFrom@modStatic@@QAEXPBV1@@Z
    ARTS_IMPORT void CopyFrom(const modStatic* arg1);

    // ?Draw@modStatic@@QBEXPAVmodShader@@@Z
    ARTS_IMPORT void Draw(modShader* pShaders) const;

    // ?DrawEnvMapped@modStatic@@QBEXPAVmodShader@@PAVgfxTexture@@M@Z
    ARTS_IMPORT void DrawEnvMapped(modShader* arg1, gfxTexture* arg2, f32 arg3) const;

    // ?DrawNoAlpha@modStatic@@QBEXPAVmodShader@@@Z
    ARTS_IMPORT void DrawNoAlpha(modShader* pShaders) const;

    // ?DrawOrthoMapped@modStatic@@QBEXPAVmodShader@@PAVgfxTexture@@MI@Z
    ARTS_IMPORT void DrawOrthoMapped(modShader* shaders, gfxTexture* orthoMap, f32 arg3, u32 arg4) const;

    // ?DrawWithTexGenAndTexMatrix@modStatic@@QBEXXZ
    ARTS_IMPORT void DrawWithTexGenAndTexMatrix() const;

    // ?GetAdjunctCount@modStatic@@QBEHXZ
    ARTS_EXPORT i32 GetAdjunctCount() const;

    // ?GetTriCount@modStatic@@QBEHXZ
    ARTS_EXPORT i32 GetTriCount() const;

    // ?Optimize@modStatic@@QAEXPAVmodShader@@@Z
    ARTS_IMPORT void Optimize(modShader* pShaders);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 PacketCount; // 0x000
    u8 Flags; // 0x001
    u16 FvfFlags; // 0x002
    u8* ShaderIndices; // 0x004
    gfxPacket** ppPackets; // 0x008
    gfxPacketList** ppPacketLists; // 0x00C
};

check_size(modStatic, 0x10);
