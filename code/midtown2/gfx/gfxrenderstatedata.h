/*
    OpenMM2 - An Open Source Re-Implementation of Midtown Madness 2

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

// THE CLASS gfx/gfxrenderstate.h NAMES BUT NOBODY DEFINES.
//
// gfxRenderState holds `gfxRenderStateData State;` BY VALUE at offset 0, so gfxrenderstate.h does
// not compile without this - which is why agigl/glpacket.cpp reaches RSTATE through an incomplete
// type and a hand-computed byte offset instead of naming a member, and why nothing had been able
// to define a gfxRenderState member function at all.
//
// tools/genheaders.py does not emit it. The layout is in data/layouts.json and is complete, but a
// class with no functions in data/symbols.json never gets a header of its own, so the type is
// referenced and never declared. This file is that header, written from the same layouts.json
// entry genheaders would have used, and it should be DELETED the moment genheaders learns to emit
// data-only classes - it is a gap-filler, not a decision.
//
// Generated from data/layouts.json, 37 members, size 0x50.
// Only the size and placement of these two matter here, so they stay incomplete - including the
// real headers would drag gfxtexture.h and gfxmaterial.h into every consumer of the render state.
class gfxTexture;
class gfxMaterial;

// CLASS, NOT STRUCT, and the difference is not stylistic.
//
// MSVC encodes the class-key in the mangled name: a struct is U and a class is V. Retail spells the
// globals ?RSTATE@@3VgfxRenderState@@A and ?LASTRSTATE@@3VgfxRenderStateData@@A - both V - so
// declaring this as a struct produces ?LASTRSTATE@@3UgfxRenderStateData@@A, a symbol that is not in
// the binary, and the link fails naming it. Everything is public, so the only thing `class` changes
// here is that one letter.
class gfxRenderStateData
{
public:
    u8 Clipping; // 0x000
    u8 Lighting; // 0x001
    u8 SrcBlend; // 0x002
    u8 DestBlend; // 0x003
    u8 VertexBlend; // 0x004
    u8 pad_05[0x3]; // 0x005
    i32 TextureFactor; // 0x008
    i32 BlendState; // 0x00C
    f32 FogStart; // 0x010
    f32 FogEnd; // 0x014
    u8 ZEnable; // 0x018
    u8 Shademode; // 0x019
    u8 ZWriteEnable; // 0x01A
    u8 Cullmode; // 0x01B
    u8 ZFunc; // 0x01C
    u8 AlphaRef; // 0x01D
    u8 Alphafunc; // 0x01E
    u8 DitherEnable; // 0x01F
    u8 AlphaEnable; // 0x020
    u8 TexturePerspective; // 0x021
    u8 Fillmode; // 0x022
    u8 ZBias; // 0x023
    u8 AntiAlias; // 0x024
    u8 FogVertexMode; // 0x025
    i8 AddressU[2]; // 0x026
    i8 AddressV[2]; // 0x028
    u8 pad_2A[0x2]; // 0x02A
    i32 field_2C; // 0x02C
    i32 Ambient; // 0x030
    i32 FogColor; // 0x034
    i32 FogDensity; // 0x038
    u8 m_COLOROP1; // 0x03C
    u8 field_3D; // 0x03D
    u8 SpecularEnable; // 0x03E
    u8 FogEnable; // 0x03F
    u8 NormalizeNormals; // 0x040
    u8 RangeFogEnable; // 0x041
    u8 pad_42[0x2]; // 0x042
    gfxTexture* m_Texture[2]; // 0x044
    gfxMaterial* m_Material; // 0x04C
};

check_size(gfxRenderStateData, 0x50);
