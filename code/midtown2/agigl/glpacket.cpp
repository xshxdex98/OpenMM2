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
#include "glstub.h" // agiGLProbeVerbose

define_dummy_symbol(agigl_glpacket);

i32 agiGLProbeDraws = 0; // TEMPORARY PROBE

// TEMPORARY PROBE - the ordered frame trace in agigl/glframe.cpp counts draws between events.
extern bool agiGLTrace;
extern i32 agiGLTracePending;

// THE DEVICE HALF OF THE PACKET DRAW PATH.
//
// This file is guarded by ARTS_AGIGL ALONE. It defines no symbol that exists in midtown2.exe, so
// it can be compiled and type-checked as soon as -DARTS_AGIGL is on, without registering anything
// and without stripping a byte out of game.asm. The half that DOES claim the retail names -
// gfxPacket::Draw's two overloads - is in glgfxpacket.cpp behind a second guard, because
// registering those is a one-way door. See docs/opengl_activation.md, "Set C".
#ifdef ARTS_AGIGL

#    include "data7/printer.h"
#    include "misc/freefuncs.h"

#    include "glcontext.h"
#    include "glpacket.h"

// ?RSTATE@@3VgfxRenderState@@A
//
// Declared with an INCOMPLETE type on purpose. gfx/gfxrenderstate.h cannot be included here: it
// declares `gfxRenderStateData State;` by value and nothing in the tree defines gfxRenderStateData,
// so the header does not compile. An extern declaration of an incomplete class type is legal as
// long as nothing needs its size, and taking its address does not - while the mangled name it
// produces, ?RSTATE@@3VgfxRenderState@@A, is exactly the one in the retail linker map.
class gfxRenderState;
ARTS_IMPORT extern gfxRenderState RSTATE;

// gfxRenderStateData::Lighting, at offset 0x01 of RSTATE - so 0x006856A1, which is the address
// gfxPacket::Draw(const Matrix44*, uint) reads at 0x004B3509. The offset is from
// data/layouts.json's gfxRenderStateData, whose first five members are one byte each: Clipping,
// Lighting, SrcBlend, DestBlend, VertexBlend.
static constexpr usize kRenderStateLightingOffset = 0x01;

// NOT static any more: agigl/gldraw.cpp needs the same answer when it decides GL_LIGHTING, and
// there must be exactly one place that knows where the bit lives.
// The blend state, pulled from RSTATE the same way lighting is.
//
// Offsets from data/layouts.json's gfxRenderStateData: SrcBlend 0x02, DestBlend 0x03 and
// AlphaEnable 0x20, all u8. modShader::BeginEnvMap sets AlphaEnable = 1 and
// SetBlendSet(RSTATE, 7, 128), and blend set 7 is SrcBlend = DestBlend = 2 - both D3DBLEND_ONE.
static constexpr usize kRenderStateSrcBlendOffset = 0x02;
static constexpr usize kRenderStateDestBlendOffset = 0x03;
static constexpr usize kRenderStateAlphaEnableOffset = 0x20;

// gfxRenderStateData::ZWriteEnable, 0x01A. Read straight from RSTATE for the same reason the others
// are: it is what the GAME asked for, which is the only authority on whether a draw should write
// depth.
static constexpr usize kRenderStateZWriteOffset = 0x1A;

bool agiGLAlphaBlendEnabled()
{
    return reinterpret_cast<const u8*>(&RSTATE)[kRenderStateAlphaEnableOffset] != 0;
}

u8 agiGLSrcBlendD3D()
{
    return reinterpret_cast<const u8*>(&RSTATE)[kRenderStateSrcBlendOffset];
}

u8 agiGLDestBlendD3D()
{
    return reinterpret_cast<const u8*>(&RSTATE)[kRenderStateDestBlendOffset];
}

bool agiGLDepthWriteEnabled()
{
    return reinterpret_cast<const u8*>(&RSTATE)[kRenderStateZWriteOffset] != 0;
}

bool agiGLLightingEnabled()
{
    return reinterpret_cast<const u8*>(&RSTATE)[kRenderStateLightingOffset] != 0;
}

// THE SCRATCH THE SKINNED PATH TRANSFORMS INTO.
//
// In retail these are two 0x6000-byte blocks of .data? at 0x00685CE8 (normals) and 0x0068BCE8
// (positions), and they are ADJACENT - 0x00685CE8 + 0x6000 == 0x0068BCE8 - which is why a packet
// with more than 2048 adjuncts overruns the first straight into the second. The adjacency is
// reproduced here so that the memory picture matches the one in docs/gfx_layouts.md §19; the
// overrun is not, see agiGLSkinPacket.
//
// Holding them as file-scope statics rather than at their retail addresses is safe because a scan
// of every absolute dword in every section of midtown2.exe finds all six references to the two
// buffers inside 0x004B34E0..0x004B3890 - the one function that owns them.
struct agiGLSkinScratch
{
    f32 Normals[kMaxSkinnedAdjuncts * 3]; // 0x00685CE8 in retail
    f32 Positions[kMaxSkinnedAdjuncts * 3]; // 0x0068BCE8, immediately after
};

