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

// WHAT GOES IN THE IDirectDrawSurface7* SLOTS.
//
// gfxBitmap::Surface sits at offset 0x008 and gfxTexture::m_Surface at 0x014, both typed
// IDirectDrawSurface7*, and both are read by machine code that has not been ported yet. The field
// cannot move, cannot change size, and cannot become a bare GLuint: a texture name of 3 stored in a
// pointer slot passes every `if (surface)` test in the binary and then dereferences address 3.
//
// So the slot keeps holding a pointer, to one of these instead. Three consequences follow, and all
// three are deliberate:
//
//  1. Null checks in unported code keep working, because a live surface is a real allocation.
//  2. A stray dereference reads our memory rather than random memory, and Magic makes it obvious in
//     a crash dump which kind of object was found.
//  3. A stray CALL through it still crashes, because there is no vtable here. That is on purpose.
//     Faking an IDirectDrawSurface7 vtable would mean writing 49 stubs in an order taken from
//     memory, and one wrong slot is a silent call to the wrong function. A crash at a known address
//     is a better outcome, and it names the function that still has to be ported.
//
// The list of functions that dereference these two fields, and therefore have to be ported in the
// same wave as the device layer, is in docs/opengl_plan.md.
struct agiGLSurface
{
    // 'AGIS'. First member so it lands where a vtable pointer would, which is what makes a wrong
    // dereference recognisable rather than merely fatal.
    // A STUB IDirectDrawSurface7 VTABLE, AND IT HAS TO BE FIRST.
    //
    // gfxBitmap::Surface is declared IDirectDrawSurface7* and under -gl it holds one of these, so
    // any original code that calls a DirectDraw method on a bitmap reads offset 0 as a vtable
    // pointer. mmText::GetDC does exactly that - `mov ecx, [eax] / call [ecx+44h]` at 0x005322D6 -
    // and with Magic at offset 0 it read 0x53494741 + 0x44. The fault address was 0x53494785,
    // which is the magic plus the slot, and named the cause unmistakably.
    //
    // Pointing this at a table of do-nothing methods makes every such call harmless in one place,
    // the way agigl/glstubdevice.cpp does for lpD3DDev. Magic remains the identity check, just no
    // longer at offset zero.
    const void* lpVtbl;


    u32 Magic;

    u32 Texture; // GL texture name
    u32 Framebuffer; // GL framebuffer name, non-zero only for a render target
    u16 Width;
    u16 Height;
    u16 LevelCount;

    // Whether level 0 has ever been written. A texture that is created and bound but never uploaded
    // samples undefined memory, which is how a surface the game fills by some route agigl does not
    // implement - Blt and BltFast are still stubs - shows up as a flat white patch rather than as
    // an error.
    bool Uploaded;
    u16 Flags;

    // The format triple the upload path hands glTexSubImage2D. Held rather than recomputed because
    // gfxTexture::Load re-derives its row converter from the surface description on every call, and
    // this is that description.
    GLenum Format;
    GLenum Type;
    GLenum Internal;

    // THE GDI SIDE, created on the first GetDC and kept afterwards.
    //
    // mmText draws every dynamic string - the race name, the dropdown rows, the location and
    // weather fields - with ordinary GDI into a device context it asks the SURFACE for. Under -gl
    // those surfaces are these, and until they could answer GetDC every one of those strings came
    // back blank while the static menu art drew perfectly. The shim in agigl/glddshim.cpp could
    // already do it, but only for the surfaces IT allocated, and a gfxBitmap never goes through
    // that path.
    //
    // Dc is a memory DC, Dib the 32-bit top-down section selected into it, and DibPixels its bits,
    // which are uploaded to Texture when the DC is released.
    void* Dc;
    void* Dib;
    u8* DibPixels;
};

constexpr u32 kAgiGLSurfaceMagic = 0x53494741; // 'AGIS', little endian

// Flags. Alpha drives whether a bitmap blit honours the colour key, and matches the meaning of
// gfxTexture's TexEnv bit 0x20000.
constexpr u16 kAgiGLSurfaceAlpha = 0x0001;
constexpr u16 kAgiGLSurfaceMipMapped = 0x0002;
constexpr u16 kAgiGLSurfaceRenderTarget = 0x0004;

// Turns a slot value back into a surface, checking Magic. Returns null for a null slot, and quits
// on a non-null slot that is not one of ours - that means a DirectDraw surface is still alive
// somewhere and the two paths are now mixed, which produces wrong pixels rather than a crash and is
// far worse to debug later.
agiGLSurface* agiGLSurfaceFromSlot(void* slot);

// Allocates the GL texture. level_count of 1 means no mipmaps.
agiGLSurface* agiGLSurfaceCreate(i32 width, i32 height, GLenum internal, GLenum format, GLenum type, i32 level_count);

void agiGLSurfaceDestroy(agiGLSurface* surface);

// Uploads one mip level. pitch is the source row stride in bytes, which is gfxImage::BytesPerRow
// and is not always width times bytes-per-texel.
void agiGLSurfaceUpload(agiGLSurface* surface, i32 level, const void* pixels, i32 pitch);
