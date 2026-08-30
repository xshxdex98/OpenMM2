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

enum _D3DPRIMITIVETYPE : i32;
enum _D3DRENDERSTATETYPE : i32;
enum _D3DSTATEBLOCKTYPE : i32;
enum _D3DTEXTURESTAGESTATETYPE : i32;
enum _D3DTRANSFORMSTATETYPE : i32;

struct IDirect3D7;
struct IDirect3DVertexBuffer7;
struct IDirectDrawSurface7;
struct _D3DCLIPSTATUS;
struct _D3DDRAWPRIMITIVESTRIDEDDATA;
struct _D3DDeviceDesc7;
struct _D3DLIGHT7;
struct _D3DMATERIAL7;
struct _D3DMATRIX;
struct _D3DRECT;
struct _D3DVECTOR;
struct _D3DVIEWPORT7;
struct _DDPIXELFORMAT;
struct _GUID;
struct tagPOINT;
struct tagRECT;

class AgeDevice
{
public:
    // ?AddRef@AgeDevice@@UAGKXZ
    ARTS_IMPORT virtual ulong AddRef();

    // ?ApplyStateBlock@AgeDevice@@UAGJK@Z
    ARTS_IMPORT virtual ilong ApplyStateBlock(ulong arg1);

    // ?BeginScene@AgeDevice@@UAGJXZ
    ARTS_IMPORT virtual ilong BeginScene();

    // ?BeginStateBlock@AgeDevice@@UAGJXZ
    ARTS_IMPORT virtual ilong BeginStateBlock();

    // ?CaptureStateBlock@AgeDevice@@UAGJK@Z
    ARTS_IMPORT virtual ilong CaptureStateBlock(ulong arg1);

    // ?Clear@AgeDevice@@UAGJKPAU_D3DRECT@@KKMK@Z
    ARTS_IMPORT virtual ilong Clear(ulong arg1, _D3DRECT* arg2, ulong arg3, ulong arg4, f32 arg5, ulong arg6);

    // ?ComputeSphereVisibility@AgeDevice@@UAGJPAU_D3DVECTOR@@PAMKKPAK@Z
    ARTS_IMPORT virtual ilong ComputeSphereVisibility(_D3DVECTOR* arg1, f32* arg2, ulong arg3, ulong arg4, ulong* arg5);

    // ?CreateStateBlock@AgeDevice@@UAGJW4_D3DSTATEBLOCKTYPE@@PAK@Z
    ARTS_IMPORT virtual ilong CreateStateBlock(_D3DSTATEBLOCKTYPE arg1, ulong* arg2);

    // ?DeleteStateBlock@AgeDevice@@UAGJK@Z
    ARTS_IMPORT virtual ilong DeleteStateBlock(ulong arg1);

    // ?DrawIndexedPrimitive@AgeDevice@@UAGJW4_D3DPRIMITIVETYPE@@KPAXKPAGKK@Z
    ARTS_IMPORT virtual ilong DrawIndexedPrimitive(_D3DPRIMITIVETYPE arg1, ulong arg2, void* arg3, ulong arg4, u16* arg5, ulong arg6, ulong arg7);

    // ?DrawIndexedPrimitiveStrided@AgeDevice@@UAGJW4_D3DPRIMITIVETYPE@@KPAU_D3DDRAWPRIMITIVESTRIDEDDATA@@KPAGKK@Z
    ARTS_IMPORT virtual ilong DrawIndexedPrimitiveStrided(_D3DPRIMITIVETYPE arg1, ulong arg2, _D3DDRAWPRIMITIVESTRIDEDDATA* arg3, ulong arg4, u16* arg5, ulong arg6, ulong arg7);

    // ?DrawIndexedPrimitiveVB@AgeDevice@@UAGJW4_D3DPRIMITIVETYPE@@PAUIDirect3DVertexBuffer7@@KKPAGKK@Z
    ARTS_IMPORT virtual ilong DrawIndexedPrimitiveVB(_D3DPRIMITIVETYPE arg1, IDirect3DVertexBuffer7* arg2, ulong arg3, ulong arg4, u16* arg5, ulong arg6, ulong arg7);

    // ?DrawPrimitive@AgeDevice@@UAGJW4_D3DPRIMITIVETYPE@@KPAXKK@Z
    ARTS_IMPORT virtual ilong DrawPrimitive(_D3DPRIMITIVETYPE arg1, ulong arg2, void* arg3, ulong arg4, ulong arg5);

