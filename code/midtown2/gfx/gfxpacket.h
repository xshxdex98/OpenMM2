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

struct IDirect3DVertexBuffer7;
class Matrix34;
class Matrix44;
class Vector2;
class Vector3;
class gfxPacketList;
class gfxReskin;

class gfxPacket
{
public:
    // ??0gfxPacket@@QAE@IIIPBEIPBGIPAVgfxReskin@@IE00_N@Z
    ARTS_IMPORT gfxPacket(u32 arg1, u32 arg2, u32 arg3, unsigned const char* arg4, u32 arg5, unsigned const i16* arg6, u32 arg7, gfxReskin* arg8, u32 arg9, u8 arg10, unsigned const char* arg11, unsigned const char* arg12, bool arg13);

    // ??0gfxPacket@@QAE@PBV0@@Z
    ARTS_IMPORT gfxPacket(const gfxPacket* arg1);

    // ?AutoSetPacking@gfxPacket@@SAXIIPBE@Z
    ARTS_IMPORT static void AutoSetPacking(u32 arg1, u32 arg2, unsigned const char* arg3);

    // ?BeginProp@gfxPacket@@SAXXZ
    ARTS_IMPORT static void BeginProp();

    // ?BeginRef@gfxPacket@@SAXXZ
    ARTS_IMPORT static void BeginRef();

    // ?Clone@gfxPacket@@QBEPAV1@XZ
    ARTS_IMPORT gfxPacket* Clone() const;

    // ?CopyFrom@gfxPacket@@QAEXPBV1@@Z
    ARTS_IMPORT void CopyFrom(const gfxPacket* arg1);

    // ?Draw@gfxPacket@@QBEXI@Z
    ARTS_IMPORT void Draw(u32 arg1) const;

    // ?Draw@gfxPacket@@QBEXPBVMatrix44@@I@Z
    ARTS_IMPORT void Draw(const Matrix44* arg1, u32 arg2) const;

    // ?DrawList@gfxPacket@@SAXPBVgfxPacketList@@@Z
    ARTS_IMPORT static void DrawList(const gfxPacketList* arg1);

    // ?ForceCPV@gfxPacket@@SAXI@Z
    ARTS_IMPORT static void ForceCPV(u32 arg1);

    // ?FreeAllVertexBuffers@gfxPacket@@SAXXZ
    ARTS_IMPORT static void FreeAllVertexBuffers();

    // ?GetNormal@gfxPacket@@QBEXAAVVector3@@H@Z
    ARTS_IMPORT void GetNormal(Vector3& arg1, i32 arg2) const;

    // ?GetPosition@gfxPacket@@QBEXAAVVector3@@H@Z
    ARTS_IMPORT void GetPosition(Vector3& arg1, i32 arg2) const;

    // ?GetSkinnedVert@gfxPacket@@QBEXPBVMatrix34@@AAVVector3@@H@Z
    ARTS_IMPORT void GetSkinnedVert(const Matrix34* arg1, Vector3& arg2, i32 arg3) const;

    // ?GetTexCoord@gfxPacket@@QBEXAAVVector2@@H@Z
    ARTS_IMPORT void GetTexCoord(Vector2& arg1, i32 arg2) const;

    // ?GetTri@gfxPacket@@QBEXQAHH@Z
    ARTS_IMPORT void GetTri(i32*const arg1, i32 arg2) const;

    // ?MakeList@gfxPacket@@SAPAVgfxPacketList@@PAV1@@Z
    ARTS_IMPORT static gfxPacketList* MakeList(gfxPacket* arg1);

    // ?OrthoMap@gfxPacket@@QAEXM@Z
    ARTS_IMPORT void OrthoMap(f32 arg1);

    // ?ReserveVertexBuffer@gfxPacket@@SAXII@Z
    ARTS_IMPORT static void ReserveVertexBuffer(u32 arg1, u32 arg2);

    // ?RestoreAllVertexBuffers@gfxPacket@@SAXXZ
    ARTS_IMPORT static void RestoreAllVertexBuffers();

    // ?SetDefaultPacking@gfxPacket@@SAXXZ
    ARTS_IMPORT static void SetDefaultPacking();

    // ?SetNormal@gfxPacket@@QBEXABVVector3@@H@Z
    ARTS_IMPORT void SetNormal(const Vector3& arg1, i32 arg2) const;

    // ?SetPacking@gfxPacket@@SAXMEEEME@Z
    ARTS_IMPORT static void SetPacking(f32 arg1, u8 arg2, u8 arg3, u8 arg4, f32 arg5, u8 arg6);

    // ?SetPosition@gfxPacket@@QBEXABVVector3@@H@Z
    ARTS_IMPORT void SetPosition(const Vector3& arg1, i32 arg2) const;

public:
    static u8& Access_sm_VertBits() { return sm_VertBits; }
    static u8& Access_sm_NormBits() { return sm_NormBits; }
    static u8& Access_sm_CpvBits() { return sm_CpvBits; }
    static u8& Access_sm_TexCoordBits() { return sm_TexCoordBits; }
    static f32& Access_sm_TexCoordScale() { return sm_TexCoordScale; }
    static f32& Access_sm_VertScale() { return sm_VertScale; }

protected:
    // ?AllocateVertexBuffer@gfxPacket@@IAEXXZ
    ARTS_IMPORT void AllocateVertexBuffer();

    // ?DoLock@gfxPacket@@IAEXXZ
    ARTS_IMPORT void DoLock();

    // ?DoUnlock@gfxPacket@@IAEXXZ
    ARTS_IMPORT void DoUnlock();

    // ?Persist@gfxPacket@@KAPAEPAUIDirect3DVertexBuffer7@@IPAXI@Z
    ARTS_IMPORT static u8* Persist(IDirect3DVertexBuffer7* arg1, u32 arg2, void* arg3, u32 arg4);

    // ?sm_VertBits@gfxPacket@@1EA
    ARTS_IMPORT static u8 sm_VertBits;

    // ?sm_NormBits@gfxPacket@@1EA
    ARTS_IMPORT static u8 sm_NormBits;

    // ?sm_CpvBits@gfxPacket@@1EA
    ARTS_IMPORT static u8 sm_CpvBits;

    // ?sm_TexCoordBits@gfxPacket@@1EA
    ARTS_IMPORT static u8 sm_TexCoordBits;

    // ?sm_TexCoordScale@gfxPacket@@1MA
    ARTS_IMPORT static f32 sm_TexCoordScale;

    // ?sm_VertScale@gfxPacket@@1MA
    ARTS_IMPORT static f32 sm_VertScale;

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    gfxPacket* Next; // 0x000
    IDirect3DVertexBuffer7* VertexBuffer; // 0x004
    i32* pPositions; // 0x008
    void* Vertices; // 0x00C
    u16 VertexTypeDesc; // 0x010
    i16 StartVertex; // 0x012
    u16 PacketSize; // 0x014
    u16 TriCount; // 0x016
    u16* Indices; // 0x018
    u8* field_1C; // 0x01C
    u8* field_20; // 0x020
    u8 field_24; // 0x024
    u32 field_28; // 0x028
};

check_size(gfxPacket, 0x2C);
