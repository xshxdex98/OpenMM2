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

#include "dgbangeractive.h"

class Base_vtbl;
class dgBangerInstance;

class dgBangerActiveManager : public asNode
{
public:
    // ??0dgBangerActiveManager@@QAE@XZ
    ARTS_IMPORT dgBangerActiveManager();

    // ??1dgBangerActiveManager@@UAE@XZ
    ARTS_IMPORT virtual ~dgBangerActiveManager();

    // ?Update@dgBangerActiveManager@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@dgBangerActiveManager@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Attach@dgBangerActiveManager@@QAEPAVdgBangerActive@@PAVdgBangerInstance@@@Z
    ARTS_IMPORT dgBangerActive* Attach(dgBangerInstance* arg1);

    // ?Detach@dgBangerActiveManager@@QAEXPAVdgBangerActive@@@Z
    ARTS_IMPORT void Detach(dgBangerActive* arg1);

    // ?GetActive@dgBangerActiveManager@@QAEPAVdgBangerActive@@PAVdgBangerInstance@@@Z
    ARTS_IMPORT dgBangerActive* GetActive(dgBangerInstance* arg1);

public:
    static dgBangerActiveManager*& Access_Instance() { return Instance; }

protected:
    // ?Instance@dgBangerActiveManager@@1PAV1@A
    ARTS_IMPORT static dgBangerActiveManager* Instance;

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    dgBangerActive** ActiveInstances[32]; // 0x018
    dgBangerActive BangerPool[32]; // 0x098
    i32 NumActive; // 0x6398
};

// ??_7dgBangerActiveManager@@6B@
// vtable at 0x005B13C4
check_size(dgBangerActiveManager, 0x639C);