static agiGLSkinScratch agiGLSkin;

// The zeroed local at ebp-0x18 that gfxPacket::Draw(const Matrix44*, uint) points its specular
// stream at with dwStride 0. Its whole purpose is to force specular to black on the skinned path,
// so a file-scope constant is the same thing with a longer life.
static const u32 kZeroColor = 0;

// What D3D uses for diffuse when the FVF declares none and lighting is off. GL uses the current
// colour in the same situation, so setting it white is what makes the two agree.
static const u32 kWhiteColor = 0xFFFFFFFF;

// D3DCOLOR is 0xAARRGGBB held in a u32, which on x86 puts the bytes in memory as B, G, R, A.
// glColor4ubv and glColorPointer(4, GL_UNSIGNED_BYTE, ...) both read them as R, G, B, A. Getting
// this wrong swaps red and blue everywhere and looks like a texture-format bug rather than a vertex
// one, which is why it is spelled out in one place and used from everywhere.
static void agiGLUnpackD3DColor(u32 color, GLubyte out[4])
{
    out[0] = static_cast<GLubyte>((color >> 16) & 0xFF);
    out[1] = static_cast<GLubyte>((color >> 8) & 0xFF);
    out[2] = static_cast<GLubyte>(color & 0xFF);
    out[3] = static_cast<GLubyte>((color >> 24) & 0xFF);
}

// The same reordering for a whole array, into a tightly packed scratch. Only needed on a driver
// without GL_ARB_vertex_array_bgra; with it, glColorPointer can be told to read BGRA directly and
// the D3DCOLOR array is consumed in place with no copy at all.
//
// AdjunctCount is a u16, so kMaxPacketAdjuncts is a ceiling by construction rather than a chosen
// one and these two arrays cannot overflow.
static u32 agiGLDiffuseSwizzle[kMaxPacketAdjuncts];
static u32 agiGLSpecularSwizzle[kMaxPacketAdjuncts];

static void agiGLSwizzleColors(const agiGLStream& stream, u32 count, u32* dst)
{
    const u8* src = static_cast<const u8*>(stream.Data);

    for (u32 i = 0; i < count; ++i)
    {
        u32 color = *reinterpret_cast<const u32*>(src);

        // 0xAARRGGBB -> the byte order glColorPointer wants, which is 0xAABBGGRR read as a u32.
        dst[i] = (color & 0xFF00FF00) | ((color & 0x00FF0000) >> 16) | ((color & 0x000000FF) << 16);

        src += stream.Stride;
    }
}

// GL_ARB_vertex_array_bgra, core since 3.2. Queried once, because HasExtension walks a string.
static i32 agiGLBgraArrays = -1;

static bool agiGLHasBgraArrays()
{
    if (agiGLBgraArrays < 0)
    {
        bool supported = agiGL->HasVersion(320) || agiGL->HasExtension("GL_ARB_vertex_array_bgra")
            || agiGL->HasExtension("GL_EXT_vertex_array_bgra");

        agiGLBgraArrays = supported ? 1 : 0;

        Displayf("GL: D3DCOLOR vertex arrays %s", supported ? "read in place as BGRA" : "swizzled on the CPU");
    }

    return agiGLBgraArrays != 0;
}

// EXT_secondary_color is the only fixed-function spelling of D3DFVF_SPECULAR. Said once rather than
// per draw, because a per-draw Warningf in this path would be tens of thousands of lines a second.
static bool agiGLSaidNoSpecular;

// Likewise for the two FVF shapes this decode cannot describe.
static bool agiGLSaidNoTex2;

// Sets one gl*Pointer from a stream, refusing a zero stride.
//
// THIS REFUSAL IS THE POINT OF THE FUNCTION. D3D's dwStride of 0 means "every vertex reads the same
// element"; GL's stride of 0 means "tightly packed, derive the stride from the format". They are
// opposite meanings for the same value, so forwarding a D3D constant stream to glVertexPointer
// would silently walk off the end of a single vertex instead of repeating it. Only the two colour
// streams are ever constant, and they go through glColor4ubv instead.
static bool agiGLCheckStride(const agiGLStream& stream, const char* what)
{
    if (stream.Stride != 0)
        return true;

    Errorf("agiGLDrawIndexedStrided: %s has a zero stride, which GL reads as 'tightly packed'", what);

    return false;
}

