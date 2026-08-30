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

#include "glcommon.h"

// THE REAL DRAW PATH.
//
// docs/gfx_layouts.md §15 established, by scanning every PROC in game.asm.pristine for symbol
// references, that gfxPipeline::Draw, DrawIdx, DrawVB, DrawIdxVB, Render and RenderIdx have ZERO
// callers, and so does the whole gfxPipeline vertex-buffer creation path. They are dead code in the
// retail image. Every triangle MM2 actually draws goes through gfxPacket::Draw calling lpD3DDev
// directly:
//
//     modStatic::Draw / DrawEnvMapped / DrawWithTexGenAndTexMatrix
//         -> gfxPacket::DrawList -> gfxPacket::Draw(uint)
//     modStatic::DrawOrthoMapped
//         -> gfxPacket::OrthoMap
//     modModel::Draw / modModel::DrawPlain
//         -> gfxPacket::Draw(const Matrix44*, uint)   (skinned)
//         -> gfxPacket::Draw(uint)                    (unskinned)
//
// So the GL layer needs gfxPacket's system-memory FVF path, not a vertex buffer object. This
// header is that path: the FVF decode, the two stream builders the two Draw overloads need, and
// one submit function they share.
//
// WHAT IS NOT HERE, AND MUST BE SOMEWHERE ELSE BEFORE A TRIANGLE APPEARS. Neither Draw overload
// touches a gfxTexture, a gfxMaterial, a light, a transform or a render state. All of that is set
// before the call, by modStatic::Draw, gfxRenderState::DoFlush and gfxViewport::DoFlush - which
// means this file draws into whatever GL state those have left behind, and if they have not been
// ported it draws into the default GL state. See docs/opengl_activation.md, "Set C".

// ---------------------------------------------------------------------------------------------
// The flexible vertex format, as D3D7 encodes it.
//
// The packet stores the FVF verbatim in a u16 at gfxPacket+0x10 and hands it straight to
// IDirect3DDevice7 as dwVertexTypeDesc. Nothing in MM2 translates it, so the GL layer is the first
// thing that ever has to decode it. Every value below is from the D3D7 headers.

constexpr u32 D3DFVF_RESERVED0 = 0x0001;
constexpr u32 D3DFVF_POSITION_MASK = 0x000E;
constexpr u32 D3DFVF_XYZ = 0x0002;
constexpr u32 D3DFVF_XYZRHW = 0x0004;
constexpr u32 D3DFVF_XYZB1 = 0x0006;
constexpr u32 D3DFVF_XYZB2 = 0x0008;
constexpr u32 D3DFVF_XYZB3 = 0x000A;
constexpr u32 D3DFVF_XYZB4 = 0x000C;
constexpr u32 D3DFVF_XYZB5 = 0x000E;
constexpr u32 D3DFVF_NORMAL = 0x0010;
constexpr u32 D3DFVF_RESERVED1 = 0x0020;
constexpr u32 D3DFVF_DIFFUSE = 0x0040;
constexpr u32 D3DFVF_SPECULAR = 0x0080;
constexpr u32 D3DFVF_TEX1 = 0x0100;
constexpr u32 D3DFVF_TEX2 = 0x0200;

// D3DFVF_XYZ | RESERVED1 | DIFFUSE | SPECULAR | TEX1. gfxPacket's constructor forces this format
// when gfxForceLVERTEX is set, and gfxPacket::OrthoMap assumes it - OrthoMap is the one method that
// walks Vertices with a hardcoded 0x20 stride instead of calling gfxFVFSize.
constexpr u32 D3DFVF_LVERTEX = 0x01E2;

// D3DPT_TRIANGLELIST. Both Draw overloads push the literal 4 as the primitive type at every one of
// their four D3D call sites; no packet is ever drawn as anything else.
constexpr u32 D3DPT_TRIANGLELIST = 4;

