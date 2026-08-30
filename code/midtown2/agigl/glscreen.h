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

// RESOLUTION AS A RENDER TARGET SIZE, NOT A DISPLAY MODE.
//
// This is the structural decision from docs/resolution_blueprint.md, and it is the reason dgVoodoo
// stops being needed. dgVoodoo exists to translate DirectDraw mode setting and D3D7 rasterisation
// into something a modern driver accepts. Once the device layer never sets a mode - it renders into
// a framebuffer object and blits the result into a window - there is nothing left to translate.
//
// The consequence is that the resolution list stops describing what the hardware can do and starts
// describing what the game should render at. It is synthesised from the desktop size rather than
// enumerated, and every entry in it is achievable by construction, because a framebuffer accepts
// any size.
//
// FIVE COORDINATE SYSTEMS, ALL LIVE AT ONCE. Keeping them apart is most of the work:
//
//   horz_res/vert_res   real window client pixels
//   blit_*              the rectangle inside the window the game occupies, from -scaling
//   width/height        the LOGICAL resolution. gfxPipeline::Access_m_iWidth()/m_iHeight, and every game
//                       and UI coordinate, lives here
//   render_*            where 3D actually rasterises. Native by default, decoupled via the FBO
//   the UI box          derived from width/height by the UI layer, not by this file
struct agiGLScreen
{
    // The window's client area, in real pixels.
    i32 horz_res;
    i32 vert_res;

    // The logical resolution. This is what gets written into gfxPipeline::Access_m_iWidth()/m_iHeight, and
    // therefore what every piece of game code believes the screen size to be.
    i32 width;
    i32 height;

    // Where inside the window the blit lands, after -scaling.
    i32 blit_x;
    i32 blit_y;
    i32 blit_width;
    i32 blit_height;

    // Where 3D rasterises. Equal to the blit rect when -nativeres is on, which is the default.
    i32 render_x;
    i32 render_y;
    i32 render_width;
    i32 render_height;

    // GL_LINEAR when the render target is larger than the blit rect, GL_NEAREST when it is not.
    // Filtering a downscale is worth it; filtering a 1:1 blit only costs sharpness.
    u32 blit_filter;

    i32 msaa_level;
    bool fullscreen;
};

extern agiGLScreen agiScreen;

// Replaces the DirectDraw EnumDisplayModes callback (ResCallback) with a list built from the
// desktop bounds. Writes gfxInterfaces[0] and gfxInterfaceCount, so everything downstream -
// GraphicsOptions' resolution dropdown, gfxconf.dat, gfxFindSafeAdapter - keeps working unchanged.
void agiGLSynthesizeModes();

// Derives width/height from the selected mode (or -width/-height), then blit_* from -scaling and
// render_* from -nativeres. Call after the window exists and before the framebuffer is created.
void agiGLInitScaling(HWND window);
