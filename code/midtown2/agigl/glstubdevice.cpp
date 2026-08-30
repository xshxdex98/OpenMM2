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

#include "core/arts.h"

define_dummy_symbol(agigl_glstubdevice);

#ifdef ARTS_AGIGL

#    include "data7/printer.h"
#    include "gfx/ddrawshim.h"

#    include "glstub.h"

// A DO-NOTHING IDirect3DDevice7, SO THE 1999 CODE CAN RUN WITH NO DEVICE BEHIND IT.
//
// Sixty-nine functions in midtown2.exe dereference lpD3DDev, lpdsRend or one of their siblings, and
// under -gl every one of them is a null pointer. Porting all sixty-nine - each with a -gl branch
// and a fallback - is a great deal of work whose only purpose is to stop code running that would
// have been harmless if it had run. gfxRenderState is the clearest case: SetLight, LightEnable,
// SetTexSource and the rest are thin wrappers that do useful bookkeeping in globals the rest of
// the engine reads, and then call one device method. The bookkeeping is wanted. Only the last line
// is not.
//
// So instead of removing the callers, give them a device. This object's methods do nothing and
// report success, which lets every one of those functions run its original code to completion,
// keep its globals correct, and drop exactly the one call that would have gone to hardware.
// Nothing is drawn by it - drawing is agigl/'s job - and that is the point: it is scaffolding for
// bring-up, not a renderer.
//
// IT IS ALSO WHERE THE RENDERER WILL GROW. Replacing a stub with a real implementation is how the
// 3D path gets written: DrawIndexedPrimitiveVB becomes a GL draw call, SetTexture becomes a bind,
// SetRenderState becomes glEnable/glBlendFunc. Each one can be done and tested on its own, in one
// place, instead of being spread across sixty-nine game functions.
//
// THE ONE THING THAT MUST BE EXACTLY RIGHT IS THE STACK.
//
// COM methods are __stdcall: the CALLEE pops the arguments, and `this` is pushed as the first of
// them. A stub that pops the wrong number of bytes does not fail visibly - it returns to a
// corrupted stack inside original code, and the damage surfaces somewhere else entirely. There is
// no way to write "pop however many the caller pushed" in C, so each slot below carries its own
// argument count from the IDirect3DDevice7 declaration, and the naked stub returns with exactly
// that many bytes. The count INCLUDES this.

namespace
{
    // Each stub zeroes eax - S_OK, and also a benign zero for the handful of methods whose return
    // value is read as something other than an HRESULT - and pops its own arguments.
    //
    // Out parameters are deliberately left untouched. The game caches every piece of device state
    // it cares about in its own globals (that is what gfxRenderStateData is for), so the Get side
    // is almost never read back; where it is, a zero would be no more truthful than whatever is
    // already there, and pretending to answer would hide the fact that this device knows nothing.
#    define D3D_STUB(name, bytes)                                                                  \
        __declspec(naked) void __stdcall name()                                                    \
        {                                                                                          \
            __asm { xor eax, eax }                                                                 \
            __asm { ret bytes }                                                                    \
        }

// THE FAILING VARIANT, AND WHY IT IS NOT OPTIONAL.
//
// A stub that reports S_OK is only honest for a method that CONSUMES its arguments. Any method
// that hands something back through an out parameter is lying: the caller reads a pointer this
// device never wrote and uses whatever was already there.
//
// gfxTexture::Load showed exactly how that goes wrong. It walks a mipmap chain with
// `if (surface->GetAttachedSurface(&caps, &next)) break;` - failure is how the loop ENDS. With
// GetAttachedSurface reporting success and leaving `next` untouched, the loop believed in a mip
// level that was not there and called Release through a stale pointer, faulting at 0x004AD437 on
// a vtable of 0xC483F893.
//
// So everything that returns an interface, a handle or a DC returns E_FAIL, which is a normal
// answer the 1999 code already knows how to handle.
#    define D3D_FAIL(name, bytes)                                                                          __declspec(naked) void __stdcall name()                                                            {                                                                                                      __asm { mov eax, 0x80004005 }                                                                      __asm { ret bytes }                                                                            }


