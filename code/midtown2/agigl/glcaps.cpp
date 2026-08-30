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

#include "core/arts.h"

define_dummy_symbol(agigl_glcaps);

#ifdef ARTS_AGIGL

#    include "data7/printer.h"

#    include "glcommon.h"

// IDirect3DDevice7::GetCaps, which used to be a stub that returned zero and wrote nothing.
//
// This is not a cosmetic gap. gfxPipeline reads this structure ONCE at device creation and derives
// the renderer's whole shape from it:
//
//     sm_SupportsBlendWithOne = (dpcTriCaps.dwSrcBlendCaps & dwDestBlendCaps & D3DPBLENDCAPS_ONE)
//     sm_MaxActiveLights      = dwMaxActiveLights
//     sm_MaxBlendMatrices     = wMaxVertexBlendMatrices - 1
//     maxTextures             = wMaxSimultaneousTextures
//
// With the structure left untouched every one of those came out wrong, and the failure was silent
// and total rather than localised. sm_SupportsBlendWithOne being false is the one that showed:
// gfxRenderState::SetBlendSet collapses EVERY blend set to set 0 when it is clear, so the car paint
// reflection - which asks for set 7, D3DBLEND_ONE / D3DBLEND_ONE, plain additive - was drawn with
// SRCALPHA / INVSRCALPHA instead. The environment map's alpha is 255 everywhere, so that blend is
// opaque, and the reflection replaced the paint rather than adding light to it. That is the orange
// and purple banding across car bodies in the vehicle showcase.
//
// sm_MaxBlendMatrices came out as -1 and sm_MaxActiveLights as 0 for the same reason, so this was
// never only a showcase bug.
//
// What is reported here describes THIS renderer, not a 1999 card: fixed-function GL with one
// texture stage bound. Claiming more than agigl implements would move the failure rather than fix
// it - wMaxSimultaneousTextures is 1 precisely because agiGLStubSetTexture binds stage 0 only, and
// MM2 answers that honestly by doing the environment map as a second additive pass, which is a path
// this renderer can draw correctly.

namespace
{
    // D3DPRIMCAPS, 0x38 bytes, at offsets 0x04 (line) and 0x3C (tri) of D3DDEVICEDESC7.
    struct D3DPrimCaps
    {
        u32 dwSize;
        u32 dwMiscCaps;
        u32 dwRasterCaps;
        u32 dwZCmpCaps;
        u32 dwSrcBlendCaps;
        u32 dwDestBlendCaps;
        u32 dwAlphaCmpCaps;
        u32 dwShadeCaps;
        u32 dwTextureCaps;
        u32 dwTextureFilterCaps;
        u32 dwTextureBlendCaps;
        u32 dwTextureAddressCaps;
        u32 dwStippleWidth;
        u32 dwStippleHeight;
    };

    // D3DDEVICEDESC7, 0xEC bytes. Offsets are from the type library in MM2_RE_KIT.
    struct D3DDeviceDesc7
    {
        u32 dwDevCaps;                 // 0x000
        D3DPrimCaps dpcLineCaps;       // 0x004
        D3DPrimCaps dpcTriCaps;        // 0x03C
        u32 dwDeviceRenderBitDepth;    // 0x074
        u32 dwDeviceZBufferBitDepth;   // 0x078
        u32 dwMinTextureWidth;         // 0x07C
        u32 dwMinTextureHeight;        // 0x080
        u32 dwMaxTextureWidth;         // 0x084
        u32 dwMaxTextureHeight;        // 0x088
        u32 dwMaxTextureRepeat;        // 0x08C
        u32 dwMaxTextureAspectRatio;   // 0x090
        u32 dwMaxAnisotropy;           // 0x094
        f32 dvGuardBandLeft;           // 0x098
        f32 dvGuardBandTop;            // 0x09C
        f32 dvGuardBandRight;          // 0x0A0
        f32 dvGuardBandBottom;         // 0x0A4
        f32 dvExtentsAdjust;           // 0x0A8
        u32 dwStencilCaps;             // 0x0AC
        u32 dwFVFCaps;                 // 0x0B0
        u32 dwTextureOpCaps;           // 0x0B4
        u16 wMaxTextureBlendStages;    // 0x0B8
        u16 wMaxSimultaneousTextures;  // 0x0BA
        u32 dwMaxActiveLights;         // 0x0BC
        f32 dvMaxVertexW;              // 0x0C0
        u8 deviceGUID[16];             // 0x0C4
        u16 wMaxUserClipPlanes;        // 0x0D4
        u16 wMaxVertexBlendMatrices;   // 0x0D6
        u32 dwVertexProcessingCaps;    // 0x0D8
        u32 dwReserved1;               // 0x0DC
        u32 dwReserved2;               // 0x0E0
        u32 dwReserved3;               // 0x0E4
        u32 dwReserved4;               // 0x0E8
    };