bool agiGLBuildPacketStreams(u32 fvf, const void* vertices, const u32* constant_color, agiGLStridedDraw& out)
{
    out.Fvf = fvf;

    // gfxFVFOffset answers 0 for position and 12 for normal before it looks at a single format
    // flag, and every packet method is built on those two answers - so the whole 1999 offset
    // machinery already assumes a 12-byte D3DFVF_XYZ position. An XYZRHW or blend-weighted packet
    // would be described wrongly by gfxFVFOffset itself, not merely by this decode, so refusing is
    // the honest response rather than a limitation introduced here.
    if ((fvf & D3DFVF_POSITION_MASK) != D3DFVF_XYZ)
    {
        Errorf("agiGLBuildPacketStreams: FVF 0x%03X has position type 0x%X, and only D3DFVF_XYZ is decodable", fvf,
            fvf & D3DFVF_POSITION_MASK);

        return false;
    }

    if ((fvf & D3DFVF_TEX2) != 0 && !agiGLSaidNoTex2)
    {
        // gfxFVFSize treats 0x100 and 0x200 as independent bits worth 8 each, but D3D7 reads them
        // as a two-bit texture COUNT: 0x200 alone means two coordinate sets and sixteen bytes, not
        // eight. Every stride in the game would already be wrong for such a packet, so this is a
        // report about the FVF rather than about GL.
        Warningf("agiGLBuildPacketStreams: FVF 0x%03X asks for a second texture coordinate set; "
                 "gfxFVFSize does not size one correctly and only set 0 is drawn",
            fvf);

        agiGLSaidNoTex2 = true;
    }

    u32 stride = gfxFVFSize(fvf);


    // FVF 0x102 IS NOT A 32-BYTE STRIDE. Tested and disproved.
    //
    // The kit gives vglVCT1_t and vglVNT1_t as sizeof 0x20, and the city arrives as FVF 0x102 which
    // gfxFVFSize sizes at 20 - so forcing 32 for that format looked like the fix for the flickering
    // city. It is not: gfxPacket geometry uses 0x102 at the honest 20-byte stride, and forcing 32
    // smeared every building, the sky and the minimap into streaks.
    //
    // So either the vgl path does not reach this function, or it passes a format that already sizes
    // correctly. gfxFVFSize is the game's own answer and it stays.
    const u8* base = static_cast<const u8*>(vertices);

    out.Position.Data = base + kPacketPositionOffset;
    out.Position.Stride = stride;

    // The original sets the normal and texture streams UNCONDITIONALLY and lets D3D ignore the ones
    // the FVF does not declare. GL has no FVF to ignore them by, so the bit has to be tested here
    // instead. Same result, one level earlier.
    if ((fvf & D3DFVF_NORMAL) != 0)
    {
        out.Normal.Data = base + kPacketNormalOffset;
        out.Normal.Stride = stride;
    }
    else
    {
        out.Normal.Data = nullptr;
        out.Normal.Stride = 0;
    }

    if ((fvf & D3DFVF_DIFFUSE) != 0)
    {
        out.Diffuse.Data = base + gfxFVFOffset(fvf, kFVFComponentDiffuse);
        out.Diffuse.Stride = stride;
    }
    else
    {
        // The strided branch of gfxPacket::Draw(uint): diffuse.lpvData = &colour, dwStride = 0, and
        // D3DFVF_DIFFUSE is OR-ed into the format handed to the device. With no override there is
        // no diffuse at all and D3D falls back to white, which is what the caller's null means.
        out.Diffuse.Data = (constant_color != nullptr) ? constant_color : &kWhiteColor;
        out.Diffuse.Stride = 0;
    }

    if ((fvf & D3DFVF_SPECULAR) != 0)
    {
        // gfxFVFOffset has no selector for specular. Its body is
        //
        //     result = 12; if (fvf & NORMAL) result = 0x18; if (fvf & RESERVED1) result += 4;
        //     if (sel == DIFFUSE) return result;
        //     if (fvf & DIFFUSE) result += 4;
        //     if (fvf & SPECULAR) result += 4;
        //     if (sel == TEX1) return result;
        //     return result + 8;
        //
        // so specular sits immediately after diffuse: the diffuse answer, plus four when the format
        // actually has a diffuse. That is read off the function rather than invented, which is the
        // only reason it is written here at all.
        out.Specular.Data
            = base + gfxFVFOffset(fvf, kFVFComponentDiffuse) + ((fvf & D3DFVF_DIFFUSE) != 0 ? 4u : 0u);
        out.Specular.Stride = stride;

        // A DELIBERATE DIVERGENCE, RECORDED RATHER THAN HIDDEN. In the original's strided branch the
        // specular slots of the stack gfxStridedData - ebp-0x18 and ebp-0x14 - are NEVER WRITTEN, so
        // an FVF carrying D3DFVF_SPECULAR but not D3DFVF_DIFFUSE (the only way to reach that branch)
        // would hand D3D an uninitialised stack pointer. Reading an uninitialised local is undefined
        // behaviour MSVC is entitled to exploit, so the literal form is not available to C++ at all
        // - the same argument agigl/glbeginphase.cpp makes about ChangeDisplaySettingsA's 148
        // uninitialised bytes. This reads the vertex block instead. No observed FVF has SPECULAR
        // without DIFFUSE, so nothing should reach it.
    }
    else
    {
        out.Specular.Data = nullptr;
        out.Specular.Stride = 0;
    }

    if ((fvf & D3DFVF_TEX1) != 0)
    {
        out.TexCoord0.Data = base + gfxFVFOffset(fvf, kFVFComponentTexCoord0);
        out.TexCoord0.Stride = stride;
    }
    else
    {
        out.TexCoord0.Data = nullptr;
        out.TexCoord0.Stride = 0;
    }

    return true;
}

