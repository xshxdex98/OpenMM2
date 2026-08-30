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

#include "core/arts.h"

define_dummy_symbol(agigl_glgfxpacket);

// THE TWO gfxPacket::Draw OVERLOADS, ON GL.
//
// These are the only two functions in the whole image that draw a triangle, and they are a ONE-WAY
// DOOR. Both dereference lpD3DDev in their original bodies - `mov ecx, [0x6830c8]` at 0x004B335B,
// 0x004B3441, 0x004B3470 and 0x004B386A - so the rule that governs the whole activation design
// disqualifies them from carrying a runtime -gl branch:
//
//     A function may carry the -gl branch only if its original body contains no DirectDraw or
//     Direct3D call, because the else arm is not the original code - it is a fresh retyping of it
//     against COM vtables this build has no headers for, and it has to be right first time.
//
// So there is no branch here and there cannot be one. Registering either overload replaces the
// D3D7 draw path for EVERY run, with or without -gl, and they go in as part of a set or not at all.
// docs/opengl_activation.md, "Set C", is the set and what breaks if it is short.
//
// The second guard is what keeps that honest. ARTS_AGIGL turns the directory on; ARTS_AGIGL_PACKET
// additionally turns this file on, and nothing defines it. The device half in glpacket.cpp needs
// only the first, so the FVF decode, the CPU skin and the GL submit can all be compiled and
// type-checked long before anybody decides to walk through this door.
#if defined(ARTS_AGIGL) && defined(ARTS_AGIGL_PACKET)

#    include "data7/printer.h"
#    include "gfx/gfxpacket.h"

#    include "glpacket.h"

// ?sm_UseInternal@@3_NA
//
// A free global, not gfxPacket::sm_UseInternal - the mangled name has no class scope. Written in
// exactly one place in the image, gfxPipeline::BeginGfx3D at 0x004A9C92, which under -gl is
// replaced by agiGLBeginGfx3D. It selects MM2's own software submission path.
ARTS_IMPORT extern bool sm_UseInternal;

// gfxPacket's member NAMES in the generated gfx/gfxpacket.h are wrong at four offsets, three of
// them in ways that corrupt memory if believed - pPositions is really the Lock out-parameter,
// PacketSize is really a vertex count, and TriCount is really an INDEX count. The offsets and the
// size are right, so the object is the right shape and only the labels lie.
//
// agiGLPacketFields is that same object with the corrected names and a static_assert on every
// offset. See the long note above it in glpacket.h and docs/gfx_layouts.md §16.1.
static const agiGLPacketFields* agiGLFields(const gfxPacket* packet)
{
    return reinterpret_cast<const agiGLPacketFields*>(packet);
}

// ?Draw@gfxPacket@@QBEXI@Z
//
// The original, at 0x004B3340, is four branches:
//
//   StartVertex >= 0   DrawIndexedPrimitiveVB   vtable +0x80
//   sm_UseInternal     an unnamed __cdecl wrapper at 0x004B73C0 which sets the FPU control word,
//                      calls gfxPipeline::RenderIdx and submits the result as FVF 0x1C4
//   colour != -1 and the FVF has no D3DFVF_DIFFUSE
//                      DrawIndexedPrimitiveStrided   vtable +0x78, with a 0x30-byte gfxStridedData
//                      built on the stack purely to paint one constant colour onto vertices that
//                      have no diffuse component of their own
//   otherwise          DrawIndexedPrimitive   vtable +0x68
//
// The last two collapse into one here. D3D distinguishes an interleaved block from a set of
// pointer-and-stride pairs; GL does not, because glVertexPointer takes a stride either way. So the
// only real difference between those two branches is where the diffuse colour comes from, and that
// is a parameter to agiGLBuildPacketStreams rather than a branch.
void gfxPacket::Draw(u32 arg1) const
{
    const agiGLPacketFields* packet = agiGLFields(this);

    if (packet->StartVertex >= 0)
    {
        // THE VERTEX-BUFFER BRANCH IS UNREACHABLE UNDER GL, AND THAT IS A CONTRACT, NOT A HOPE.
        //
        // StartVertex is 0xFFFF - the value written at 0x004B3A97 - unless the constructor called
        // AllocateVertexBuffer, and it only does that when the packet has no matrix indices AND
        // useNativeVBs is set. useNativeVBs (0x006844F8) is written in exactly one place in the
        // image, gfxPipeline::BeginGfx3D, which under -gl is replaced by agiGLBeginGfx3D - and
        // agiGLBeginGfx3D clears it, precisely so that no packet is ever built around an
        // IDirect3DVertexBuffer7 that cannot exist.
        //
        // If this fires, something set useNativeVBs after bring-up and gfxPacket::AllocateVertexBuffer
        // has already called lpD3D->CreateVertexBuffer on a null lpD3D, which crashes earlier than
        // this. Quitting names the reason at a known address; drawing nothing would hide it.
        Quitf("gfxPacket::Draw: packet is vertex-buffer backed (StartVertex %d) - useNativeVBs must be "
              "false under -gl",
            packet->StartVertex);

        return;
    }

    if (sm_UseInternal)
    {
        // The original calls the internal rasteriser here. It has no symbol - 0x004B73C0 lies
        // inside the extent data/symbols.json records for gfxPipeline::RenderIdx - and it ends in
        // lpD3DDev->DrawIndexedPrimitive with FVF 0x1C4 either way, so there is nothing to call and
        // nothing worth transcribing. BLADE and AgeDevice are both being deleted, and
        // agiGLBeginGfx3D clears sm_UseInternal for the same reason it clears useNativeVBs.
        Quitf("gfxPacket::Draw: sm_UseInternal is set, and the internal rasteriser is gone under -gl");

        return;
    }

    // `cmp dword ptr [ebp + 8], -1` at 0x004B33C0, then `test al, 0x40` on the FVF at 0x004B33CE.
    // A colour is only an override when the vertices have no colour of their own; a packet whose
    // FVF already carries D3DFVF_DIFFUSE ignores the argument entirely, in the original and here.
    u32 fvf = packet->VertexTypeDesc;
    const u32* constant_color = (arg1 != 0xFFFFFFFF && (fvf & D3DFVF_DIFFUSE) == 0) ? &arg1 : nullptr;

    agiGLStridedDraw streams;

    if (!agiGLBuildPacketStreams(fvf, packet->Vertices, constant_color, streams))
        return;

    // AdjunctCount is a vertex count and IndexCount is an index count. Both are read here under the
    // names that survive the evidence, not the ones data/layouts.json still carries.
    agiGLDrawIndexedStrided(streams, packet->AdjunctCount, packet->Indices, packet->IndexCount);
}

