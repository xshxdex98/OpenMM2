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

// The GL half of gfxPipeline::CopyBitmap, as a plain free function.
//
// Split out of agigl/glgfxpipeline.cpp for two reasons, both structural rather than stylistic.
//
// FIRST, QUARANTINE IS PER-OBJECT. tools/build.py holds back any object containing a symbol from
// data/symbols.json that is not yet in data/ported.json, and glgfxpipeline.obj carries eleven
// unregistered gfxPipeline statics. Leaving the blit in there would mean the whole object is held
// back and this function does not reach the link, however ready it is. A file whose only symbol is
// this one - a name that appears in no MM2 binary and so is not a game symbol at all - is not
// subject to quarantine and links whenever ARTS_AGIGL is defined.
//
// SECOND, THE SLOT IS A void*, NOT A gfxBitmap*. That is what keeps this file free of
// gfx/gfxbitmap.h, and with it the generated-header dependency that made ARTS_AGIGL_PIPELINE
// necessary. gfxBitmap::Surface is declared IDirectDrawSurface7* because that is what retail
// stores there; under GL the same field holds an agiGLSurface slot, and agiGLSurfaceFromSlot is
// what turns one into the other. Passing the field through untyped says exactly that, and means
// the caller in gfx/gfxpipeline.cpp needs nothing from agigl/ but this declaration.
//
// Coordinates are the caller's, unchanged: a destination point and a source rectangle, in the
// logical space the original blitted in. The projection set up by agiGLBegin2D is what scales
// them to the render target, which is how a 640x480 loading screen comes to fill a 2560x1440
// window instead of sitting in the corner of one.

#ifdef ARTS_AGIGL

// Draws one bitmap. `surface_slot` is gfxBitmap::Surface; `color_key` is the original's
// DDBLTFAST_SRCCOLORKEY, which every bitmap surface carries set to black.
//
// Callers must have checked that the bitmap exists and the rectangle is non-empty - this does not
// re-check, because the DirectDraw path it stands in for does not either and the two have to agree
// about what a degenerate call does.
void agiGLCopyBitmap(
    i32 dst_x, i32 dst_y, void* surface_slot, i32 src_x, i32 src_y, i32 width, i32 height, bool color_key);

// The GL halves of the three gfxBitmap members in Set A, living in agigl/glbitmap.cpp.
//
// All three take the surface SLOT rather than the bitmap, for the same reason agiGLCopyBitmap
// does: it keeps them out of gfx/gfxbitmap.h and it keeps their object free of game symbols, so
// quarantine has nothing to hold back. Creating the gfxBitmap object itself is identical on both
// paths and is done once by the caller, after the branch.
class gfxImage;

void* agiGLBitmapCreate(i32 width, i32 height);
bool agiGLBitmapLoad(void* surface_slot, gfxImage* image, i32 width, i32 height);
void agiGLBitmapClear(void* surface_slot, u32 color, i32 width, i32 height);

#endif