void agiGLDrawIndexedStrided(
    const agiGLStridedDraw& streams, u32 vertex_count, const u16* indices, u32 index_count, u32 gl_mode)
{
    // Unindexed is a legitimate call rather than a degenerate one: gfxPacket always supplies
    // indices, but IDirect3DDevice7::DrawPrimitive does not, and the device shim routes it here so
    // that both share one piece of state setup.
    const bool indexed = (indices != nullptr && index_count != 0);

    if (streams.Position.Data == nullptr)
        return;

    if (!indexed && vertex_count == 0)
        return;

    if (vertex_count > kMaxPacketAdjuncts)
    {
        // Unreachable through gfxPacket, whose AdjunctCount is a u16. Checked anyway because the
        // swizzle scratch is sized by this and a wrong count would write past it.
        Errorf("agiGLDrawIndexedStrided: %u vertices exceeds the %u a u16 adjunct count can hold", vertex_count,
            kMaxPacketAdjuncts);

        return;
    }

    if (!agiGLCheckStride(streams.Position, "position"))
        return;

    // TEMPORARY PROBE: how many draws reach GL per frame. If the main view issues none while the
    // mirror issues many, the city is being culled game-side; if it issues many and nothing appears,
    // the fault is in GL state. Remove when solved.
    {
        extern void agiGLProbeNoteDraw();

        agiGLProbeNoteDraw();
    }

    ++agiGLProbeDraws;

    if (agiGLTrace)
        ++agiGLTracePending;

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, static_cast<GLsizei>(streams.Position.Stride), streams.Position.Data);

    if (streams.Normal.Data != nullptr && agiGLCheckStride(streams.Normal, "normal"))
    {
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, static_cast<GLsizei>(streams.Normal.Stride), streams.Normal.Data);
    }
    else
    {
        glDisableClientState(GL_NORMAL_ARRAY);

        // LIGHTING NEEDS NORMALS, so geometry that carries none must not be lit.
        //
        // With no normal array GL lights every vertex by the single current normal, which is
        // whatever the last draw left behind. That is how the sky dome came out as a solid black
        // shape over the middle of the scene: it has no normals, and it was being shaded by a
        // leftover one pointing away from the sun.
        //
        // D3D's rule is the same - D3DFVF_NORMAL is required for lighting - so switching lighting
        // off for these draws is the translation, not a workaround. agiGLApplyRenderState puts it
        // back for the next draw that asks for it.
        agiGL->EnableDisable(GL_LIGHTING, false);
    }

    // DIFFUSE. A stride of 0 is D3D's "one colour for every vertex", and in GL that is not an array
    // at all - it is the current colour with the array turned off.
    if (streams.Diffuse.Data == nullptr || streams.Diffuse.Stride == 0)
    {
        GLubyte rgba[4];

        // Not white unconditionally: when gfxRenderState has aimed stage 0's COLORARG2 at the
        // texture factor, that factor is the constant the texture is modulated by.
        extern u32 agiGLPacketConstantColor();

        agiGLUnpackD3DColor(streams.Diffuse.Data != nullptr ? *static_cast<const u32*>(streams.Diffuse.Data)
                                                            : agiGLPacketConstantColor(),
            rgba);

        glDisableClientState(GL_COLOR_ARRAY);
        glColor4ubv(rgba);
    }
    else if (agiGLHasBgraArrays())
    {
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(static_cast<GLint>(GL_BGRA), GL_UNSIGNED_BYTE,
            static_cast<GLsizei>(streams.Diffuse.Stride), streams.Diffuse.Data);
    }
    else
    {
        agiGLSwizzleColors(streams.Diffuse, vertex_count, agiGLDiffuseSwizzle);

        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(4, GL_UNSIGNED_BYTE, static_cast<GLsizei>(sizeof(u32)), agiGLDiffuseSwizzle);
    }

    // GL_COLOR_MATERIAL ONLY WHEN THE FORMAT ACTUALLY CARRIES A DIFFUSE. This is the white wheels.
    //
    // MM2 never sets COLORVERTEX or any of the *MATERIALSOURCE render states - gfxRenderState's
    // DoFlush transmits neither - so they hold their D3D defaults: COLORVERTEX on and
    // DIFFUSEMATERIALSOURCE = D3DMCS_COLOR1. D3D's rule for that pair is that a vertex format
    // WITHOUT D3DFVF_DIFFUSE falls back to the MATERIAL's diffuse. GL has no such fallback: with
    // GL_COLOR_MATERIAL on it takes the material from the current colour whether or not an array
    // supplies one, and for these formats the builder above sets that colour to kWhiteColor. Every
    // lit surface whose format carries no colour was therefore lit as if its material were white.
    //
    // The tyres are exactly that geometry - a 106-vertex mesh, a dark 64x64 texture, and a material
    // diffuse of 0.10 that was being replaced by 1.00. Body panels carry a material of 1.00 already,
    // which is why they looked right and hid this.
    //
    // THE TEST IS THE FVF BIT, NOT Diffuse.Data. An earlier attempt at this asked whether
    // Diffuse.Data was null and did nothing at all, because both builders point that at a white
    // constant when the format has no diffuse - it is never null. Stride 0 with real data is D3D's
    // "one colour for every vertex", which IS a diffuse and must keep overriding the material.
    {
        // THE DEPTH MASK MUST FOLLOW THE GAME, NOT WHATEVER THE 2D PATH LEFT BEHIND.
        //
        // Measured in the vehicle preview: every draw of the car ran with depth=1 mask=0 - depth
        // testing on, depth WRITES off. With no depth writes nothing occludes anything, so the car
        // is painted in submission order and geometry that belongs inside the body - wheel-well
        // interiors and the near-black 0.008 pieces - lands on top of the paint as flat black
        // rectangles. That is the artefact in the showcase.
        //
        // agigl/glpipe.cpp's agiGLBegin2D turns depth writes off for the 2D layer, and agiGLEnd2D
        // deliberately does not force them back on - it relies on agiGLApplyRenderState restoring
        // the mask from the game's own ZWriteEnable. In the menu the showcase car is drawn without
        // that restore happening in between, so it inherits the 2D mask.
        //
        // Taking the mask from RSTATE at draw time closes that hole by construction: whatever the
        // 2D path did, a 3D draw writes depth exactly when gfxRenderState says it should. The call
        // is cached in agiGLContext, so a run of draws with the same state costs nothing.
        // The depth mask follows the game, the way Open1560's glrsys.cpp does it:
        //
        //     if (bool zwrite = agiCurState.GetZWrite(); zwrite != agiLastState.ZWrite)
        //         agiGL->DepthMask(zwrite);
        //
        // An earlier version here forced opaque draws to write depth, to make the showcase car
        // solid. It did not change what was on screen and it departs from both MM1's proven
        // translation and MM2's own state, so it is gone. The preview bug is still open - see
        // docs/gameplay_defects.md defect 12.
        agiGL->DepthMask(agiGLDepthWriteEnabled());

        // See agiGLEnsureSomeLightEnabled: lighting on with nothing enabled can only draw black.
        {
            extern void agiGLEnsureSomeLightEnabled();

            agiGLEnsureSomeLightEnabled();
        }

        const bool track = agiGLLightingEnabled() && (streams.Fvf & D3DFVF_DIFFUSE) != 0;

        agiGL->EnableDisable(GL_COLOR_MATERIAL, track);

        if (track)
        {
            // Tracking is about to take the material from the current colour, so anything the
            // re-issue below last pushed is now stale in GL.
            extern void agiGLInvalidateMaterialColors();

            agiGLInvalidateMaterialColors();
        }

        // Turning tracking off does NOT give the material back the value SetMaterial supplied - GL
        // kept whatever the current colour last tracked into it. It has to be re-issued.
        if (!track)
        {
            extern void agiGLApplyMaterialColors();

            agiGLApplyMaterialColors();
        }
    }

    // DIAGNOSTIC: count WORLD-SIZED draws that go down while GL_LIGHTING is off.
    //
    // Retail does not flicker and we do, so the fullbright frames are ours. A whole-scene fullbright
    // frame means the city was drawn unlit, which this counts directly instead of inferring it from
    // draw signatures. agigl/glframe.cpp reports and clears it per frame.
    if (vertex_count >= 50)
    {
        extern i32 agiGLUnlitBigDraws;
        extern i32 agiGLFlatBigDraws;
        extern i32 agiGLEnabledLightCount();

        // Lighting is always on and always has a light (both measured), so record HOW MANY are
        // contributing at world draws. A scene that brightens and dims is a light count - or a
        // light value - that changes when it should not.
        extern i32 agiGLFrameLightMin;
        extern i32 agiGLFrameLightMax;

        const i32 lights = agiGLEnabledLightCount();

        if (lights < agiGLFrameLightMin)
            agiGLFrameLightMin = lights;

        if (lights > agiGLFrameLightMax)
            agiGLFrameLightMax = lights;

        if (!agiGLLightingEnabled())
            ++agiGLUnlitBigDraws;
        else if (lights == 0)
            ++agiGLFlatBigDraws;
    }

    // TEMPORARY PROBE: characterise every distinct kind of draw once.
    //
    // The wheels are still white after the material rule above, so the remaining question is what
    // those draws actually consist of - textured or not, which texture, what material, how many
    // vertices. Reporting each distinct combination once keeps this to a few dozen lines while
    // naming every kind of geometry on screen.
    {
        extern f32 agiGLProbeMaterial[4];
        extern u32 agiGLProbeBoundTexture;

        const bool textured = agiGL->ProbeCap(GL_TEXTURE_2D);
        const bool lit = agiGLLightingEnabled();
        // Hashed from the float BITS rather than converted to bytes: a float-to-int cast pulls in
        // __ftol2, which this runtime does not link.
        u32 mat = 0;

        for (i32 i = 0; i < 3; ++i)
        {
            const u32 bits = *reinterpret_cast<const u32*>(&agiGLProbeMaterial[i]);

            mat = (mat * 31u) ^ bits;
        }

        const u64 key = (static_cast<u64>(textured ? 1u : 0u) << 63) | (static_cast<u64>(lit ? 1u : 0u) << 62) |
            (static_cast<u64>(agiGLProbeBoundTexture) << 40) | (static_cast<u64>(mat & 0xFFFFFF) << 16) |
            (vertex_count & 0xFFFF);

        static u64 seen[96] = {};
        static i32 seen_count = 0;

        bool known = false;

        for (i32 i = 0; i < seen_count; ++i)
        {
            if (seen[i] == key)
                known = true;
        }

        if (!known && seen_count < 96)
        {
            seen[seen_count++] = key;

            if (agiGLProbeVerbose())
                Displayf("DRAWKIND tex=%d id=%u lit=%d diffuse=%.2f,%.2f,%.2f v=%u hasdiff=%d", textured ? 1 : 0,
                    agiGLProbeBoundTexture, lit ? 1 : 0, agiGLProbeMaterial[0], agiGLProbeMaterial[1],
                    agiGLProbeMaterial[2], vertex_count, (streams.Fvf & D3DFVF_DIFFUSE) != 0 ? 1 : 0);
        }
    }

    // DIAGNOSTIC: the first lit mesh draws, with everything that decides whether they come out
    // black. The vehicle preview is drawn in the MENU, where there is almost nothing else lit, so a
    // small unsampled window catches the car itself rather than a race's worth of city.
    //
    // Three different faults all render a car black and need opposite fixes:
    //   lights=0            - nothing is lighting it
    //   material 0,0,0      - it is being lit correctly to black
    //   texture bound but 0 - the paint texture is black, which was the in-game bug
    if (vertex_count >= 20 && agiGLLightingEnabled())
    {
        extern f32 agiGLMaterialDiffuse[4];
        extern i32 agiGLEnabledLightCount();
        extern u32 agiGLProbeBoundTextureName();

        static i32 shown = 0;

        if (shown < 30 && agiGLProbeVerbose())
        {
            ++shown;

            // Blend state included because the environment-map pass is ADDITIVE by design -
            // modShader::BeginEnvMap asks for blend set 7, which is D3DBLEND_ONE/ONE - and a black
            // env map added to the paint is invisible. It only paints the panel black if GL_BLEND
            // is OFF, which turns the additive pass into an opaque replace.
            // Depth included: the black rectangles sit ON TOP of the paint rather than being part
            // of it, and geometry with a near-black material that should be hidden inside the body
            // will do exactly that if its depth test is off. Textures are all confirmed real, so
            // colour is no longer the question.
            Displayf("LITMESH %d v=%u lights=%d diffuse=%.3f tex=%u blend=%d(%X,%X) depth=%d mask=%d fvf=%03X",
                shown, vertex_count, agiGLEnabledLightCount(), agiGLMaterialDiffuse[0],
                agiGLProbeBoundTextureName(), agiGL->ProbeCap(GL_BLEND) ? 1 : 0, agiGL->ProbeBlendSrc(),
                agiGL->ProbeBlendDst(), agiGL->ProbeCap(GL_DEPTH_TEST) ? 1 : 0,
                agiGL->ProbeDepthMask() ? 1 : 0, streams.Fvf);
        }
    }

    // SPECULAR. D3D7 adds it after texturing, which is GL_COLOR_SUM plus a secondary colour and
    // nothing else in fixed function. A driver with neither GL 1.4 nor EXT_secondary_color loses
    // the specular highlight; drawing it as the primary colour instead would be worse than losing
    // it, so it is dropped and said once.
    if (glSecondaryColorPointer == nullptr || glSecondaryColor3ubv == nullptr)
    {
        if (streams.Specular.Data != nullptr && !agiGLSaidNoSpecular)
        {
            Warningf("GL: no secondary colour support, D3DFVF_SPECULAR is being dropped");

            agiGLSaidNoSpecular = true;
        }
    }
    else if (streams.Specular.Data == nullptr)
    {
        glDisableClientState(GL_SECONDARY_COLOR_ARRAY);
        agiGL->EnableDisable(GL_COLOR_SUM, false);
    }
    else if (streams.Specular.Stride == 0)
    {
        GLubyte rgba[4];

        agiGLUnpackD3DColor(*static_cast<const u32*>(streams.Specular.Data), rgba);

        glDisableClientState(GL_SECONDARY_COLOR_ARRAY);
        glSecondaryColor3ubv(rgba);
        agiGL->EnableDisable(GL_COLOR_SUM, true);
    }
    else
    {
        // glSecondaryColorPointer takes only size 3, so GL_BGRA is not available to it and the
        // swizzle is unconditional. The alpha byte of a D3D specular is unused anyway.
        agiGLSwizzleColors(streams.Specular, vertex_count, agiGLSpecularSwizzle);

        glEnableClientState(GL_SECONDARY_COLOR_ARRAY);
        glSecondaryColorPointer(3, GL_UNSIGNED_BYTE, static_cast<GLsizei>(sizeof(u32)), agiGLSpecularSwizzle);
        agiGL->EnableDisable(GL_COLOR_SUM, true);
    }

    if (streams.TexCoord0.Data != nullptr && agiGLCheckStride(streams.TexCoord0, "texture coordinate"))
    {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, static_cast<GLsizei>(streams.TexCoord0.Stride), streams.TexCoord0.Data);
    }
    else
    {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    // index_count is an INDEX count, not a triangle count. data/layouts.json and MM2Hook both call
    // gfxPacket+0x16 TriCount and both are wrong; see docs/gfx_layouts.md §16.1. Multiplying by
    // three here is the exact mistake that field name invites.
    if (indexed)
        glDrawElements(gl_mode, static_cast<GLsizei>(index_count), GL_UNSIGNED_SHORT, indices);
    else
        glDrawArrays(gl_mode, 0, static_cast<GLsizei>(vertex_count));

    // A forced error check on the first few draws only.
    //
    // Rendering faults are otherwise invisible without looking at the screen, and a GL error here
    // names the cause directly - INVALID_OPERATION for a bad client array, INVALID_ENUM for a mode
    // this build got wrong. Bounded because it costs a pipeline stall: glGetError has to see every
    // command issued before it.
    {
        static i32 checked = 0;

        if (checked < 24)
        {
            ++checked;
            agiGL->CheckErrors(false);
        }
    }
}

