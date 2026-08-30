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

enum gfxDrawMode : i32;

struct HWND__;
class Vector4;
struct _DDPIXELFORMAT;
struct _GUID;
class gfxBitmap;
class gfxImage;
struct gfxStridedData;
class gfxTexture;
struct gfxVertexBuffer;
class gfxViewport;

class gfxPipeline
{
public:
    // ?BeginFrame@gfxPipeline@@SAXXZ
    ARTS_EXPORT static void BeginFrame();

    // ?BeginGfx@gfxPipeline@@SA_NXZ
    ARTS_EXPORT static bool BeginGfx();

    // ?BeginGfx2D@gfxPipeline@@SA_NXZ
    ARTS_IMPORT static bool BeginGfx2D();

    // ?BeginGfx3D@gfxPipeline@@SA_NXZ
    ARTS_EXPORT static bool BeginGfx3D();

    // ?BeginInternal@gfxPipeline@@SAXXZ
    ARTS_IMPORT static void BeginInternal();

    // ?BeginOffscreen@gfxPipeline@@SAXXZ
    ARTS_IMPORT static void BeginOffscreen();

    // ?BeginScene@gfxPipeline@@SAXXZ
    ARTS_IMPORT static void BeginScene();

    // ?Blit2D@gfxPipeline@@SAXABVVector4@@PAGPAEHI@Z
    ARTS_IMPORT static void Blit2D(const Vector4& arg1, u16* arg2, u8* arg3, i32 arg4, u32 arg5);

    // ?BlitText@gfxPipeline@@SAXABVVector4@@PAGPAEHI@Z
    ARTS_IMPORT static void BlitText(const Vector4& arg1, u16* arg2, u8* arg3, i32 arg4, u32 arg5);

    // ?Clear@gfxPipeline@@SAXHIMI@Z
    ARTS_IMPORT static void Clear(i32 arg1, u32 arg2, f32 arg3, u32 arg4);

    // ?ClearRect@gfxPipeline@@SAXHHHHI@Z
    ARTS_IMPORT static void ClearRect(i32 arg1, i32 arg2, i32 arg3, i32 arg4, u32 arg5);

    // ?CopyBitmap@gfxPipeline@@SAXHHPAVgfxBitmap@@HHHH_N@Z
    ARTS_EXPORT static void CopyBitmap(i32 arg1, i32 arg2, gfxBitmap* arg3, i32 arg4, i32 arg5, i32 arg6, i32 arg7, bool arg8);

    // ?CopyClippedBitmap@gfxPipeline@@SA_NHHPAVgfxBitmap@@HHHHHHHH_N@Z
    ARTS_IMPORT static bool CopyClippedBitmap(i32 arg1, i32 arg2, gfxBitmap* arg3, i32 arg4, i32 arg5, i32 arg6, i32 arg7, i32 arg8, i32 arg9, i32 arg10, i32 arg11, bool arg12);

    // ?CopyClippedBitmap@gfxPipeline@@SA_NHHPAVgfxBitmap@@HHHH_N@Z
    ARTS_IMPORT static bool CopyClippedBitmap(i32 arg1, i32 arg2, gfxBitmap* arg3, i32 arg4, i32 arg5, i32 arg6, i32 arg7, bool arg8);

    // ?CreateReadbackImage@gfxPipeline@@SAPAVgfxImage@@XZ
    ARTS_IMPORT static gfxImage* CreateReadbackImage();

    // ?CreateVertexBuffer@gfxPipeline@@SAPAUgfxVertexBuffer@@II_N@Z
    ARTS_IMPORT static gfxVertexBuffer* CreateVertexBuffer(u32 arg1, u32 arg2, bool arg3);

    // ?CreateViewport@gfxPipeline@@SAPAVgfxViewport@@XZ
    ARTS_IMPORT static gfxViewport* CreateViewport();

    // ?DestroyVertexBuffer@gfxPipeline@@SAXPAUgfxVertexBuffer@@@Z
    ARTS_IMPORT static void DestroyVertexBuffer(gfxVertexBuffer* arg1);

    // ?DoLockVertexBuffer@gfxPipeline@@SAPAXPAUgfxVertexBuffer@@I@Z
    ARTS_IMPORT static void* DoLockVertexBuffer(gfxVertexBuffer* arg1, u32 arg2);

    // ?Draw@gfxPipeline@@SAXW4gfxDrawMode@@IHABUgfxStridedData@@@Z
    ARTS_IMPORT static void Draw(gfxDrawMode arg1, u32 arg2, i32 arg3, const gfxStridedData& arg4);

    // ?Draw@gfxPipeline@@SAXW4gfxDrawMode@@IHPAX@Z
    ARTS_IMPORT static void Draw(gfxDrawMode arg1, u32 arg2, i32 arg3, void* arg4);