    // ?DrawPrimitiveStrided@AgeDevice@@UAGJW4_D3DPRIMITIVETYPE@@KPAU_D3DDRAWPRIMITIVESTRIDEDDATA@@KK@Z
    ARTS_IMPORT virtual ilong DrawPrimitiveStrided(_D3DPRIMITIVETYPE arg1, ulong arg2, _D3DDRAWPRIMITIVESTRIDEDDATA* arg3, ulong arg4, ulong arg5);

    // ?DrawPrimitiveVB@AgeDevice@@UAGJW4_D3DPRIMITIVETYPE@@PAUIDirect3DVertexBuffer7@@KKK@Z
    ARTS_IMPORT virtual ilong DrawPrimitiveVB(_D3DPRIMITIVETYPE arg1, IDirect3DVertexBuffer7* arg2, ulong arg3, ulong arg4, ulong arg5);

    // ?EndScene@AgeDevice@@UAGJXZ
    ARTS_IMPORT virtual ilong EndScene();

    // ?EndStateBlock@AgeDevice@@UAGJPAK@Z
    ARTS_IMPORT virtual ilong EndStateBlock(ulong* arg1);

    // ?EnumTextureFormats@AgeDevice@@UAGJP6GJPAU_DDPIXELFORMAT@@PAX@Z1@Z
    ARTS_IMPORT virtual ilong EnumTextureFormats(ilong (__stdcall* arg1)(_DDPIXELFORMAT*,void*), void* arg2);

    // ?GetCaps@AgeDevice@@UAGJPAU_D3DDeviceDesc7@@@Z
    ARTS_IMPORT virtual ilong GetCaps(_D3DDeviceDesc7* arg1);

    // ?GetClipPlane@AgeDevice@@UAGJKPAM@Z
    ARTS_IMPORT virtual ilong GetClipPlane(ulong arg1, f32* arg2);

    // ?GetClipStatus@AgeDevice@@UAGJPAU_D3DCLIPSTATUS@@@Z
    ARTS_IMPORT virtual ilong GetClipStatus(_D3DCLIPSTATUS* arg1);

    // ?GetDirect3D@AgeDevice@@UAGJPAPAUIDirect3D7@@@Z
    ARTS_IMPORT virtual ilong GetDirect3D(IDirect3D7** arg1);

    // ?GetInfo@AgeDevice@@UAGJKPAXK@Z
    ARTS_IMPORT virtual ilong GetInfo(ulong arg1, void* arg2, ulong arg3);

    // ?GetLight@AgeDevice@@UAGJKPAU_D3DLIGHT7@@@Z
    ARTS_IMPORT virtual ilong GetLight(ulong arg1, _D3DLIGHT7* arg2);

    // ?GetLightEnable@AgeDevice@@UAGJKPAH@Z
    ARTS_IMPORT virtual ilong GetLightEnable(ulong arg1, i32* arg2);

    // ?GetMaterial@AgeDevice@@UAGJPAU_D3DMATERIAL7@@@Z
    ARTS_IMPORT virtual ilong GetMaterial(_D3DMATERIAL7* arg1);

    // ?GetRenderState@AgeDevice@@UAGJW4_D3DRENDERSTATETYPE@@PAK@Z
    ARTS_IMPORT virtual ilong GetRenderState(_D3DRENDERSTATETYPE arg1, ulong* arg2);

    // ?GetRenderTarget@AgeDevice@@UAGJPAPAUIDirectDrawSurface7@@@Z
    ARTS_IMPORT virtual ilong GetRenderTarget(IDirectDrawSurface7** arg1);

    // ?GetTexture@AgeDevice@@UAGJKPAPAUIDirectDrawSurface7@@@Z
    ARTS_IMPORT virtual ilong GetTexture(ulong arg1, IDirectDrawSurface7** arg2);

    // ?GetTextureStageState@AgeDevice@@UAGJKW4_D3DTEXTURESTAGESTATETYPE@@PAK@Z
    ARTS_IMPORT virtual ilong GetTextureStageState(ulong arg1, _D3DTEXTURESTAGESTATETYPE arg2, ulong* arg3);

    // ?GetTransform@AgeDevice@@UAGJW4_D3DTRANSFORMSTATETYPE@@PAU_D3DMATRIX@@@Z
    ARTS_IMPORT virtual ilong GetTransform(_D3DTRANSFORMSTATETYPE arg1, _D3DMATRIX* arg2);