// One row-vector times a row-major Matrix44. Read off the FPU sequence at 0x004B355E onwards:
// y' takes m[+0x04], m[+0x14], m[+0x24] and m[+0x34], which is column 1 at a 16-byte row stride,
// and x' and z' take columns 0 and 2 the same way.
static void agiGLTransformPoint(const f32* m, const f32* v, f32* out)
{
    out[0] = v[0] * m[0] + v[1] * m[4] + v[2] * m[8] + m[12];
    out[1] = v[0] * m[1] + v[1] * m[5] + v[2] * m[9] + m[13];
    out[2] = v[0] * m[2] + v[1] * m[6] + v[2] * m[10] + m[14];
}

// The same without the translation row, which is what the normal half of the loop does - it never
// touches m[+0x30], m[+0x34] or m[+0x38].
//
// It also does not renormalise, and neither does this. A non-uniform scale in the palette therefore
// produces normals of the wrong length, exactly as in 1999; GL_NORMALIZE is left to whatever set the
// rest of the lighting state.
static void agiGLTransformVector(const f32* m, const f32* v, f32* out)
{
    out[0] = v[0] * m[0] + v[1] * m[4] + v[2] * m[8];
    out[1] = v[0] * m[1] + v[1] * m[5] + v[2] * m[9];
    out[2] = v[0] * m[2] + v[1] * m[6] + v[2] * m[10];
}

