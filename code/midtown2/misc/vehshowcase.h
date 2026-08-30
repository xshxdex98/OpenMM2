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

#include "ui/uimenu.h"

class VehShowcase : public UIMenu
{
public:
    // ??0VehShowcase@@QAE@H@Z
    ARTS_IMPORT VehShowcase(i32 arg1);

    // ??1VehShowcase@@UAE@XZ
    ARTS_IMPORT virtual ~VehShowcase();

    // ?PreSetup@VehShowcase@@UAEXXZ
    ARTS_IMPORT virtual void PreSetup();

    // ?PostSetup@VehShowcase@@UAEXXZ
    ARTS_IMPORT virtual void PostSetup();
};

// ??_7VehShowcase@@6B@
// vtable at 0x005B44F4
// check_size(VehShowcase, 0x0); // TODO: no layout in the IDB type library