    // clang-format off
    D3D_STUB(QueryInterface,               12)
    D3D_STUB(AddRef,                        4)
    D3D_STUB(Release,                       4)
    D3D_STUB(GetCaps,                       8)
    D3D_STUB(EnumTextureFormats,           12)
    D3D_STUB(BeginScene,                    4)
    D3D_STUB(EndScene,                      4)
    D3D_FAIL(GetDirect3D, 8)
    D3D_STUB(SetRenderTarget,              12)
    D3D_FAIL(GetRenderTarget, 8)
    D3D_STUB(Clear,                        28)
    D3D_STUB(SetTransform,                 12)
    D3D_STUB(GetTransform,                 12)
    D3D_STUB(SetViewport,                   8)
    D3D_STUB(MultiplyTransform,            12)
    D3D_STUB(GetViewport,                   8)
    D3D_STUB(SetMaterial,                   8)
    D3D_STUB(GetMaterial,                   8)
    D3D_STUB(SetLight,                     12)
    D3D_STUB(SetRenderState,               12)
    D3D_STUB(GetRenderState,               12)
    D3D_STUB(BeginStateBlock,               4)
    D3D_FAIL(EndStateBlock, 8)
    D3D_STUB(PreLoad,                       8)
    D3D_STUB(DrawPrimitive,                24)
    D3D_STUB(DrawIndexedPrimitive,         32)
    D3D_STUB(SetClipStatus,                 8)
    D3D_STUB(GetClipStatus,                 8)
    D3D_STUB(DrawPrimitiveStrided,         24)
    D3D_STUB(DrawPrimitiveVB,              24)
    D3D_STUB(DrawIndexedPrimitiveVB,       32)
    D3D_STUB(ComputeSphereVisibility,      24)
    D3D_FAIL(GetTexture, 12)
    D3D_STUB(SetTexture,                   12)
    D3D_STUB(GetTextureStageState,         16)
    D3D_STUB(SetTextureStageState,         16)
    D3D_STUB(ValidateDevice,                8)
    D3D_STUB(ApplyStateBlock,               8)
    D3D_STUB(CaptureStateBlock,             8)
    D3D_STUB(DeleteStateBlock,              8)
    D3D_FAIL(CreateStateBlock, 12)
    D3D_STUB(LightEnable,                  12)
    D3D_STUB(GetLightEnable,               12)
    D3D_STUB(SetClipPlane,                 12)
    D3D_STUB(GetClipPlane,                 12)
    D3D_STUB(GetInfo,                      16)
    // clang-format on

#    define D3D_STUB2(name, bytes)                                                                         __declspec(naked) void __stdcall name()                                                            {                                                                                                      __asm { xor eax, eax }                                                                             __asm { ret bytes }                                                                            }

#    undef D3D_STUB

    using D3DSlot = void(__stdcall*)();

