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

// AS MUCH OF ddraw.h AS THE PORTED 2D PATH ACTUALLY CALLS.
//
// The build has no Windows headers on its include path at all - tools/build.py adds only the UCRT
// directory - so every platform structure a port needs has to be declared by hand. Until now each
// file that needed one declared its own, which was fine while gfxPipeline::CopyBitmap was the only
// port that touched DirectDraw and stops being fine the moment Set A is transcribed: six more
// functions want the same surface vtable, and three want DDSURFACEDESC2.
//
// EVERY OFFSET AND VTABLE INDEX HERE IS READ OUT OF THE BINARY, not copied from memory of ddraw.h.
// They agree with ddraw.h, but agreeing is a check rather than the source - a layout that is
// merely plausible would produce a call through the wrong slot, and the failure would be an
// access violation inside DirectDraw with nothing pointing back here.

// Win32's RECT. Four LONGs, and DirectDraw reads them in this order.
struct gfxBlitRect
{
    i32 left;
    i32 top;
    i32 right;
    i32 bottom;
};

// DDCOLORKEY.
struct gfxDDColorKey
{
    u32 dwColorSpaceLowValue;
    u32 dwColorSpaceHighValue;
};

// DDPIXELFORMAT, 0x20 bytes. gfxBitmap::Create sets dwSize to 0x20 at 0x004AE4FD before asking the
// primary surface to fill it in, which is what fixes the size.
struct gfxDDPixelFormat
{
    u32 dwSize;
    u32 dwFlags;
    u32 dwFourCC;
    u32 dwRGBBitCount;
    u32 dwRBitMask;
    u32 dwGBitMask;
    u32 dwBBitMask;
    u32 dwRGBAlphaBitMask;
};

check_size(gfxDDPixelFormat, 0x20);

// DDSCAPS2, 0x10 bytes.
struct gfxDDSCaps2
{
    u32 dwCaps;
    u32 dwCaps2;
    u32 dwCaps3;
    u32 dwCaps4;
};

check_size(gfxDDSCaps2, 0x10);

// DDSURFACEDESC2, 0x7C bytes.
//
// The size and four of the member offsets are pinned directly by gfxBitmap::Create at 0x004AE4D0,
// which is worth writing down because it makes the layout checkable rather than remembered:
//
//   sub esp, 0x80 / mov ecx, 0x1F / rep stosd   -> 0x7C bytes, zeroed
//   mov [ebp-0x80], 0x7C                        -> dwSize at 0x00
//   mov [ebp-0x7C], 0x11007                     -> dwFlags at 0x04
//   mov [ebp-0x78], edi  (arg2)                 -> dwHeight at 0x08
//   mov [ebp-0x74], esi  (arg1)                 -> dwWidth at 0x0C
//   mov [ebp-0x40], 0 / mov [ebp-0x3C], 0       -> ddckCKSrcBlt at 0x40
//   mov [ebp-0x38], 0x20                        -> ddpfPixelFormat at 0x48
//   mov [ebp-0x18], 0x800                       -> ddsCaps at 0x68
//
// Height BEFORE width, which is the order that catches people out and is not a typo here.
struct gfxDDSurfaceDesc2
{
    u32 dwSize;                          // 0x00
    u32 dwFlags;                         // 0x04
    u32 dwHeight;                        // 0x08
    u32 dwWidth;                         // 0x0C
    i32 lPitch;                          // 0x10
    u32 dwBackBufferCount;               // 0x14
    u32 dwMipMapCount;                   // 0x18
    u32 dwAlphaBitDepth;                 // 0x1C
    u32 dwReserved;                      // 0x20
    void* lpSurface;                     // 0x24
    gfxDDColorKey ddckCKDestOverlay;     // 0x28
    gfxDDColorKey ddckCKDestBlt;         // 0x30
    gfxDDColorKey ddckCKSrcOverlay;      // 0x38
    gfxDDColorKey ddckCKSrcBlt;          // 0x40
    gfxDDPixelFormat ddpfPixelFormat;    // 0x48
    gfxDDSCaps2 ddsCaps;                 // 0x68
    u32 dwTextureStage;                  // 0x78
};

check_size(gfxDDSurfaceDesc2, 0x7C);

// D3DVIEWPORT7, 0x18 bytes.
//
// gfx/gfxviewport.h names this as a member at offset 0x15C and nothing in the tree defines it, so
// that header does not compile on its own - the same defect gfxbitmap.h had with
// IDirectDrawSurface7. Defining it here means including ddrawshim.h before gfxviewport.h is
// enough to fix it, rather than every consumer redeclaring it.
//
// The size is checked against the class it sits in: gfxViewport must be 0x17C, and m_Viewport
// starts at 0x15C, so anything other than 0x18 here trips the check_size in that header rather
// than silently shifting every member after it.
struct D3DVIEWPORT7
{
    u32 dwX;
    u32 dwY;
    u32 dwWidth;
    u32 dwHeight;
    f32 dvMinZ;
    f32 dvMaxZ;
};

check_size(D3DVIEWPORT7, 0x18);

struct IDirectDrawSurface7;
struct IDirectDraw7;
struct IDirect3DDevice7;

