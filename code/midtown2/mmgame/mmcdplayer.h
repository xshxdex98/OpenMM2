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

#include "arts7/asnode.h"

class mmHUD;

class mmCDPlayer : public asNode
{
public:
    // ??0mmCDPlayer@@QAE@XZ
    ARTS_IMPORT mmCDPlayer();

    // ??1mmCDPlayer@@UAE@XZ
    ARTS_IMPORT virtual ~mmCDPlayer();

    // ?Cull@mmCDPlayer@@UAEXXZ
    ARTS_IMPORT virtual void Cull();

    // ?Update@mmCDPlayer@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@mmCDPlayer@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Init@mmCDPlayer@@QAEXPAVmmHUD@@@Z
    ARTS_IMPORT void Init(mmHUD* node);

    // ?NextTrack@mmCDPlayer@@QAEXXZ
    ARTS_IMPORT void NextTrack();

    // ?PlayStop@mmCDPlayer@@QAEXXZ
    ARTS_IMPORT void PlayStop();

    // ?PrevTrack@mmCDPlayer@@QAEXXZ
    ARTS_IMPORT void PrevTrack();

    // ?Toggle@mmCDPlayer@@QAEXXZ
    ARTS_IMPORT void Toggle();
};

// ??_7mmCDPlayer@@6B@
// vtable at 0x005B0E6C
// check_size(mmCDPlayer, 0x154); // size known, members are not - cannot verify
