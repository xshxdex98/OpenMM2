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

#include "arts7/ascamera.h"
#include "arts7/asnode.h"

class Base_vtbl;
class mmGame;
class mmMultiBlitz;
class mmMultiCR;
class mmMultiCircuit;
class mmMultiRace;
class mmMultiRoam;
class mmSingleBlitz;
class mmSingleCircuit;
class mmSingleRace;
class mmSingleRoam;
class mmSingleStunt;

class mmGameManager : public asNode
{
public:
    // ??0mmGameManager@@QAE@XZ
    ARTS_IMPORT mmGameManager();

    // ??1mmGameManager@@UAE@XZ
    ARTS_IMPORT virtual ~mmGameManager();

    // ?Cull@mmGameManager@@UAEXXZ
    ARTS_IMPORT virtual void Cull();

    // ?Update@mmGameManager@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@mmGameManager@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?BeDone@mmGameManager@@QAEXXZ
    ARTS_IMPORT void BeDone();

    // ?ForcePopupUI@mmGameManager@@QAEXXZ
    ARTS_IMPORT void ForcePopupUI();

    // ?ForceReplayUI@mmGameManager@@QAEXXZ
    ARTS_IMPORT void ForceReplayUI();

public:
    static mmGameManager*& Access_Instance() { return Instance; }

private:
    // ?Instance@mmGameManager@@0PAV1@A
    ARTS_IMPORT static mmGameManager* Instance;

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    asCamera m_Camera; // 0x018
    mmGame* m_CurrentGame; // 0x188
    mmSingleRoam* m_SingleRoamGame; // 0x18C
    mmSingleRace* m_SingleRaceGame; // 0x190
    mmSingleStunt* m_SingleStuntGame; // 0x194
    mmSingleCircuit* m_SingleCircuitGame; // 0x198
    mmSingleBlitz* m_SingleBlitzGame; // 0x19C
    mmMultiRace* m_MultiRaceGame; // 0x1A0
    mmMultiRoam* m_MultiRoamGame; // 0x1A4
    mmMultiCR* m_MultiCRGame; // 0x1A8
    mmMultiCircuit* m_MultiCircuitGame; // 0x1AC
    mmMultiBlitz* m_MultiBlitzGame; // 0x1B0
    i32 NumUpdateCallsSinceReset; // 0x1B4
};

// ??_7mmGameManager@@6B@
// vtable at 0x005B0368
check_size(mmGameManager, 0x1B8);
