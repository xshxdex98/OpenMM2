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

class datRefCount
{
public:
    // ?DecRef@datRefCount@@QBEHXZ
    ARTS_IMPORT i32 DecRef() const;

    // ?IncRef@datRefCount@@QBEHXZ
    ARTS_IMPORT i32 IncRef() const;

public:
    // The destructor is protected in midtown2.exe; this reaches it without
    // changing its mangled name. It does not free the storage.
    void Access_Destroy() { this->~datRefCount(); }

protected:
    // ??0datRefCount@@IAE@XZ
    ARTS_IMPORT datRefCount();

    // ??1datRefCount@@MAE@XZ
    ARTS_IMPORT virtual ~datRefCount();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u32 RefCount; // 0x004
};

// ??_7datRefCount@@6B@
// vtable at 0x005B8740
check_size(datRefCount, 0x8);
