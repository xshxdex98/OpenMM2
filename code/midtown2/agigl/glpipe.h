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

#include "glcommon.h"

struct agiGLSurface;

// The framebuffer the game renders into, and the screen-space quad path everything 2D goes through.
// Both live here because they are two halves of one decision.
//
// MM2's 2D and 3D drawing share a single DirectDraw surface, lpdsRend, and they are interleaved in
// a fixed order by asCullManager::Update:
//
//   BeginFrame  Clear  [2D background]  BeginScene  [3D]  EndScene  [2D foreground]  EndFrame
//
// The 2D passes are DirectDraw BltFast calls and the 3D pass is D3D, into the same pixels. There is
// no equivalent split in GL and there does not need to be: a BltFast becomes a textured quad with
// depth testing off, drawn into the same framebuffer, and the ordering falls out of the draw order
// asCullManager already imposes.
//
// The one thing that does not survive the translation is the colour key. DDBLTFAST_SRCCOLORKEY keys
// on the surface's black, so gfxBitmap::Load bumps opaque black to (1,1,1) to keep it from being
// keyed out. Under GL the upload writes a real alpha channel instead, and the blit turns alpha
// testing on only when the caller asked for the key - which is both closer to what the artists
// meant and free of the black-bump hack.

// Creates the render framebuffer at agiScreen.render_width by render_height. Falls back to the
// default framebuffer, with render_* forced to the blit rect, when the driver has no FBO support.
bool agiGLCreateFramebuffer();

void agiGLDestroyFramebuffer();

// Binds the render framebuffer, or the default one when there is none.
void agiGLBindRenderTarget();

// Resolves the render framebuffer into the window and swaps. This is the whole of what used to be
// lpdsFront->Blt and lpdsFront->Flip, and it is where arbitrary resolution actually happens: the
// render size and the window size are independent, and the blit reconciles them.
void agiGLPresent();

// Enters and leaves the screen-space drawing state. Sets an orthographic projection over the
// LOGICAL resolution, so a caller passing 640x480 coordinates is scaled to the render target rather
// than landing in its top-left corner - which is the BltFast behaviour docs/ui_scaling.md
// identifies as the single biggest visual break at high resolution.
// The resolution MM2's entire 2D layer is authored against. Menu bitmaps, CSV widget offsets, HUD
// positions and the loading bar are all in this space and are blitted 1:1, so it is the space
// agiGLBegin2D projects - see the note there.
constexpr i32 kAgiGLUIWidth = 640;
constexpr i32 kAgiGLUIHeight = 480;

void agiGLBegin2D();
void agiGLEnd2D();

// One screen-space quad, in logical coordinates. surface may be null for an untextured fill. color
// is 0xAARRGGBB, the packing every MM2 call site uses.
void agiGLDrawQuad2D(const agiGLSurface* surface, f32 dst_x, f32 dst_y, f32 dst_width, f32 dst_height, f32 src_u0,
    f32 src_v0, f32 src_u1, f32 src_v1, u32 color, bool alpha_test);