    // ?GetViewport@AgeDevice@@UAGJPAU_D3DVIEWPORT7@@@Z
    ARTS_IMPORT virtual ilong GetViewport(_D3DVIEWPORT7* arg1);

    // ?Initialize@AgeDevice@@UAGJPAUIDirectDrawSurface7@@@Z
    ARTS_IMPORT virtual ilong Initialize(IDirectDrawSurface7* arg1);

    // ?LightEnable@AgeDevice@@UAGJKH@Z
    ARTS_IMPORT virtual ilong LightEnable(ulong arg1, i32 arg2);

    // ?Load@AgeDevice@@UAGJPAUIDirectDrawSurface7@@PAUtagPOINT@@0PAUtagRECT@@K@Z
    ARTS_IMPORT virtual ilong Load(IDirectDrawSurface7* arg1, tagPOINT* arg2, IDirectDrawSurface7* arg3, tagRECT* arg4, ulong arg5);

    // ?MultiplyTransform@AgeDevice@@UAGJW4_D3DTRANSFORMSTATETYPE@@PAU_D3DMATRIX@@@Z
    ARTS_IMPORT virtual ilong MultiplyTransform(_D3DTRANSFORMSTATETYPE arg1, _D3DMATRIX* arg2);

    // ?PreLoad@AgeDevice@@UAGJPAUIDirectDrawSurface7@@@Z
    ARTS_IMPORT virtual ilong PreLoad(IDirectDrawSurface7* arg1);

    // ?QueryInterface@AgeDevice@@UAGJABU_GUID@@PAPAX@Z
    ARTS_IMPORT virtual ilong QueryInterface(const _GUID& arg1, void** arg2);

    // ?Release@AgeDevice@@UAGKXZ
    ARTS_IMPORT virtual ulong Release();

    // ?SetClipPlane@AgeDevice@@UAGJKPAM@Z
    ARTS_IMPORT virtual ilong SetClipPlane(ulong arg1, f32* arg2);

    // ?SetClipStatus@AgeDevice@@UAGJPAU_D3DCLIPSTATUS@@@Z
    ARTS_IMPORT virtual ilong SetClipStatus(_D3DCLIPSTATUS* arg1);

    // ?SetLight@AgeDevice@@UAGJKPAU_D3DLIGHT7@@@Z
    ARTS_IMPORT virtual ilong SetLight(ulong arg1, _D3DLIGHT7* arg2);

    // ?SetMaterial@AgeDevice@@UAGJPAU_D3DMATERIAL7@@@Z
    ARTS_IMPORT virtual ilong SetMaterial(_D3DMATERIAL7* arg1);

    // ?SetRenderState@AgeDevice@@UAGJW4_D3DRENDERSTATETYPE@@K@Z
    ARTS_IMPORT virtual ilong SetRenderState(_D3DRENDERSTATETYPE arg1, ulong arg2);

    // ?SetRenderTarget@AgeDevice@@UAGJPAUIDirectDrawSurface7@@K@Z
    ARTS_IMPORT virtual ilong SetRenderTarget(IDirectDrawSurface7* arg1, ulong arg2);

    // ?SetTexture@AgeDevice@@UAGJKPAUIDirectDrawSurface7@@@Z
    ARTS_IMPORT virtual ilong SetTexture(ulong arg1, IDirectDrawSurface7* arg2);

    // ?SetTextureStageState@AgeDevice@@UAGJKW4_D3DTEXTURESTAGESTATETYPE@@K@Z
    ARTS_IMPORT virtual ilong SetTextureStageState(ulong arg1, _D3DTEXTURESTAGESTATETYPE arg2, ulong arg3);

    // ?SetTransform@AgeDevice@@UAGJW4_D3DTRANSFORMSTATETYPE@@PAU_D3DMATRIX@@@Z
    ARTS_IMPORT virtual ilong SetTransform(_D3DTRANSFORMSTATETYPE arg1, _D3DMATRIX* arg2);

    // ?SetViewport@AgeDevice@@UAGJPAU_D3DVIEWPORT7@@@Z
    ARTS_IMPORT virtual ilong SetViewport(_D3DVIEWPORT7* arg1);

    // ?ValidateDevice@AgeDevice@@UAGJPAK@Z
    ARTS_IMPORT virtual ilong ValidateDevice(ulong* arg1);
};

// ??_7AgeDevice@@6B@
// vtable at 0x005B2A34
// check_size(AgeDevice, 0x0); // TODO: no layout in the IDB type library
