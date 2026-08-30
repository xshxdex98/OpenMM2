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

// THE VIDEO ADAPTER TABLE MM2 BUILDS AT STARTUP.
//
// Hand written, because tools/genheaders.py cannot emit it: gfxInterface has no methods, so it owns
// no symbols and never appears in the symbol table the generator walks. Same situation as
// gfx/gfxrenderstatedata.h. Layout confirmed against the type library in MM2_RE_KIT, where
// gfxInterface is 0x270 bytes with a 64-entry resolution array at 0x70.

// mmGraphicsInterface::mmRenderer
enum mmRenderer : i32
{
    Software = 0,
    Hardware = 1,
    HardwareWithTnL = 2,
};

// mmGraphicsInterface::mmColorDepthFlag. These are the DirectDraw DDBD_ bits, so 32-bit is the
// SMALL number - reading them as a bit width is a mistake waiting to happen.
enum mmColorDepthFlag : i32
{
    Depth32 = 0x100,
    Depth24 = 0x200,
    Depth16 = 0x400,
};

// mmGraphicsInterface::mmResolution.
struct gfxResData
{
    u16 ScreenWidth;  // 0x00
    u16 ScreenHeight; // 0x02
    u16 ColorDepth;   // 0x04

    // ResCallback writes 7 for a 16-bit mode and 6 for anything else.
    u16 Flags; // 0x06
};

static_assert(sizeof(gfxResData) == 0x8, "gfxResData must be 8 bytes");

// The resolution array is fixed at 64 entries, and agigl/glscreen.cpp generates into it, so the
// bound is checked there rather than assumed.
constexpr i32 kMaxResolutions = 64;

struct gfxInterface
{
    u8 GUID[16];                              // 0x000
    char Name[64];                            // 0x010
    u32 DeviceCaps;                           // 0x050
    mmRenderer Renderer;                      // 0x054
    u32 ResolutionCount;                      // 0x058
    u32 ResolutionChoice;                     // 0x05C
    mmColorDepthFlag AcceptableDepths;        // 0x060
    u32 AvailableMemory;                      // 0x064
    u32 VendorID;                             // 0x068
    u32 DeviceID;                             // 0x06C
    gfxResData Resolutions[kMaxResolutions];  // 0x070
};

static_assert(sizeof(gfxInterface) == 0x270, "gfxInterface must be 0x270 bytes to match midtown2.exe");
static_assert(offsetof(gfxInterface, Renderer) == 0x54, "layout drift");
static_assert(offsetof(gfxInterface, AvailableMemory) == 0x64, "layout drift");
static_assert(offsetof(gfxInterface, Resolutions) == 0x70, "layout drift");

// ?gfxInterfaces@@3PAUgfxInterface@@A - a POINTER to the array, not the array itself.
ARTS_IMPORT extern gfxInterface* gfxInterfaces;

// ?gfxInterfaceCount@@3HA
ARTS_IMPORT extern i32 gfxInterfaceCount;

// ?gfxInterfaceChoice@@3HA
ARTS_IMPORT extern i32 gfxInterfaceChoice;

// The bounds MM2 filters candidate modes against. The maximums used to be written by
// AutoDetectCallback, which the -gl path does not run, so agigl/glscreen.cpp sets them from the
// desktop instead - see the note there.
ARTS_IMPORT extern i32 gfxMinScreenWidth;
ARTS_IMPORT extern i32 gfxMinScreenHeight;
ARTS_IMPORT extern i32 gfxMaxScreenWidth;
ARTS_IMPORT extern i32 gfxMaxScreenHeight;
