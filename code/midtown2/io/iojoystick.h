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

struct DIDEVICEINSTANCEA;
struct DIDEVICEOBJECTINSTANCEA;
struct GUID;

class ioJoystick
{
public:
    // ?BeginAll@ioJoystick@@SAXXZ
    ARTS_IMPORT static void BeginAll();

    // ?EndAll@ioJoystick@@SAXXZ
    ARTS_IMPORT static void EndAll();

    // ?PollAll@ioJoystick@@SAXXZ
    ARTS_IMPORT static void PollAll();

    // ?UpdateAll@ioJoystick@@SAXXZ
    ARTS_IMPORT static void UpdateAll();

public:
    static ioJoystick*& Access_STICKS() { return STICKS; }
    static i32& Access_StickCount() { return StickCount; }

private:
    // ?Begin@ioJoystick@@AAEXXZ
    ARTS_IMPORT void Begin();

    // ?End@ioJoystick@@AAEXXZ
    ARTS_IMPORT void End();

    // ?EnumDeviceProc@ioJoystick@@CGHPBUDIDEVICEINSTANCEA@@PAX@Z
    ARTS_IMPORT static i32 EnumDeviceProc(const DIDEVICEINSTANCEA* diDevInstance, void* lpContext);

    // ?EnumObjectProc@ioJoystick@@CGHPBUDIDEVICEOBJECTINSTANCEA@@PAX@Z
    ARTS_IMPORT static i32 EnumObjectProc(const DIDEVICEOBJECTINSTANCEA* arg1, void* arg2);

    // ?Poll@ioJoystick@@AAEXXZ
    ARTS_IMPORT void Poll();

    // ?Update@ioJoystick@@AAEXXZ
    ARTS_IMPORT void Update();

    // ?STICKS@ioJoystick@@0PAV1@A
    ARTS_IMPORT static ioJoystick* STICKS;

    // ?StickCount@ioJoystick@@0HA
    ARTS_IMPORT static i32 StickCount;

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i32 field_0; // 0x000
    i32 field_4; // 0x004
    i32 field_8; // 0x008
    i32 field_C; // 0x00C
    i32 field_10; // 0x010
    i32 field_14; // 0x014
    i32 field_18; // 0x018
    i32 field_1C; // 0x01C
    i32 field_20; // 0x020
    i32 field_24; // 0x024
    i32 field_28; // 0x028
    i32 field_2C; // 0x02C
    i32 field_30; // 0x030
    GUID GUID; // 0x034
    i32 field_44; // 0x044
    i32 field_48; // 0x048
    f32 field_0; // 0x04C
};

check_size(ioJoystick, 0x50);
