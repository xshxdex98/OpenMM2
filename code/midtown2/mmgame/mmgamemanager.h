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

class mmGameManager : public asNode
{
public:
    // ??0mmGameManager@@QAE@XZ
    ARTS_IMPORT mmGameManager();

    // ??1mmGameManager@@UAE@XZ
    ARTS_IMPORT virtual ~mmGameManager();

    // ?BeDone@mmGameManager@@QAEXXZ
    ARTS_IMPORT void BeDone();

    // ?Cull@mmGameManager@@UAEXXZ
    ARTS_IMPORT virtual void Cull();

    // ?ForcePopupUI@mmGameManager@@QAEXXZ
    ARTS_IMPORT void ForcePopupUI();

    // ?ForceReplayUI@mmGameManager@@QAEXXZ
    ARTS_IMPORT void ForceReplayUI();

    // ?Reset@mmGameManager@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Update@mmGameManager@@UAEXXZ
    ARTS_IMPORT virtual void Update();

public:
    static mmGameManager*& Access_Instance() { return Instance; }

private:
    // ?Instance@mmGameManager@@0PAV1@A
    ARTS_IMPORT static mmGameManager* Instance;
};

// ??_7mmGameManager@@6B@
// vtable at 0x005B0368
// check_size(mmGameManager, 0x1B8); // size known, members are not - cannot verify
