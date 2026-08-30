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

class ioMouse
{
public:
    // ?Begin@ioMouse@@SAX_N@Z
    ARTS_IMPORT static void Begin(bool arg1);

    // ?End@ioMouse@@SAXXZ
    ARTS_IMPORT static void End();

    // ?Update@ioMouse@@SAXXZ
    ARTS_IMPORT static void Update();

public:
    static i32& Access_m_dX() { return m_dX; }
    static bool& Access_m_UseWindow() { return m_UseWindow; }
    static f32& Access_m_InvHeight() { return m_InvHeight; }
    static i32& Access_m_Y() { return m_Y; }
    static i32& Access_m_X() { return m_X; }
    static u32& Access_m_Buttons() { return m_Buttons; }
    static u32& Access_m_LastButtons() { return m_LastButtons; }
    static i32& Access_m_dZ() { return m_dZ; }
    static f32& Access_m_InvWidth() { return m_InvWidth; }
    static i32& Access_m_dY() { return m_dY; }
    static u32& Access_m_WindowButtons() { return m_WindowButtons; }

private:
    // ?m_dX@ioMouse@@0HA
    ARTS_IMPORT static i32 m_dX;

    // ?m_UseWindow@ioMouse@@0_NA
    ARTS_IMPORT static bool m_UseWindow;

    // ?m_InvHeight@ioMouse@@0MA
    ARTS_IMPORT static f32 m_InvHeight;

    // ?m_Y@ioMouse@@0HA
    ARTS_IMPORT static i32 m_Y;

    // ?m_X@ioMouse@@0HA
    ARTS_IMPORT static i32 m_X;

    // ?m_Buttons@ioMouse@@0IA
    ARTS_IMPORT static u32 m_Buttons;

    // ?m_LastButtons@ioMouse@@0IA
    ARTS_IMPORT static u32 m_LastButtons;

    // ?m_dZ@ioMouse@@0HA
    ARTS_IMPORT static i32 m_dZ;

    // ?m_InvWidth@ioMouse@@0MA
    ARTS_IMPORT static f32 m_InvWidth;

    // ?m_dY@ioMouse@@0HA
    ARTS_IMPORT static i32 m_dY;

    // ?m_WindowButtons@ioMouse@@0IA
    ARTS_IMPORT static u32 m_WindowButtons;
};

// check_size(ioMouse, 0x1); // size known, members are not - cannot verify