// sizeof(Matrix44). The original reaches palette entry n with `shl eax, 6`.
static constexpr u32 kMatrix44Size = 64;

bool agiGLSkinPacket(
    const agiGLPacketFields* packet, const void* palette, const u32* constant_color, agiGLStridedDraw& out)
{
    u32 fvf = packet->VertexTypeDesc;

    out.Fvf = fvf;
    u32 adjunct_count = packet->AdjunctCount;

    if (adjunct_count > kMaxSkinnedAdjuncts)
    {
        // The original does not check. Its two scratch buffers are adjacent, so it writes the
        // normals of adjunct 2048 onwards over the positions of adjuncts 0, 1, 2 - garbage
        // geometry, not a crash, and nothing anywhere reports it. Reproducing an out-of-bounds
        // write is not available to C++ (it is undefined behaviour the compiler may assume cannot
        // happen), so this refuses the packet and says so instead.
        Errorf("agiGLSkinPacket: %u adjuncts exceeds the %u the retail scratch holds", adjunct_count,
            kMaxSkinnedAdjuncts);

        return false;
    }

    if ((fvf & D3DFVF_POSITION_MASK) != D3DFVF_XYZ)
    {
        Errorf("agiGLSkinPacket: FVF 0x%03X has position type 0x%X, and only D3DFVF_XYZ is skinnable", fvf,
            fvf & D3DFVF_POSITION_MASK);

        return false;
    }

    u32 stride = gfxFVFSize(fvf);

    const u8* vertices = static_cast<const u8*>(packet->Vertices);
    const u8* matrices = static_cast<const u8*>(palette);
    const u8* groups = packet->AdjunctMatrixGroups;
    const u8* matrix_indices = packet->MatrixIndices;

    // MatrixIndices is a counted array: [0] holds the count and [1..n] the palette indices, so the
    // group index is always offset by one. That +1 is in the machine code as
    // `mov al, byte ptr [ebx + edi + 1]`, not as an off-by-one to be tidied away.
    bool lighting = agiGLLightingEnabled();

    // The original gates the normal pass on RSTATE.State.Lighting ALONE and reads the normal from
    // Vertices + 12 with a bare `lea edx, [ecx + 0xc]`. With lighting on and an FVF that has no
    // D3DFVF_NORMAL, that reads up to twelve bytes past the vertex block and transforms garbage
    // into the normal scratch - which D3D then ignores, because the format does not declare a
    // normal. Testing the bit as well removes the out-of-bounds read and cannot change what is
    // drawn, since the only case it suppresses is the one whose result was never consumed.
    bool transform_normals = lighting && (fvf & D3DFVF_NORMAL) != 0;

    for (u32 i = 0; i < adjunct_count; ++i)
    {
        const f32* m = reinterpret_cast<const f32*>(matrices + matrix_indices[groups[i] + 1] * kMatrix44Size);
        const f32* src = reinterpret_cast<const f32*>(vertices + i * stride + kPacketPositionOffset);

        agiGLTransformPoint(m, src, &agiGLSkin.Positions[i * 3]);

        if (transform_normals)
        {
            const f32* normal = reinterpret_cast<const f32*>(vertices + i * stride + kPacketNormalOffset);

            agiGLTransformVector(m, normal, &agiGLSkin.Normals[i * 3]);
        }
    }

    // THE RESKIN PASS REWRITES POSITIONS AND LEAVES NORMALS ALONE. That is not an omission in the
    // transcription: the loop at 0x004B3637 touches 0x0068BCE8 three times and 0x00685CE8 never, so
    // a blended vertex keeps the normal of its primary bone.
    const agiGLReskin* reskin = static_cast<const agiGLReskin*>(packet->Reskin);
    u32 reskin_count = packet->ReskinCount;

    for (u32 r = 0; r < reskin_count; ++r)
    {
        const agiGLReskin& rec = reskin[r];
        const f32* m = reinterpret_cast<const f32*>(matrices + matrix_indices[rec.MatrixGroup + 1] * kMatrix44Size);

        f32 blended[3];

        agiGLTransformPoint(m, rec.Position, blended);

        f32 t = static_cast<f32>(rec.Weight) * kReskinWeightScale;
        f32* dst = &agiGLSkin.Positions[rec.AdjunctIndex * 3];

        // (new - old) * t + old, in that order, because that is the order of the fsub/fmul/fadd
        // triple in the original and float addition is not associative.
        dst[0] = (blended[0] - dst[0]) * t + dst[0];
        dst[1] = (blended[1] - dst[1]) * t + dst[1];
        dst[2] = (blended[2] - dst[2]) * t + dst[2];
    }

    out.Position.Data = agiGLSkin.Positions;
    out.Position.Stride = 3 * static_cast<u32>(sizeof(f32));

    if (transform_normals)
    {
        out.Normal.Data = agiGLSkin.Normals;
        out.Normal.Stride = 3 * static_cast<u32>(sizeof(f32));
    }
    else
    {
        // The original still declares the normal stream when lighting is off - it writes
        // 0x00685CE8 and a stride of 12 into the strided block unconditionally - and D3D still
        // ignores it, because with lighting off there is nothing to consume a normal. Leaving it
        // null here is the same outcome without handing GL a stale buffer.
        out.Normal.Data = nullptr;
        out.Normal.Stride = 0;
    }

    // THE SKINNED PATH'S DIFFUSE RULE IS NOT Draw(uint)'s. It tests only the FVF's D3DFVF_DIFFUSE
    // bit; the colour argument is used whenever the format has no diffuse of its own, WITHOUT the
    // `!= -1` test the unskinned overload makes first. When the argument is -1 and the format has no
    // diffuse, the format handed to the device is left unchanged and D3D ignores the stream, which
    // is white - so passing 0xFFFFFFFF through has the same effect either way.
    if ((fvf & D3DFVF_DIFFUSE) != 0)
    {
        out.Diffuse.Data = vertices + gfxFVFOffset(fvf, kFVFComponentDiffuse);
        out.Diffuse.Stride = stride;
    }
    else
    {
        out.Diffuse.Data = (constant_color != nullptr) ? constant_color : &kWhiteColor;
        out.Diffuse.Stride = 0;
    }

    // Specular is always the zeroed local with a zero stride. The skinned path deliberately kills
    // the specular highlight rather than transforming it.
    out.Specular.Data = (fvf & D3DFVF_SPECULAR) != 0 ? &kZeroColor : nullptr;
    out.Specular.Stride = 0;

    // The original fills the texture stream unconditionally and lets the FVF decide whether D3D
    // reads it. Gating on the bit is the same outcome, and it also avoids the second half of that
    // block: the original never writes texcoord[1] at all, so an FVF asking for two coordinate sets
    // would have D3D read an uninitialised stack slot. Only set 0 is offered here.
    if ((fvf & D3DFVF_TEX1) != 0)
    {
        out.TexCoord0.Data = vertices + gfxFVFOffset(fvf, kFVFComponentTexCoord0);
        out.TexCoord0.Stride = stride;
    }
    else
    {
        out.TexCoord0.Data = nullptr;
        out.TexCoord0.Stride = 0;
    }

    return true;
}

#endif
