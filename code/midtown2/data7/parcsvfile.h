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

class parCsvFile
{
public:
    // ?GetColumn@parCsvFile@@QBEHPBD@Z
    ARTS_IMPORT i32 GetColumn(const char* arg1) const;

    // ?GetFloat@parCsvFile@@QBEMHH@Z
    ARTS_IMPORT f32 GetFloat(i32 arg1, i32 arg2) const;

    // ?GetInt@parCsvFile@@QBEHHH@Z
    ARTS_IMPORT i32 GetInt(i32 arg1, i32 arg2) const;

    // ?GetRow@parCsvFile@@QBEHHPBD@Z
    ARTS_IMPORT i32 GetRow(i32 arg1, const char* arg2) const;

    // ?GetRowSafe@parCsvFile@@QBEHHPBD@Z
    ARTS_IMPORT i32 GetRowSafe(i32 arg1, const char* arg2) const;

    // ?Kill@parCsvFile@@QAEXXZ
    ARTS_IMPORT void Kill();

    // ?Load@parCsvFile@@QAE_NPBD0_NH@Z
    ARTS_IMPORT bool Load(const char* prefix, const char* path, bool arg3, i32 arg4);

    // ?Print@parCsvFile@@QBEXXZ
    ARTS_IMPORT void Print() const;

    // ?SetFloat@parCsvFile@@QAEXHHM@Z
    ARTS_IMPORT void SetFloat(i32 arg1, i32 arg2, f32 arg3);

    // ?SetInt@parCsvFile@@QAEXHHH@Z
    ARTS_IMPORT void SetInt(i32 arg1, i32 arg2, i32 arg3);

    // ?SetString@parCsvFile@@QAEXHHPBD@Z
    ARTS_IMPORT void SetString(i32 arg1, i32 arg2, const char* arg3);
};

// check_size(parCsvFile, 0x0); // TODO: no layout in the IDB type library
