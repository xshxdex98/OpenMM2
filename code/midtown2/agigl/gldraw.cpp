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

define_dummy_symbol(agigl_gldraw);

#ifdef ARTS_AGIGL

#    include "data7/printer.h"

#    include "gfx/ddrawshim.h"

#    include "glcontext.h"
#    include "glframe.h"
#    include "glpacket.h"
#    include "glsurface.h"
#    include "gfx/gfxrenderstatedata.h"
#    include "gfx/gfxrenderstate.h"

#    include "glstub.h"

// WHERE GEOMETRY FINALLY REACHES THE SCREEN.
//
// Everything else in agigl/ has been about getting the game to the point of asking for a triangle.
// These two functions are the answer to that question: the game's own render path calls
// lpD3DDev->DrawPrimitive and DrawIndexedPrimitive, the device shim routes them here, and here they
// become glDrawArrays and glDrawElements.
//
// It is worth being precise about why this is small. agigl/glpacket.cpp already contains the hard
// half - agiGLBuildPacketStreams decodes a D3D flexible vertex format into pointer-and-stride pairs
// and agiGLDrawIndexedStrided sets up every client array and issues the draw - because gfxPacket
// needed exactly that. The device shim needs the same thing from a different caller, so this is
// the translation between the two conventions and nothing more.
//
// AND WHY IT IS NOT THE WHOLE JOB. The shim's SetTexture, SetRenderState and SetTextureStageState
// are still no-ops, so what these draw is untextured and unblended. Geometry appearing at all is
// the milestone; making it look right is the render-state work that follows.
//
// vertex buffers are not handled. gfxPipeline forces useNativeVBs false at bring-up
// (agigl/glgfx.cpp), so MM2 submits system-memory arrays and DrawPrimitiveVB is never called. If
// that ever changes, the VB entry points are still stubs and will silently draw nothing rather
// than crash.

namespace
{
    // D3DPRIMITIVETYPE, which is 1-based and in the same order GL declares its modes - but only
    // by coincidence for the first few, so it is written out rather than arithmetic.
    u32 GLModeFor(u32 d3d_primitive_type)
    {
        switch (d3d_primitive_type)
        {
            case 1: return GL_POINTS;         // D3DPT_POINTLIST
            case 2: return GL_LINES;          // D3DPT_LINELIST
            case 3: return GL_LINE_STRIP;     // D3DPT_LINESTRIP
            case 4: return GL_TRIANGLES;      // D3DPT_TRIANGLELIST
            case 5: return GL_TRIANGLE_STRIP; // D3DPT_TRIANGLESTRIP
            case 6: return GL_TRIANGLE_FAN;   // D3DPT_TRIANGLEFAN
        }

        // Not a silent fallback. An unknown primitive type means the caller and this table
        // disagree, and drawing triangles from a list that is not triangles produces garbage that
        // looks like a geometry bug rather than a mapping bug.
        Errorf("agiGL: unknown D3DPRIMITIVETYPE %u", d3d_primitive_type);

        return GL_TRIANGLES;
    }
} // namespace

// THE RENDER STATE THE GAME SET, AND WHY IT HAS TO BE CACHED RATHER THAN JUST APPLIED.
//
// The game sets a render state once and expects it to stay set. agiGLBegin2D does not honour that:
// it disables depth testing, disables depth writes and turns culling off every time any 2D is
// drawn, and the HUD draws every frame. So even with SetRenderState applying correctly, the first
// menu blit of the frame would undo it and the world would render with no depth test at all -
// faces painted in submission order, which looks like a mangled model rather than a state bug.
//
// So each state is remembered as well as applied, and agiGLEnd2D puts the whole set back. Same
// reasoning as the matrix cache in agigl/glframe.cpp: these are the game's values, so restoring
// them cannot drift out of step with what it believes is current.
// ?RSTATE@@3VgfxRenderState@@A - the game's own authoritative render state.
ARTS_IMPORT extern gfxRenderState RSTATE;

extern bool agiGLTrace;
extern i32 agiGLTracePending;
extern void agiGLTraceFlush();

void agiGLApplyTextureAddress();
void agiGLApplyTexGen();

namespace
{
    struct GLRenderState
    {
        bool DepthTest = true;
        bool DepthWrite = true;
        u32 DepthFunc = GL_LEQUAL;
        bool Blend = false;
        u32 SrcBlend = GL_ONE;
        u32 DstBlend = GL_ZERO;
        bool AlphaTest = false;

        // D3D's defaults: D3DCMP_ALWAYS with a reference of 0.
        u32 AlphaFunc = GL_ALWAYS;

        // Held as D3D holds it, 0..255, and converted to GL's 0..1 only at the call. Keeping a f32
        // here meant GetRenderState had to convert back, and float-to-int lowers to __ftol2, which
        // does not exist in the CRT reproduced in game.asm.
        u32 AlphaRef = 0;
        bool Lighting = false;
        bool Fog = false;
        bool Cull = true;
        u32 FrontFace = GL_CCW;

        // D3DRENDERSTATE_TEXTUREFACTOR, an ARGB constant. gfxRenderState points stage 0's
        // COLORARG2 at it whenever it is non-zero, so it scales the texture rather than sitting
        // unused - it is how MM2 dims the paint reflection.
        u32 TextureFactor = 0xFFFFFFFF;
        bool TextureFactorIsArg = false;

        // D3DTSS_COLOROP, kept so the texture factor is only honoured by ops that read ARG2.
        u32 ColorOp = 4; // D3DTOP_MODULATE, the D3D default for stage 0


        // D3DTSS_ADDRESSU / ADDRESSV for stage 0. GL holds these per texture rather than per
        // stage, so they are remembered here and applied to whatever is bound.
        u32 AddressU = GL_REPEAT;
        u32 AddressV = GL_REPEAT;

        // D3DTSS_TCI_CAMERASPACENORMAL, and whether a texture matrix accompanies it.
        bool TexGenNormal = false;
        bool TexTransform = false;
    };

    GLRenderState g_state;

    // TEMPORARY PROBE: monotonic counter so ZENABLE traffic and city draws can be put in order.
    i32 g_probe_seq = 0;

    // TEMPORARY PROBE - see agiGLTraceFlush in agigl/glframe.cpp.
    

    // D3DBLEND is 1-based and dense; GL's factors are not contiguous, so this is a table rather
    // than arithmetic.
    u32 GLBlendFor(u32 d3d)
    {
        switch (d3d)
        {
            case 1: return GL_ZERO;
            case 2: return GL_ONE;
            case 3: return GL_SRC_COLOR;
            case 4: return GL_ONE_MINUS_SRC_COLOR;
            case 5: return GL_SRC_ALPHA;
            case 6: return GL_ONE_MINUS_SRC_ALPHA;
            case 7: return GL_DST_ALPHA;
            case 8: return GL_ONE_MINUS_DST_ALPHA;
            case 9: return GL_DST_COLOR;
            case 10: return GL_ONE_MINUS_DST_COLOR;
        }

        return GL_ONE;
    }

    // D3DTADDRESS: 1 WRAP, 2 MIRROR, 3 CLAMP, 4 BORDER. GL_CLAMP_TO_EDGE stands in for BORDER as
    // well - agigl sets no border colour, and clamping to the edge is far closer to what MM2 wants
    // than GL's legacy GL_CLAMP, which samples the border and shows as a pale seam.
    u32 GLAddressFor(u32 d3d)
    {
        switch (d3d)
        {
            case 1: return GL_REPEAT;
            case 2: return GL_MIRRORED_REPEAT;
            case 3: return GL_CLAMP_TO_EDGE;
            case 4: return GL_CLAMP_TO_EDGE;
        }

        return GL_REPEAT;
    }