    // The order is the IDirect3DDevice7 declaration order and must not be rearranged - the game
    // calls through fixed offsets, and gfxRenderState::SetCamera reaching SetTransform at
    // [ecx+2Ch] is the check that this table starts where it should: 0x2C / 4 == 11.
    const D3DSlot kStubVtbl[] = {
        QueryInterface, AddRef, Release, reinterpret_cast<D3DSlot>(agiGLStubGetCaps),
        reinterpret_cast<D3DSlot>(agiGLStubEnumTextureFormats), BeginScene, EndScene,
        GetDirect3D, SetRenderTarget, GetRenderTarget, reinterpret_cast<D3DSlot>(agiGLStubClear), reinterpret_cast<D3DSlot>(agiGLStubSetTransform), GetTransform,
        reinterpret_cast<D3DSlot>(agiGLStubSetViewport), MultiplyTransform, GetViewport, reinterpret_cast<D3DSlot>(agiGLStubSetMaterial), GetMaterial,
        reinterpret_cast<D3DSlot>(agiGLStubSetLight), reinterpret_cast<D3DSlot>(agiGLStubGetLight),
        reinterpret_cast<D3DSlot>(agiGLStubSetRenderState), reinterpret_cast<D3DSlot>(agiGLStubGetRenderState), BeginStateBlock, EndStateBlock, PreLoad, reinterpret_cast<D3DSlot>(agiGLStubDrawPrimitive),
        reinterpret_cast<D3DSlot>(agiGLStubDrawIndexedPrimitive), SetClipStatus, GetClipStatus, DrawPrimitiveStrided,
        reinterpret_cast<D3DSlot>(agiGLStubDrawIndexedPrimitiveStrided), DrawPrimitiveVB, DrawIndexedPrimitiveVB,
        ComputeSphereVisibility, GetTexture, reinterpret_cast<D3DSlot>(agiGLStubSetTexture), GetTextureStageState, reinterpret_cast<D3DSlot>(agiGLStubSetTextureStageState),
        ValidateDevice, ApplyStateBlock, CaptureStateBlock, DeleteStateBlock, CreateStateBlock,
        reinterpret_cast<D3DSlot>(agiGLStubDeviceLoadTexture), reinterpret_cast<D3DSlot>(agiGLStubLightEnable),
        reinterpret_cast<D3DSlot>(agiGLStubGetLightEnable), SetClipPlane, GetClipPlane, GetInfo};

    // The object itself: a vtable pointer and nothing else, which is all a COM interface is.
    struct StubDevice
    {
        const D3DSlot* lpVtbl;
    };

    StubDevice g_stub_device = {kStubVtbl};
} // namespace


// THE SURFACE HALF: A STUB IDirectDrawSurface7.
//
// gfxBitmap::Surface holds an agiGLSurface under -gl, and its first member is a pointer to this
// table, so original code that calls a DirectDraw method on a bitmap lands here instead of
// reading the surface's magic number as a vtable.
//
// GetDC and Lock return FAILURE rather than success, and that distinction matters. Both hand back
// a resource through an out parameter, and this device has none to give; answering S_OK with the
// caller's uninitialised HDC or surface pointer left untouched would send it on to use whatever
// was on the stack. mmText::GetDC turns the HRESULT into a boolean with `neg eax / sbb eax, eax`
// immediately afterwards, so a failure is a value it already knows how to handle. Everything else
// here is a setter or a query the game keeps its own copy of, and S_OK is honest for those.
namespace
{
    D3D_STUB2(s_QueryInterface, 12)
    D3D_STUB2(s_AddRef, 4)
    D3D_STUB2(s_Release, 4)
    D3D_STUB2(s_AddAttachedSurface, 8)
    D3D_STUB2(s_AddOverlayDirtyRect, 8)
    D3D_STUB2(s_Blt, 24)
    D3D_STUB2(s_BltBatch, 16)
    D3D_STUB2(s_BltFast, 24)
    D3D_STUB2(s_DeleteAttachedSurface, 12)
    D3D_STUB2(s_EnumAttachedSurfaces, 12)
    D3D_STUB2(s_EnumOverlayZOrders, 16)
    D3D_STUB2(s_Flip, 12)
    D3D_FAIL(s_GetAttachedSurface, 12)
    D3D_STUB2(s_GetBltStatus, 8)
    D3D_STUB2(s_GetCaps, 8)
    D3D_FAIL(s_GetClipper, 8)
    D3D_STUB2(s_GetColorKey, 12)
    __declspec(naked) void __stdcall s_GetDC()
    {
        __asm { mov eax, 0x80004005 }
        __asm { ret 8 }
    }

