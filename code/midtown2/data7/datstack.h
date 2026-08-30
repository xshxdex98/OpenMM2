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

struct _EXCEPTION_POINTERS;
struct _iobuf;

class datStack
{
public:
    // ?DoTraceback@datStack@@SAXHPAHPAU_iobuf@@PBD@Z
    ARTS_IMPORT static void DoTraceback(i32 arg1, i32* arg2, _iobuf* arg3, const char* arg4);

    // ?DumpTraceback@datStack@@SAXPAHHPAU_iobuf@@@Z
    ARTS_IMPORT static void DumpTraceback(i32* arg1, i32 arg2, _iobuf* arg3);

    // ?ExceptionFilter@datStack@@SAHPAU_EXCEPTION_POINTERS@@@Z
    ARTS_IMPORT static i32 ExceptionFilter(_EXCEPTION_POINTERS* arg1);

    // ?GetTimestamp@datStack@@SAPBDXZ
    ARTS_IMPORT static const char* GetTimestamp();

    // ?LogTraceback@datStack@@SAHPAHH@Z
    ARTS_IMPORT static i32 LogTraceback(i32* arg1, i32 arg2);

    // ?LookupAddress@datStack@@SAXPADH@Z
    ARTS_IMPORT static void LookupAddress(char* arg1, i32 arg2);

    // ?Traceback@datStack@@SAXHPAU_iobuf@@@Z
    ARTS_IMPORT static void Traceback(i32 arg1, _iobuf* arg2);
};

// check_size(datStack, 0x1); // size known, members are not - cannot verify
