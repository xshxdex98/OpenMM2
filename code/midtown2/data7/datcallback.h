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

class Base;

class datCallback
{
public:
    // ??0datCallback@@QAE@P6AXPAX0@Z0@Z
    ARTS_IMPORT datCallback(void (__cdecl* arg1)(void*,void*), void* arg2);

    // ??0datCallback@@QAE@P6AXPAX@Z0@Z
    ARTS_IMPORT datCallback(void (__cdecl* arg1)(void*), void* arg2);

    // ??0datCallback@@QAE@P6AXXZ@Z
    ARTS_IMPORT datCallback(void (__cdecl* arg1)(void));

    // ??0datCallback@@QAE@P8Base@@AEXPAX0@ZPAV1@0@Z
    ARTS_IMPORT datCallback(void (__thiscall Base::* arg1)(void*,void*), Base* arg2, void* arg3);

    // ??0datCallback@@QAE@P8Base@@AEXPAX@ZPAV1@0@Z
    ARTS_IMPORT datCallback(void (__thiscall Base::* arg1)(void*), Base* arg2, void* arg3);

    // ??0datCallback@@QAE@P8Base@@AEXXZPAV1@@Z
    ARTS_IMPORT datCallback(void (__thiscall Base::* arg1)(void), Base* arg2);

    // ??0datCallback@@QAE@XZ
    ARTS_IMPORT datCallback();

    // ?Call@datCallback@@QAEXPAX@Z
    ARTS_IMPORT void Call(void* arg1);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    void* Class; // 0x000
    void* Callback; // 0x004
    void* Parameter; // 0x008
};

check_size(datCallback, 0xC);
