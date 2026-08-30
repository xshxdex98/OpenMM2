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

class Stream;

class modPackage
{
public:
    // ??0modPackage@@QAE@XZ
    ARTS_IMPORT modPackage();

    // ??1modPackage@@QAE@XZ
    ARTS_IMPORT ~modPackage();

    // ?Close@modPackage@@QAEXXZ
    ARTS_IMPORT void Close();

    // ?CloseFile@modPackage@@QAEXXZ
    ARTS_IMPORT void CloseFile();

    // ?Open@modPackage@@QAE_NPBD0@Z
    ARTS_IMPORT bool Open(const char* arg1, const char* arg2);

    // ?OpenFile@modPackage@@QAEPAVStream@@PBD@Z
    ARTS_IMPORT Stream* OpenFile(const char* arg1);

    // ?Skip@modPackage@@QAEXXZ
    ARTS_IMPORT void Skip();

    // ?SkipTo@modPackage@@QAEXPBD@Z
    ARTS_IMPORT void SkipTo(const char* arg1);

private:
    // ?NextItem@modPackage@@AAEXXZ
    ARTS_IMPORT void NextItem();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    modPackage* Next; // 0x000
    Stream* pPackageStream; // 0x004
    i8 CurrentKey[32]; // 0x008
    u32 Magic; // 0x028
    u32 CurrentValueSize; // 0x02C
};

check_size(modPackage, 0x30);
