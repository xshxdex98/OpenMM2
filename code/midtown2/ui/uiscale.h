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

#include "gfx/gfxpipeline.h"

// THE VIRTUAL 640x480 DESIGN SPACE.
//
// Nothing in this file comes from midtown2.exe. It is the one place the port decides what an
// authored UI coordinate means, and every reimplemented function that turns authored pixels into
// engine coordinates goes through it. See docs/ui_scaling.md for the analysis this implements.
//
// The problem it solves: MM2 stores UI positions as 0..1 fractions and multiplies by the live
// screen size at draw time, which is genuinely resolution-independent - but the CSV loaders and
// mmTextNode::Init PRODUCE those fractions by dividing authored pixels by the live screen size
// too. The two divisions cancel exactly, so a widget authored at x=100 lands at pixel 100 at any
// resolution, and at 2560x1440 the whole menu collapses into the top-left 640x480 corner.
//
// The fix is to divide by the size the artists actually worked at. That is all NormX/NormY do.
//
// WHERE THE MAPPING IS APPLIED. At the PRODUCER, never at the consumer. A fraction that leaves
// one of these functions is a fraction of the real screen, so the ~11,000 functions still in
// game.asm that multiply it back by gfxPipeline::Access_m_iWidth() keep working untouched. Applying the
// same mapping again at a consumer - Card2D::Cull is the tempting one - would scale twice.
//
// EVERYTHING IS static AND INLINE ON PURPOSE. tools/build.py quarantines any object file that
// defines a mangled symbol data/ported.json does not list, and these are ours, so they must not
// reach the linker as externals. Internal linkage guarantees that.

// The resolution the artists worked at. Every .csv coordinate, every literal pixel offset passed
// to mmTextNode::Init, and every UI bitmap in the 640 set is expressed in this space.
#define UI_DESIGN_WIDTH 640.0f
#define UI_DESIGN_HEIGHT 480.0f

// STRETCH (the default) maps the design box onto the whole screen, independently per axis.
// UNIFORM keeps 4:3, scales both axes by the same factor and centres the result - what Open1560
// does for MM1's menus (docs/resolution_blueprint.md).
//
// STRETCH is the default because MM2's UI is authored in two ways at once. The .csv widgets are
// authored in pixels, but MainMenu, uiNavBar and PUResults carry hand-written fractional literals
// that are already fractions of the WHOLE screen. Under UNIFORM those two populations end up in
// different coordinate systems - the .csv widgets pillarboxed, the hand-written ones not - and
// the menus come apart. Under STRETCH a .csv widget lands exactly where it lands at 640x480,
// relative to the screen, which is the same rule the hand-written ones already follow.
//
// Glyph size does not stretch either way: it always follows the vertical factor, so text stays
// square. See Scale() below.
#define UI_SCALE_STRETCH 0
#define UI_SCALE_UNIFORM 1

#ifndef UI_SCALE_MODE
#    define UI_SCALE_MODE UI_SCALE_STRETCH
#endif

// Whether UI BITMAPS are resampled when they are loaded, in gfxBitmap::Create(gfxImage*, bool).
//
// This is a separate switch from UI_SCALE_MODE because it is the one piece of this work that
// changes what a gfxBitmap IS rather than where it is drawn, and it is the one to turn off first
// if the UI misbehaves. -DUI_SCALE_BITMAPS=0 leaves every bitmap at its authored size and the
// rest of the scaling in place.
//
// Resampling has to happen at LOAD, not at blit time. Around forty callers lay their widgets out
// in assembly from gfxBitmap::Width - mmHUD walks a row of digits with `v4 += bmp->Width` - so
// the only way to scale a multi-part element without porting all forty is to make Width itself
// report the scaled size. See docs/ui_scaling_progress.md.
#ifndef UI_SCALE_BITMAPS
#    define UI_SCALE_BITMAPS 1
#endif