    D3D_STUB2(s_GetFlipStatus, 8)
    D3D_STUB2(s_GetOverlayPosition, 12)
    D3D_FAIL(s_GetPalette, 8)
    D3D_STUB2(s_GetPixelFormat, 8)
    D3D_STUB2(s_GetSurfaceDesc, 8)
    D3D_STUB2(s_Initialize, 12)
    D3D_STUB2(s_IsLost, 4)
    __declspec(naked) void __stdcall s_Lock()
    {
        __asm { mov eax, 0x80004005 }
        __asm { ret 20 }
    }

    D3D_STUB2(s_ReleaseDC, 8)
    D3D_STUB2(s_Restore, 4)
    D3D_STUB2(s_SetClipper, 8)
    D3D_STUB2(s_SetColorKey, 12)
    D3D_STUB2(s_SetOverlayPosition, 12)
    D3D_STUB2(s_SetPalette, 8)
    D3D_STUB2(s_Unlock, 8)
    D3D_STUB2(s_UpdateOverlay, 24)
    D3D_STUB2(s_UpdateOverlayDisplay, 8)
    D3D_STUB2(s_UpdateOverlayZOrder, 12)
    D3D_FAIL(s_GetDDInterface, 8)
    D3D_STUB2(s_PageLock, 8)
    D3D_STUB2(s_PageUnlock, 8)
    D3D_STUB2(s_SetSurfaceDesc, 12)
    D3D_STUB2(s_SetPrivateData, 20)
    D3D_STUB2(s_GetPrivateData, 16)
    D3D_STUB2(s_FreePrivateData, 8)
    D3D_STUB2(s_GetUniquenessValue, 8)
    D3D_STUB2(s_ChangeUniquenessValue, 4)
    D3D_STUB2(s_SetPriority, 8)
    D3D_STUB2(s_GetPriority, 8)
    D3D_STUB2(s_SetLOD, 8)
    D3D_STUB2(s_GetLOD, 8)

    const D3DSlot kStubSurfaceVtbl[] = {
        s_QueryInterface, reinterpret_cast<D3DSlot>(agiGLStubSurfaceAddRef),
        reinterpret_cast<D3DSlot>(agiGLStubSurfaceRelease), s_AddAttachedSurface, s_AddOverlayDirtyRect, s_Blt, s_BltBatch, s_BltFast, s_DeleteAttachedSurface, s_EnumAttachedSurfaces, s_EnumOverlayZOrders, s_Flip, s_GetAttachedSurface, s_GetBltStatus, s_GetCaps, s_GetClipper, s_GetColorKey, reinterpret_cast<D3DSlot>(agiGLStubSurfaceGetDC), s_GetFlipStatus, s_GetOverlayPosition, s_GetPalette, reinterpret_cast<D3DSlot>(agiGLStubSurfaceGetPixelFormat), reinterpret_cast<D3DSlot>(agiGLStubSurfaceGetSurfaceDesc), s_Initialize, s_IsLost, reinterpret_cast<D3DSlot>(agiGLStubSurfaceLock), reinterpret_cast<D3DSlot>(agiGLStubSurfaceReleaseDC), s_Restore, s_SetClipper, s_SetColorKey, s_SetOverlayPosition, s_SetPalette, reinterpret_cast<D3DSlot>(agiGLStubSurfaceUnlock), s_UpdateOverlay, s_UpdateOverlayDisplay, s_UpdateOverlayZOrder, s_GetDDInterface, s_PageLock, s_PageUnlock, s_SetSurfaceDesc, s_SetPrivateData, s_GetPrivateData, s_FreePrivateData, s_GetUniquenessValue, s_ChangeUniquenessValue, s_SetPriority, s_GetPriority, s_SetLOD, s_GetLOD};
} // namespace

const void* agiGLStubSurfaceVtbl()
{
    return kStubSurfaceVtbl;
}