// ?Draw@gfxPacket@@QBEXPBVMatrix44@@I@Z
//
// The original, at 0x004B34E0, is the CPU skinning path. It never touches VertexBuffer,
// LockedVerts or StartVertex, and it always ends in one DrawIndexedPrimitiveStrided at vtable
// +0x78. In order:
//
//   1. Return immediately if sm_UseInternal.
//   2. Read RSTATE.State.Lighting (0x006856A1). Lit packets transform positions AND normals;
//      unlit ones transform positions only.
//   3. Per adjunct i, take palette[MatrixIndices[AdjunctMatrixGroups[i] + 1]] and transform into
//      the two scratch buffers.
//   4. Per gfxReskin record, lerp the already-transformed position of one adjunct toward the
//      record's own transformed position by Weight / 32767.
//   5. Build a 0x30-byte gfxStridedData and draw.
//
// Steps 2 to 4 are agiGLSkinPacket; step 5 is agiGLDrawIndexedStrided.
void gfxPacket::Draw(const Matrix44* arg1, u32 arg2) const
{
    if (sm_UseInternal)
    {
        // The original really does return without drawing - `test al, al; jne` straight to the
        // epilogue at 0x004B388B. The internal rasteriser has no skinned entry point, so a skinned
        // model is simply invisible under the software renderers. Transcribed rather than fixed.
        return;
    }

    const agiGLPacketFields* packet = agiGLFields(this);

    // The skinned overload does NOT test the colour against -1 the way Draw(uint) does. It chooses
    // the diffuse source from the FVF alone and uses the argument whenever the format has none;
    // the -1 test it does make only decides whether D3DFVF_DIFFUSE is OR-ed into the format handed
    // to the device, which under GL is the same as whether the colour is white.
    agiGLStridedDraw streams;

    if (!agiGLSkinPacket(packet, arg1, &arg2, streams))
        return;

    agiGLDrawIndexedStrided(streams, packet->AdjunctCount, packet->Indices, packet->IndexCount);
}

// gfxPacket::DrawList AND gfxPacket::MakeList ARE DELIBERATELY NOT HERE.
//
// MakeList (0x004B4860) is `push ebp; mov ebp, esp; mov eax, [ebp+8]; pop ebp; ret` - it returns
// its argument unchanged, which is what makes gfxPacketList and gfxPacket the same 0x2C bytes.
// DrawList (0x004B4870) is a nine-instruction loop over the Next chain calling
// gfxPacket::Draw(uint) with -1.
//
// Neither contains COM, so both could carry a -gl branch - and neither needs one, or needs
// registering at all. DrawList calls ?Draw@gfxPacket@@QBEXI@Z through a rel32 to the mangled
// symbol, so once that symbol is ours its untouched machine code calls straight into the function
// above. Registering it would strip 0x70 bytes and buy nothing.

#endif
