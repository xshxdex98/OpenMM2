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

define_dummy_symbol(agigl_glgfx);

// THE DEVICE BRING-UP. Moved out of glgfxpipeline.cpp and renamed, so that no DirectDraw entry
// point has to be registered to reach a GL context. The argument for the renaming is in glgfx.h;
// the registration consequences are in docs/opengl_activation.md.
//
// This file is guarded by ARTS_AGIGL alone, NOT by ARTS_AGIGL_PIPELINE. That is deliberate: it is
// the half of the port that can be brought up and tested before the frame loop is touched, and it
// has no dependency on gfx/gfxbitmap.h or gfx/gfximage.h, the two generated headers that still do
// not compile. It does include gfx/gfxpipeline.h - which does parse today, its EnumDDAdapters
// syntax error and missing forward declarations having been fixed since docs/opengl_plan.md was
// written.
//
// Nothing here defines a symbol that exists in midtown2.exe, so this object is never quarantined
// and never strips anything out of game.asm.
#ifdef ARTS_AGIGL

#    include "data7/datargparser.h"
#    include "data7/printer.h"
#    include "gfx/gfxpipeline.h"
#    include "gfx/gfxrenderstate.h"
#    include "gfx/ddrawshim.h"
#    include "gfx/gfxviewport.h"

#    include "glcontext.h"
#    include "glgfx.h"
#    include "glpipe.h"
#    include "glscreen.h"

// ?hwndMain@@3PAUHWND__@@A - PAU, so HWND__ must be a struct. See the note in glcommon.h.
ARTS_IMPORT extern HWND__* hwndMain;

// TWO GLOBALS gfxPipeline::BeginGfx3D WRITES THAT agiGLBeginGfx3D HAS TO WRITE TOO.
//
// A scan of every absolute dword in every section of midtown2.exe finds these two written in
// exactly one function each, and it is BeginGfx3D both times - so with -gl on, nothing else ever
// sets them and they keep whatever .data? left them, which is zero. Zero happens to be right for
// both, but relying on that is relying on a coincidence, and the reason each one matters is not
// obvious.
//
// useHWTnL (0x00684501) is deliberately NOT written. The same scan shows it read only by
// gfxPacket::AllocateVertexBuffer, gfxPacket::Persist, gfxVertexBuffer::RestoreAll and two
// gfxPipeline vertex-buffer functions - all of them unreachable once useNativeVBs is clear.

// ?sm_UseInternal@@3_NA
//
// Selects MM2's own software submission path. Read by vglBegin, vglEnd, gfxPipeline::DrawVB and
// DrawIdxVB, gfxViewport::SetWindow and Perspective, gfxRenderState::DoFlush and BOTH
// gfxPacket::Draw overloads. Every one of those paths ends in a call the GL layer cannot make.
ARTS_IMPORT extern bool sm_UseInternal;

// ?useNativeVBs@@3_NA
//
// Read by gfxPipeline::CreateVertexBuffer and, far more importantly, by the gfxPacket CONSTRUCTOR
// at 0x004B38E5. When it is set and the packet has no matrix indices, the constructor calls
// gfxPacket::AllocateVertexBuffer, whose body is lpD3D->CreateVertexBuffer - a null dereference
// under -gl, at model load time, long before any frame. With it clear the constructor writes
// StartVertex = 0xFFFF instead, which is what puts every packet on the system-memory path the GL
// draw expects.
ARTS_IMPORT extern bool useNativeVBs;

// Debug level for the GL context: -1 disables error checking entirely, 0 checks lazily, 1 or more
// checks every frame. -gldebug <n> selects it.
//
// datArgParser::Get(name, index, value) takes the WORD INDEX after the flag, and that is no longer
// an assumption - docs/opengl_plan.md listed it as open question 4 and the pseudocode settles it.
// ?Get@datArgParser@@SA_NPBDIAAH@Z at 0x004C61C0 is
//
//     entry = HashTable::Access(&ArgHash, name);
//     if (!entry || index >= entry->WordCount) return false;
//     *out = atoi(entry->Words[index]);
//
// so index subscripts the values collected after the flag. gfxPipeline::SetRes passes 0 for
// -width/-height/-cdepth/-zdepth in its own machine code (xor edi,edi; push eax; push edi; push
// "width"), which is the same convention.
static constexpr i32 kDefaultDebugLevel = 0;

// The window title gfxWindowCreate is given on the GL path. The original passes "Midtown Madness"
// through gfxPipeline::SetTitle; this string is only the class-registration name and is never
// shown, because SetTitle overwrites the caption later.
static const char kGLWindowName[] = "gfxWindow";

static agiGLContext* agiGLOwnedContext;

static bool agiGLDeviceReady;
static bool agiGLGfx2DReady;
static bool agiGLGfx3DReady;