// The component selectors gfxFVFOffset takes as its second argument. They are FVF bits, except
// that position is asked for as D3DFVF_XYZ and answered with a hardcoded 0 - so gfxFVFOffset
// cannot describe an XYZRHW or blend-weighted vertex at all, and neither can this file. See
// agiGLBuildPacketStreams.
constexpr u32 kFVFComponentPosition = D3DFVF_XYZ;
constexpr u32 kFVFComponentNormal = D3DFVF_NORMAL;
constexpr u32 kFVFComponentDiffuse = D3DFVF_DIFFUSE;
constexpr u32 kFVFComponentTexCoord0 = D3DFVF_TEX1;

// gfxFVFOffset answers 12 for D3DFVF_NORMAL before it looks at a single format flag, and 0 for
// D3DFVF_XYZ. Both Draw overloads rely on that: the skinned one walks positions from Vertices + 0
// and normals from Vertices + 12 with a `lea edx, [ecx + 0xc]` rather than a call. So the two
// offsets are part of the contract, not a derived quantity.
constexpr u32 kPacketPositionOffset = 0;
constexpr u32 kPacketNormalOffset = 12;

// ---------------------------------------------------------------------------------------------
// gfxPacket's confirmed member layout, mirrored.
//
// WHY THIS EXISTS RATHER THAN AN #include OF gfx/gfxpacket.h. That header is generated from
// data/layouts.json, and four of its thirteen member NAMES are wrong. The offsets and the size are
// right - it is only the names - but three of the four are wrong in a way that corrupts memory if
// believed, and the hand edit that fixes layouts.json is still outstanding
// (docs/gfx_layouts.md §16.1, "pending_hand_edits" in data/layouts_gfx_round3.json):
//
//   0x08  layouts.json: `i32* pPositions`     ACTUALLY: void* LockedVerts.
//         gfxPacket::DoLock (0x004B4720) passes `this + 8` as IDirect3DVertexBuffer7::Lock's
//         ppbData OUT parameter and DoUnlock (0x004B4740) writes 0 back into it. It is the mapped
//         base of the shared vertex buffer, valid only between a DoLock and its DoUnlock and null
//         at every other time. Neither Draw overload reads it.
//
//   0x14  layouts.json: `u16 PacketSize`      ACTUALLY: u16 AdjunctCount, a VERTEX COUNT.
//         The constructor stores it and separately computes AdjunctCount * gfxFVFSize(FVF) as the
//         allocation size; Draw passes it as dwVertexCount / dwNumVertices;
//         modStatic::GetAdjunctCount sums it over the Next chain.
//
//   0x16  layouts.json AND MM2Hook: `u16 TriCount`   ACTUALLY: u16 IndexCount.
//         THIS IS THE IMPORTANT ONE, and the two sources agreeing is not corroboration because
//         they are wrong the same way. Four proofs in docs/gfx_layouts.md §16.1: it is pushed as
//         DrawIndexedPrimitive's dwIndexCount; the constructor and the copy constructor both
//         `shl eax, 1` it to size a u16 array; modModel::LoadAscii computes `lea ecx, [edi+edi*2]`
//         - three times the triangle count - and passes that; and modStatic::GetTriCount reads it
//         and DIVIDES BY THREE. Sizing an index buffer as 3 * [0x16] over-allocates threefold and
//         looping [0x16] triangles reads three times past the end.
//
//   0x28  layouts.json: `u32 field_28`        ACTUALLY: gfxReskin* Reskin. Right width, wrong type.
//
// Mirroring the struct here rather than editing the generated header keeps the correction in one
// place, keeps it next to its evidence, and cannot be undone by the next run of
// tools/genheaders.py. The static_asserts below are what make the mirror safe: if either
// definition ever moves a field, this file stops compiling instead of reading the wrong bytes.
//
// TWO 1999 DEFECTS THIS TYPE DELIBERATELY DOES NOT FIX, because reproducing them is the job:
//
//  - The copy constructor (0x004B4170) writes 0x00, 0x08, 0x0C, 0x10, 0x12, 0x14, 0x16, 0x18, 0x1C
//    and 0x20, and CopyFrom (0x004B4260) writes 0x04, 0x0C, 0x10, 0x12, 0x14, 0x16 and 0x18.
//    NEITHER TOUCHES 0x24 OR 0x28, so a cloned packet inherits whatever operator new(0x2c) last
//    left in ReskinCount and Reskin - and Draw(const Matrix44*, uint) uses ReskinCount as a loop
//    bound. Both also SHALLOW-COPY 0x1C and 0x20, so a clone aliases the original's matrix arrays.
//  - There is no destructor anywhere, in the binary or in the linker map. Nothing frees a packet,
//    its vertex block, its index array, its two byte arrays or its reskin array.
//
// This struct is therefore trivially copyable and has no destructor, which is what the original
// is. A C++ reimplementation that adds either would behave differently - arguably better - and the
// divergence has to be a deliberate decision recorded somewhere, not a side effect of writing
// idiomatic C++.
struct agiGLPacketFields
{
    void* Next; // 0x00  gfxPacket*
    void* VertexBuffer; // 0x04  IDirect3DVertexBuffer7*, null on the system-memory path
    void* LockedVerts; // 0x08  OUT of Lock, null outside a DoLock/DoUnlock pair
    void* Vertices; // 0x0C  AdjunctCount * gfxFVFSize(FVF) bytes of interleaved vertices
    u16 VertexTypeDesc; // 0x10  the FVF
    i16 StartVertex; // 0x12  SIGNED. 0xFFFF on the system-memory path
    u16 AdjunctCount; // 0x14  vertices, not bytes
    u16 IndexCount; // 0x16  indices, not triangles
    u16* Indices; // 0x18  2 * IndexCount bytes
    u8* MatrixIndices; // 0x1C  counted: [0] is the count, [1..n] the palette indices
    u8* AdjunctMatrixGroups; // 0x20  AdjunctCount bytes, one group index per adjunct
    u8 ReskinCount; // 0x24
    u8 Pad25[3]; // 0x25  natural alignment padding, never read or written
    void* Reskin; // 0x28  gfxReskin*
};

