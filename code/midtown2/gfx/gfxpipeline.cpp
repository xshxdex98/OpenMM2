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

#include "gfxpipeline.h"

#include "gfxbitmap.h"

#include "data7/printer.h"
#include "ddrawshim.h"
#include "origfallback.h"

#include "misc/freefuncs.h"

#include "gfxrenderstatedata.h"
#include "gfxrenderstate.h"
#include "gfxviewport.h"

#include "io/ioinput.h"

#ifdef ARTS_AGIGL
#    include "agigl/glblit.h"
#    include "agigl/glframe.h"
#    include "agigl/glgfx.h"
#    include "agigl/glstub.h"
#    include "agigl/glswitch.h"
#endif

define_dummy_symbol(gfx_gfxpipeline);

// ?useSoftware@@3_NA at 0x006830D4. Declared here rather than included because no generated header
// carries it; agigl/glsetres.cpp declares it the same way.
ARTS_IMPORT extern bool useSoftware;

// The render state and the copy DoFlush diffs against.
//
// Declared here rather than included from a generated header because neither is emitted into one -
// agigl/glpacket.cpp reaches RSTATE the same way, through an extern declaration. The class-keys
// matter: the retail names are ?RSTATE@@3VgfxRenderState@@A and
// ?LASTRSTATE@@3VgfxRenderStateData@@A, both V for class, so declaring either as a struct would
// produce a symbol that is not in the binary.
ARTS_IMPORT extern gfxRenderState RSTATE;
ARTS_IMPORT extern gfxRenderStateData LASTRSTATE;

// This function used to exist twice - once here and once in agigl/glgfxpipeline.cpp - behind
// complementary #ifs, so that defining ARTS_AGIGL_PIPELINE swapped one for the other. That was
// wrong in a way that only showed when the flag was actually tried: the flag switched this
// definition OFF at the same moment it switched the GL one on, but the GL object was still held
// back by quarantine, so the exported symbol ended up defined by nobody and the build failed at
// symbol verification. Worse, had it linked, the GL body has no DirectDraw path in it at all -
// so a build WITHOUT -gl would have stopped drawing the loading screen entirely.
//
// A compile-time swap cannot express what is wanted here. tools/asm.py strips the original PROC
// out of game.asm for every registered symbol, so there is exactly one CopyBitmap in the
// executable and it has to serve both paths. The choice is therefore made at RUNTIME, on the same
// agiGLEnabled() latch every other activated function uses, and the rule from agigl/glswitch.h
// applies unchanged: with -gl absent this must do exactly what it does today, which is why the
// DirectDraw body below is untouched and the GL branch returns before reaching it.

// Everything DirectDraw this file needs now lives in gfx/ddrawshim.h, so that Set A's
// transcriptions share one declaration of each structure instead of six. The comments justifying
// each vtable index and structure offset went with it.

// Transcribed from the disassembly of ?CopyBitmap@gfxPipeline@@SAXHHPAVgfxBitmap@@HHHH_N@Z at
// 0x004AB4C0. Nine instructions of substance: build a source RECT from (arg4, arg5, arg6, arg7),
// then BltFast it onto lpdsRend at (arg1, arg2).
//
// The last two arguments are a SOURCE extent, not a destination size - BltFast cannot scale. That
// is the whole reason MM2's UI does not scale, and it is deliberately still true below for every
// caller whose rectangle fits inside its bitmap.
//
// WHAT CHANGED, AND WHY IT IS NOT A SCALING POLICY.
//
// BltFast rejects a source rectangle that does not lie inside the source surface: it returns
// DDERR_INVALIDRECT and draws nothing at all. Exactly one caller in the binary does that.
// asUnderlay::Cull (0x004A22B0) passes gfxPipeline::m_iWidth and m_iHeight as the source extent,
// which is correct only while the underlay bitmap happens to be the size of the screen. At
// 2560x1440 against a 640x480 backdrop the call fails and there is NO MENU BACKGROUND AT ALL.
//
// So the oversize case is not a case that currently works - it is a case that currently draws
// nothing, and anything is an improvement. When the requested source rectangle does not fit, the
// extent is read as what asUnderlay plainly meant by it: the size to cover. The whole bitmap is
// stretched into that destination rectangle with Blt.
//
// This is NOT the general scaling blit docs/ui_scaling.md asks for as fix 3, and must not grow
// into one. Roughly forty callers compute their layout in assembly from unscaled
// gfxBitmap::Width - mmHUD advances a row of digits with `v4 += bmp->Width` - so a blit that drew
// 3x while the caller advanced 1x would make every multi-part HUD element overlap itself. Those
// callers all pass a rectangle that fits, take the BltFast path below, and see no change whatever.
// Bitmaps are resampled at LOAD instead, in gfxBitmap::Create(gfxImage*, bool), which is what
// makes Width and Height report the scaled size so the untouched assembly follows along.
//
// At 640x480 nothing reaches the second path, and this function is the original.