    static_assert(sizeof(D3DPrimCaps) == 0x38, "D3DPRIMCAPS must be 0x38 bytes");
    static_assert(sizeof(D3DDeviceDesc7) == 0xEC, "D3DDEVICEDESC7 must be 0xEC bytes");
    static_assert(offsetof(D3DDeviceDesc7, dpcTriCaps) == 0x3C, "dpcTriCaps must be at 0x3C");
    static_assert(offsetof(D3DDeviceDesc7, wMaxSimultaneousTextures) == 0xBA, "layout drift");
    static_assert(offsetof(D3DDeviceDesc7, wMaxVertexBlendMatrices) == 0xD6, "layout drift");

    // All eleven D3DPBLENDCAPS bits. GL's glBlendFunc accepts every factor D3D7 can name, and
    // GLBlendFor in agigl/gldraw.cpp already maps all of them.
    constexpr u32 kAllBlendCaps = 0x000007FF;

    // All eight comparison functions, for both the depth and the alpha test.
    constexpr u32 kAllCmpCaps = 0x000000FF;

    void FillPrimCaps(D3DPrimCaps* caps)
    {
        caps->dwSize = sizeof(D3DPrimCaps);

        // D3DPMISCCAPS: MASKPLANES, MASKZ, CULLNONE, CULLCW, CULLCCW.
        caps->dwMiscCaps = 0x00000001 | 0x00000002 | 0x00000010 | 0x00000020 | 0x00000040;

        // D3DPRASTERCAPS: DITHER, ZTEST, FOGVERTEX, FOGTABLE, MIPMAPLODBIAS, ZBIAS, ANISOTROPY.
        caps->dwRasterCaps =
            0x00000001 | 0x00000010 | 0x00000080 | 0x00000100 | 0x00002000 | 0x00004000 | 0x00020000;

        caps->dwZCmpCaps = kAllCmpCaps;
        caps->dwSrcBlendCaps = kAllBlendCaps;
        caps->dwDestBlendCaps = kAllBlendCaps;
        caps->dwAlphaCmpCaps = kAllCmpCaps;

        // D3DPSHADECAPS: COLORGOURAUDRGB, SPECULARGOURAUDRGB, ALPHAGOURAUDBLEND, FOGGOURAUD.
        caps->dwShadeCaps = 0x00000008 | 0x00000200 | 0x00004000 | 0x00080000;

        // D3DPTEXTURECAPS: PERSPECTIVE, ALPHA, TRANSPARENCY, PROJECTED, ALPHAPALETTE.
        // NON_POW2CONDITIONAL is deliberately absent: agiGLSurfaceCreate makes ordinary
        // power-of-two textures and MM2's own images are power of two.
        caps->dwTextureCaps = 0x00000001 | 0x00000004 | 0x00000008 | 0x00000400;

        // D3DPTFILTERCAPS: NEAREST, LINEAR, MIPNEAREST, MIPLINEAR, LINEARMIPNEAREST,
        // LINEARMIPLINEAR, plus the MAG/MIN forms.
        caps->dwTextureFilterCaps = 0x00000001 | 0x00000002 | 0x00000004 | 0x00000008 | 0x00000010 |
            0x00000020 | 0x00000100 | 0x00000200 | 0x00000400 | 0x00001000 | 0x00002000;

        // D3DPTBLENDCAPS: DECAL, MODULATE, DECALALPHA, MODULATEALPHA, COPY, ADD.
        caps->dwTextureBlendCaps =
            0x00000001 | 0x00000002 | 0x00000004 | 0x00000008 | 0x00000040 | 0x00000080;

        // D3DPTADDRESSCAPS: WRAP, MIRROR, CLAMP, BORDER, INDEPENDENTUV.
        caps->dwTextureAddressCaps = 0x00000001 | 0x00000002 | 0x00000004 | 0x00000008 | 0x00000010;

        caps->dwStippleWidth = 0;
        caps->dwStippleHeight = 0;
    }
} // namespace

