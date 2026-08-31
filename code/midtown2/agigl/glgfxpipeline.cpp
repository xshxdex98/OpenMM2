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

define_dummy_symbol(agigl_glgfxpipeline);

// THE GL FRAME LOOP.
//
// Everything else under agigl/ is device code that answers only to itself. This file is the seam:
// it defines the gfxPipeline frame-loop statics the rest of the game calls, in terms of that
// device code.
//
// It is guarded twice over. ARTS_AGIGL gates the whole of agigl/; ARTS_AGIGL_PIPELINE additionally
// gates this file and glbitmap.cpp, the two that include gfx/gfxbitmap.h and gfx/gfximage.h.
//
// THE REASON FOR THE SECOND GUARD IS GONE. It was that those two generated headers did not
// compile, and both have since been fixed: gfxbitmap.h forward declares IDirectDrawSurface7, and
// gfximage.h declares gfxImage::gfxImageFormat as an opaque enum with a fixed underlying type,
// which is complete enough to pass by value. Defining ARTS_AGIGL_PIPELINE now compiles every file
// under agigl/ with zero errors.
//
// What the flag cannot do is switch this file in and gfx/gfxpipeline.cpp out, which is what it was
// built to do - see the note above CopyBitmap's absence below. The remaining statics here still
// need a runtime branch each before any of them can be registered, and once they all have one the
// flag has nothing left to gate and should be deleted rather than defined.
//
// NOTHING HERE IS REGISTERED IN data/ported.json, and registering it is a ONE-WAY DOOR - see the
// note below. The harness treats a registered function as ported and strips the original out of
// game.asm, so registering these early would replace a working DirectDraw path with untested C++
// and there would be no way back except a rebuild. Compiled-but-unregistered is the safe state:
// tools/build.py holds the object back and says so.
#if defined(ARTS_AGIGL) && defined(ARTS_AGIGL_PIPELINE)

#    include "data7/printer.h"
#    include "gfx/gfxpipeline.h"

// gfx/gfxbitmap.h names IDirectDrawSurface7 without declaring it. Declared here so the include
// parses; see docs/opengl_plan.md.
struct IDirectDrawSurface7;

#    include "gfx/gfxbitmap.h"

#    include "glcontext.h"
#    include "glpipe.h"
#    include "glscreen.h"
#    include "glsurface.h"

// ?gfxCreateFont@@YAXXZ
ARTS_IMPORT void gfxCreateFont();

// THE DEVICE BRING-UP HAS MOVED, AND ITS NAMES HAVE CHANGED.
//
// BeginGfx2D, BeginGfx3D, EndGfx2D and EndGfx3D used to be defined here under their real mangled
// names. They are now agiGLBeginGfx2D / agiGLBeginGfx3D / agiGLEndGfx2D / agiGLEndGfx3D in
// glgfx.cpp, because claiming the real names would have meant registering them - and registering
// them destroys the DirectDraw bring-up they are supposed to be an alternative to. The full
// argument is in glgfx.h and docs/opengl_activation.md.
//
// EVERYTHING THAT REMAINS IN THIS FILE IS A ONE-WAY DOOR. Each of these functions dereferences
// lpD3DDev, lpdsRend or lpdsFront in its original body, so there is no COM-free transcription to
// put in an else arm: registering any of them replaces the DirectDraw frame loop permanently, for
// every run, with or without -gl. They therefore carry NO runtime branch - a branch would be a lie,
// since only one arm could ever be written. They go in as a set or not at all.

