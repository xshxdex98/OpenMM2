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

#include "gfxinterface.h"

#include "agigl/glscreen.h"
#include "agigl/glswitch.h"
#include "data7/printer.h"
#include "gfx/origfallback.h"

define_dummy_symbol(gfx_gfxautodetect);

// ?gfxAutoDetect@@YA_NPA_N@Z at 0x004ABE00.
//
// WHAT THE ORIGINAL DOES, AND WHY -gl SHOULD NOT DO IT.
//
// gfxAutoDetect loads DDRAW.DLL and calls gfxPipeline::EnumDDAdapters with AutoDetectCallback, which
// for EVERY adapter in the machine creates a DirectDraw object, creates Direct3D on top of it and
// enumerates its devices to test capabilities (DeviceCallback checks D3DPTEXTURECAPS_PERSPECTIVE,
// D3DPTFILTERCAPS_LINEAR, D3DPBLENDCAPS_SRCALPHA, D3DPRASTERCAPS_FOGVERTEX, and consults
// badvideo.txt). It then picks the adapter with the most video memory and saves the result.
//
// On this machine that enumerates three adapters - the primary display driver, the Radeon on
// DISPLAY1, and a second Radeon entry on DISPLAY6 - and probing them is what makes the window sit
// black, flicker, jump to another monitor and jump back before the Angel Studios intro plays. The
// window is titled from the detection step, which is the "autodetect" the title bar shows.
//
// Under -gl every bit of that is answering a question that has already been answered. The renderer
// is OpenGL on whichever adapter the GL context lands on; no DirectDraw device will be created for
// drawing, and agigl/glcaps.cpp reports the capabilities from the live GL context rather than from
// a probe. So the -gl branch fills in the one interface the rest of the game reads and returns.
//
// With -gl absent this calls the original machine code, so a DirectDraw run detects exactly as it
// always has. That is the rule agigl/glswitch.h sets out.

bool gfxAutoDetect(bool* successOut)
{
    if (!agiGLEnabled())
        return ArtsOrigAutoDetect(successOut);

    // The original clears this before deciding and only sets it when it actually re-detected.
    // Nothing was re-detected here, so it stays clear - saying otherwise would invite the caller to
    // treat the saved settings as freshly changed.
    if (successOut != nullptr)
        *successOut = false;

    // The adapter entry is built by agiGLSynthesizeModes rather than here - it already fills
    // gfxInterfaces[0] with the name, caps and renderer, generates a resolution list from the
    // desktop at both the display's own aspect and 4:3, and sets gfxInterfaceCount and
    // gfxInterfaceChoice. Writing a second, worse version of that table here would race the good
    // one.
    //
    // But it MUST be called here, not just left to the GL bring-up that already calls it. Skipping
    // the probe without building the table left gfxInterfaceCount at zero and the resolution list
    // empty, and the first thing to read gfxInterfaces[0] took an access violation with 640x480 in
    // its registers. Autodetect's job IS to leave a usable adapter behind; only the DirectDraw
    // probing is what -gl has no use for.
    //
    // Everything this branch does NOT do is the point: no LoadLibrary("DDRAW.DLL"), no
    // EnumDDAdapters, no creating a DirectDraw and a Direct3D device on every adapter in the machine
    // to read its capabilities, and no gfxResetAdapter at the end.
    // THE ARRAY HAS TO BE PROVIDED, NOT JUST FILLED.
    //
    // gfxInterfaces is a pointer and nothing in the decompiled tree ever assigns it - the storage is
    // attached by AutoDetectCallback as it accepts the first adapter, which is exactly the path this
    // branch skips. So it is still null here, and agiGLSynthesizeModes writing gfxInterfaces[0]
    // faulted immediately after printing the desktop size.
    //
    // One entry is enough and is honest: there is exactly one renderer under -gl. gfxSaveSettings
    // writes 624 * gfxInterfaceCount bytes out of this pointer, and gfxInterfaceCount is set to 1 by
    // agiGLSynthesizeModes, so the write stays inside the array.
    static gfxInterface g_gl_interface[1];

    if (gfxInterfaces == nullptr)
        gfxInterfaces = g_gl_interface;

    agiGLSynthesizeModes();

    Displayf("gfxAutoDetect: -gl, skipped DirectDraw adapter probing");

    return true;
}