    // The inverse of GLBlendFor, for GetRenderState. Kept beside it so the two cannot drift.
    u32 D3DBlendFor(u32 gl)
    {
        switch (gl)
        {
            case GL_ZERO: return 1;
            case GL_ONE: return 2;
            case GL_SRC_COLOR: return 3;
            case GL_ONE_MINUS_SRC_COLOR: return 4;
            case GL_SRC_ALPHA: return 5;
            case GL_ONE_MINUS_SRC_ALPHA: return 6;
            case GL_DST_ALPHA: return 7;
            case GL_ONE_MINUS_DST_ALPHA: return 8;
            case GL_DST_COLOR: return 9;
            case GL_ONE_MINUS_DST_COLOR: return 10;
        }

        return 2;
    }

    // D3DCMPFUNC 1..8 maps onto GL_NEVER..GL_ALWAYS, which ARE contiguous from 0x200 and in the
    // same order. Written as the arithmetic it is, with the range checked.
    u32 GLCompareFor(u32 d3d)
    {
        return (d3d >= 1 && d3d <= 8) ? (0x0200u + (d3d - 1u)) : GL_LEQUAL;
    }
} // namespace

// GL keeps the wrap modes on the texture object, so they are re-applied whenever either the mode or
// the bound texture changes. MM2 asks for CLAMP on parts whose texture must not tile - the tyre
// tread strip is one - and leaving it at REPEAT wraps the far edge back into view.
void agiGLApplyTextureAddress()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(g_state.AddressU));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(g_state.AddressV));
}

// The environment-map coordinate generation from modShader::BeginEnvMap.
//
// D3D is asked for D3DTSS_TCI_CAMERASPACENORMAL plus a texture matrix built from the camera with its
// Z row negated; GL_NORMAL_MAP produces the same eye-space normal, and the matrix rides on the
// GL_TEXTURE stack. Without this the reflection sampled the sphere map through the car's own paint
// UVs, which is why it appeared as smears following the panel seams.
void agiGLApplyTexGen()
{
    const bool on = g_state.TexGenNormal;

    if (on)
    {
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, static_cast<GLint>(GL_NORMAL_MAP));
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, static_cast<GLint>(GL_NORMAL_MAP));
        glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, static_cast<GLint>(GL_NORMAL_MAP));
    }

    agiGL->EnableDisable(GL_TEXTURE_GEN_S, on);
    agiGL->EnableDisable(GL_TEXTURE_GEN_T, on);
    agiGL->EnableDisable(GL_TEXTURE_GEN_R, on);

    // With the transform switched off the texture matrix has to go back to identity, or the last
    // environment-mapped object leaves its camera matrix on everything drawn afterwards.
    if (!g_state.TexTransform)
    {
        glMatrixMode(GL_TEXTURE);
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW);
    }
}

// The constant colour for geometry whose vertex format carries no diffuse.
//
// D3D substitutes white in that case, which is what the draw path used unconditionally. But when
// gfxRenderState has pointed stage 0's COLORARG2 at D3DTA_TFACTOR, the texture is meant to be
// modulated by the texture factor instead - and for the paint reflection that factor is what stops
// an additive pass blowing the bodywork out to white.
u32 agiGLPacketConstantColor()
{
    // WHITE, UNCONDITIONALLY, until D3DTSS_COLOROP is actually translated.
    //
    // This used to return the texture factor whenever gfxRenderState pointed stage 0's COLORARG2 at
    // it. That was written for the vehicle showcase and never actually fired there - the probe
    // showed the game sets neither COLORARG2 nor TEXTUREFACTOR in the frontend - so it went in
    // unverified. In a race it DOES fire, and it is wrong there: COLORARG2 only participates when
    // COLOROP is a blend that reads it, and agiGLStubSetTextureStageState translates COLOROP as
    // nothing more than "is texturing on". With SELECTARG1 the factor is ignored by D3D and
    // multiplied in here, which darkens geometry the game never meant to darken.
    //
    // Restoring the factor means translating the colour op properly first, so the two agree about
    // when it applies - which is what the test below finally does.
    //
    // D3DTOP values: 1 DISABLE, 2 SELECTARG1, 3 SELECTARG2, 4 MODULATE, 5 MODULATE2X, 6 MODULATE4X,
    // 7 ADD. Only the ops from SELECTARG2 upwards read ARG2 at all; with SELECTARG1 the factor is
    // ignored by D3D, and multiplying it in here is what darkened geometry the game never meant to
    // darken. That is the whole reason the earlier version of this was reverted.
    //
    // Why it matters beyond the paint reflection: vehCarModel::DrawShadow draws the car's shadow
    // mesh with SetBlendSet(RSTATE, 0, 128) - SRCALPHA/INVSRCALPHA, so the shadow is meant to be
    // translucent - and the alpha it blends with rides on the texture factor. Discarding the factor
    // gives the shadow alpha 255, and a translucent shadow becomes an opaque black rectangle, which
    // is what the vehicle preview shows at each wheel arch.
    const bool op_reads_arg2 = g_state.ColorOp >= 3 && g_state.ColorOp <= 7;

    if (g_state.TextureFactorIsArg && op_reads_arg2)
        return g_state.TextureFactor;

    return 0xFFFFFFFFu;
}