check_size(agiGLPacketFields, 0x2C);

static_assert(offsetof(agiGLPacketFields, Next) == 0x00, "gfxPacket::Next must be at 0x00");
static_assert(offsetof(agiGLPacketFields, VertexBuffer) == 0x04, "gfxPacket::VertexBuffer must be at 0x04");
static_assert(offsetof(agiGLPacketFields, LockedVerts) == 0x08, "gfxPacket::LockedVerts must be at 0x08");
static_assert(offsetof(agiGLPacketFields, Vertices) == 0x0C, "gfxPacket::Vertices must be at 0x0C");
static_assert(offsetof(agiGLPacketFields, VertexTypeDesc) == 0x10, "gfxPacket::VertexTypeDesc must be at 0x10");
static_assert(offsetof(agiGLPacketFields, StartVertex) == 0x12, "gfxPacket::StartVertex must be at 0x12");
static_assert(offsetof(agiGLPacketFields, AdjunctCount) == 0x14, "gfxPacket::AdjunctCount must be at 0x14");
static_assert(offsetof(agiGLPacketFields, IndexCount) == 0x16, "gfxPacket::IndexCount must be at 0x16");
static_assert(offsetof(agiGLPacketFields, Indices) == 0x18, "gfxPacket::Indices must be at 0x18");
static_assert(offsetof(agiGLPacketFields, MatrixIndices) == 0x1C, "gfxPacket::MatrixIndices must be at 0x1C");
static_assert(
    offsetof(agiGLPacketFields, AdjunctMatrixGroups) == 0x20, "gfxPacket::AdjunctMatrixGroups must be at 0x20");
static_assert(offsetof(agiGLPacketFields, ReskinCount) == 0x24, "gfxPacket::ReskinCount must be at 0x24");
static_assert(offsetof(agiGLPacketFields, Reskin) == 0x28, "gfxPacket::Reskin must be at 0x28");