// THE DIRECTDRAW OBJECT ITSELF.
//
// Only CreateSurface does anything, and it lives in agigl/glddshim.cpp because it needs to make a
// GL texture. Everything else here is a capability query whose answer the game already has, or a
// mode change the GL layer made before this object existed.
namespace
{
    D3D_STUB2(dd_QueryInterface, 12)
    D3D_STUB2(dd_AddRef, 4)
    D3D_STUB2(dd_Release, 4)
    D3D_STUB2(dd_Compact, 4)
    D3D_FAIL(dd_CreateClipper, 16)
    D3D_FAIL(dd_CreatePalette, 20)
    D3D_FAIL(dd_DuplicateSurface, 12)
    D3D_STUB2(dd_EnumDisplayModes, 20)
    D3D_STUB2(dd_EnumSurfaces, 20)
    D3D_STUB2(dd_FlipToGDISurface, 4)
    D3D_STUB2(dd_GetCaps, 12)
    D3D_STUB2(dd_GetDisplayMode, 8)
    D3D_STUB2(dd_GetFourCCCodes, 12)
    D3D_FAIL(dd_GetGDISurface, 8)
    D3D_STUB2(dd_GetMonitorFrequency, 8)
    D3D_STUB2(dd_GetScanLine, 8)
    D3D_STUB2(dd_GetVerticalBlankStatus, 8)
    D3D_STUB2(dd_Initialize, 8)
    D3D_STUB2(dd_RestoreDisplayMode, 4)
    D3D_STUB2(dd_SetCooperativeLevel, 12)
    D3D_STUB2(dd_SetDisplayMode, 24)
    D3D_STUB2(dd_WaitForVerticalBlank, 12)
    D3D_STUB2(dd_GetAvailableVidMem, 16)
    D3D_FAIL(dd_GetSurfaceFromDC, 12)
    D3D_STUB2(dd_RestoreAllSurfaces, 4)
    D3D_STUB2(dd_TestCooperativeLevel, 4)
    D3D_STUB2(dd_GetDeviceIdentifier, 12)
    D3D_STUB2(dd_StartModeTest, 16)
    D3D_STUB2(dd_EvaluateMode, 12)

    const D3DSlot kStubDDVtbl[] = {
        dd_QueryInterface, dd_AddRef, dd_Release, dd_Compact, dd_CreateClipper, dd_CreatePalette, reinterpret_cast<D3DSlot>(agiGLStubCreateSurface), dd_DuplicateSurface, reinterpret_cast<D3DSlot>(agiGLStubEnumDisplayModes), dd_EnumSurfaces, dd_FlipToGDISurface, dd_GetCaps, dd_GetDisplayMode, dd_GetFourCCCodes, dd_GetGDISurface, dd_GetMonitorFrequency, dd_GetScanLine, dd_GetVerticalBlankStatus, dd_Initialize, dd_RestoreDisplayMode, dd_SetCooperativeLevel, dd_SetDisplayMode, dd_WaitForVerticalBlank, dd_GetAvailableVidMem, dd_GetSurfaceFromDC, dd_RestoreAllSurfaces, dd_TestCooperativeLevel, dd_GetDeviceIdentifier, dd_StartModeTest, dd_EvaluateMode};

    struct StubDD
    {
        const D3DSlot* lpVtbl;
    };

    StubDD g_stub_dd = {kStubDDVtbl};
} // namespace

void agiGLInstallStubDirectDraw()
{
    if (lpDD != nullptr)
        return;

    lpDD = reinterpret_cast<IDirectDraw7*>(&g_stub_dd);

    Displayf("GL: installed the DirectDraw shim so the original texture loader can allocate surfaces");
}

void agiGLInstallStubDevice()
{
    if (lpD3DDev != nullptr)
        return;

    lpD3DDev = reinterpret_cast<IDirect3DDevice7*>(&g_stub_device);

    Displayf("GL: installed the do-nothing D3D device (%d slots) so the original render state can run",
        static_cast<i32>(sizeof(kStubVtbl) / sizeof(kStubVtbl[0])));
}

#endif