// ?CopyBitmap@gfxPipeline@@SAXHHPAVgfxBitmap@@HHHH_N@Z
void gfxPipeline::CopyBitmap(
    i32 arg1, i32 arg2, gfxBitmap* arg3, i32 arg4, i32 arg5, i32 arg6, i32 arg7, bool arg8)
{
#ifdef ARTS_AGIGL
    if (agiGLEnabled())
    {
        // The emptiness checks live here rather than in agiGLCopyBitmap because they are a
        // DIVERGENCE, and divergences belong on the branch that owns them. The original walks
        // straight into arg3->Surface and hands a degenerate rectangle to BltFast, which is
        // harmless when a DirectDraw surface is on the other end and is not harmless when the
        // other end is a texture bind and a quad. Guarding here leaves the DirectDraw path below
        // reading exactly as it always has.
        if (arg3 != nullptr && arg6 > 0 && arg7 > 0)
            agiGLCopyBitmap(arg1, arg2, arg3->Surface, arg4, arg5, arg6, arg7, arg8);

        return;
    }
#endif

    gfxBlitRect source_rect;

    source_rect.left = arg4;
    source_rect.top = arg5;
    source_rect.right = arg4 + arg6;
    source_rect.bottom = arg5 + arg7;

    IDirectDrawSurface7* dest = lpdsRend;
    IDirectDrawSurface7* source = arg3->Surface;

    i32 bitmap_width = arg3->Width;
    i32 bitmap_height = arg3->Height;

    // An empty or inverted rectangle counts as fitting, and reaches BltFast exactly as it does in
    // the original. Only a rectangle that genuinely runs off the source surface is redirected.
    if (source_rect.left >= 0 && source_rect.top >= 0 && source_rect.right <= bitmap_width &&
        source_rect.bottom <= bitmap_height)
    {
        u32 flags = MM_DDBLTFAST_WAIT | (arg8 ? MM_DDBLTFAST_SRCCOLORKEY : 0u);

        dest->lpVtbl->BltFast(dest, arg1, arg2, source, &source_rect, flags);

        return;
    }

    source_rect.left = 0;
    source_rect.top = 0;
    source_rect.right = bitmap_width;
    source_rect.bottom = bitmap_height;

    gfxBlitRect dest_rect;

    dest_rect.left = arg1;
    dest_rect.top = arg2;
    dest_rect.right = arg1 + arg6;
    dest_rect.bottom = arg2 + arg7;

    // No DDBLTFX and no DDBLTFX_ARITHSTRETCHY, so the stretch is point sampled. That is the right
    // choice rather than a lazy one: every bitmap surface is created with a source colour key of
    // zero (gfxBitmap::Create sets DDSD_CKSRCBLT with both bounds at 0), and a filtered stretch
    // would blend key pixels into their neighbours and leave a fringe the key no longer catches.
    u32 flags = MM_DDBLT_WAIT | (arg8 ? MM_DDBLT_KEYSRC : 0u);

    dest->lpVtbl->Blt(dest, &dest_rect, source, &source_rect, flags, nullptr);
}

// THE FRAME LOOP, AND THE LOADING SCREEN'S PROGRESS BAR.
//
// Same shape as the gfxBitmap members in gfx/gfxbitmap.cpp: a -gl branch, with the else arm
// calling the ORIGINAL machine code that tools/asm.py kept under an alias. See
// data/keep_original.json and gfx/origfallback.h.
//
// These three are what agigl/glgfxpipeline.cpp said could never carry a runtime branch, on the
// grounds that each dereferences lpD3DDev, lpdsRend or lpdsFront in its original body and so had
// no COM-free transcription to put in an else arm - meaning they would have had to be registered
// as a set, replacing the DirectDraw frame loop permanently for every run. Keeping the original
// under an alias removes the premise: the else arm is the DirectDraw frame loop, unchanged, and
// the set can go in one function at a time.

