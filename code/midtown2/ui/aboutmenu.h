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

class AboutMenu : public UIMenu
{
public:
    // ??0AboutMenu@@QAE@H@Z
    ARTS_IMPORT AboutMenu(i32 arg1);

    // ??1AboutMenu@@UAE@XZ
    ARTS_IMPORT virtual ~AboutMenu();

    // ?Cull@AboutMenu@@UAEXXZ
    ARTS_IMPORT virtual void Cull();

    // ?PreSetup@AboutMenu@@UAEXXZ
    ARTS_IMPORT virtual void PreSetup();

    // ?Update@AboutMenu@@UAEXXZ
    ARTS_IMPORT virtual void Update();
};

// ??_7AboutMenu@@6B@
// vtable at 0x005B4584
// check_size(AboutMenu, 0x0); // TODO: no layout in the IDB type library
