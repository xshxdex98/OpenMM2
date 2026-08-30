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

#include "data7/printer.h"
#include "gfx/gfxpipeline.h"

#include "glswitch.h"
#include "glwin32.h"

#ifdef ARTS_AGIGL
#    include "glgfx.h"
#endif

define_dummy_symbol(agigl_glactivate);

// THE ACTIVATION SURFACE.
//
// This file holds three of the four functions that carry the -gl runtime branch. The fourth is
// BeginPhase, in agigl/glbeginphase.cpp, and it is written but not enabled - see the caller list
// below. The set is small on purpose and the selection rule is exact:
//
//     A function may carry the branch only if its ORIGINAL body can be transcribed here with no
//     DirectDraw or Direct3D COM call in it.
//
// The rule follows from how the harness works. Registering a symbol in data/ported.json makes
// tools/asm.py overwrite its original machine code with 0xCC padding, permanently and for every
// run. So the else arm of the branch is not "the original code" - it is a C++ retyping of the
// original code, and it has to be right, because the original is gone. Retyping a COM call means
// writing a vtable index from memory into a build that cannot see ddraw.h, and docs/harness.md
// spends a whole section on why nothing here can catch a wrong-but-plausible value.
//
// Applying the rule to the bring-up path leaves exactly these:
//
//   ?InitDirectDraw@@YAXXZ          0x00401690  0x3D bytes   BeginGfx2D, gfxSafeMode, MessageBoxA,
//                                                            AngelReadString, APPTITLE, exit
//   ?BeginGfx@gfxPipeline@@SA_NXZ   0x004A9350  0x1B bytes   BeginGfx2D() && BeginGfx3D()
//   ?EndGfx@gfxPipeline@@SAXXZ      0x004AA750  0x0A bytes   EndGfx3D(); EndGfx2D();
//
// THIS FILE USED TO CLAIM THAT THOSE THREE WERE THE ONLY CALLERS OF BeginGfx2D, BeginGfx3D,
// EndGfx2D AND EndGfx3D. THEY ARE NOT. That claim came from the pseudocode kit. A scan done against
// the instruction bytes - over the symbol references in game.asm.pristine, and again by
// disassembling midtown2.exe with capstone and matching every rel32 and every absolute dword
// against the four addresses - gives the real set:
//
//   BeginGfx2D  0x004A9370  <- InitDirectDraw (x2), gfxPipeline::BeginGfx
//   BeginGfx3D  0x004A96C0  <- BeginPhase, gfxPipeline::BeginGfx
//   EndGfx2D    0x004AAA10  <- Main (x2), EndPhase, gfxPipeline::EndGfx
//   EndGfx3D    0x004AA760  <- EndPhase, gfxPipeline::EndGfx
//   BeginGfx    0x004A9350  <- gfxPipeline::SafeBeginGfx (x4), rglOpenPipe
//   EndGfx      0x004AA750  <- BeginGfx2D (x4), BeginGfx3D (x20), rglClosePipe
//
// Only BeginPhase's direct call to BeginGfx3D is fatal under -gl, and closing it is what
// agigl/glbeginphase.cpp is for. Main's and EndPhase's teardown calls are safe: every COM pointer
// in EndGfx2D and EndGfx3D is null-tested before use, so under -gl they run to completion and do
// nothing but ShowCursor, DestroyWindow and the texture/bitmap list teardown.
//
// With BeginPhase enabled and -gl on, no DirectDraw bring-up code is reachable; with -gl off, all
// of it runs from its own untouched bytes. The frame loop is a different problem entirely - see
// docs/opengl_activation.md.

// ?gfxSafeMode@@YAXXZ - the function docs/opengl_plan.md calls gfxFindSafeAdapter. The kit's name
// is wrong; the linker map says gfxSafeMode, and this is the name the call site in InitDirectDraw
// resolves to.
ARTS_IMPORT void gfxSafeMode();

// ?AngelReadString@@YAPAULocString@@I@Z
//
// The return type is part of the mangled name for a free function - PAULocString@@ is "pointer to
// struct LocString" - so this has to be declared with LocString or it becomes a different symbol
// that does not exist. LocString is only forward-declared, because no field of it is needed: the
// original pushes the returned pointer straight into MessageBoxA's lpText with no offset added, so
// the text buffer is at offset 0 and a reinterpret_cast is the faithful transcription.
struct LocString;

ARTS_IMPORT LocString* AngelReadString(u32 id);

// ?APPTITLE@@3PADA
ARTS_IMPORT extern char* APPTITLE;

// ?hwndMain@@3PAUHWND__@@A - PAU, so HWND__ is a struct. gfx/gfxpipeline.h forward-declares it that
// way; declaring it `class` would mangle to PAV and silently fail to resolve.
ARTS_IMPORT extern HWND__* hwndMain;

// The game's own CRT exit, at 0x00581944, which game.asm emits as `_exit PROC`. extern "C" plus
// __cdecl decorates to exactly `_exit`, so this resolves to the same function the original call
// site targets. It is NOT <cstdlib>'s exit: including <cstdlib> here would bind to the modern CRT,
// which this link does not have (-NODEFAULTLIB against import libraries built from midtown2.exe's
// own import table).
extern "C" void __cdecl exit(int code);