// ?BeginFrame@gfxPipeline@@SAXXZ
void gfxPipeline::BeginFrame()
{
#ifdef ARTS_AGIGL
    if (agiGLEnabled())
    {
        agiGLBeginFrame();

        return;
    }
#endif

    ArtsOrigBeginFrame();
}

// ?EndFrame@gfxPipeline@@SAXXZ
void gfxPipeline::EndFrame()
{
#ifdef ARTS_AGIGL
    if (agiGLEnabled())
    {
        agiGLEndFrame();

        return;
    }
#endif

    ArtsOrigEndFrame();
}

// A free function rather than a gfxPipeline member, which is why it is spelled without a class
// qualifier and still mangles to the name the loading screen calls.
//
// Declared as well as defined because tools/genheaders.py does not emit it - no generated header
// mentions ProgressRect at all - and tools/asm.py decides what to strip by looking for a
// mangled-name comment followed by a line carrying a marker. Without the declaration below the
// original PROC is never renamed, and the link fails with the C++ and the assembly both defining
// the symbol, which is exactly what happened the first time.

// ?ProgressRect@@YAXHHHHI@Z
ARTS_EXPORT void ProgressRect(i32 arg1, i32 arg2, i32 arg3, i32 arg4, u32 arg5);

void ProgressRect(i32 arg1, i32 arg2, i32 arg3, i32 arg4, u32 arg5)
{
#ifdef ARTS_AGIGL
    if (agiGLEnabled())
    {
        agiGLProgressRect(arg1, arg2, arg3, arg4, arg5);

        return;
    }
#endif

    ArtsOrigProgressRect(arg1, arg2, arg3, arg4, arg5);
}

