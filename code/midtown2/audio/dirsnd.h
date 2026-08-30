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

struct IUnknown;
struct _DSBUFFERDESC;
struct _GUID;
class mmDirSndVTable;
class mmSoundDriver;
struct tag_dsdevdesc;

class DirSnd
{
public:
    // ??0DirSnd@@QAE@XZ
    ARTS_IMPORT DirSnd();

    // ??1DirSnd@@QAE@XZ
    ARTS_IMPORT ~DirSnd();

    // ?InitPrimarySoundBuffer@DirSnd@@UAEHKEPAD@Z
    ARTS_IMPORT virtual i32 InitPrimarySoundBuffer(ulong sampleRate, u8 arg2, char* lpString2);

    // ?ClearDSDeviceList@DirSnd@@QAEXXZ
    ARTS_IMPORT void ClearDSDeviceList();

    // ?DeInit@DirSnd@@QAEXXZ
    ARTS_IMPORT void DeInit();

    // ?EnumDSDevices@DirSnd@@QAEHXZ
    ARTS_IMPORT i32 EnumDSDevices();

    // ?GetDeviceNames@DirSnd@@QAEPAPADXZ
    ARTS_IMPORT char** GetDeviceNames();

    // ?GetNum3DHallBufs@DirSnd@@QAEIXZ
    ARTS_IMPORT u32 GetNum3DHallBufs();

    // ?Init@DirSnd@@SAPAV1@KE_NPAD@Z
    ARTS_IMPORT static DirSnd* Init(ulong samplesPerSec, u8 allowStero, bool enable3D, char* deviceName);

    // ?Init3DListener@DirSnd@@QAEHMMMMM@Z
    ARTS_IMPORT i32 Init3DListener(f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5);

    // ?IsDSDeviceInList@DirSnd@@QAEHPAU_GUID@@@Z
    ARTS_IMPORT i32 IsDSDeviceInList(_GUID* guid);

    // ?ScanAvailableDSFormats@DirSnd@@QAEXXZ
    ARTS_IMPORT void ScanAvailableDSFormats();

    // ?SetBitDepth@DirSnd@@QAEXH@Z
    ARTS_IMPORT void SetBitDepth(i32 bitDepth);

    // ?SetDeviceRating@DirSnd@@QAEXPAUtag_dsdevdesc@@@Z
    ARTS_IMPORT void SetDeviceRating(tag_dsdevdesc* deviceEntry);

    // ?SetPrimaryBufferFormat@DirSnd@@QAEXKE@Z
    ARTS_IMPORT void SetPrimaryBufferFormat(ulong sampleRate, u8 allowStero);

    // ?TranslateDSError@DirSnd@@QAEPADJ@Z
    ARTS_IMPORT char* TranslateDSError(ilong hResult);

protected:
    // ?CreatePrimaryInterfaceAndBuffer@DirSnd@@IAEHPAU_DSBUFFERDESC@@@Z
    ARTS_IMPORT i32 CreatePrimaryInterfaceAndBuffer(_DSBUFFERDESC* dsBufferDesc);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    mmDirSndVTable* lpVTbl; // 0x000
    LPDIRECTSOUNDBUFFER lpDSBuffer; // 0x004
    LPDIRECTSOUND lpDS; // 0x008
    HWND hWnd; // 0x00C
    i32 field_10; // 0x010
    mmSoundDriver* FirstDriver; // 0x014
    mmSoundDriver* CurrentDriver; // 0x018
    IUnknown* field_1C; // 0x01C
    i32 DeviceCount; // 0x020
    i32 BitDepth; // 0x024
    i32 DeviceFlags; // 0x028
    u8 Enable3D; // 0x02C
    u8 Initialized; // 0x02D
    i32 SoundEnabled; // 0x030
    i32 DeviceCaps; // 0x034
    mmSoundDriver** SoundDevices; // 0x038
};

// ??_7DirSnd@@6B@
// vtable at 0x005B8A1C
check_size(DirSnd, 0x3C);