    // ?DrawIdx@gfxPipeline@@SAXW4gfxDrawMode@@IHABUgfxStridedData@@IPAG@Z
    ARTS_IMPORT static void DrawIdx(gfxDrawMode arg1, u32 arg2, i32 arg3, const gfxStridedData& arg4, u32 arg5, u16* arg6);

    // ?DrawIdx@gfxPipeline@@SAXW4gfxDrawMode@@IHPAXIPAG@Z
    ARTS_IMPORT static void DrawIdx(gfxDrawMode arg1, u32 arg2, i32 arg3, void* arg4, u32 arg5, u16* arg6);

    // ?DrawIdxVB@gfxPipeline@@SAXW4gfxDrawMode@@IIPAUgfxVertexBuffer@@IPAG@Z
    ARTS_IMPORT static void DrawIdxVB(gfxDrawMode arg1, u32 arg2, u32 arg3, gfxVertexBuffer* arg4, u32 arg5, u16* arg6);

    // ?DrawVB@gfxPipeline@@SAXW4gfxDrawMode@@IIPAUgfxVertexBuffer@@@Z
    ARTS_IMPORT static void DrawVB(gfxDrawMode arg1, u32 arg2, u32 arg3, gfxVertexBuffer* arg4);

    // ?EndFrame@gfxPipeline@@SAXXZ
    ARTS_EXPORT static void EndFrame();

    // ?EndGfx@gfxPipeline@@SAXXZ
    ARTS_EXPORT static void EndGfx();

    // ?EndGfx2D@gfxPipeline@@SAXXZ
    ARTS_IMPORT static void EndGfx2D();

    // ?EndGfx3D@gfxPipeline@@SAXXZ
    ARTS_IMPORT static void EndGfx3D();

    // ?EndInternal@gfxPipeline@@SAXXZ
    ARTS_IMPORT static void EndInternal();

    // ?EndOffscreen@gfxPipeline@@SAXXZ
    ARTS_IMPORT static void EndOffscreen();

    // ?EndScene@gfxPipeline@@SAXXZ
    ARTS_IMPORT static void EndScene();

    // ?EnumDDAdapters@gfxPipeline@@SAXHP6GHPAU_GUID@@PADPADPAX@Z3@Z
    ARTS_IMPORT static void EnumDDAdapters(i32 arg1, i32 (__stdcall* arg2)(_GUID*,char*,char*,void*), void* arg3);

    // ?FlushFrame@gfxPipeline@@SAXXZ
    ARTS_IMPORT static void FlushFrame();

    // ?GetWidth@gfxPipeline@@SAHXZ
    ARTS_IMPORT static i32 GetWidth();

    // ?Manage@gfxPipeline@@SAXXZ
    ARTS_IMPORT static void Manage();

    // ?PersistVertexBuffer@gfxPipeline@@SAXPAUgfxVertexBuffer@@@Z
    ARTS_IMPORT static void PersistVertexBuffer(gfxVertexBuffer* arg1);

    // ?Readback@gfxPipeline@@SAXPAVgfxImage@@HHHH@Z
    ARTS_IMPORT static void Readback(gfxImage* arg1, i32 arg2, i32 arg3, i32 arg4, i32 arg5);

    // ?RefreshFrame@gfxPipeline@@SAXXZ
    ARTS_IMPORT static void RefreshFrame();

    // ?Render@gfxPipeline@@SAXW4gfxDrawMode@@IIPAX@Z
    ARTS_IMPORT static void Render(gfxDrawMode arg1, u32 arg2, u32 arg3, void* arg4);

    // ?RenderIdx@gfxPipeline@@SAXW4gfxDrawMode@@IIPAXIPAG@Z
    ARTS_IMPORT static void RenderIdx(gfxDrawMode arg1, u32 arg2, u32 arg3, void* arg4, u32 arg5, u16* arg6);

    // ?SafeBeginGfx@gfxPipeline@@SAXXZ
    ARTS_IMPORT static void SafeBeginGfx();

    // ?SetFade@gfxPipeline@@SAXI@Z
    ARTS_IMPORT static void SetFade(u32 arg1);

    // ?SetRenderTarget@gfxPipeline@@SA_NPBVgfxTexture@@@Z
    ARTS_IMPORT static bool SetRenderTarget(const gfxTexture* arg1);

    // ?SetRes@gfxPipeline@@SAXHHHH_N@Z
    ARTS_IMPORT static void SetRes(i32 arg1, i32 arg2, i32 arg3, i32 arg4, bool arg5);

    // ?SetTitle@gfxPipeline@@SAXPBD@Z
    ARTS_IMPORT static void SetTitle(const char* arg1);

    // ?SetWindow@gfxPipeline@@SAX_NHH@Z
    ARTS_IMPORT static void SetWindow(bool arg1, i32 arg2, i32 arg3);

    // ?StartFade@gfxPipeline@@SAXIM@Z
    ARTS_IMPORT static void StartFade(u32 arg1, f32 arg2);

    // ?VP@gfxPipeline@@2PAVgfxViewport@@A
    ARTS_IMPORT static gfxViewport* VP;