// What the framebuffer was last built for. A second BeginGfx3D with a different render size has to
// rebuild it; a second one with the same size must not, because rebuilding drops the contents and
// the caller has no idea it happened.
static i32 agiGLBuiltRenderWidth;
static i32 agiGLBuiltRenderHeight;

bool agiGLBeginDevice()
{
    if (agiGLDeviceReady)
        return true;

    // The one failure the caller has to survive. Everything after this point happens on a window
    // whose pixel format has been set, and a pixel format can be set on a window exactly once - so
    // this is the last moment at which falling back to DirectDraw is still free.
    if (!agiGLLoadLibrary())
    {
        Errorf("gfxPipeline: -gl was given but there is no usable OpenGL - falling back");

        return false;
    }

    // SYNTHESISING THE MODE LIST IS OPT-IN, AND THAT IS NOT TIMIDITY.
    //
    // agiGLSynthesizeModes overwrites gfxInterfaces[0] and gfxInterfaceCount. The natural place to
    // call it is gfxAutoDetect (0x004ABE00), which is where the DirectDraw enumeration happens -
    // but gfxAutoDetect's only caller is Main(), which is a JUMPOUT stub and cannot be
    // reimplemented, so there is no function above it whose original body can be transcribed
    // without COM. Registering gfxAutoDetect itself is therefore a one-way door of exactly the kind
    // this design exists to avoid.
    //
    // Calling it from here instead would run it AFTER gfxLoadSettings and gfxPipeline::SetRes have
    // already read the table, so the list the resolution menu shows and the resolution the game is
    // actually running at would come from different sources.
    //
    // Meanwhile, nothing about GL requires a synthesised list: whatever resolution DirectDraw
    // enumerated is a resolution a framebuffer object can be built at. So the first milestone is
    // "GL renders at a DirectDraw-enumerated resolution", and raising the resolution ceiling is a
    // separate, later step. -glmodes forces the synthesis for anyone testing that step.
    if (datArgParser::Get("glmodes"))
    {
        Displayf("GL: -glmodes given, replacing the enumerated mode list");

        agiGLSynthesizeModes();
    }

    agiGLDeviceReady = true;

    return true;
}

bool agiGLBeginGfx2D()
{
    if (hwndMain == nullptr)
    {
        Errorf("agiGLBeginGfx2D - no window; gfxWindowCreate must run first");

        return false;
    }

    // Rerunning agiGLInitScaling is how a resolution change lands: it re-reads -width/-height and
    // the mode table, re-measures the client rect, and recomputes the blit and render rectangles.
    // It touches no GL state, so it is safe to call with a live context.
    agiGLInitScaling(hwndMain);

    if (!agiGLGfx2DReady)
    {
        agiGLOwnedContext = new agiGLContext(hwndMain, kDefaultDebugLevel);

        if (agiGLOwnedContext == nullptr || !agiGLOwnedContext->IsValid())
        {
            Errorf("agiGLBeginGfx2D - could not create an OpenGL context");

            delete agiGLOwnedContext;
            agiGLOwnedContext = nullptr;

            return false;
        }

        agiGLGfx2DReady = true;

        agiGL->SetSwapInterval(1);
    }

    // These four are what the whole game believes the screen to be, and they are set from the
    // LOGICAL resolution rather than from the window. That is the entire point of the design: the
    // window can be any size and the render target can be any size, and neither fact reaches game
    // code.
    //
    // They are written through the generated Access_ accessors rather than directly, because
    // m_iWidth and friends are PRIVATE statics (?m_iWidth@gfxPipeline@@0HA - the 0 is private) and
    // access is part of the mangled name, so they cannot be redeclared public without becoming a
    // different symbol that does not exist. See tools/genheaders.py.
    gfxPipeline::Access_m_iWidth() = agiScreen.width;
    gfxPipeline::Access_m_iHeight() = agiScreen.height;
    gfxPipeline::Access_m_fWidth() = static_cast<f32>(agiScreen.width);
    gfxPipeline::Access_m_fHeight() = static_cast<f32>(agiScreen.height);

    // 32 whatever the mode list says. The framebuffer is RGBA8 and there is no longer a display
    // mode to disagree with. Anything still branching on 16 - and gfxTexture::Create used to,
    // through g_Tex565 and g_Tex555 - is picking a DirectDraw texture format that no longer exists.
    gfxPipeline::Access_m_ColorDepth() = 32;

    return true;
}

