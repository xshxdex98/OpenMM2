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

#include "vehicleselectbase.h"

class Vehicle : public VehicleSelectBase
{
public:
    // ??0Vehicle@@QAE@H@Z
    ARTS_IMPORT Vehicle(i32 arg1);

    // ??1Vehicle@@UAE@XZ
    ARTS_IMPORT virtual ~Vehicle();

    // ?PreSetup@Vehicle@@UAEXXZ
    ARTS_IMPORT virtual void PreSetup();

    // ?PostSetup@Vehicle@@UAEXXZ
    ARTS_IMPORT virtual void PostSetup();

    // ?SetSubMenu@Vehicle@@QAEXH@Z
    ARTS_IMPORT void SetSubMenu(i32 arg1);

    // ?SetSubMenuButtons@Vehicle@@QAEXXZ
    ARTS_IMPORT void SetSubMenuButtons();
};

// ??_7Vehicle@@6B@
// vtable at 0x005B453C
// check_size(Vehicle, 0x178); // size known, members are not - cannot verify