extern "C" __declspec(dllimport) i32 __stdcall MessageBoxA(
    HWND__* window, const char* text, const char* caption, u32 type);

// MB_ICONHAND, the 0x10 the original pushes.
static constexpr u32 kMessageBoxIconHand = 0x10;

// String resource 0xF6 - "cannot initialise the display" in the localisation table.
static constexpr u32 kDisplayFailedString = 0xF6;

// ?InitDirectDraw@@YAXXZ
void InitDirectDraw()
{
#ifdef ARTS_AGIGL
    if (agiGLEnabled())
    {
        // The GL path brings up 2D and 3D together, because under GL they are the same object: a
        // WGL context and one framebuffer. Doing both here is safe because gfxPipeline::BeginGfx
        // below is idempotent - rglOpenPipe's later call finds the device already up and only
        // reapplies the resolution.
        //
        // Note what is NOT called: gfxSafeMode. It forces adapter 0 to the Software renderer and
        // hunts the mode list for 640x480x16, which is a DirectDraw recovery and means nothing to
        // a framebuffer object. The GL retry ladder is "try again at 640x480", nothing more.
        if (gfxPipeline::BeginGfx())
            return;

        Errorf("InitDirectDraw - the OpenGL device failed to come up; falling back to DirectDraw");

        // A CAVEAT ON THIS FALLBACK. It is clean when agiGLBeginDevice failed, because nothing has
        // been created yet. It is NOT clean when the failure came later, after gfxWindowCreate: the
        // window then exists with a GL pixel format already set on it, and the DirectDraw
        // BeginGfx2D below will call gfxWindowCreate a second time. A failure of a failure is worth
        // one wasted window and is still better than exiting, but it is not free and should be
        // tightened once there is a way to test it.

        // Falling back rather than quitting. The DirectDraw path is known to work (docs/STATUS.md)
        // and this is the whole reason the switch is a runtime one: a GL failure costs the user a
        // renderer, not a session. Everything downstream must now see the flag as off, including
        // code that has already asked once, which is what agiGLSetEnabled is for.
        agiGLSetEnabled(false);
    }
#endif

    // Transcribed from 0x00401690, 0x3D bytes, exactly. Nothing below this line may change
    // behaviour: with -gl absent this is the whole of what the process does here today.
    if (!gfxPipeline::BeginGfx2D())
    {
        gfxSafeMode();

        if (!gfxPipeline::BeginGfx2D())
        {
            MessageBoxA(nullptr, reinterpret_cast<const char*>(AngelReadString(kDisplayFailedString)), APPTITLE,
                kMessageBoxIconHand);

            exit(0);
        }
    }
}

// ?BeginGfx@gfxPipeline@@SA_NXZ
bool gfxPipeline::BeginGfx()
{
#ifdef ARTS_AGIGL
    if (agiGLEnabled())
    {
        // Order matters and is not interchangeable. opengl32.dll has to resolve BEFORE the window
        // exists, because a pixel format can be set on a window exactly once - so the moment the
        // window is created is the last moment at which falling back to DirectDraw is free.
        if (!agiGLBeginDevice())
        {
            agiGLSetEnabled(false);

            return gfxPipeline::BeginGfx2D() && gfxPipeline::BeginGfx3D();
        }

        // gfxWindowCreate is a PRIVATE static of gfxPipeline, and this is the only place in the GL
        // path that can call it - which is a large part of why the branch lives in BeginGfx rather
        // than somewhere more convenient. Access is part of a mangled name, so it cannot be
        // reached from a free function without inventing a symbol that does not exist.
        //
        // The window is kept, not remade: it is also the input window (gfxWindowProc forwards to
        // InputWindowProc, the entry to the whole DirectInput path), and agiGLEndGfx2D
        // deliberately does not destroy it.
        if (hwndMain == nullptr)
            gfxPipeline::gfxWindowCreate("gfxWindow");

        if (hwndMain == nullptr)
        {
            Errorf("gfxPipeline::BeginGfx - gfxWindowCreate produced no window");

            return false;
        }

        // gfxPipeline::BeginGfx3D rather than agiGLBeginGfx3D: the pipeline function branches on -gl
        // itself now and adds the viewport setup the device layer cannot do.
        return agiGLBeginGfx2D() && gfxPipeline::BeginGfx3D();
    }
#endif

    // Transcribed from 0x004A9350, 0x1B bytes: `return BeginGfx2D() && BeginGfx3D();`. The
    // short-circuit is the original's - BeginGfx3D is not called when BeginGfx2D fails, and
    // BeginGfx2D has already called EndGfx on its own failure paths.
    return gfxPipeline::BeginGfx2D() && gfxPipeline::BeginGfx3D();
}

// ?EndGfx@gfxPipeline@@SAXXZ
void gfxPipeline::EndGfx()
{
#ifdef ARTS_AGIGL
    if (agiGLEnabled())
    {
        agiGLEndGfx3D();
        agiGLEndGfx2D();

        return;
    }
#endif

    // Transcribed from 0x004AA750, 0x0A bytes. 3D first, then 2D - the order is the original's and
    // it matters on both paths, since the device is torn down before the thing it was created on.
    gfxPipeline::EndGfx3D();
    gfxPipeline::EndGfx2D();
}