    // ?OrthoVP@gfxPipeline@@2PAVgfxViewport@@A
    ARTS_IMPORT static gfxViewport* OrthoVP;

public:
    static i32& Access_m_ZDepth() { return m_ZDepth; }
    static i32& Access_m_X() { return m_X; }
    static f32& Access_m_fWidth() { return m_fWidth; }
    static i32& Access_m_ColorDepth() { return m_ColorDepth; }
    static i32& Access_m_iHeight() { return m_iHeight; }
    static i32& Access_m_Y() { return m_Y; }
    static u32& Access_m_EvtFlags() { return m_EvtFlags; }
    static f32& Access_m_fHeight() { return m_fHeight; }
    static gfxViewport*& Access_m_Viewport() { return m_Viewport; }
    static i32& Access_m_iWidth() { return m_iWidth; }
    static const gfxTexture*const& Access_m_RenderTarget() { return m_RenderTarget; }
    static f32& Access_FadeValue() { return FadeValue; }
    static u32& Access_StartFadeColor() { return StartFadeColor; }
    static u32& Access_EndFadeColor() { return EndFadeColor; }
    static f32& Access_FadeSpeed() { return FadeSpeed; }
    static u32& Access_FadeColor() { return FadeColor; }

private:
    // ?DoUnlockVertexBuffer@gfxPipeline@@CAXPAUgfxVertexBuffer@@@Z
    ARTS_IMPORT static void DoUnlockVertexBuffer(gfxVertexBuffer* arg1);

    // ?ForceSetViewport@gfxPipeline@@CAXPAVgfxViewport@@@Z
    ARTS_IMPORT static void ForceSetViewport(gfxViewport* arg1);

    // ?gfxEnumTexs@gfxPipeline@@CGJPAU_DDPIXELFORMAT@@PAX@Z
    ARTS_IMPORT static ilong gfxEnumTexs(_DDPIXELFORMAT* arg1, void* arg2);

    // ?gfxEnumZ@gfxPipeline@@CGJPAU_DDPIXELFORMAT@@PAX@Z
    ARTS_IMPORT static ilong gfxEnumZ(_DDPIXELFORMAT* arg1, void* arg2);

    // ?gfxWindowCreate@gfxPipeline@@CAXPBD@Z
    ARTS_IMPORT static void gfxWindowCreate(const char* arg1);

    // ?gfxWindowProc@gfxPipeline@@CGJPAUHWND__@@IIJ@Z
    ARTS_IMPORT static ilong gfxWindowProc(HWND__* arg1, u32 arg2, u32 arg3, ilong arg4);

    // ?OptimizeVertexBuffer@gfxPipeline@@CAXPAUgfxVertexBuffer@@@Z
    ARTS_IMPORT static void OptimizeVertexBuffer(gfxVertexBuffer* arg1);

    // ?m_ZDepth@gfxPipeline@@0HA
    ARTS_IMPORT static i32 m_ZDepth;

    // ?m_X@gfxPipeline@@0HA
    ARTS_IMPORT static i32 m_X;

    // ?m_fWidth@gfxPipeline@@0MA
    ARTS_IMPORT static f32 m_fWidth;

    // ?m_ColorDepth@gfxPipeline@@0HA
    ARTS_IMPORT static i32 m_ColorDepth;

    // ?m_iHeight@gfxPipeline@@0HA
    ARTS_IMPORT static i32 m_iHeight;

    // ?m_Y@gfxPipeline@@0HA
    ARTS_IMPORT static i32 m_Y;

    // ?m_EvtFlags@gfxPipeline@@0IA
    ARTS_IMPORT static u32 m_EvtFlags;

    // ?m_fHeight@gfxPipeline@@0MA
    ARTS_IMPORT static f32 m_fHeight;

    // ?m_Viewport@gfxPipeline@@0PAVgfxViewport@@A
    ARTS_IMPORT static gfxViewport* m_Viewport;

    // ?m_iWidth@gfxPipeline@@0HA
    ARTS_IMPORT static i32 m_iWidth;

    // ?m_RenderTarget@gfxPipeline@@0PBVgfxTexture@@B
    ARTS_IMPORT static const gfxTexture*const m_RenderTarget;

    // ?FadeValue@gfxPipeline@@0MA
    ARTS_IMPORT static f32 FadeValue;

    // ?StartFadeColor@gfxPipeline@@0IA
    ARTS_IMPORT static u32 StartFadeColor;

    // ?EndFadeColor@gfxPipeline@@0IA
    ARTS_IMPORT static u32 EndFadeColor;

    // ?FadeSpeed@gfxPipeline@@0MA
    ARTS_IMPORT static f32 FadeSpeed;

    // ?FadeColor@gfxPipeline@@0IA
    ARTS_IMPORT static u32 FadeColor;
};

// check_size(gfxPipeline, 0x1); // size known, members are not - cannot verify