// ?BeginGfx3D@gfxPipeline@@SA_NXZ
//
// PORTED FOR ONE REASON: ForceSetViewport IS PRIVATE.
//
// The GL device layer can build a framebuffer perfectly well on its own, and agiGLBeginGfx3D does.
// What it cannot do is finish the job, because the last third of the original is not device work
// at all - it creates the two viewports every camera and cull pass reads, and hands one to
// gfxPipeline::ForceSetViewport, a private static that no free function in agigl/ can reach. Doing
// it from outside by assigning through Access_m_Viewport() would set the pointer and skip
// everything else ForceSetViewport does, which is how asCamera::SetViewport came to dereference
// null at 0x004A2E30 with both viewports sitting there fully constructed.
//
// Inside a gfxPipeline member there is no access problem, so the tail is simply transcribed.
bool gfxPipeline::BeginGfx3D()
{
#ifdef ARTS_AGIGL
    if (agiGLEnabled())
    {
        if (!agiGLBeginGfx3D())
            return false;

        // BEFORE the viewport work below, because ForceSetViewport reaches
        // gfxRenderState::SetCamera, which ends in lpD3DDev->SetTransform. That was the third
        // null-device fault in a row down one call chain, and installing the stub here is what
        // stops the rest of the chain being discovered one crash at a time.
        agiGLInstallStubDevice();

        // And the DirectDraw object, which is what lets gfxTexture::Create and the original
        // texture loader allocate surfaces instead of failing. Without it gfxCreateFont returns
        // null, and mmTextNode::Init then calls through a font that does not exist.
        agiGLInstallStubDirectDraw();
        agiGLInstallStubPrimary(static_cast<i32>(m_fWidth), static_cast<i32>(m_fHeight));

        // THE RENDER-STATE BASELINE, ESTABLISHED THE WAY THE ORIGINAL DOES IT.
        //
        // The original runs gfxRenderState::Init(&RSTATE) and then Default(&RSTATE) just before
        // this point, and both were missing - so every default the game expects to be sitting in the
        // device was never sent to OURS. gfxRenderState::Default is the half that matters here:
        //
        //     SetLight(0, &gfxLight::Sun);   LightEnable(0, 1);
        //     State.m_Material = &gfxMaterial::FlatWhite;   (touched)
        //     Cullmode, DitherEnable, ZEnable ...           (touched)
        //
        // Without it no light is ever enabled. That is why the vehicle showcase rendered pitch
        // black: MenuManager POSITIONS gfxLight::Sun but only calls SetLight, relying on Default
        // having enabled light 0 already, and a lit draw with no light enabled can only be black.
        //
        // It has to run AFTER the stub device is installed. Default reaches the device directly for
        // the light calls, so running it while lpD3DDev is still the original null-ish pointer sends
        // that state nowhere - which is exactly what was happening, and why a LightEnable probe saw
        // zero calls in a whole session.
        //
        // THIS REPLACES THE 0xFF TRICK DESCRIBED BELOW, TRUTHFULLY. Filling LASTRSTATE with 0xFF did
        // fix the preview, by forcing one complete transmit, but it left DoFlush diffing against a
        // state the device never held. Default touches the fields it changes and lets DoFlush send
        // them normally, so LASTRSTATE stays an honest mirror.
        RSTATE.Default();

        // Confirming this ran at all: a LightEnable probe saw zero calls in a whole session before
        // this line existed, which is what said the baseline was never reaching our device.
        Displayf("GL: gfxRenderState::Default() applied against the stub device");

        // THE RENDER-STATE BASELINE IS NOT SEEDED HERE, AND MUST NOT BE FILLED WITH 0xFF.
        //
        // The original runs gfxRenderState::Init(&RSTATE), copies it into LASTRSTATE, then
        // Default(&RSTATE). Init is private with no generated accessor, so an attempt was made to
        // serve the same purpose by filling LASTRSTATE with 0xFF - on the reasoning that making
        // every field differ forces the first DoFlush to transmit the complete state.
        //
        // IT BROKE THE WORLD. LASTRSTATE is not a dirty-flag scratch: it is DoFlush's MIRROR of
        // what the device actually holds. Filling it with a value the device never had does not
        // force one honest full transmit - it leaves DoFlush computing every later diff against
        // fiction, so states are dropped or sent wrongly from then on. The visible result was the
        // city collapsing to a strip of road under a flat cream sky.
        //
        // It did fix the car preview, which is why it looked right at first. That fix was real but
        // the mechanism was not; whatever replaces it has to leave LASTRSTATE truthful.

        // WE ARE NOT A SOFTWARE RASTERISER, AND SAYING OTHERWISE SHRINKS THE WORLD ON EVERY LAUNCH.
        //
        // mmGame::FarClipCB is the only place in the game that halves the far plane:
        //
        //     *(float*)0x005D4420 = gfxPipeline::VP->field_130 * (useSoftware ? 0.5 : 1.0);
        //
        // and that result travels back through PUGraphics::SetFarClip into MMSTATE.FarClip, which is
        // SAVED to players/playerN.cfg. So while useSoftware is true the draw distance is halved,
        // written to disk, reloaded halved, and halved again - once per launch, forever.
        //
        // Measured, not deduced: the saved value at +0x53 of player0.cfg went 1000 -> 500 across a
        // single run, and had reached 75 earlier, which is 600 - the seed below - halved three
        // times. That is why the distance kept "regressing" after code reverts and even after
        // log-only builds: the value was never in the code, it was in the save.
        //
        // Nothing we pass sets this flag (agigl/glsetres.cpp only raises it for -sw, -ref, -blade
        // and -swage), so it is being set elsewhere or inferred from the caps our stub device does
        // not fill in. Either way the honest value for a hardware GL renderer is false.
        useSoftware = false;

        // Transcribed from 0x004AA091 onwards.
        //
        // gfxRenderState::Init and Default, which the original runs just before this, are missing:
        // Init is private with no generated accessor, and nothing drawn so far reads RSTATE. They
        // belong with the first thing that submits geometry.
        OrthoVP = CreateViewport();

        if (OrthoVP != nullptr)
        {
            // The original reads the rectangle back out of the viewport it just made - the
            // D3DVIEWPORT7 at offset 0x15C - and builds Ortho(x, x + w, y + h, y, -1, 1) from it.
            // CreateViewport fills that rectangle from the pipeline size, and this viewport is for
            // 2D, so the logical size is the right one. Top and bottom are the way round they look:
            // Y grows downwards here, which is what every 2D coordinate in the game assumes.
            OrthoVP->Ortho(0.0f, m_fWidth, m_fHeight, 0.0f, -1.0f, 1.0f);
        }

        VP = CreateViewport();

        // SEED THE 3D VIEWPORT WITH A VALID FRUSTUM, BECAUSE A ZERO FAR PLANE PROPAGATES.
        //
        // CreateViewport hands back a zeroed gfxViewport, so field_130 - the far plane - starts at
        // 0. The original gets away with that because MM2's own setup calls Perspective on VP with
        // the far value from .data before anything reads it back. We do not reliably: BeginGfx3D
        // runs more than once per session under -gl (measured three times, a fresh VP each time),
        // and a VP whose far is still 0 when the game caches it poisons everything downstream.
        //
        // The chain that does the poisoning, all of it 1999 code:
        //   mmGame        this+1204 = gfxPipeline::VP->field_130      (caches the far)
        //   PUGraphics::SetFarClip(v)   gfxFarClip = v, then Perspective(VP, fov, aspect, near, v)
        //   mmGame::FarClipCB           *(float*)0x005D4420 = VP->field_130 * (useSoftware?0.5:1)
        //   every gameplay camera       Perspective(m_Viewport, fov, 0, near, *(float*)0x005D4420)
        //
        // So one zero at the top becomes a far plane of zero on the main camera. Perspective then
        // builds m22 = far/(far-near) and m32 = -near*far/(far-near), both of which collapse to
        // about 1e-7 - measured in a race as m10=-0.0000005, m14=+0.00000025, near=0.5, far=2.5e-7.
        // With those entries near zero every vertex in the city projects to the SAME clip-space
        // depth, the depth test can no longer order anything, and geometry resolves purely by the
        // order it was submitted. That order changes as the camera turns, so the world flickers.
        // The minimap was never affected because mmHudMap passes its own literals
        // (Perspective(vp, 60.0, 1.25, 10.0, 1600.0)) instead of reading the global.
        //
        // 600 is not invented: it is the float the retail binary ships at 0x005D4420, read out of
        // midtown2.exe's .data, and it is the far plane the good projection had at setup
        // (m10=1.000834 m14=-0.5004 => near=0.5 far=600). Seeding it only matters until the game
        // sets its own; SetFarClip overwrites this the moment a real draw distance is chosen, so
        // this changes no behaviour except replacing a degenerate frustum with the stock one.
        if (VP != nullptr)
// 600, AND DO NOT "IMPROVE" THIS TO A LARGER NUMBER WITHOUT TESTING IT.
            //
            // It was raised to 1000 on the reasoning that PERSP traces show the game's own gameplay
            // cameras asking for near=1 far=1000, so seeding 600 looked like an arbitrary cap. The
            // draw distance COLLAPSED - the world clipped to a few car lengths. That is the opposite
            // of what a larger far plane should do, so the value is not a simple far plane: it is
            // cached by mmGame and fed through PUGraphics::SetFarClip and mmGame::FarClipCB before
            // any camera sees it, and something in that chain does not scale the way it reads.
            //
            // 600 is the value the game ships in .data at 0x005D4420 and the value the world was
            // last seen rendering correctly at.
            VP->Perspective(70.0f, 0.0f, 0.5f, 600.0f);

        // The comparison is the original's. It matters on a resolution change, when BeginGfx3D runs
        // again and the new viewport may be the one already installed.
        if (VP != m_Viewport)
            ForceSetViewport(VP);

        // TEMPORARY PROBE: which object is which, so the DoFlush trace can name them.
        Displayf("VPPTR ortho=%p vp3d=%p current=%p", OrthoVP, VP, m_Viewport);

        // ioInput::Begin RIDES ALONG INSIDE THE 3D BRING-UP, which is easy to miss and was: the
        // original ends with `push 1 / call ioInput::Begin` at 0x004AA062. Taking the GL arm used
        // to skip the whole function, so nothing started the input devices and diInit - the only
        // caller of DirectInputCreateA in the binary, reached lazily from ioKeyboard::Begin - never
        // ran. lpDI stayed null, mmInput::AttachToPipe copied that null into the global at
        // 0x006B1D18, and mmJoyMan::Init dereferenced it seconds after the loading screen.
        //
        // It has nothing to do with the renderer, so it runs unchanged. The argument is the
        // original's literal 1.
        ioInput::Begin(true);

        BeginInternal();

        return true;
    }
#endif

    return ArtsOrigBeginGfx3D();
}