// ?BeginFrame@gfxPipeline@@SAXXZ
void gfxPipeline::BeginFrame()
{
    // WHAT IS MISSING HERE, AND WHY IT DECIDES THE SIZE OF THE PORT.
    //
    // The original ends with `if (!m_RenderTarget) ForceSetViewport(m_Viewport);`, and
    // gfxPipeline::ForceSetViewport (0x004B2EE0) calls gfxViewport::DoFlush, whose first statement
    // is lpD3DDev->SetViewport. Under -gl lpD3DDev is null, so that call is an access violation and
    // the line cannot simply be transcribed.
    //
    // Leaving it out is correct for the loading screen - ProgressCB draws one bitmap and one
    // rectangle and reads no viewport - and WRONG for anything 3D, because m_Viewport is then never
    // assigned and the projection is never flushed. gfxViewport::DoFlush is therefore in the
    // minimum set for a game frame but not for the first visible frame, which is exactly the split
    // docs/opengl_activation.md uses to size the two registration waves.
    agiGL->MakeCurrent();

    gfxCreateFont();

    agiGLBindRenderTarget();

    agiGL->EnableDisable(GL_SCISSOR_TEST, false);
    agiGL->DepthMask(true);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// ?BeginScene@gfxPipeline@@SAXXZ
void gfxPipeline::BeginScene()
{
    // The original only did anything here when useSoftware was set - it called
    // lpD3DDev->BeginScene, because the software device needed the render surface locked for the
    // duration. GL has no such boundary.
}

// ?EndScene@gfxPipeline@@SAXXZ
void gfxPipeline::EndScene()
{
    // Same. The one piece of the original that is not software-only is unbinding texture stage 0
    // and flushing the render state, which belongs with gfxRenderState rather than here.
}

// ?EndFrame@gfxPipeline@@SAXXZ
void gfxPipeline::EndFrame()
{
    // A render target set means this frame was drawn into a texture - the rear-view mirror, or the
    // map. Nothing is presented.
    if (gfxPipeline::m_RenderTarget != nullptr)
        return;

    u32 fade = gfxPipeline::FadeColor;

    if ((fade & 0xFF000000) != 0)
    {
        agiGLBegin2D();
        agiGLDrawQuad2D(nullptr, 0.0f, 0.0f, gfxPipeline::m_fWidth, gfxPipeline::m_fHeight, 0.0f, 0.0f, 0.0f, 0.0f,
            fade, false);
        agiGLEnd2D();
    }

    // TODO: the original advances the fade here, LerpRGBA(FadeColor, EndFadeColor, FadeValue) with
    // FadeValue += delta * FadeSpeed clamped to 1. The frame-time global the decompiler calls
    // `delta` has no confirmed symbol, so the advance is left out rather than guessed at - the
    // visible effect is a fade that starts and never finishes.

    agiGLPresent();
}

// ?RefreshFrame@gfxPipeline@@SAXXZ
void gfxPipeline::RefreshFrame()
{
    if (gfxPipeline::m_RenderTarget != nullptr)
        return;

    agiGLPresent();
}

// ?FlushFrame@gfxPipeline@@SAXXZ
void gfxPipeline::FlushFrame()
{
    // Empty in the original too - one `ret` at 0x004AA5B0.
}

// ?Clear@gfxPipeline@@SAXHIMI@Z
void gfxPipeline::Clear(i32 arg1, u32 arg2, f32 arg3, u32 arg4)
{
    if (arg1 == 0)
        return;

    u32 alpha = arg2 & 0xFF000000;

    // The original splits here on exactly this test, and the split is not an optimisation: a
    // partially transparent clear colour cannot be expressed as a device Clear, so it draws a
    // full-screen blended quad instead and ignores the flags entirely. That is the fade path.
    if (alpha != 0 && alpha != 0xFF000000)
    {
        agiGLBegin2D();
        agiGLDrawQuad2D(nullptr, 0.0f, 0.0f, gfxPipeline::m_fWidth, gfxPipeline::m_fHeight, 0.0f, 0.0f, 0.0f, 0.0f,
            arg2, false);
        agiGLEnd2D();

        return;
    }

    GLbitfield mask = 0;

    // D3DCLEAR_TARGET is 1 and D3DCLEAR_ZBUFFER is 2; asCullManager::Update passes 3.
    if (arg1 & 1)
    {
        glClearColor(static_cast<f32>((arg2 >> 16) & 0xFF) / 255.0f, static_cast<f32>((arg2 >> 8) & 0xFF) / 255.0f,
            static_cast<f32>(arg2 & 0xFF) / 255.0f, 1.0f);

        mask |= GL_COLOR_BUFFER_BIT;
    }

    if (arg1 & 2)
    {
        glClearDepth(static_cast<GLclampd>(arg3));

        // glClear is masked by the depth write mask, and a Clear that arrives with depth writes
        // off silently does nothing. DirectDraw's Clear had no such interaction, so this is a
        // difference the original code cannot have accounted for.
        agiGL->DepthMask(true);

        mask |= GL_DEPTH_BUFFER_BIT;
    }

    // A CLEAR WITH NO RECT LIST CLEARS THE VIEWPORT, NOT THE WHOLE TARGET.
    //
    // D3D clears the CURRENT VIEWPORT rectangle; GL's glClear covers everything unless a scissor
    // stops it. mmMirror::Cull sets the mirror's small viewport, calls gfxPipeline::Clear(3, ...) -
    // colour AND depth - draws the world into it, then restores the viewport. Clearing the whole
    // target there wipes the depth buffer the main view depends on, which showed as the road
    // flickering and MORE of it appearing with the mirror switched off.
    //
    // This was tried once and reverted because it did not fix the black world; that was the wrong
    // reason to drop it. It is what D3D does, and with depth now mattering it is load-bearing.
    i32 rect[4];

    if (agiGLGetViewportRect(rect))
    {
        glScissor(rect[0], rect[1], rect[2], rect[3]);
        agiGL->EnableDisable(GL_SCISSOR_TEST, true);
    }

    // TEMPORARY PROBE: how many main-view draws have already happened this frame when the clear
    // runs. The kit's frame order is BeginFrame -> Clear -> ... -> world, so this should always be
    // zero for the full-screen clear. Anything else means the world is drawn and then wiped.
    {
        extern i32 agiGLProbeMainDraws;

        static i32 shown = 0;

        if (shown < 40)
        {
            ++shown;
            Displayf("CLEAR mask=%X maindraws_so_far=%d rect=%d,%d %dx%d", mask, agiGLProbeMainDraws,
                rect[0], rect[1], rect[2], rect[3]);
        }
    }

    glClear(mask);

    agiGL->EnableDisable(GL_SCISSOR_TEST, false);
}

// ?ClearRect@gfxPipeline@@SAXHHHHI@Z
void gfxPipeline::ClearRect(i32 arg1, i32 arg2, i32 arg3, i32 arg4, u32 arg5)
{
    // A CAVEAT THE CALLERS DO NOT KNOW ABOUT. The original passes arg5 straight into DDBLTFX
    // .dwFillColor, which is a value in the surface's own pixel format - 565 or 555, not ARGB. The
    // one caller in the game, mmExternalView, therefore hands over a pre-converted value, while
    // ProgressRect does the same conversion by hand because ClearRect will not do it for you.
    //
    // Here the fill is a quad and the colour is ARGB, so a caller that still pre-converts will
    // produce a wrong colour rather than a crash. Both call sites have to be corrected when this
    // is registered as ported; they are listed in docs/opengl_plan.md.
    agiGLBegin2D();
    agiGLDrawQuad2D(nullptr, static_cast<f32>(arg1), static_cast<f32>(arg2), static_cast<f32>(arg3),
        static_cast<f32>(arg4), 0.0f, 0.0f, 0.0f, 0.0f, arg5 | 0xFF000000, false);
    agiGLEnd2D();
}

// gfxPipeline::CopyBitmap is NOT here.
//
// It lives in gfx/gfxpipeline.cpp, which now carries both paths behind an agiGLEnabled() branch,
// and calls agiGLCopyBitmap in agigl/glblit.cpp for the GL half. It was moved because a
// compile-time swap between two definitions could not work: the flag that switched the GL one on
// switched the DirectDraw one off, while quarantine still held this object back, so the symbol
// ended up defined by nobody - and even had it linked, this body has no DirectDraw path in it, so
// a build without -gl would have stopped drawing the loading screen.
//
// The rest of this file is still waiting on the same treatment. Every one of the statics below
// needs the original transcribed into an else arm before it can be registered; see
// docs/opengl_activation.md, step 4.

// ?CopyClippedBitmap@gfxPipeline@@SA_NHHPAVgfxBitmap@@HHHHHHHH_N@Z
bool gfxPipeline::CopyClippedBitmap(i32 arg1, i32 arg2, gfxBitmap* arg3, i32 arg4, i32 arg5, i32 arg6, i32 arg7,
    i32 arg8, i32 arg9, i32 arg10, i32 arg11, bool arg12)
{
    // Integer scissoring, transcribed from 0x004B2D80. It is done on the CPU rather than with
    // GL_SCISSOR_TEST on purpose: the scissor is in render-target pixels and these coordinates are
    // logical, so the two would have to be converted, and the source rect has to be trimmed to
    // match anyway.
    if (arg2 < arg9)
    {
        i32 trim = arg9 - arg2;

        arg7 -= trim;
        arg5 += trim;
        arg2 = arg9;
    }
    else if (arg2 >= arg11)
    {
        return false;
    }

    if (arg1 < arg8)
    {
        i32 trim = arg8 - arg1;

        arg6 -= trim;
        arg4 += trim;
        arg1 = arg8;
    }
    else if (arg1 >= arg10)
    {
        return false;
    }

    if (arg6 <= 0 || arg7 <= 0)
        return false;

    if (arg1 + arg6 > arg10)
        arg6 = arg10 - arg1;

    if (arg2 + arg7 > arg11)
        arg7 = arg11 - arg2;

    gfxPipeline::CopyBitmap(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg12);

    return true;
}

// ?CopyClippedBitmap@gfxPipeline@@SA_NHHPAVgfxBitmap@@HHHH_N@Z
bool gfxPipeline::CopyClippedBitmap(
    i32 arg1, i32 arg2, gfxBitmap* arg3, i32 arg4, i32 arg5, i32 arg6, i32 arg7, bool arg8)
{
    return gfxPipeline::CopyClippedBitmap(
        arg1, arg2, arg3, arg4, arg5, arg6, arg7, 0, 0, gfxPipeline::Access_m_iWidth(), gfxPipeline::Access_m_iHeight(), arg8);
}

// ?ProgressRect@@YAXHHHHI@Z
//
// A free function, not a gfxPipeline static, and it is in the minimum set anyway: ProgressCB draws
// the loading bar with it, and its original body is lpdsRend->GetPixelFormat followed by
// lpdsRend->Blt with DDBLT_COLORFILL. Under -gl lpdsRend is null, so the loading screen crashes
// here if this is left out even though every gfxPipeline function around it has been ported.
//
// The whole of the original is now redundant. It reads the render surface's pixel format and hand-
// converts the incoming ARGB down to 555 or 565 - `(w & 0x1F) | (8 * (g & 0xFC | 32 * (r & 0xF8)))`
// - purely because ClearRect takes a value in the surface's own format rather than a colour. The GL
// ClearRect takes ARGB, so the conversion goes and the two functions finally agree on what a colour
// is. That is the same convention change docs/opengl_activation.md flags for mmExternalView.c:160,
// and it is the one failure in the set that is a wrong colour rather than a crash.
void ProgressRect(i32 x, i32 y, i32 width, i32 height, u32 color)
{
    gfxPipeline::ClearRect(x, y, width, height, color);
}

#endif