void agiGLApplyRenderState()
{
    // EXPERIMENT: depth test and depth writes forced ON.
    //
    // Measured: the main view's draws run with depth=0 and mask=0, while the mirror - which renders
    // the same world correctly - runs with depth=1 and mask=1. lvlSky turns both off for the sky and
    // restores them through RSTATE; the restore is not reaching GL.
    //
    // With no depth buffer and ~470 overlapping draws a frame, whatever is drawn last covers
    // everything, which is what a black view and a "sphere that kept growing" both look like.
    //
    // If the world appears, the real fix is finding why the restore is lost, not this.
    // DEPTH COMES FROM RSTATE, NOT FROM THE SetRenderState CALLS WE HAPPEN TO RECEIVE.
    //
    // Measured: the city draws with depth test AND depth writes off, and `wantdepth=0` shows that is
    // our own tracked state - the game never sent us the enable. But cityLevel::DrawRooms says
    // plainly what it wants:
    //
    //     vglBeginBatch();
    //     if (RSTATE.State.ZWriteEnable) { RSTATE.State.ZWriteEnable = 0; m_Touched |= 1; }
    //     if (!RSTATE.State.ZEnable)     { RSTATE.State.ZEnable = 1;     m_Touched |= 1; }
    //
    // depth test ON, depth writes OFF. It marks that in RSTATE and sets m_Touched, but whether it
    // ever reaches the device depends on m_TouchedMask, which DrawRooms sets to m_TouchedMasks[0] -
    // and with the sky having turned depth off just before, the city renders with no depth buffering
    // at all. Everything then draws in submission order, so whichever room is drawn last wins and
    // the picture changes as the camera moves. That is the flicker.
    //
    // RSTATE is the game's own view of what the device state should be, so reading depth from it is
    // more faithful than reading a cache built only from the calls that survived the gating.
    agiGL->EnableDisable(GL_DEPTH_TEST, RSTATE.State.ZEnable != 0);
    agiGL->DepthMask(RSTATE.State.ZWriteEnable != 0);
    agiGL->DepthFunc(g_state.DepthFunc);
    // Blend state comes from what SetRenderState pushed, as it did in the last state confirmed
    // working. Pulling it live from RSTATE was tried and reverted - see the note in
    // gfxPipeline::BeginGfx3D about sm_SupportsBlendWithOne, which is the real reason blend set 7
    // never arrives, and which must be retried on its own against a confirmed baseline.
    agiGL->EnableDisable(GL_BLEND, g_state.Blend);
    agiGL->BlendFunc(g_state.SrcBlend, g_state.DstBlend);
    agiGL->EnableDisable(GL_ALPHA_TEST, g_state.AlphaTest);

    // THE ALPHA FUNCTION HAS TO BE RE-ISSUED HERE, not just the enable.
    //
    // Two things were wrong without this. D3DRENDERSTATE_ALPHAFUNC and ALPHAREF were not translated
    // at all, so every alpha-tested draw ran with GL's default of ALWAYS/0 and every texel passed.
    // And agiGLBegin2D sets its own alpha function for the 2D colour key - GL_GREATER at 0.5 - which
    // agiGLEnd2D was not restoring, so whatever ran next inherited it. It is the same trap the
    // matrices, the viewport and the rest of the render state each already had.
    //
    // What it cost: MM2 draws car body panels a second time with the paint reflection map, and
    // relies on the alpha test to keep only the highlight texels. With everything passing, that
    // second pass covered the panels edge to edge and the body showed the reflection instead of its
    // paint. Wheels never showed it because nothing redraws them.
    glAlphaFunc(g_state.AlphaFunc, static_cast<f32>(g_state.AlphaRef) * (1.0f / 255.0f));
    // LIGHTING FOLLOWS THE GAME'S OWN RENDER STATE. Restored at the user's request to the state
    // that rendered on 2026-08-29 around 12:10 - good draw distance, black cars, sky dome present.
    //
    // Turning this off did NOT just remove the dome: it collapsed the visible world to a few car
    // lengths, with a flat cream expanse above the road. That is the same sky geometry either way -
    // lit to black with this on, unlit and covering the scene with it off - which is why the "draw
    // distance" regression and the "black dome" are one object misbehaving, not two faults.
    //
    // lvlSky (MM2_RE_KIT lvlSky.c:218-246) draws the sky with Lighting = 0, ZWriteEnable = 0 and
    // ZEnable = 0, restoring all three afterwards. If that state reached GL the sky would be unlit
    // AND unable to occlude anything, and neither symptom could happen. It does not reach GL, and
    // THAT is the bug worth fixing - not this flag, which only decides which way the sky is wrong.
    // Lighting follows the pushed render state, as it did in the last state confirmed working.
    //
    // Reading it from RSTATE instead - Open1560's approach, and probably still the right one - was
    // tried and is NOT in place, because it went in on top of an already-broken baseline and there
    // was no way to tell what it did. It should be retried on its own, against a binary that is
    // known good, with the RSTATEDIFF probe to say whether the pushed value was ever wrong.
    // DIAGNOSTIC: every transition of the lighting enable, with who asked for it.
    //
    // "Fullbright then back to normal" is what GL_LIGHTING being OFF looks like - geometry drawn at
    // full unmodulated colour. Lights enabled but none contributing would be dark instead, so this
    // narrows to the toggle itself. Transitions are rare, so this is cheap and ungated.
    {
        static i32 last = -1;

        if (static_cast<i32>(g_state.Lighting) != last)
        {
            last = static_cast<i32>(g_state.Lighting);

            if (agiGLProbeVerbose())
                Displayf("LIGHTTOGGLE now=%d", last);
        }
    }

    agiGL->EnableDisable(GL_LIGHTING, g_state.Lighting);

    // GL_COLOR_MATERIAL IS WHAT MAKES VERTEX COLOUR SURVIVE LIGHTING.
    //
    // D3D's COLORVERTEX is on by default, so a lit vertex that carries a diffuse colour uses THAT
    // as its diffuse material. GL does not do this unless told: with lighting on and no
    // glColorMaterial it ignores the colour array entirely and lights everything by the current
    // glMaterial. Turning lighting on without this turned the whole city black - buildings, lamp
    // posts, traffic lights - because all of that geometry carries baked vertex colour and suddenly
    // none of it counted.
    //
    // GL_DIFFUSE ALONE, AND THIS IS THE FULLBRIGHT FLICKER.
    //
    // The old line tracked GL_AMBIENT_AND_DIFFUSE on the stated grounds that "D3D's default ambient
    // material source is the vertex colour too". It is not:
    //
    //     D3DRENDERSTATE_DIFFUSEMATERIALSOURCE  default D3DMCS_COLOR1    - the vertex colour
    //     D3DRENDERSTATE_AMBIENTMATERIALSOURCE  default D3DMCS_MATERIAL  - the MATERIAL
    //
    // MM2 sets neither, so both defaults stand. Tracking ambient from the vertex colour makes the
    // ambient material WHITE for every surface whose vertices have no colour of their own.
    //
    // On its own that is a brightness error. What made it a FLICKER is what the game does with the
    // global ambient, caught by logging every change while driving:
    //
    //     AMBIENT FF7F7F7F
    //     AMBIENT 00000000
    //     AMBIENT FF7F7F7F      ... every frame
    //
    // MM2 raises the ambient to mid-grey for one pass and drops it to black for another, which is
    // ordinary D3D behaviour: retail multiplies that by each material's ambient, which is usually
    // dark, so the swing is invisible. We multiplied it by white, so the 0x7F phase added half a
    // grey to every lit surface - fullbright - and the 0x00 phase left dark materials with nothing
    // lifting them at all. Both halves of the reported symptom, one cause.
    //
    // The material's own ambient still reaches GL: agiGLStubSetMaterial's glMaterialfv(GL_AMBIENT)
    // is no longer writing to a tracked property, so it now takes effect instead of being
    // overwritten by the current colour.
    if (g_state.Lighting && glColorMaterial != nullptr)
        glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);

    agiGL->EnableDisable(GL_COLOR_MATERIAL, g_state.Lighting);
    // FOG IS FORCED OFF, because agigl cannot supply what this fog model needs.
    //
    // Open1560's renderer - the same engine family, working - sets up fog as:
    //
    //     glFogi(GL_FOG_MODE, GL_LINEAR);
    //     glFogi(GL_FOG_COORD_SRC, GL_FOG_COORD);
    //
    // The second line is the point: the fog factor comes from a PER-VERTEX fog coordinate, not from
    // fragment depth. agigl supplies no fog coordinate, so with GL_FOG enabled GL falls back to its
    // default GL_FRAGMENT_DEPTH and computes fog from depth against a range that was never meant
    // for it. cityLevel::DrawRooms turns fog on for the whole city, so that fogged the entire world
    // out.
    //
    // Configuring the colour, start and end - which this now also does - does not help while the
    // SOURCE is wrong. Supplying a real fog coordinate stream is the proper fix and is what
    // Open1560 does; until then, no fog is far closer to correct than black fog.
    agiGL->EnableDisable(GL_FOG, false);
    agiGL->EnableDisable(GL_CULL_FACE, g_state.Cull);
    agiGL->FrontFace(g_state.FrontFace);
}