// As much of the IDirectDrawSurface7 vtable as the ported path calls through, with the slots
// before the ones we want held as void* so the indices are right.
//
// The indices are not taken on trust. CopyBitmap at 0x004AB4C0 ends in `call [ecx+1Ch]`, and
// 0x1C/4 = 7, which is BltFast in declaration order (QueryInterface, AddRef, Release,
// AddAttachedSurface, AddOverlayDirtyRect, Blt, BltBatch, BltFast). Blt is index 5, reached at
// [ecx+14h] by gfxBitmap::Clear at 0x004AE580. GetPixelFormat is index 21, reached at [ecx+54h]
// by gfxBitmap::Create at 0x004AE508 - the call that faults under -gl, because lpdsRend is null
// when no DirectDraw primary surface was ever made.
struct IDirectDrawSurface7Vtbl
{
    void* QueryInterface;
    void* AddRef;
    void* Release;
    void* AddAttachedSurface;
    void* AddOverlayDirtyRect;

    ilong(__stdcall* Blt)(IDirectDrawSurface7* self, gfxBlitRect* dest_rect, IDirectDrawSurface7* source,
        gfxBlitRect* source_rect, u32 flags, void* fx);

    void* BltBatch;

    ilong(__stdcall* BltFast)(
        IDirectDrawSurface7* self, i32 x, i32 y, IDirectDrawSurface7* source, gfxBlitRect* source_rect, u32 flags);

    void* DeleteAttachedSurface;   // 8
    void* EnumAttachedSurfaces;    // 9
    void* EnumOverlayZOrders;      // 10
    void* Flip;                    // 11
    void* GetAttachedSurface;      // 12
    void* GetBltStatus;            // 13
    void* GetCaps;                 // 14
    void* GetClipper;              // 15
    void* GetColorKey;             // 16
    void* GetDC;                   // 17
    void* GetFlipStatus;           // 18
    void* GetOverlayPosition;      // 19
    void* GetPalette;              // 20

    ilong(__stdcall* GetPixelFormat)(IDirectDrawSurface7* self, gfxDDPixelFormat* format);
};

struct IDirectDrawSurface7
{
    const IDirectDrawSurface7Vtbl* lpVtbl;
};

// IDirectDraw7, for CreateSurface at index 6 - `call [ecx+18h]` at 0x004AE53F, and 0x18/4 = 6,
// which is CreateSurface after QueryInterface, AddRef, Release, Compact, CreateClipper and
// CreatePalette.
struct IDirectDraw7Vtbl
{
    void* QueryInterface;
    void* AddRef;
    void* Release;
    void* Compact;
    void* CreateClipper;
    void* CreatePalette;

    ilong(__stdcall* CreateSurface)(
        IDirectDraw7* self, gfxDDSurfaceDesc2* desc, IDirectDrawSurface7** surface, void* outer);
};

struct IDirectDraw7
{
    const IDirectDraw7Vtbl* lpVtbl;
};

// The two DirectDraw objects the 2D path blits between. Free globals, not gfxPipeline statics.
//
// U, not V: the retail mangling spells both interfaces as structs - ?lpdsRend@@3PAUIDirectDrawSurface7@@A
// and ?lpDD@@3PAUIDirectDraw7@@A. Getting the class-key wrong produces a PAV name that is not in
// the binary, and the link failure names the symbol rather than the mistake.
ARTS_IMPORT extern IDirectDrawSurface7* lpdsRend;
ARTS_IMPORT extern IDirectDraw7* lpDD;

// The Direct3D device, ?lpD3DDev@@3PAUIDirect3DDevice7@@A at 0x006830C8. Sixty-nine functions
// dereference it and under -gl it is null, which is what agigl/glstubdevice.cpp exists to fix.
// Left as an incomplete type on purpose: nothing in the tree calls a method on it by name, and
// giving it a vtable declaration here would invite someone to.
ARTS_IMPORT extern IDirect3DDevice7* lpD3DDev;

// DDBLTFAST_WAIT and DDBLTFAST_SRCCOLORKEY. Readable straight off the original: CopyBitmap does
// `xor edx, edx / test bl, bl / setnz dl / add edx, 10h`, so the flag word is 0x10 plus one when
// the colour-key argument is set.
#define MM_DDBLTFAST_SRCCOLORKEY 0x00000001u
#define MM_DDBLTFAST_WAIT 0x00000010u

// DDBLT_WAIT and DDBLT_KEYSRC, for the stretching path. DDBLT_WAIT is confirmed in the binary too:
// gfxPipeline::ClearRect pushes 0x01000400, which is DDBLT_WAIT | DDBLT_COLORFILL.
#define MM_DDBLT_KEYSRC 0x00008000u
#define MM_DDBLT_COLORFILL 0x00000400u
#define MM_DDBLT_WAIT 0x01000000u

// The dwFlags word gfxBitmap::Create writes, 0x11007, spelled out. DDSD_CKSRCBLT is the
// interesting one: every bitmap surface is created carrying a source colour key, and both bounds
// are left at zero - so black is transparent on every blit that asks for the key.
#define MM_DDSD_CAPS 0x00000001u
#define MM_DDSD_HEIGHT 0x00000002u
#define MM_DDSD_WIDTH 0x00000004u
#define MM_DDSD_PIXELFORMAT 0x00001000u
#define MM_DDSD_CKSRCBLT 0x00010000u

#define MM_DDSCAPS_OFFSCREENPLAIN 0x00000040u
#define MM_DDSCAPS_SYSTEMMEMORY 0x00000800u
