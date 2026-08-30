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

#include "uimenu.h"

class OptionsMenu : public UIMenu
{
public:
    // ??0OptionsMenu@@QAE@H@Z
    ARTS_IMPORT OptionsMenu(i32 getter);

    // ??1OptionsMenu@@UAE@XZ
    ARTS_IMPORT virtual ~OptionsMenu();

    // ?PreSetup@OptionsMenu@@UAEXXZ
    ARTS_IMPORT virtual void PreSetup();

    // ?PostSetup@OptionsMenu@@UAEXXZ
    ARTS_IMPORT virtual void PostSetup();

    // ?FocusDescription@OptionsMenu@@QAEXHH@Z
    ARTS_IMPORT void FocusDescription(i32 arg1, i32 arg2);
};

// ??_7OptionsMenu@@6B@
// vtable at 0x005B434C
// check_size(OptionsMenu, 0x8C); // size known, members are not - cannot verify