namespace uiScale {

// The horizontal factor: one design pixel is this many screen pixels.
static ARTS_FORCEINLINE f32 ScaleX()
{
#if UI_SCALE_MODE == UI_SCALE_UNIFORM
    f32 sx = static_cast<f32>(gfxPipeline::Access_m_iWidth()) / UI_DESIGN_WIDTH;
    f32 sy = static_cast<f32>(gfxPipeline::Access_m_iHeight()) / UI_DESIGN_HEIGHT;

    return (sx < sy) ? sx : sy;
#else
    return static_cast<f32>(gfxPipeline::Access_m_iWidth()) / UI_DESIGN_WIDTH;
#endif
}

// The vertical factor.
static ARTS_FORCEINLINE f32 ScaleY()
{
#if UI_SCALE_MODE == UI_SCALE_UNIFORM
    return ScaleX();
#else
    return static_cast<f32>(gfxPipeline::Access_m_iHeight()) / UI_DESIGN_HEIGHT;
#endif
}

// The single factor for anything that must stay square - glyph heights above all. It follows the
// VERTICAL axis, which is what mmCRHUD already does with `20 * m_iHeight / 480`. Using the
// horizontal one instead would make 16:9 text a third too large for the space it sits in.
static ARTS_FORCEINLINE f32 Scale()
{
    return ScaleY();
}

// Left edge of the design box in screen pixels. Zero unless the box is being pillarboxed.
static ARTS_FORCEINLINE f32 OriginX()
{
#if UI_SCALE_MODE == UI_SCALE_UNIFORM
    return (static_cast<f32>(gfxPipeline::Access_m_iWidth()) - UI_DESIGN_WIDTH * ScaleX()) * 0.5f;
#else
    return 0.0f;
#endif
}

// Top edge of the design box in screen pixels.
static ARTS_FORCEINLINE f32 OriginY()
{
#if UI_SCALE_MODE == UI_SCALE_UNIFORM
    return (static_cast<f32>(gfxPipeline::Access_m_iHeight()) - UI_DESIGN_HEIGHT * ScaleY()) * 0.5f;
#else
    return 0.0f;
#endif
}

// A design-space X POSITION in screen pixels.
static ARTS_FORCEINLINE f32 MapX(f32 design_x)
{
    return OriginX() + design_x * ScaleX();
}

// A design-space Y POSITION in screen pixels.
static ARTS_FORCEINLINE f32 MapY(f32 design_y)
{
    return OriginY() + design_y * ScaleY();
}

// A design-space WIDTH in screen pixels. An extent must not pick up the origin - that is the one
// mistake this pair of functions exists to prevent.
static ARTS_FORCEINLINE f32 MapW(f32 design_w)
{
    return design_w * ScaleX();
}

// A design-space HEIGHT in screen pixels.
static ARTS_FORCEINLINE f32 MapH(f32 design_h)
{
    return design_h * ScaleY();
}

// A design-space X position as a fraction of the screen - the form the engine stores.
//
// Under STRETCH this reduces to design_x / 640, which is exactly the two-character fix
// docs/ui_scaling.md describes: divide by the size the coordinate was authored against instead
// of the size it is about to be multiplied by.
static ARTS_FORCEINLINE f32 NormX(f32 design_x)
{
    return MapX(design_x) / static_cast<f32>(gfxPipeline::Access_m_iWidth());
}

// A design-space Y position as a fraction of the screen.
static ARTS_FORCEINLINE f32 NormY(f32 design_y)
{
    return MapY(design_y) / static_cast<f32>(gfxPipeline::Access_m_iHeight());
}

// A design-space width as a fraction of the screen.
static ARTS_FORCEINLINE f32 NormW(f32 design_w)
{
    return MapW(design_w) / static_cast<f32>(gfxPipeline::Access_m_iWidth());
}

// A design-space height as a fraction of the screen.
static ARTS_FORCEINLINE f32 NormH(f32 design_h)
{
    return MapH(design_h) / static_cast<f32>(gfxPipeline::Access_m_iHeight());
}

// A design-space pixel count rounded to whole screen pixels, for surface sizes.
//
// This pair is also the bitmap resampling rule. A UI bitmap is authored in the design space, so
// its own pixel width IS a design-space extent and PixelsW is the size it should occupy on
// screen. Under STRETCH that means a bitmap picks up the horizontal factor on its width and the
// vertical factor on its height, and so is stretched by the same 4:3-into-16:9 amount as the
// widget box it sits in - which is the point. Under UNIFORM both factors are equal and art keeps
// its aspect ratio, at the cost of the pillarboxing UI_SCALE_MODE documents above.
static ARTS_FORCEINLINE i32 PixelsW(i32 design_w)
{
    return static_cast<i32>(MapW(static_cast<f32>(design_w)) + 0.5f);
}

static ARTS_FORCEINLINE i32 PixelsH(i32 design_h)
{
    return static_cast<i32>(MapH(static_cast<f32>(design_h)) + 0.5f);
}

// The smallest font GDI can render legibly. mmCRHUD hard-codes this floor at every one of its
// call sites - `xa = 12; if (12 * m_iHeight / 480 >= 12) xa = 12 * m_iHeight / 480;` - so it is
// the game's own number, not one invented here.
#define UI_MIN_FONT_HEIGHT 12

// A font height authored for 640x480, in screen pixels.
//
// mmCRHUD computes `base * m_iHeight / 480` in integer arithmetic, which truncates. This rounds
// instead: the two agree at 480 and at every exact multiple, and rounding is a better answer in
// between. If a glyph metric ever has to match mmCRHUD's to the pixel, that is the difference to
// look at first.
static ARTS_FORCEINLINE i32 FontHeight(i32 base_height)
{
    i32 scaled = static_cast<i32>(static_cast<f32>(base_height) * Scale() + 0.5f);

    return (scaled < UI_MIN_FONT_HEIGHT) ? UI_MIN_FONT_HEIGHT : scaled;
}

} // namespace uiScale
