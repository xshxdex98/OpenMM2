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

class internal_state;

class z_stream
{
public:

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8* next_in; // 0x000
    u32 avail_in; // 0x004
    u32 total_in; // 0x008
    u8* next_out; // 0x00C
    u32 avail_out; // 0x010
    u32 total_out; // 0x014
    i8* msg; // 0x018
    internal_state* state; // 0x01C
    alloc_func zalloc; // 0x020
    free_func zfree; // 0x024
    void* opaque; // 0x028
    i32 data_type; // 0x02C
    u32 adler; // 0x030
    u32 reserved; // 0x034
};

check_size(z_stream, 0x38);
