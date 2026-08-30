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

class audManager;

class audFileSystem
{
public:
    // ?Create@audFileSystem@@QAEPAV1@PAVaudManager@@@Z
    ARTS_IMPORT audFileSystem* Create(audManager* arg1);

    // ?Destroy@audFileSystem@@QAEXXZ
    ARTS_IMPORT void Destroy();

    // ?FileClose@audFileSystem@@QAEXXZ
    ARTS_IMPORT void FileClose();

    // ?FileOpenRead@audFileSystem@@QAE_NPAD0_N@Z
    ARTS_IMPORT bool FileOpenRead(char* arg1, char* arg2, bool arg3);

    // ?FileRead@audFileSystem@@QAEHPAXH@Z
    ARTS_IMPORT i32 FileRead(void* arg1, i32 arg2);

    // ?FileSeek@audFileSystem@@QAEXHH@Z
    ARTS_IMPORT void FileSeek(i32 arg1, i32 arg2);

    // ?GetFileHandle@audFileSystem@@QAEPAXXZ
    ARTS_IMPORT void* GetFileHandle();

    // ?GetFileSize@audFileSystem@@QAEHXZ
    ARTS_IMPORT i32 GetFileSize();

    // ?GetName@audFileSystem@@QAEPADXZ
    ARTS_IMPORT char* GetName();

    // ?GetPath@audFileSystem@@QAEPADXZ
    ARTS_IMPORT char* GetPath();

    // ?Init@audFileSystem@@QAEXXZ
    ARTS_IMPORT void Init();

    // ?IsOpen@audFileSystem@@QAE_NXZ
    ARTS_IMPORT bool IsOpen();

    // ?IsPersistent@audFileSystem@@QAE_NXZ
    ARTS_IMPORT bool IsPersistent();
};

// check_size(audFileSystem, 0x0); // TODO: no layout in the IDB type library
