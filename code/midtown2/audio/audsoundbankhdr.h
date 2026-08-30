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

struct _bank_attrib;

class audSoundBankHdr
{
public:
    // ?AddFromMemory@audSoundBankHdr@@QAEHPAXPAV1@@Z
    ARTS_IMPORT i32 AddFromMemory(void* arg1, audSoundBankHdr* arg2);

    // ?CreateEmptyBank@audSoundBankHdr@@QAEPAV1@PAPAX@Z
    ARTS_IMPORT audSoundBankHdr* CreateEmptyBank(void** arg1);

    // ?CreateFromFile@audSoundBankHdr@@QAEPAV1@PADPAPAX1@Z
    ARTS_IMPORT audSoundBankHdr* CreateFromFile(char* arg1, void** arg2, void** arg3);

    // ?CreateFromMemory@audSoundBankHdr@@QAEPAV1@PAXH0HPAU_bank_attrib@@HPAPAX@Z
    ARTS_IMPORT audSoundBankHdr* CreateFromMemory(void* arg1, i32 arg2, void* arg3, i32 arg4, _bank_attrib* arg5, i32 arg6, void** arg7);

    // ?Destroy@audSoundBankHdr@@QAEXXZ
    ARTS_IMPORT void Destroy();

    // ?GetBankHandle@audSoundBankHdr@@QAEHXZ
    ARTS_IMPORT i32 GetBankHandle();

    // ?GetNumberOfAttribs@audSoundBankHdr@@QAEHXZ
    ARTS_IMPORT i32 GetNumberOfAttribs();

    // ?GetSoundIndex@audSoundBankHdr@@QAEHPAD@Z
    ARTS_IMPORT i32 GetSoundIndex(char* arg1);

    // ?Init@audSoundBankHdr@@QAEXXZ
    ARTS_IMPORT void Init();
};

// check_size(audSoundBankHdr, 0x0); // TODO: no layout in the IDB type library
