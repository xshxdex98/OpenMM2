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

#include "glblit.h"
#include "gfx/gfxpipeline.h" // m_iWidth, the space the HUD lays out in
#include "glstub.h" // agiGLProbeVerbose
#include "data7/printer.h" // Displayf

#include "core/arts.h"

define_dummy_symbol(agigl_glblit);

#ifdef ARTS_AGIGL


#    include "glpipe.h"
#    include "glsurface.h"

void agiGLCopyBitmap(
    i32 dst_x, i32 dst_y, void* surface_slot, i32 src_x, i32 src_y, i32 width, i32 height, bool color_key)
{
    agiGLSurface* surface = agiGLSurfaceFromSlot(surface_slot);

    if (surface == nullptr)
        return;

    const f32 inv_width = 1.0f / static_cast<f32>(surface->Width);
    const f32 inv_height = 1.0f / static_cast<f32>(surface->Height);

    agiGLBegin2D();

    // The destination is the SOURCE size, because BltFast could not scale and this stands in for a
    // BltFast. What has changed is only the space the numbers live in: they are logical
    // coordinates now, and agiGLBegin2D's projection maps them onto the render target.
    // DIAGNOSTIC: every CopyBitmap call, in order, unfiltered.
    //
    // mmHUD::Cull draws its eight-glyph time readout through here, starting at
    // m_iWidth/2 - 3*digitWidth - colonWidth and advancing x by each bitmap's own width. Watching
    // only DISTINCT SIZES hid most of the loop and reported the first occurrence's position, which
    // is why four glyphs appeared at the top right with no visible start. The start x and the
    // per-glyph widths together say whether the layout or the artwork is at fault.
    {
        // SKIP THE FULL-SCREEN BLITS. They are the backdrop, they happen every frame from the very
        // first one, and with a fixed budget they consumed the whole log before the HUD was ever
        // toggled on - which is why four unrelated draws kept turning up and the eight-glyph loop
        // never did. The budget now goes to draws small enough to be HUD art.
        const bool full_screen = width >= 2000 || height >= 1200;

        static i32 shown = 0;

        if (!full_screen && shown < 40 && agiGLProbeVerbose())
        {
            ++shown;

            Displayf("BLIT2D %d dst=%d,%d size=%dx%d surface=%dx%d space=%d", shown, dst_x, dst_y, width,
                height, surface->Width, surface->Height, gfxPipeline::Access_m_iWidth());
        }
    }

    agiGLDrawQuad2D(surface, static_cast<f32>(dst_x), static_cast<f32>(dst_y), static_cast<f32>(width),
        static_cast<f32>(height), static_cast<f32>(src_x) * inv_width, static_cast<f32>(src_y) * inv_height,
        static_cast<f32>(src_x + width) * inv_width, static_cast<f32>(src_y + height) * inv_height, 0xFFFFFFFF,
        color_key);

    agiGLEnd2D();
}

#endif