// IDirect3DDevice7::GetRenderState, which used to be a stub that returned zero and left the
// caller's variable untouched.
//
// gfxRenderState::Init memsets itself to zero and then reads seven states BACK from the device to
// seed the dirty-flag baseline it compares every later commit against:
//
//     GetRenderState(D3DRENDERSTATE_CLIPPING, &v); this->State.Clipping = v != 0;
//     ... LIGHTING, TEXTUREPERSPECTIVE, ZENABLE, ZWRITEENABLE, ZFUNC, FILLMODE
//
// `v` is an uninitialised local. With the stub writing nothing, that baseline was seeded from stack
// rubbish, so any state whose rubbish happened to match the value the game later wanted was
// silently never sent to the device at all - and which states those were changed run to run.
//
// Answering from the tracked state makes the baseline true by construction: the game is told
// exactly what this renderer is currently set to.
// The last D3DRENDERSTATE_AMBIENT the game set. D3D's default is black and MM2 does not set it at
// startup, so black is also the right initial value here.
static u32 g_ambient = 0;

ilong __stdcall agiGLStubGetRenderState(void* self, u32 state, u32* out)
{
    (void) self;

    if (out == nullptr)
        return 0x80004005;

    switch (state)
    {
        case 4: *out = 1; break;  // D3DRENDERSTATE_TEXTUREPERSPECTIVE, always on under GL
        case 7: *out = g_state.DepthTest ? 1u : 0u; break;   // D3DRENDERSTATE_ZENABLE
        case 8: *out = 3; break;  // D3DRENDERSTATE_FILLMODE, D3DFILL_SOLID
        case 14: *out = g_state.DepthWrite ? 1u : 0u; break; // D3DRENDERSTATE_ZWRITEENABLE
        case 15: *out = g_state.AlphaTest ? 1u : 0u; break;  // D3DRENDERSTATE_ALPHATESTENABLE

        // The blend factors are held as GL enums, so they come back through the inverse of
        // GLBlendFor rather than being guessed at.
        case 19: *out = D3DBlendFor(g_state.SrcBlend); break;
        case 20: *out = D3DBlendFor(g_state.DstBlend); break;

        // D3DCMPFUNC 1..8 and GL_NEVER..GL_ALWAYS are the same order from 0x200, which is what
        // GLCompareFor relies on going the other way.
        case 23: *out = (g_state.DepthFunc - 0x0200u) + 1u; break; // D3DRENDERSTATE_ZFUNC
        case 24: *out = g_state.AlphaRef; break;
        case 25: *out = (g_state.AlphaFunc - 0x0200u) + 1u; break; // D3DRENDERSTATE_ALPHAFUNC
        case 27: *out = g_state.Blend ? 1u : 0u; break;            // D3DRENDERSTATE_ALPHABLENDENABLE
        case 28: *out = g_state.Fog ? 1u : 0u; break;              // D3DRENDERSTATE_FOGENABLE
        case 136: *out = 1; break;                                 // D3DRENDERSTATE_CLIPPING
        case 137: *out = g_state.Lighting ? 1u : 0u; break;        // D3DRENDERSTATE_LIGHTING

        // D3DRENDERSTATE_AMBIENT, AND LEAVING IT OUT HAD TEETH.
        //
        // modShader::BeginEnvMap saves the ambient with GetRenderState and modShader::EndEnvMap
        // writes it straight back:
        //
        //     BeginEnvMap:  GetRenderState(D3DRENDERSTATE_AMBIENT, &saved)
        //     EndEnvMap:    SetRenderState(D3DRENDERSTATE_AMBIENT, saved)
        //
        // Falling through to `default: *out = 0` therefore did not merely lose a read - it made the
        // game itself force the global ambient to black at the end of every environment-mapped
        // pass, which is once per car per frame. Whatever cityLevel's per-room lighting had set was
        // wiped by the first reflective car drawn, and the buildings that depend on that ambient
        // went dark.
        //
        // The same shape as the GetLightEnable pair a few lines up in BeginEnvMap: MM2 saves device
        // state and restores it, so a getter that lies is not a missing feature, it is the game
        // being told to overwrite its own state with a fiction.
        case 139: *out = g_ambient; break;

        // Anything not tracked answers zero rather than leaving the caller's variable alone. An
        // untouched out-param is the failure this function exists to remove.
        default: *out = 0; break;
    }

    return 0;
}

// IDirect3DDevice7::SetViewport.
//
// gfxViewport::DoFlush is ported and routes its viewport through agiGLFlushViewport, so the common
// path was covered - but anything that sets the viewport DIRECTLY on the device was landing in a
// stub that did nothing, leaving whatever rectangle happened to be current. Geometry then draws at
// the wrong scale rather than in the wrong place, because the projection it is paired with assumes
// the rectangle that was asked for.
//
// The vehicle showcase is the case that matters: it renders into a panel occupying part of the
// screen, so its viewport is small and its projection is built for that. Ignoring the request left
// the full-screen viewport in place and the car came out filling the window.
// IDirect3DDevice7::SetTransform, and leaving it a stub is why the showcase car did not turn.
//
// D3D carries THREE transforms - world, view and projection - and GL's fixed pipeline has two,
// because it folds world and view into one modelview. Ignoring SetTransform therefore does not
// merely lose a detail: the world matrix is where an object's own placement, rotation and scale
// live, so every model was drawn at the origin, unrotated, at whatever size its vertices happen to
// be in model space. A car that does not spin and is far too big is exactly that.
//
// THE MULTIPLY IS IN D3D'S ORDER, NOT GL'S. D3D multiplies row vectors on the left, v * W * V, and
// GL multiplies column vectors on the right, V * W * v. The two conventions store the transpose of
// each other, so the bytes of a D3D matrix are already a valid GL matrix - and the product that
// belongs in GL's modelview is the one D3D would compute as W * V. Composing them the other way
// round is the classic way to get a scene that is subtly, consistently wrong.
namespace
{
    f32 g_world[16];
    f32 g_view[16];
    bool g_have_world;
    bool g_have_view;