// gfxReskin - 0x10, CONFIRMED in docs/gfx_layouts.md §17 and emitted in
// data/layouts_gfx_round3.json, where tools/merge_layouts.py accepts it. It is MISSING from
// data/layouts.json, and no symbol in midtown2.exe is scoped to it, so tools/genheaders.py will
// never emit a gfxreskin.h - like gfxInterface and gfxResData it has to be hand written, and this
// is that hand-written form until gfx/gfxreskin.h exists.
struct agiGLReskin
{
    u8 AdjunctIndex; // 0x00  which already-transformed adjunct this record rewrites
    u8 MatrixGroup; // 0x01  index into MatrixIndices, offset by one
    u16 Weight; // 0x02  UNSIGNED. Weight / 32767 is the lerp factor
    f32 Position[3]; // 0x04  Vector3, model space
};

check_size(agiGLReskin, 0x10);

static_assert(offsetof(agiGLReskin, AdjunctIndex) == 0x00, "gfxReskin::AdjunctIndex must be at 0x00");
static_assert(offsetof(agiGLReskin, MatrixGroup) == 0x01, "gfxReskin::MatrixGroup must be at 0x01");
static_assert(offsetof(agiGLReskin, Weight) == 0x02, "gfxReskin::Weight must be at 0x02");
static_assert(offsetof(agiGLReskin, Position) == 0x04, "gfxReskin::Position must be at 0x04");

// The reskin weight is stored as __ftol(weight * 32767.0f) at load - the 32767.0f is the verified
// constant at 0x005B873C - and read back at draw as fild + fmul against the verified constant at
// 0x005B2A24, which is 3.0518509e-05 = 1/32767. Both were read out of the binary rather than
// assumed, because 32768 and 65535 are equally plausible and all three are wrong differently.
constexpr f32 kReskinWeightScale = 3.0518509e-05f;

// gfxPacket::Draw(const Matrix44*, uint) writes its transformed positions to the 0x6000-byte
// buffer at 0x0068BCE8 and its transformed normals to the 0x6000-byte buffer at 0x00685CE8, both
// with a 12-byte stride, so each holds 2048 entries. NOTHING BOUNDS-CHECKS AGAINST THAT, and the
// two buffers are adjacent - 0x00685CE8 + 0x6000 == 0x0068BCE8 - so a packet with more than 2048
// adjuncts overruns the normal scratch straight into the position scratch.
//
// A scan of every absolute dword in every section of midtown2.exe finds references to both buffers
// at exactly six addresses, ALL of them inside 0x004B34E0..0x004B3890 - that one function. They are
// private to it, which is what lets the GL port hold them as file-scope statics instead.
constexpr u32 kMaxSkinnedAdjuncts = 2048;

// AdjunctCount is a u16, so this is a real ceiling rather than a chosen one, and a scratch array
// sized by it cannot overflow.
constexpr u32 kMaxPacketAdjuncts = 0x10000;

// ---------------------------------------------------------------------------------------------
// The stream description, which is D3DDRAWPRIMITIVESTRIDEDDATA with MM2's truncation applied.
//
// Both Draw overloads build one of these on the stack - `sub esp, 0x30` in Draw(uint) and the
// ebp-0x48..ebp-0x18 span in Draw(const Matrix44*, uint) - and that pair of independent 0x30-byte
// frames is where docs/gfx_layouts.md §10's `sizeof(gfxStridedData) == 0x30` comes from. D3D7's own
// D3DDRAWPRIMITIVESTRIDEDDATA has D3DDP_MAXTEXCOORD == 8 texture streams and would be 0x60; MM2
// truncates it to 2, and only ever fills the first.
//
// ONE TRAP, AND IT IS THE EASIEST WAY TO GET THIS WRONG. D3D's dwStride of 0 means "every vertex
// reads the same element" - that is how Draw(uint) paints a constant colour, by pointing
// diffuse.lpvData at the address of its own `colour` argument with dwStride 0. GL's stride of 0
// means the OPPOSITE: "tightly packed, work the stride out from the format". So a zero stride can
// never be forwarded to a gl*Pointer call; it has to become a glColor4ub. agiGLDrawIndexedStrided
// enforces that.
struct agiGLStream
{
    const void* Data;
    u32 Stride;
};