ilong __stdcall agiGLStubGetCaps(void* self, void* desc)
{
    (void) self;

    if (desc == nullptr)
        return 0x80004005;

    D3DDeviceDesc7* caps = static_cast<D3DDeviceDesc7*>(desc);

    // The caller passes a structure it has not cleared, and only some of these fields get written
    // below. Everything starts at zero so nothing is left as stack rubbish.
    for (usize i = 0; i < sizeof(D3DDeviceDesc7); ++i)
        reinterpret_cast<u8*>(caps)[i] = 0;

    // D3DDEVCAPS: EXECUTESYSTEMMEMORY, TLVERTEXSYSTEMMEMORY, TEXTURESYSTEMMEMORY,
    // DRAWPRIMTLVERTEX, HWRASTERIZATION, HWTRANSFORMANDLIGHT, DRAWPRIMITIVES2, DRAWPRIMITIVES2EX.
    //
    // 0x4000 - D3DDEVCAPS_SEPARATETEXTUREMEMORIES - is deliberately CLEAR. gfxPipeline treats it
    // as a reason to fall back to a single texture, and while that is the answer here anyway, it
    // should follow from wMaxSimultaneousTextures rather than from a bit that means something else.
    caps->dwDevCaps = 0x00000010 | 0x00000040 | 0x00000100 | 0x00000200 | 0x00080000 | 0x00010000 |
        0x00200000 | 0x00400000;

    FillPrimCaps(&caps->dpcLineCaps);
    FillPrimCaps(&caps->dpcTriCaps);

    // DDBD_16 | DDBD_32 for colour; DDBD_16 | DDBD_24 | DDBD_32 for depth.
    caps->dwDeviceRenderBitDepth = 0x00000400 | 0x00000100;
    caps->dwDeviceZBufferBitDepth = 0x00000400 | 0x00000200 | 0x00000100;

    caps->dwMinTextureWidth = 1;
    caps->dwMinTextureHeight = 1;

    // Asked of the driver rather than assumed, because this is the number MM2 uses to decide how
    // far it may scale a texture up.
    GLint max_texture = 0;

    if (glGetIntegerv != nullptr)
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture);

    if (max_texture < 256)
        max_texture = 256;

    caps->dwMaxTextureWidth = static_cast<u32>(max_texture);
    caps->dwMaxTextureHeight = static_cast<u32>(max_texture);
    caps->dwMaxTextureRepeat = 32768;
    caps->dwMaxTextureAspectRatio = static_cast<u32>(max_texture);
    caps->dwMaxAnisotropy = 1;

    caps->dvGuardBandLeft = 0.0f;
    caps->dvGuardBandTop = 0.0f;
    caps->dvGuardBandRight = 0.0f;
    caps->dvGuardBandBottom = 0.0f;
    caps->dvExtentsAdjust = 0.0f;

    // No stencil buffer is requested by the GL context, so claiming stencil support would be a lie
    // MM2 could act on.
    caps->dwStencilCaps = 0;

    // D3DFVFCAPS_TEXCOORDCOUNTMASK is the low 16 bits: one texture coordinate set.
    caps->dwFVFCaps = 1;

    // D3DTEXOPCAPS: DISABLE, SELECTARG1, SELECTARG2, MODULATE, MODULATE2X, MODULATE4X, ADD,
    // BLENDTEXTUREALPHA, BLENDFACTORALPHA, BLENDCURRENTALPHA.
    caps->dwTextureOpCaps = 0x00000001 | 0x00000002 | 0x00000004 | 0x00000008 | 0x00000010 |
        0x00000020 | 0x00000040 | 0x00004000 | 0x00008000 | 0x00020000;

    // ONE texture stage, because agiGLStubSetTexture binds stage 0 and ignores the rest. MM2 reads
    // this and falls back to multi-pass rendering, which is the path agigl can actually draw.
    caps->wMaxTextureBlendStages = 1;
    caps->wMaxSimultaneousTextures = 1;

    // GL fixed-function guarantees at least eight lights.
    caps->dwMaxActiveLights = 8;

    caps->dvMaxVertexW = 1.0e10f;

    caps->wMaxUserClipPlanes = 0;

    // One matrix, so gfxRenderState::sm_MaxBlendMatrices lands on 0 rather than the -1 a zeroed
    // structure produced.
    caps->wMaxVertexBlendMatrices = 1;

    caps->dwVertexProcessingCaps = 0;

    Displayf("agiGL: reported device caps, max texture %dx%d, 1 stage, 8 lights", max_texture, max_texture);

    return 0;
}

#endif
