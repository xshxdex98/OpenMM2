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

enum mmCRGameClass : i32;

class Matrix34;
class Vector4;

class mmCRHUD : public asNode
{
public:
    // ??0mmCRHUD@@QAE@XZ
    ARTS_IMPORT mmCRHUD();

    // ??1mmCRHUD@@UAE@XZ
    ARTS_IMPORT virtual ~mmCRHUD();

    // ?Update@mmCRHUD@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@mmCRHUD@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?PostUpdate@mmCRHUD@@UAEXXZ
    ARTS_IMPORT virtual void PostUpdate();

    // ?ActivateGold@mmCRHUD@@QAEXXZ
    ARTS_IMPORT void ActivateGold();

    // ?ActivateRosterGold@mmCRHUD@@QAEXK@Z
    ARTS_IMPORT void ActivateRosterGold(ulong arg1);

    // ?AddPlayer@mmCRHUD@@QAEXPBDKHI@Z
    ARTS_IMPORT void AddPlayer(const char* shared_font_atlas, ulong arg4, i32 arg3, u32 arg4);

    // ?DeactivateGold@mmCRHUD@@QAEXXZ
    ARTS_IMPORT void DeactivateGold();

    // ?DeactivateRosterGold@mmCRHUD@@QAEXXZ
    ARTS_IMPORT void DeactivateRosterGold();

    // ?Init@mmCRHUD@@QAEXPAVMatrix34@@PADW4mmCRGameClass@@@Z
    ARTS_IMPORT void Init(Matrix34* shared_data, char* arg2, mmCRGameClass x);

    // ?RemovePlayer@mmCRHUD@@QAEXK@Z
    ARTS_IMPORT void RemovePlayer(ulong arg1);

    // ?SetBlueScore@mmCRHUD@@QAEXH@Z
    ARTS_IMPORT void SetBlueScore(i32 arg1);

    // ?SetName@mmCRHUD@@QAEXPADH@Z
    ARTS_IMPORT void SetName(char* arg1, i32 arg4);

    // ?SetRedScore@mmCRHUD@@QAEXH@Z
    ARTS_IMPORT void SetRedScore(i32 arg1);

    // ?SetScore@mmCRHUD@@QAEXKH@Z
    ARTS_IMPORT void SetScore(ulong arg1, i32 arg2);

    // ?SetScore@mmCRHUD@@QAEXPAD@Z
    ARTS_IMPORT void SetScore(char* arg1);

    // ?ToggleScores@mmCRHUD@@QAEXXZ
    ARTS_IMPORT void ToggleScores();

    // ?UnPackColor@mmCRHUD@@QAEXIAAVVector4@@@Z
    ARTS_IMPORT void UnPackColor(u32 arg1, Vector4& arg2);

    // ?UpdateGold@mmCRHUD@@QAEXXZ
    ARTS_IMPORT void UpdateGold();
};

// ??_7mmCRHUD@@6B@
// vtable at 0x005B1020
// check_size(mmCRHUD, 0x0); // TODO: no layout in the IDB type library
