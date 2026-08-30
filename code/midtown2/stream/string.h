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

class string
{
public:
    // ??0string@@QAE@ABV0@@Z
    ARTS_IMPORT string(const string& arg1);

    // ??0string@@QAE@PBD@Z
    ARTS_IMPORT string(const char* arg1);

    // ??0string@@QAE@XZ
    ARTS_IMPORT string();

    // ?ChangeExtension@string@@QAEXABV1@@Z
    ARTS_IMPORT void ChangeExtension(const string& arg1);

    // ?CommaFile@string@@QBEHXZ
    ARTS_IMPORT i32 CommaFile() const;

    // ?Contains@string@@QBEHAAV1@@Z
    ARTS_IMPORT i32 Contains(string& arg1) const;

    // ?DirFileExt@string@@QBEXAAV1@00@Z
    ARTS_IMPORT void DirFileExt(string& arg1, string& arg2, string& arg3) const;

    // ?ExpandEnvs@string@@QAEXXZ
    ARTS_IMPORT void ExpandEnvs();

    // ?Extension@string@@QBE?AV1@XZ
    ARTS_IMPORT string Extension() const;

    // ?FileExists@string@@QBEHXZ
    ARTS_IMPORT i32 FileExists() const;

    // ?FileName@string@@QBE?AV1@XZ
    ARTS_IMPORT string FileName() const;

    // ?FindFile@string@@QAEHABV1@H00@Z
    ARTS_IMPORT i32 FindFile(const string& arg0, i32 arg2, const string& arg3, const string& arg4);

    // ?Init@string@@QAEXH@Z
    ARTS_IMPORT void Init(i32 arg1);

    // ?IsNumeric@string@@QBEHXZ
    ARTS_IMPORT i32 IsNumeric() const;

    // ?MinusEqual@string@@QAEXD@Z
    ARTS_IMPORT void MinusEqual(char arg1);

    // ?NumSubStrings@string@@QBEHXZ
    ARTS_IMPORT i32 NumSubStrings() const;

    // ??Hstring@@QBE?AV0@ABV0@@Z
    ARTS_IMPORT string operator+(const string& arg1) const;

    // ??Hstring@@QBE?AV0@PBD@Z
    ARTS_IMPORT string operator+(const char* arg1) const;

    // ??Ystring@@QAEXD@Z
    ARTS_IMPORT void operator+=(char arg1);

    // ??Ystring@@QAEXPBD@Z
    ARTS_IMPORT void operator+=(const char* arg1);

    // ??Gstring@@QBE?AV0@PBD@Z
    ARTS_IMPORT string operator-(const char* arg1) const;

    // ??Zstring@@QAEXPBD@Z
    ARTS_IMPORT void operator-=(const char* arg1);

    // ??4string@@QAEXPBD@Z
    ARTS_IMPORT void operator=(const char* arg1);

    // ?RemoveExtension@string@@QAEXXZ
    ARTS_IMPORT void RemoveExtension();

    // ?SaveName@string@@QAEXABV1@H00@Z
    ARTS_IMPORT void SaveName(const string& arg0, i32 arg2, const string& arg3, const string& arg4);

    // ?SubString@string@@QBE?AV1@H@Z
    ARTS_IMPORT string SubString(i32 arg1) const;

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i8* pBuffer; // 0x000
    i32 nBufferSize; // 0x004
};

check_size(string, 0x8);
