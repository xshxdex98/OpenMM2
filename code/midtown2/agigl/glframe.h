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

#pragma once

#include "core/arts.h"

// The GL halves of the frame loop, as free functions.
//
// These bodies used to live in agigl/glgfxpipeline.cpp as gfxPipeline members, and that file
// argued they could never carry a runtime branch: each one dereferences lpD3DDev, lpdsRend or
// lpdsFront in its original body, so there was no COM-free transcription to put in an else arm,
// and registering any of them would have replaced the DirectDraw frame loop permanently for every
// run, with or without -gl. They had to go in as a set or not at all.
//
// THAT ARGUMENT NO LONGER HOLDS, and the reason is worth stating because it changes how the rest
// of the activation gets done. tools/asm.py can now keep a registered function's ORIGINAL machine
// code under an alias rather than padding it away - see data/keep_original.json and
// gfx/origfallback.h - so the else arm is a call into the 1999 code itself. There is nothing left
// to transcribe and nothing to get subtly wrong, so the functions can go in one at a time, each
// with a fallback that is exact by construction rather than by careful work.
//
// They live in their own translation unit because quarantine is per-object: tools/build.py holds
// back any object containing a game symbol that is not yet registered, and glgfxpipeline.cpp
// carries a pile of them. A file whose symbols appear in no MM2 binary is never held back.

#ifdef ARTS_AGIGL

// ?BeginFrame@gfxPipeline@@SAXXZ - make the context current, bind the render target, clear.
void agiGLBeginFrame();

// ?EndFrame@gfxPipeline@@SAXXZ - draw the fade overlay if there is one, then present.
void agiGLEndFrame();

// ?RefreshFrame@gfxPipeline@@SAXXZ - present without drawing anything new.
void agiGLRefreshFrame();

// ?DoFlush@gfxViewport@@AAEXXZ - apply the viewport rectangle and projection matrix. `flags` is
// gfxViewport::Flags: bit 0 means the rectangle changed, bit 1 the projection.
void agiGLFlushViewport(const f32* projection, i32 flags, i32 x, i32 y, i32 width, i32 height, f32 min_z, f32 max_z);

// ?SetCamera@gfxRenderState@@SAXABVMatrix44@@@Z - load the view matrix.
void agiGLSetViewMatrix(const f32* view);

// Re-applies the last projection and view the game set, which agiGLBegin2D overwrote. Called from
// agiGLEnd2D - see the note in agigl/glframe.cpp.
bool agiGLGetViewportRect(i32 out[4]);
void agiGLRestore3DMatrices();

// Loads a D3D projection matrix, remapping D3D's [0,1] depth range onto GL's [-1,1].
void agiGLSetProjectionMatrix(const f32* projection);

// ?ProgressRect@@YAXHHHHI@Z - the loading screen's progress bar. A free function in the original
// too, not a gfxPipeline member.
void agiGLProgressRect(i32 x, i32 y, i32 width, i32 height, u32 color);

#endif
