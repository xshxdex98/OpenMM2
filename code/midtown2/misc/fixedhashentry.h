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

class HashEntry;

class FixedHashEntry
{
public:
    // ??1FixedHashEntry@@QAE@XZ
    ARTS_IMPORT ~FixedHashEntry();

private:
    // ??0FixedHashEntry@@AAE@XZ
    ARTS_IMPORT FixedHashEntry();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    HashEntry field_0; // 0x000
    u8 IsFixed; // 0x00C
};

check_size(FixedHashEntry, 0x10);
