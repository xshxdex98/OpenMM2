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

class CLoader;
struct IDirectMusicLoader;
struct IStream;
struct _GUID;
union _LARGE_INTEGER;
union _ULARGE_INTEGER;
struct tagSTATSTG;

class CFileStream
{
public:
    // ??0CFileStream@@QAE@PAVCLoader@@@Z
    ARTS_IMPORT CFileStream(CLoader* arg1);

    // ??1CFileStream@@QAE@XZ
    ARTS_IMPORT ~CFileStream();

    // ?AddRef@CFileStream@@UAGKXZ
    ARTS_IMPORT virtual ulong AddRef();

    // ?Clone@CFileStream@@UAGJPAPAUIStream@@@Z
    ARTS_IMPORT virtual ilong Clone(IStream** arg1);

    // ?Close@CFileStream@@QAEJXZ
    ARTS_IMPORT ilong Close();

    // ?Commit@CFileStream@@UAGJK@Z
    ARTS_IMPORT virtual ilong Commit(ulong arg1);

    // ?CopyTo@CFileStream@@UAGJPAUIStream@@T_ULARGE_INTEGER@@PAT3@2@Z
    ARTS_IMPORT virtual ilong CopyTo(IStream* arg1, _ULARGE_INTEGER arg2, _ULARGE_INTEGER* arg3, _ULARGE_INTEGER* arg4);

    // ?GetLoader@CFileStream@@UAGJPAPAUIDirectMusicLoader@@@Z
    ARTS_IMPORT virtual ilong GetLoader(IDirectMusicLoader** arg1);

    // ?GetNextPtr@CFileStream@@QAEPAV1@XZ
    ARTS_IMPORT CFileStream* GetNextPtr();

    // ?LockRegion@CFileStream@@UAGJT_ULARGE_INTEGER@@0K@Z
    ARTS_IMPORT virtual ilong LockRegion(_ULARGE_INTEGER arg1, _ULARGE_INTEGER arg2, ulong arg3);

    // ?Open@CFileStream@@QAEJPAGK@Z
    ARTS_IMPORT ilong Open(u16* arg1, ulong arg2);

    // ?QueryInterface@CFileStream@@UAGJABU_GUID@@PAPAX@Z
    ARTS_IMPORT virtual ilong QueryInterface(const _GUID& arg1, void** arg2);

    // ?Read@CFileStream@@UAGJPAXKPAK@Z
    ARTS_IMPORT virtual ilong Read(void* arg1, ulong arg2, ulong* arg3);

    // ?Release@CFileStream@@UAGKXZ
    ARTS_IMPORT virtual ulong Release();

    // ?Revert@CFileStream@@UAGJXZ
    ARTS_IMPORT virtual ilong Revert();

    // ?Seek@CFileStream@@UAGJT_LARGE_INTEGER@@KPAT_ULARGE_INTEGER@@@Z
    ARTS_IMPORT virtual ilong Seek(_LARGE_INTEGER arg1, ulong arg2, _ULARGE_INTEGER* arg3);

    // ?SetNextPtr@CFileStream@@QAEXPAV1@@Z
    ARTS_IMPORT void SetNextPtr(CFileStream* arg1);

    // ?SetSize@CFileStream@@UAGJT_ULARGE_INTEGER@@@Z
    ARTS_IMPORT virtual ilong SetSize(_ULARGE_INTEGER arg1);

    // ?Stat@CFileStream@@UAGJPAUtagSTATSTG@@K@Z
    ARTS_IMPORT virtual ilong Stat(tagSTATSTG* arg1, ulong arg2);

    // ?UnlockRegion@CFileStream@@UAGJT_ULARGE_INTEGER@@0K@Z
    ARTS_IMPORT virtual ilong UnlockRegion(_ULARGE_INTEGER arg1, _ULARGE_INTEGER arg2, ulong arg3);

    // ?Write@CFileStream@@UAGJPBXKPAK@Z
    ARTS_IMPORT virtual ilong Write(const void* arg1, ulong arg2, ulong* arg3);
};

// ??_7CFileStream@@6BIDirectMusicGetLoader@@@
// vtable at 0x005B4D6C
// check_size(CFileStream, 0x0); // TODO: no layout in the IDB type library