bool agiGLBeginGfx3D()
{
    // NOT calling agiGLResetAmbientToD3DDefault() here, though D3D's default ambient really is
    // black and GL's really is 0.2 grey. Starting at black was tried and made buildings visibly too
    // dark, which means our lighting is short of a term retail has and the 0.2 was masking it.
    // Matching the API default is correct in principle and wrong on screen until that is found, and
    // what is on screen wins. The function stays for when the real deficit is fixed.

    if (!agiGLGfx2DReady)
    {
        Errorf("agiGLBeginGfx3D - no context; agiGLBeginGfx2D must run first");

        return false;
    }

    // Asserted on every call, not just the first, because the only other writer in the image is the
    // function this one replaces. See the note above the declarations.
    sm_UseInternal = false;
    useNativeVBs = false;

    // THE CAPABILITY-DERIVED STATICS, WHICH ONLY gfxPipeline::BeginGfx3D EVER SETS.
    //
    // The original queries the device with GetCaps and derives four values from D3DDEVICEDESC7.
    // This function stands in for it under -gl, so with the query gone those four keep whatever
    // .data left them, which is zero - and unlike sm_UseInternal and useNativeVBs above, zero is
    // WRONG for every one of them:
    //
    //   sm_SupportsBlendWithOne false makes gfxRenderState::SetBlendSet collapse EVERY blend set
    //     to set 0, SRCALPHA/INVSRCALPHA. The car paint reflection asks for set 7 - ONE/ONE, plain
    //     additive - and the environment map is opaque at every texel, so instead of adding a
    //     highlight it painted the sphere map straight over the bodywork. That is the orange and
    //     purple banding in the vehicle showcase.
    //   sm_MaxBlendMatrices is wMaxVertexBlendMatrices - 1 in the original, so a zeroed structure
    //     would have produced -1.
    //   sm_MaxActiveLights of 0 describes a device that cannot light anything.
    //   sm_MaxTextures of 0 is not a legal answer; the original never writes less than 1.
    //
    // The values are what THIS renderer can do rather than what a 1999 card could. One texture,
    // because agiGLStubSetTexture binds stage 0 and ignores the rest - MM2 responds by drawing the
    // environment map as a second additive pass, which agigl can draw correctly. Eight lights is
    // the GL fixed-function guarantee, and is also what the original substitutes when a driver
    // answers -1. No extra blend matrices, because agigl has no vertex skinning.
    gfxRenderState::Access_sm_SupportsBlendWithOne() = true;
    gfxRenderState::Access_sm_MaxTextures() = 1;
    gfxRenderState::Access_sm_MaxBlendMatrices() = 0;
    gfxRenderState::Access_sm_MaxActiveLights() = 8;

    if (agiGLGfx3DReady)
    {
        if (agiGLBuiltRenderWidth == agiScreen.render_width && agiGLBuiltRenderHeight == agiScreen.render_height)
            return true;

        Displayf("GL: render size changed from %dx%d to %dx%d, rebuilding the framebuffer",
            agiGLBuiltRenderWidth, agiGLBuiltRenderHeight, agiScreen.render_width, agiScreen.render_height);

        agiGLDestroyFramebuffer();
        agiGLGfx3DReady = false;
    }

    // The original builds the front, back, render and Z surfaces, a clipper when windowed, then
    // queries IDirect3D7, enumerates Z formats, creates the device and reads its caps. All of it
    // collapses to one framebuffer object, because every capability it was negotiating is
    // guaranteed here.
    if (!agiGLCreateFramebuffer())
    {
        Errorf("agiGLBeginGfx3D - could not create the render framebuffer");

        return false;
    }

    glClearDepth(1.0);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DITHER);

    agiGL->CheckErrors();

    agiGLBuiltRenderWidth = agiScreen.render_width;
    agiGLBuiltRenderHeight = agiScreen.render_height;
    agiGLGfx3DReady = true;

    // The tail of gfxPipeline::BeginGfx3D - the viewports, the render state and BeginInternal -
    // is NOT done here. It lives in gfxPipeline::BeginGfx3D itself, in gfx/gfxpipeline.cpp, which
    // is the only place gfxPipeline::ForceSetViewport can be called from: it is a private static,
    // and setting gfxPipeline::m_Viewport without it leaves asCamera::SetViewport dereferencing
    // null at 0x004A2E30.
    return true;
}

void agiGLEndGfx3D()
{
    if (!agiGLGfx3DReady)
        return;

    agiGLDestroyFramebuffer();

    agiGLGfx3DReady = false;
    agiGLBuiltRenderWidth = 0;
    agiGLBuiltRenderHeight = 0;
}

void agiGLEndGfx2D()
{
    delete agiGLOwnedContext;
    agiGLOwnedContext = nullptr;

    agiGLGfx2DReady = false;

    // THE WINDOW IS DELIBERATELY NOT DESTROYED, and this differs from the original on purpose.
    //
    // gfxPipeline::EndGfx2D calls DestroyWindow, and it has to: a DirectDraw cooperative level and
    // a display mode are bound to the window, and gfxPipeline::SafeBeginGfx retries BeginGfx up to
    // four times. Under GL the constraint runs the other way - SetPixelFormat succeeds on a window
    // exactly once, so destroying and remaking the window is the one thing that would make a retry
    // fail. Keeping it means a second agiGLBeginGfx2D reuses the format it already set.
    //
    // The cost is that the window outlives the device. Nothing in MM2 depends on it not doing so:
    // hwndMain is also the input window, and ioInput keeps using it either way.
}

#endif
