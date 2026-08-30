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
class Matrix44;
class Stream;
class Vector3;
class datBinTokenizer;
class datTokenizer;
class gfxPacket;
class gfxTexture;
class modShader;

class modModel
{
public:
    // ?CopyShaders@modModel@@QBEPAVmodShader@@XZ
    ARTS_IMPORT modShader* CopyShaders() const;

    // ?DeleteModelHash@modModel@@SAXPBD@Z
    ARTS_IMPORT static void DeleteModelHash(const char* arg1);

    // ?Draw@modModel@@QBEXPBVMatrix44@@PAVmodShader@@I@Z
    ARTS_IMPORT void Draw(const Matrix44* arg1, modShader* name, u32 arg3) const;

    // ?DrawPlain@modModel@@QBEXPBVMatrix44@@I@Z
    ARTS_IMPORT void DrawPlain(const Matrix44* arg1, u32 arg2) const;

    // ?DrawWithTexGenCoords@modModel@@QBEXPBVMatrix44@@AAVgfxTexture@@I@Z
    ARTS_IMPORT void DrawWithTexGenCoords(const Matrix44* arg1, gfxTexture& arg2, u32 arg3) const;

    // ?GetAdjunctCount@modModel@@QBEHXZ
    ARTS_IMPORT i32 GetAdjunctCount() const;

    // ?GetBoundingBox@modModel@@QBEXAAVVector3@@0PAVMatrix34@@@Z
    ARTS_IMPORT void GetBoundingBox(Vector3& arg1, Vector3& name, Matrix34* arg3) const;

    // ?GetPosition@modModel@@QBEAAVVector3@@AAV2@H@Z
    ARTS_IMPORT Vector3& GetPosition(Vector3& arg1, i32 name) const;

    // ?ModelAlreadyLoaded@modModel@@SA_NPBD@Z
    ARTS_IMPORT static bool ModelAlreadyLoaded(const char* entryName);

    // ?SetPosition@modModel@@QBEXABVVector3@@H@Z
    ARTS_IMPORT void SetPosition(const Vector3& arg1, i32 arg2) const;

private:
    // ?LoadAscii@modModel@@AAE_NAAVdatTokenizer@@@Z
    ARTS_IMPORT bool LoadAscii(datTokenizer& arg1);

    // ?LoadBinary@modModel@@AAE_NAAVdatBinTokenizer@@@Z
    ARTS_IMPORT bool LoadBinary(datBinTokenizer& reader);

    // ?SaveToAscii@modModel@@AAE_NAAVdatBinTokenizer@@PAVStream@@@Z
    ARTS_IMPORT bool SaveToAscii(datBinTokenizer& arg1, Stream* arg2);

    // ?SaveToBinary@modModel@@AAE_NAAVdatTokenizer@@PAVStream@@@Z
    ARTS_IMPORT bool SaveToBinary(datTokenizer& arg1, Stream* arg2);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 ShaderCount; // 0x000
    u8 MatrixCount; // 0x001
    u8 bool2; // 0x002
    u8 field_3; // 0x003
    modShader* pShaders; // 0x004
    gfxPacket** ppPackets; // 0x008
};

check_size(modModel, 0xC);