// Only the streams the two overloads actually fill. Anything D3D would read that the original
// leaves uninitialised is described where it happens, in glpacket.cpp, rather than given a member
// here that would imply it holds something.
struct agiGLStridedDraw
{
    agiGLStream Position; // three f32
    agiGLStream Normal; // three f32; Data null when the FVF has no normal
    agiGLStream Diffuse; // one D3DCOLOR; Stride 0 means one constant for every vertex
    agiGLStream Specular; // one D3DCOLOR; Stride 0 means one constant for every vertex
    agiGLStream TexCoord0; // two f32; Data null when the FVF has no texture coordinates

    // The format these streams were built from. Kept because Diffuse.Data does NOT answer "does
    // this vertex carry a colour" - when the FVF has no diffuse the builders point it at a white
    // constant, so it is never null and the question has to be asked of the format itself.
    u32 Fvf;
};

// ---------------------------------------------------------------------------------------------

// Fills `out` from an interleaved vertex block, covering both of gfxPacket::Draw(uint)'s
// system-memory branches at once. That collapse is sound rather than a shortcut: D3D distinguishes
// DrawIndexedPrimitive (one interleaved block) from DrawIndexedPrimitiveStrided (one pointer and
// stride per component), and GL does not - glVertexPointer takes a stride either way - so the only
// real difference between the two branches is where the diffuse colour comes from.
//
// constant_color is a POINTER, and deliberately so. The original writes `lea edx, [ebp+8]` and
// points diffuse.lpvData at its own `colour` argument slot with dwStride 0, so the caller owns the
// storage and it has to outlive the draw. Pass null for "the FVF's own diffuse, or white".
//
// Returns false, having said why, for an FVF this decode cannot describe - which today means any
// position type other than D3DFVF_XYZ. That is not a limitation invented here: gfxFVFOffset itself
// answers 0 for position and 12 for normal before looking at a single format flag, so the whole
// 1999 offset machinery already assumes a 12-byte XYZ position.
bool agiGLBuildPacketStreams(u32 fvf, const void* vertices, const u32* constant_color, agiGLStridedDraw& out);

// One indexed triangle-list draw. This is the single place D3DPT_TRIANGLELIST becomes GL_TRIANGLES
// and the four D3D entry points the retail path used - DrawIndexedPrimitive (+0x68),
// DrawIndexedPrimitiveStrided (+0x78) and DrawIndexedPrimitiveVB (+0x80) - collapse into one
// glDrawElements.
//
// index_count is an INDEX count. It is passed straight to glDrawElements as its `count`, which is
// what DrawIndexedPrimitive's dwIndexCount was.
// gl_mode defaults to GL_TRIANGLES, which is what gfxPacket always submits. The parameter exists
// for the Direct3D device shim, which is handed a D3DPRIMITIVETYPE and may legitimately be asked
// for a strip, a fan or lines. Passing a null `indices` draws unindexed, via glDrawArrays - that is
// how IDirect3DDevice7::DrawPrimitive arrives.
void agiGLDrawIndexedStrided(const agiGLStridedDraw& streams, u32 vertex_count, const u16* indices, u32 index_count,
    u32 gl_mode = GL_TRIANGLES);

// The CPU skin, transcribed from gfxPacket::Draw(const Matrix44*, uint). Transforms every adjunct
// into the position scratch, and into the normal scratch as well when RSTATE.State.Lighting is set,
// then applies the packet's gfxReskin records, then fills every stream in `out`.
//
// palette is the caller's Matrix44 array, indexed as MatrixIndices[AdjunctMatrixGroups[i] + 1] with
// a 64-byte stride. constant_color is a pointer for the same lifetime reason as above; the skinned
// overload does NOT test it against -1 before using it, unlike Draw(uint).
//
// Returns false when the packet has more adjuncts than the 2048-entry scratch the original
// provides. The original does not check, and what it does instead is described at
// kMaxSkinnedAdjuncts.
bool agiGLSkinPacket(
    const agiGLPacketFields* packet, const void* palette, const u32* constant_color, agiGLStridedDraw& out);
