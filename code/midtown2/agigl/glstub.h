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

#include "gfx/ddrawshim.h"

#ifdef ARTS_AGIGL

// The do-nothing COM objects that let the original DirectDraw and Direct3D code run under -gl.
// See agigl/glstubdevice.cpp for what they are for and why the stack discipline matters.

// Installs the stub device into lpD3DDev, if nothing has claimed it.
void agiGLInstallStubDevice();

// Installs a DirectDraw object into lpDD whose CreateSurface really does make a surface, so the
// original texture and bitmap loaders can run unmodified. See agigl/glddshim.cpp.
void agiGLInstallStubDirectDraw();

// Installs a stub primary surface into lpdsRend. Not for drawing - the GL layer does that - but
// because it is what the image loaders ask for the display pixel format.
void agiGLInstallStubPrimary(i32 width, i32 height);

// The stub IDirectDrawSurface7 vtable, for agiGLSurface::lpVtbl and for the surfaces
// agiGLStubCreateSurface hands out.
const void* agiGLStubSurfaceVtbl();

struct agiGLSurface;

// The GL texture behind a surface pointer that came from agiGLStubCreateSurface, or null if the
// pointer is something else.
agiGLSurface* agiGLSurfaceFromStub(void* slot);

// The GL surface behind ANY slot the game holds as an IDirectDrawSurface7*, whichever of the two
// kinds it is. Null when the pointer is neither.
agiGLSurface* agiGLResolveSurface(void* slot);

// IDirect3DDevice7::SetTexture. A D3D7 texture IS a surface, so this is a bind.
ilong __stdcall agiGLStubSetTexture(void* self, u32 stage, void* texture);

// IDirect3DDevice7::SetRenderState, and the re-application agiGLEnd2D needs because 2D drawing
// clobbers depth and culling every frame.
struct D3DVIEWPORT7;
void agiGLSetViewTransform(const f32* view);
void agiGLApplyModelView();
ilong __stdcall agiGLStubSetTransform(void* self, u32 state, f32* matrix);
ilong __stdcall agiGLStubSetViewport(void* self, D3DVIEWPORT7* vp);
ilong __stdcall agiGLStubSetRenderState(void* self, u32 state, u32 value);
ilong __stdcall agiGLStubSetTextureStageState(void* self, u32 stage, u32 state, u32 value);
void agiGLApplyRenderState();

// The surface methods that do real work, rather than reporting success and doing nothing. Declared
// with their true argument lists so the compiler emits the right __stdcall cleanup - getting that
// wrong corrupts the caller's stack inside original code.
// agigl/gltexdump.cpp - writes a bound texture out as a .bmp under -gltexdump.
bool agiGLTextureDumpEnabled();

// True only with -glprobe. Gates the per-draw diagnostic logging.
bool agiGLProbeVerbose();
void agiGLDumpTexture(u32 texture, i32 width, i32 height);

// agigl/gllight.cpp - D3D7 lighting translated onto GL's fixed-function lights.
ilong __stdcall agiGLStubSetLight(void* self, u32 index, f32* light);
ilong __stdcall agiGLStubLightEnable(void* self, u32 index, u32 enable);
bool agiGLPushViewOnly();
void agiGLPopViewOnly();

ilong __stdcall agiGLStubDrawIndexedPrimitiveStrided(
    void* self, u32 ptype, u32 fvf, void* strided, u32 vcount, u16* indices, u32 icount, u32 flags);

ilong __stdcall agiGLStubGetLight(void* self, u32 index, f32* out);
ilong __stdcall agiGLStubGetLightEnable(void* self, u32 index, u32* out);
ilong __stdcall agiGLStubSetMaterial(void* self, f32* material);
void agiGLApplyLights();
void agiGLSetAmbient(u32 argb);

ilong __stdcall agiGLStubClear(void* self, u32 count, void* rects, u32 flags, u32 color, f32 z, u32 stencil);
ilong __stdcall agiGLStubGetCaps(void* self, void* desc);
ilong __stdcall agiGLStubGetRenderState(void* self, u32 state, u32* out);
ilong __stdcall agiGLStubSurfaceAddRef(void* self);
ilong __stdcall agiGLStubSurfaceRelease(void* self);
ilong __stdcall agiGLStubSurfaceLock(void* self, void* rect, gfxDDSurfaceDesc2* desc, u32 flags, void* event);
ilong __stdcall agiGLStubSurfaceUnlock(void* self, void* rect);
ilong __stdcall agiGLStubDeviceLoadTexture(void* self, void* dest, void* dest_point, void* source, void* source_rect, u32 flags);
ilong __stdcall agiGLStubSurfaceGetDC(void* self, void** out_dc);
ilong __stdcall agiGLStubSurfaceReleaseDC(void* self, void* dc);
ilong __stdcall agiGLStubSurfaceGetPixelFormat(void* self, gfxDDPixelFormat* pf);
ilong __stdcall agiGLStubSurfaceGetSurfaceDesc(void* self, gfxDDSurfaceDesc2* desc);
// The two geometry entry points, in agigl/gldraw.cpp. These are what actually put triangles on the
// screen; everything else in this header is there to get the game as far as calling them.
ilong __stdcall agiGLStubDrawPrimitive(void* self, u32 ptype, u32 fvf, void* verts, u32 vcount, u32 flags);
ilong __stdcall agiGLStubDrawIndexedPrimitive(
    void* self, u32 ptype, u32 fvf, void* verts, u32 vcount, u16* indices, u32 icount, u32 flags);

ilong __stdcall agiGLStubEnumDisplayModes(void* self, u32 flags, void* desc, void* context, void* callback);
ilong __stdcall agiGLStubEnumTextureFormats(void* self, void* callback, void* context);
ilong __stdcall agiGLStubCreateSurface(void* self, gfxDDSurfaceDesc2* desc, void** out, void* outer);

#endif
