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

class aiPedestrianInstance;
class pedActive;
class pedRagdollMgr_vTable;
class phBoundBox;

class pedRagdollMgr : public asNode
{
public:
    // ??0pedRagdollMgr@@QAE@XZ
    ARTS_IMPORT pedRagdollMgr();

    // ??1pedRagdollMgr@@UAE@XZ
    ARTS_IMPORT virtual ~pedRagdollMgr();

    // ?Update@pedRagdollMgr@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@pedRagdollMgr@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Attach@pedRagdollMgr@@QAEPAVpedActive@@PAVaiPedestrianInstance@@@Z
    ARTS_IMPORT pedActive* Attach(aiPedestrianInstance* arg1);

    // ?Detach@pedRagdollMgr@@QAEXPAVaiPedestrianInstance@@@Z
    ARTS_IMPORT void Detach(aiPedestrianInstance* arg1);

    // ?Init@pedRagdollMgr@@QAEXHPAPAD@Z
    ARTS_IMPORT void Init(i32 arg1, char** arg2);

    // ?UnusedActive@pedRagdollMgr@@QAE_NXZ
    ARTS_IMPORT bool UnusedActive();

    // ?Instance@pedRagdollMgr@@2PAV1@A
    ARTS_IMPORT static pedRagdollMgr* Instance;

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u32 pActivePedsData; // 0x018
    u32 pActivePeds; // 0x01C
    void* field_20[4]; // 0x020
    phBoundBox* pBoundBox; // 0x030
};

// ??_7pedRagdollMgr@@6B@
// vtable at 0x005B635C
check_size(pedRagdollMgr, 0x34);
