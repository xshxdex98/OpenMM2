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

#include "core/arts.h"

// THE GL DEVICE BRING-UP, UNDER NAMES THAT DO NOT EXIST IN midtown2.exe.
//
// This is the single most important structural decision in the activation design, and it is worth
// stating plainly because the obvious alternative is a trap.
//
// The obvious alternative is to reimplement gfxPipeline::BeginGfx2D and gfxPipeline::BeginGfx3D as
// C++ that branches on -gl. That cannot work. Registering ?BeginGfx2D@gfxPipeline@@SA_NXZ tells
// tools/asm.py to overwrite its 0x228 bytes of DirectDraw bring-up with 0xCC padding. The else arm
// of the branch would then have to be a hand-written C++ transcription of LoadLibraryA("DDRAW.DLL")
// -> DirectDrawCreateEx -> GetCaps -> SetCooperativeLevel -> SetDisplayMode, against COM interfaces
// this build cannot see a header for (docs/opengl_plan.md, item 4: the Windows SDK's um/ and
// shared/ directories are not on the include path). Every vtable index in that transcription would
// be written from memory, and docs/harness.md's central lesson is that a wrong-but-plausible value
// is exactly what no verifier here can catch.
//
// So BeginGfx2D and BeginGfx3D are NEVER registered. The GL device bring-up gets these names
// instead, and the runtime branch lives one level up, in functions whose original bodies contain no
// COM at all and can therefore be transcribed exactly:
//
//     InitDirectDraw           0x00401690  ->  BeginGfx2D, gfxSafeMode, MessageBoxA, exit
//     gfxPipeline::BeginGfx    0x004A9350  ->  BeginGfx2D() && BeginGfx3D()
//     gfxPipeline::EndGfx      0x004AA750  ->  EndGfx3D(); EndGfx2D();
//
//     BeginPhase               0x00401AA0  ->  BeginGfx3D, and nothing else in the device layer
//
// Those FOUR are the activation surface for device bring-up. The fourth was missed on the first
// pass: BeginPhase calls gfxPipeline::BeginGfx3D() directly, which is the null lpDD dereference a
// -gl run reports at BeginGfx3D+0x90. It is transcribed in agigl/glbeginphase.cpp and is not yet
// enabled. With -gl off, the original DirectDraw bring-up runs from its own untouched machine
// code. See docs/opengl_activation.md.

#ifdef ARTS_AGIGL

// Resolves opengl32.dll and everything that does not need a window. Must run before
// gfxPipeline::gfxWindowCreate, because a failure here is the one failure the caller can still
// recover from by falling back to DirectDraw - after the window exists it is too late, since a
// pixel format can be set on a window exactly once.
//
// Idempotent.
bool agiGLBeginDevice();

// Replaces gfxPipeline::BeginGfx2D. Expects hwndMain to already exist. Creates the WGL context,
// runs agiGLInitScaling, and writes gfxPipeline::m_iWidth/m_iHeight/m_fWidth/m_fHeight and
// m_ColorDepth from the LOGICAL resolution - which is the whole point of the design: the window
// size and the render-target size are both free, and neither fact reaches game code.
//
// Idempotent. The second call reruns agiGLInitScaling, so it is also how a resolution change is
// applied.
bool agiGLBeginGfx2D();

// Replaces gfxPipeline::BeginGfx3D. Creates the render framebuffer. Idempotent, and recreates the
// framebuffer if the logical or render size has changed since the last call.
bool agiGLBeginGfx3D();

// Puts GL_LIGHT_MODEL_AMBIENT on D3D's default (black) instead of GL's 0.2 grey. MM2 never sets
// D3DRENDERSTATE_AMBIENT - measured, zero calls in a full session - so retail runs with no global
// ambient and we must start there too.
void agiGLResetAmbientToD3DDefault();

// Points lpD3DDev at a device whose methods do nothing, so the sixty-nine functions that
// dereference it can run their original code instead of having to be ported one at a time. See
// agigl/glstubdevice.cpp - it is bring-up scaffolding and the place the 3D renderer will grow.
void agiGLInstallStubDevice();

// Replaces gfxPipeline::EndGfx3D / EndGfx2D.
void agiGLEndGfx3D();
void agiGLEndGfx2D();

#endif