    void Identity(f32* m)
    {
        for (i32 i = 0; i < 16; ++i)
            m[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }

    // out = a * b, both row-major, which is D3D's order.
    void MultiplyD3D(const f32* a, const f32* b, f32* out)
    {
        for (i32 row = 0; row < 4; ++row)
        {
            for (i32 col = 0; col < 4; ++col)
            {
                f32 sum = 0.0f;

                for (i32 k = 0; k < 4; ++k)
                    sum += a[(row * 4) + k] * b[(k * 4) + col];

                out[(row * 4) + col] = sum;
            }
        }
    }

    void ApplyModelView()
    {
        f32 world[16];
        f32 view[16];

        if (g_have_world)
        {
            for (i32 i = 0; i < 16; ++i)
                world[i] = g_world[i];
        }
        else
        {
            Identity(world);
        }

        if (g_have_view)
        {
            for (i32 i = 0; i < 16; ++i)
                view[i] = g_view[i];
        }
        else
        {
            Identity(view);
        }

        f32 mv[16];

        MultiplyD3D(world, view, mv);

        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(mv);
    }
} // namespace

// THE VIEW MATRIX DOES NOT ARRIVE THROUGH SetTransform, AND ASSUMING IT DID LOST IT ENTIRELY.
//
// MM2 sets the view through gfxRenderState::SetCamera, not through the device - a capture of every
// SetTransform call while the vehicle screen was open shows state 1 (world) and state 16
// (texture 0), and no state 2 at any point. SetCamera used to load its matrix straight into
// GL_MODELVIEW, which worked only for as long as nothing else touched that matrix.
//
// Adding SetTransform(WORLD) broke exactly that: it recomputes the modelview as world * view, and
// with no view ever recorded it composed against identity and overwrote what SetCamera had loaded.
// The car was then drawn at the world origin with the camera also at the origin - enormous, seen
// from underneath, and not turning, because an orbit lives in the view.
//
// So both writers feed one state and the composition happens in one place.
// Re-applies the composed modelview. agiGLEnd2D needs this after 2D has overwritten the matrix,
// and it must go through the composition rather than reloading the view alone - reloading the view
// is what dropped the world matrix in the first place.
// Loads the VIEW matrix alone over the current modelview, saving what was there. Lights must be
// issued under this and nothing else - see the note in agiGLSetViewTransform. Returns false when no
// view has been set yet, in which case nothing was pushed and the caller must not pop.
bool agiGLPushViewOnly()
{
    if (!g_have_view)
        return false;

    // No glPushMatrix here: the matrix stack entry points are not in agigl/glcommon.h's loader
    // table, and there is nothing to save that ApplyModelView cannot rebuild - it composes
    // world * view from the two matrices agigl already holds, which is exactly what was loaded.
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(g_view);

    return true;
}

void agiGLPopViewOnly()
{
    ApplyModelView();
}

void agiGLApplyModelView()
{
    ApplyModelView();
}

void agiGLSetViewTransform(const f32* view)
{
    if (view == nullptr)
        return;

    for (i32 i = 0; i < 16; ++i)
        g_view[i] = view[i];

    g_have_view = true;

    // LIGHTS ARE ISSUED WITH THE VIEW MATRIX ALONE LOADED, AND THE ORDER HERE IS THE WHOLE POINT.
    //
    // D3D keeps its lights in world space; GL bakes the modelview into a light's position at the
    // moment glLightfv is called. agigl/gllight.cpp states the resulting rule plainly - "a light
    // can only be issued while the modelview holds the VIEW matrix alone" - and this function used
    // to break it, by calling ApplyModelView() first and applying lights second.
    //
    // ApplyModelView() loads world * view. Issuing lights under that places every one of them
    // relative to whatever object was drawn last, so their direction is wrong by an arbitrary
    // object transform and N.L comes out at or below zero across whole models. The city never
    // showed it because its geometry carries baked vertex colour and takes no light; car bodies
    // are FVF 0x112 - position, normal, texture, and no diffuse at all - so they take ALL of their
    // shading from these lights and rendered as black silhouettes.
    //
    // So: load the view on its own, issue the lights against it, then restore the composed
    // modelview for drawing.
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(g_view);

    agiGLApplyLights();

    ApplyModelView();
}

ilong __stdcall agiGLStubSetTransform(void* self, u32 state, f32* matrix)
{
    (void) self;

    if (matrix == nullptr)
        return 0x80004005;

    switch (state)
    {
        case 1: // D3DTRANSFORMSTATE_WORLD
            for (i32 i = 0; i < 16; ++i)
                g_world[i] = matrix[i];

            g_have_world = true;
            ApplyModelView();
            break;

        case 2: // D3DTRANSFORMSTATE_VIEW
            for (i32 i = 0; i < 16; ++i)
                g_view[i] = matrix[i];

            g_have_view = true;
            ApplyModelView();
            break;

        case 3: // D3DTRANSFORMSTATE_PROJECTION
            agiGLSetProjectionMatrix(matrix);
            break;

        case 16: // D3DTRANSFORMSTATE_TEXTURE0
            glMatrixMode(GL_TEXTURE);
            glLoadMatrixf(matrix);
            glMatrixMode(GL_MODELVIEW);
            break;

        default: break;
    }

    return 0;
}

// IDirect3DDevice7::Clear, which was a stub that did nothing at all.
//
// EVERY CLEAR THE GAME ASKED FOR WAS BEING DROPPED. gfxPipeline::Clear is not one of our ported
// functions - it is ARTS_IMPORT, so the 1999 body runs - and the kit shows it ends in
//
//     lpD3DDev->lpVtbl->Clear(lpD3DDev, 0, 0, dwFlags, dwColor, dvZ, dwStencil);
//
// with count 0 and a null rect list, which in D3D clears the CURRENT VIEWPORT. That call landed on
// a do-nothing stub, so asCullManager's frame clear, mmMirror::Cull's per-viewport clear and
// mmGameManager::Cull's letterbox clears all silently did nothing. The only clear happening at all
// was agiGLBeginFrame's blanket one, so colour and depth from one pass survived into the next -
// which is what made geometry flicker in and out and the player's own car disappear.
//
// A null rect list means the viewport, so the scissor is set to it. glClear is masked by the depth
// write mask, and a clear that arrives with depth writes off would silently do nothing, so the mask
// is forced on for the clear and put back afterwards.
ilong __stdcall agiGLStubClear(void* self, u32 count, void* rects, u32 flags, u32 color, f32 z, u32 stencil)
{
    (void) self;
    (void) count;
    (void) rects;
    (void) stencil;

    // TEMPORARY PROBE. gfxPipeline::EndFrame issues Clear(1, FadeColor, ...) whenever the fade
    // colour has a non-zero alpha, which fills the viewport and would wipe the finished frame. That
    // call did nothing until Clear was implemented. Remove when solved.
    {
        static i32 shown = 0;

        if (shown < 24)
        {
            ++shown;
            i32 r[4] = {0, 0, 0, 0};

            const bool have = agiGLGetViewportRect(r);

            if (agiGLProbeVerbose())
                Displayf("DEVCLEAR flags=%X color=%08X z=%.2f rect=%d %d,%d %dx%d", flags, color, z,
                    have ? 1 : 0, r[0], r[1], r[2], r[3]);
        }
    }

    GLbitfield mask = 0;

    if (flags & 1) // D3DCLEAR_TARGET
    {
        glClearColor(static_cast<f32>((color >> 16) & 0xFF) * (1.0f / 255.0f),
            static_cast<f32>((color >> 8) & 0xFF) * (1.0f / 255.0f),
            static_cast<f32>(color & 0xFF) * (1.0f / 255.0f), 1.0f);

        mask |= GL_COLOR_BUFFER_BIT;
    }

    if (flags & 2) // D3DCLEAR_ZBUFFER
    {
        glClearDepth(static_cast<GLclampd>(z));
        agiGL->DepthMask(true);

        mask |= GL_DEPTH_BUFFER_BIT;
    }

    if (mask == 0)
        return 0;

    // ASK GL FOR THE VIEWPORT RATHER THAN TRUSTING OUR CACHE.
    //
    // A null rect list means "clear the current viewport", and this used to scissor to
    // agiGLLastViewport - the last rectangle that happened to pass through agiGLFlushViewport. That
    // cache goes stale: during a race every clear was being confined to 128,699 2431x576, which is
    // the FRONTEND's showcase panel from when the logical size was still 1920x1080. Everything
    // outside that band was never cleared, so it alternated between the two swap-chain buffers and
    // the picture flickered.
    //
    // glGetIntegerv(GL_VIEWPORT) is the viewport GL is actually going to rasterise with, so it
    // cannot disagree with the clear.
    GLint vp[4] {0, 0, 0, 0};

    glGetIntegerv(GL_VIEWPORT, vp);

    if (vp[2] > 0 && vp[3] > 0)
    {
        glScissor(vp[0], vp[1], vp[2], vp[3]);
        agiGL->EnableDisable(GL_SCISSOR_TEST, true);
    }

    if (agiGLTrace)
    {
        agiGLTraceFlush();
        Displayf("T  CLEAR flags=%X vp=%d,%d %dx%d", flags, vp[0], vp[1], vp[2], vp[3]);
    }

    glClear(mask);

    agiGL->EnableDisable(GL_SCISSOR_TEST, false);
    agiGL->DepthMask(g_state.DepthWrite);

    return 0;
}

ilong __stdcall agiGLStubSetViewport(void* self, D3DVIEWPORT7* vp)
{
    (void) self;

    if (vp == nullptr)
        return 0x80004005;

    // flags 1 is "the rectangle changed" in gfxViewport::Flags terms, which is what
    // agiGLFlushViewport reads. The projection is not touched here - D3D sets that through
    // SetTransform, and this call carries no matrix.
    agiGLFlushViewport(nullptr, 1, static_cast<i32>(vp->dwX), static_cast<i32>(vp->dwY),
        static_cast<i32>(vp->dwWidth), static_cast<i32>(vp->dwHeight), vp->dvMinZ, vp->dvMaxZ);

    return 0;
}

ilong __stdcall agiGLStubSetRenderState(void* self, u32 state, u32 value)
{
    (void) self;

    switch (state)
    {
        // D3DRENDERSTATE_ZENABLE
        case 7:
            g_state.DepthTest = (value != 0);

            if (agiGLTrace)
            {
                agiGLTraceFlush();
                Displayf("T  ZENABLE=%u", value);
            }

            {
                static i32 shown = 0;

                if (shown < 60)
                {
                    ++shown;
                    if (agiGLProbeVerbose())
                        Displayf("SEQ %d ZENABLE %u", ++g_probe_seq, value);
                }
            }
            break;
        // D3DRENDERSTATE_ZWRITEENABLE
        case 14:
            g_state.DepthWrite = (value != 0);

            if (agiGLTrace)
            {
                agiGLTraceFlush();
                Displayf("T  ZWRITE=%u", value);
            }
            {
                static i32 shown = 0;

                if (shown < 30)
                {
                    ++shown;
                    if (agiGLProbeVerbose())
                        Displayf("ZWRITE %u", value);
                }
            }
            break;
        case 15: g_state.AlphaTest = (value != 0); break;  // D3DRENDERSTATE_ALPHATESTENABLE
        case 19: g_state.SrcBlend = GLBlendFor(value); break;
        case 20: g_state.DstBlend = GLBlendFor(value); break;

        // D3DCULL: 1 NONE, 2 CW, 3 CCW. D3D culls the face it names; GL culls the BACK face and is
        // told which winding is front, so the two are inverted with respect to each other.
        case 22:
            g_state.Cull = (value != 1);
            g_state.FrontFace = (value == 2) ? GL_CCW : GL_CW;
            break;

        case 23: g_state.DepthFunc = GLCompareFor(value); break; // D3DRENDERSTATE_ZFUNC

        // D3DRENDERSTATE_ALPHAREF is 0..255 over the same range GL spells 0..1.
        case 24: g_state.AlphaRef = value & 0xFF; break;
        case 25: g_state.AlphaFunc = GLCompareFor(value); break; // D3DRENDERSTATE_ALPHAFUNC
        case 27: g_state.Blend = (value != 0); break;            // D3DRENDERSTATE_ALPHABLENDENABLE
        case 28: g_state.Fog = (value != 0); break;              // D3DRENDERSTATE_FOGENABLE

        // FOG, WHICH UNTIL NOW WAS ENABLED AND NEVER CONFIGURED.
        //
        // cityLevel::DrawRooms opens with lvlSky::SetupFog, so the whole city is drawn with fog on.
        // GL's DEFAULTS are colour (0,0,0,0) - black - mode GL_EXP and density 1.0, which fogs
        // everything past a fraction of a unit to solid black. The city was being submitted and
        // drawn correctly and then painted black by fog nobody had configured.
        //
        // The float states arrive as the bit pattern of a f32 inside a u32, which is why they are
        // reinterpreted rather than converted.
        case 34: // D3DRENDERSTATE_FOGCOLOR
        {
            const f32 fog_color[4] = {
                static_cast<f32>((value >> 16) & 0xFF) * (1.0f / 255.0f),
                static_cast<f32>((value >> 8) & 0xFF) * (1.0f / 255.0f),
                static_cast<f32>(value & 0xFF) * (1.0f / 255.0f),
                1.0f,
            };

            glFogfv(GL_FOG_COLOR, fog_color);
            break;
        }

        // D3DFOGMODE: 0 NONE, 1 EXP, 2 EXP2, 3 LINEAR. Both the table and vertex mode land here;
        // GL has one fog stage, so whichever the game sets last wins - and MM2 sets only one.
        case 35:
        case 140:
            glFogi(GL_FOG_MODE,
                static_cast<GLint>((value == 1) ? GL_EXP : ((value == 2) ? GL_EXP2 : GL_LINEAR)));
            break;

        case 36: glFogf(GL_FOG_START, *reinterpret_cast<const f32*>(&value)); break;
        case 37: glFogf(GL_FOG_END, *reinterpret_cast<const f32*>(&value)); break;
        case 38: glFogf(GL_FOG_DENSITY, *reinterpret_cast<const f32*>(&value)); break;
        case 60: g_state.TextureFactor = value; break;           // D3DRENDERSTATE_TEXTUREFACTOR

        // D3DRENDERSTATE_AMBIENT is a colour rather than a light, so it goes to the light model.
        // D3DRENDERSTATE_AMBIENT. The value is REMEMBERED as well as applied, because the game
        // reads it back - see the 139 case in agiGLStubGetRenderState.
        case 139:
            g_ambient = value;
            agiGLSetAmbient(value);
            break;
        case 137: g_state.Lighting = (value != 0); break;        // D3DRENDERSTATE_LIGHTING

        // DIAGNOSTIC: D3DRENDERSTATE_VERTEXBLEND (0x97). Not translated - GL fixed function has no
        // equivalent - so this records whether MM2 ever actually asks for it. Pedestrians vanish
        // when CLOSE, and close is the branch that draws them skinned.
        case 151:
        {
            static u32 last = 0xFFFFFFFFu;

            if (value != last)
            {
                last = value;

                if (agiGLProbeVerbose())
                    Displayf("VERTEXBLEND %u", value);
            }

            break;
        }

        default: return 0; // Everything else has no GL equivalent worth guessing at.
    }

    agiGLApplyRenderState();

    return 0;
}

// TEMPORARY PROBE STATE: what is bound, for the draw-characterisation probe in glpacket.cpp.
f32 agiGLProbeMaterial[4] = {1.0f, 1.0f, 1.0f, 1.0f};
u32 agiGLProbeBoundTexture = 0;

// The GL name of whatever is bound to stage 0, for the diagnostic in glpacket.cpp.
static u32 g_bound_texture_name = 0;

u32 agiGLProbeBoundTextureName()
{
    return g_bound_texture_name;
}

ilong __stdcall agiGLStubSetTexture(void* self, u32 stage, void* texture)
{
    (void) self;

    // Only stage 0 is bound. MM2 sets a second stage for a handful of multitexture effects, and
    // binding it here without also translating the stage state that drives it would blend two
    // textures by whatever GL's defaults happen to be - which looks like corruption rather than a
    // missing feature. The single-texture case is the overwhelming majority and is correct.
    if (stage != 0)
        return 0;

    // agiGLContext::ActiveTexture takes a UNIT INDEX and adds GL_TEXTURE0 itself. Passing the enum
    // asked for unit 0x84C0, which is where the "GL Error: Invalid Enum (00000500)" logged after
    // the first SetTexture came from. `stage` is already the index and is already known to be 0.
    agiGL->ActiveTexture(stage);

    agiGLSurface* surface = agiGLResolveSurface(texture);

    if (surface == nullptr || surface->Texture == 0)
    {
        // A null texture is a legitimate call - it is how the game turns texturing off for
        // untextured geometry - so this is not a failure.
        agiGL->EnableDisable(GL_TEXTURE_2D, false);
        agiGLProbeBoundTexture = 0;

        return 0;
    }

    // TEMPORARY PROBE: a bound-but-never-uploaded texture samples undefined memory and reads as a
    // flat white patch, which is exactly what the wheels look like. Reported once per texture name
    // so a wheel drawn every frame does not fill the log.
    if (!surface->Uploaded)
    {
        static u32 reported[32] = {};
        static i32 report_count = 0;

        bool seen = false;

        for (i32 i = 0; i < report_count; ++i)
        {
            if (reported[i] == surface->Texture)
                seen = true;
        }

        if (!seen && report_count < 32)
        {
            reported[report_count++] = surface->Texture;

            if (agiGLProbeVerbose())
                Displayf("NOTUPLOADED gl=%u %dx%d", surface->Texture, surface->Width, surface->Height);
        }
    }

    agiGL->EnableDisable(GL_TEXTURE_2D, true);
    agiGL->BindTexture2D(surface->Texture);
    g_bound_texture_name = surface->Texture;
    agiGLProbeBoundTexture = surface->Texture;

    agiGLApplyTextureAddress();

    // -gltexdump writes each distinct bound texture out once as a .bmp. This is how the vehicle
    // showcase was finally understood: the 256x256 texture drawn over every body panel turned out
    // to be the environment sphere map, and reading a paint texture back settled the channel order
    // that two rounds of reasoning had got backwards.
    if (agiGLTextureDumpEnabled())
        agiGLDumpTexture(surface->Texture, surface->Width, surface->Height);

    return 0;
}

// IDirect3DDevice7::SetTextureStageState.
//
// Only one of the many stage states matters here, and it matters a great deal: D3DTSS_COLOROP set
// to D3DTOP_DISABLE is how the game turns texturing OFF. Without it, untextured geometry keeps
// whatever texture the last 2D blit left bound - which is why the car in the vehicle screen was
// drawn wearing the interface, with "HORSEPOWER" and "MASS" legible across its bodywork.
//
// The colour and alpha argument/operation states that build MM2's multitexture effects are
// deliberately not translated. Guessing at them would blend two textures by GL's defaults, which
// looks like corruption rather than a missing feature; a single texture modulated by the vertex
// colour is what the overwhelming majority of the game asks for and is what the default GL
// environment already does.
ilong __stdcall agiGLStubSetTextureStageState(void* self, u32 stage, u32 state, u32 value)
{
    (void) self;

    if (stage != 0)
        return 0;

    switch (state)
    {
        // D3DTSS_COLOROP; D3DTOP_DISABLE is 1.
        case 1: // D3DTSS_COLOROP
            g_state.ColorOp = value;
            agiGL->EnableDisable(GL_TEXTURE_2D, value != 1);
            break;

        // D3DTSS_COLORARG2. gfxRenderState points this at D3DTA_TFACTOR (3) exactly when the
        // texture factor is non-zero, which is the signal that the factor is meant to scale the
        // texture. agiGLPacketConstantColor is where it is applied.
        case 3: g_state.TextureFactorIsArg = (value == 3); break;

        // D3DTSS_TEXCOORDINDEX. 0x10000 is D3DTSS_TCI_CAMERASPACENORMAL, which asks for the
        // camera-space normal as the texture coordinate - GL spells that GL_NORMAL_MAP.
        // D3DTSS_TEXCOORDINDEX. 0x10000 is D3DTSS_TCI_CAMERASPACENORMAL - the camera-space normal
        // the environment map is projected through.
        case 11:
            g_state.TexGenNormal = (value == 0x10000);
            agiGLApplyTexGen();
            break;

        case 13:
            g_state.AddressU = GLAddressFor(value);
            agiGLApplyTextureAddress();
            break;

        case 14:
            g_state.AddressV = GLAddressFor(value);
            agiGLApplyTextureAddress();
            break;

        // D3DTSS_TEXTURETRANSFORMFLAGS; D3DTTFF_DISABLE is 0.
        case 24:
            g_state.TexTransform = (value != 0);
            agiGLApplyTexGen();
            break;

        default: break;
    }

    return 0;
}

ilong __stdcall agiGLStubDrawPrimitive(void* self, u32 ptype, u32 fvf, void* verts, u32 vcount, u32 flags)
{
    (void) self;
    (void) flags;

    if (verts == nullptr || vcount == 0)
        return 0;

    agiGLStridedDraw streams {};

    if (!agiGLBuildPacketStreams(fvf, verts, nullptr, streams))
        return 0;

    agiGLDrawIndexedStrided(streams, vcount, nullptr, 0, GLModeFor(ptype));

    return 0;
}

// IDirect3DDevice7::DrawIndexedPrimitiveStrided - AND PEDESTRIANS VANISH WITHOUT IT.
//
// This slot was a stub returning S_OK and drawing nothing, which is invisible geometry with no
// error anywhere. gfxPacket::Draw ends in exactly this call:
//
//     lpD3DDev->lpVtbl->DrawIndexedPrimitiveStrided(lpD3DDev, D3DPT_TRIANGLELIST, fvf,
//         stridedData, PacketSize, Indices, TriCount, 0);
//
// WHY IT SHOWED UP ON PEDESTRIANS AND ONLY UP CLOSE. aiPedestrianInstance::Draw computes the
// squared distance to the camera and compares it against the threshold at 0x005B5870:
//
//     distSq < threshold  ->  drawModel = true   -> modModel::Draw, the SKINNED path
//     distSq >= threshold ->  drawModel = false  -> a cheaper draw
//
// The skinned path is the one that reaches gfxPacket::Draw with a matrix palette, and CPU skinning
// writes its transformed positions and normals into SEPARATE scratch buffers - which is precisely
// what a strided draw is for. So a pedestrian was drawn correctly until it came close enough to be
// skinned, and then vanished.
//
// D3DDRAWPRIMITIVESTRIDEDDATA is an array of {const void* lpvData; DWORD dwStride;} pairs in FVF
// order: position, normal, diffuse, specular, then eight texture coordinate sets. agiGLStridedDraw
// is the same shape, so this is a translation rather than a reimplementation, and the drawing is
// the same agiGLDrawIndexedStrided every other path already uses.
ilong __stdcall agiGLStubDrawIndexedPrimitiveStrided(
    void* self, u32 ptype, u32 fvf, void* strided, u32 vcount, u16* indices, u32 icount, u32 flags)
{
    (void) self;
    (void) flags;

    if (strided == nullptr || vcount == 0 || indices == nullptr || icount == 0)
        return 0;

    struct D3DPtrStride
    {
        const void* Data;
        u32 Stride;
    };

    const D3DPtrStride* in = static_cast<const D3DPtrStride*>(strided);

    agiGLStridedDraw streams {};

    streams.Fvf = fvf;

    streams.Position.Data = in[0].Data;
    streams.Position.Stride = in[0].Stride;

    if ((fvf & D3DFVF_NORMAL) != 0)
    {
        streams.Normal.Data = in[1].Data;
        streams.Normal.Stride = in[1].Stride;
    }

    // The same rule the interleaved builder uses: a format with no diffuse still needs a colour, and
    // D3D substitutes white. Leaving Data null would make agiGLDrawIndexedStrided read it as "no
    // colour at all" rather than "white for every vertex".
    static const u32 white = 0xFFFFFFFFu;

    if ((fvf & D3DFVF_DIFFUSE) != 0)
    {
        streams.Diffuse.Data = in[2].Data;
        streams.Diffuse.Stride = in[2].Stride;
    }
    else
    {
        streams.Diffuse.Data = &white;
        streams.Diffuse.Stride = 0;
    }

    // SPECULAR IS DELIBERATELY NOT READ, and this is not an omission.
    //
    // gfxPacket::Draw builds this array on its stack and writes only four of the pairs - position,
    // normal, diffuse and texture coordinate set 0. The specular pair is left exactly as the stack
    // found it, which agigl/glpacket.cpp already records for the interleaved path. Honouring
    // D3DFVF_SPECULAR here would therefore hand GL a pointer to whatever was on that stack.
    //
    // The FVF this call receives is `VertexTypeDesc | 0x40`, so the diffuse bit is always set and
    // in[2] is always the constant colour the caller supplied at stride 0. Specular is only ever
    // set if the packet's own format carried it, and nothing in MM2 has been observed to.

    if ((fvf & D3DFVF_TEX1) != 0)
    {
        streams.TexCoord0.Data = in[4].Data;
        streams.TexCoord0.Stride = in[4].Stride;
    }

    agiGLDrawIndexedStrided(streams, vcount, indices, icount, GLModeFor(ptype));

    return 0;
}

ilong __stdcall agiGLStubDrawIndexedPrimitive(
    void* self, u32 ptype, u32 fvf, void* verts, u32 vcount, u16* indices, u32 icount, u32 flags)
{
    // TEMPORARY PROBE: what vertex formats the city actually submits. vglEnd draws the city through
    // this entry point with vglVertexStack and DefaultDrawIndices, and vglBegin picks between two
    // vertex layouts on currentVertexFormat & 0x10 - so a mis-decoded stride here would draw from
    // misaligned memory and flicker. Remove when solved.
    {
        // The city is identifiable: FVF 0x102 with an index count far above its vertex count. vgl
        // draws always have icount == vcount, so this cannot be one of those. Sampled once every 60
        // such draws so successive frames can be compared for whatever is changing.
        static i32 seen = 0;

        if (fvf == 0x102 && icount > 1000 && ((++seen % 200) == 0))
        {
            // want= is what the GAME asked for (g_state, set by SetRenderState); depth=/mask= is
            // what GL actually has. If they disagree, our state cache is desynced; if they agree
            // and are both off, the game genuinely has depth disabled at this point.
            if (agiGLProbeVerbose())
                Displayf("SEQ %d CITYDRAW v=%u i=%u wantdepth=%d wantmask=%d depth=%d mask=%d func=%X "
                         "cull=%d front=%X blend=%d(%X,%X) alpha=%d tex=%d fog=%d",
                    ++g_probe_seq, vcount, icount, g_state.DepthTest ? 1 : 0, g_state.DepthWrite ? 1 : 0, agiGL->ProbeCap(GL_DEPTH_TEST), agiGL->ProbeDepthMask(),
                    agiGL->ProbeDepthFunc(), agiGL->ProbeCap(GL_CULL_FACE), agiGL->ProbeFrontFace(),
                    agiGL->ProbeCap(GL_BLEND), agiGL->ProbeBlendSrc(), agiGL->ProbeBlendDst(),
                    agiGL->ProbeCap(GL_ALPHA_TEST), agiGL->ProbeCap(GL_TEXTURE_2D),
                    agiGL->ProbeCap(GL_FOG));
        }
    }

    // TEMPORARY PROBE: LIT DRAWS. Remove once the cars are right.
    //
    // Cars are the only lit geometry in the game - the city carries baked vertex colour and takes
    // no light - so sampling draws with lighting enabled samples exactly the two things still
    // wrong: bodies that render black in-race, and a car preview whose body does not occlude its
    // own wheels.
    //
    // Both suspects are in one line. `normal=` says whether the FVF carries normals at all, which
    // decides whether GL lighting can produce anything but black; `depth=`/`mask=` say whether the
    // body was allowed to occlude, which decides the see-through preview. Reading GL's own state
    // rather than g_state, because the question is what the driver ended up with.
    {
        // SAMPLED PERIODICALLY AND FOREVER, not capped at the first sixteen. The first budget was
        // spent entirely on menu draws - every one of them blend set 0, GL_SRC_ALPHA /
        // GL_ONE_MINUS_SRC_ALPHA - and the race never got a look in. The interesting draws are the
        // car bodies during a race, which is thousands of draws later.
        static i32 lit_seen = 0;

        if (g_state.Lighting && ((++lit_seen % 400) == 0))
        {

            // src/dst matter more than the enable bit. modShader::BeginEnvMap asks for
            // SetBlendSet(RSTATE, 7, 128) = SrcBlend 2, DestBlend 2, both D3DBLEND_ONE, which is
            // GL_ONE/GL_ONE - additive. The BODY pass underneath it should be opaque. Seeing
            // ONE/ONE on every lit draw would mean the body is being drawn additively too, which
            // is a different fault from the env map covering it.
            //
            // light0 distinguishes the two passes without guessing: BeginEnvMap turns lights 0, 1
            // and 2 off for the duration of the sphere-map pass and EndEnvMap restores them.
            if (agiGLProbeVerbose())
                Displayf("LITDRAW %d fvf=%X normal=%d v=%u i=%u depth=%d mask=%d func=%X cull=%d "
                         "blend=%d(%X,%X) light0=%d tex=%d colmat=%d",
                    lit_seen, fvf, (fvf & 0x010u) ? 1 : 0, vcount, icount, agiGL->ProbeCap(GL_DEPTH_TEST),
                    agiGL->ProbeDepthMask(), agiGL->ProbeDepthFunc(), agiGL->ProbeCap(GL_CULL_FACE),
                    agiGL->ProbeCap(GL_BLEND), agiGL->ProbeBlendSrc(), agiGL->ProbeBlendDst(),
                    agiGL->ProbeCap(GL_LIGHT0), agiGL->ProbeCap(GL_TEXTURE_2D), agiGL->ProbeCap(GL_COLOR_MATERIAL));
        }
    }

    (void) self;
    (void) flags;

    // DIAGNOSTIC: a position type other than D3DFVF_XYZ means blend weights are present.
    if ((fvf & 0x00Eu) != 0x002u)
    {
        static u32 seen[8] = {};
        static i32 count = 0;

        bool known = false;

        for (i32 i = 0; i < count; ++i)
        {
            if (seen[i] == fvf)
                known = true;
        }

        if (!known && count < 8)
        {
            seen[count++] = fvf;

            if (agiGLProbeVerbose())
                Displayf("BLENDFVF fvf=%03X v=%u i=%u", fvf, vcount, icount);
        }
    }

    if (verts == nullptr || vcount == 0 || indices == nullptr || icount == 0)
        return 0;

    agiGLStridedDraw streams {};

    if (!agiGLBuildPacketStreams(fvf, verts, nullptr, streams))
        return 0;

    agiGLDrawIndexedStrided(streams, vcount, indices, icount, GLModeFor(ptype));

    return 0;
}

#endif
