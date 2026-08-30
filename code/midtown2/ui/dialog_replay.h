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

#include "data7/datcallback.h"
#include "stream/string.h"

#include "pumenubase.h"

class mmTextNode;
class uiWidget;

class Dialog_Replay : public PUMenuBase
{
public:
    // ??0Dialog_Replay@@QAE@HMMMMPAD@Z
    ARTS_IMPORT Dialog_Replay(i32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, char* arg6);

    // ??1Dialog_Replay@@UAE@XZ
    ARTS_IMPORT virtual ~Dialog_Replay();

    // ?PreSetup@Dialog_Replay@@UAEXXZ
    ARTS_IMPORT virtual void PreSetup();

    // ?PostSetup@Dialog_Replay@@UAEXXZ
    ARTS_IMPORT virtual void PostSetup();

    // ?CancelCB@Dialog_Replay@@QAEXXZ
    ARTS_IMPORT void CancelCB();

    // ?DeleteCB@Dialog_Replay@@QAEXXZ
    ARTS_IMPORT void DeleteCB();

    // ?DoneCB@Dialog_Replay@@QAEXXZ
    ARTS_IMPORT void DoneCB();

    // ?EditCB@Dialog_Replay@@QAEXXZ
    ARTS_IMPORT void EditCB();

    // ?GetDescription@Dialog_Replay@@QAEPADXZ
    ARTS_IMPORT char* GetDescription();

    // ?GetSelectedReplay@Dialog_Replay@@QAEPADXZ
    ARTS_IMPORT char* GetSelectedReplay();

    // ?LoadAll@Dialog_Replay@@QAEXXZ
    ARTS_IMPORT void LoadAll();

    // ?ScrollCB@Dialog_Replay@@QAEXXZ
    ARTS_IMPORT void ScrollCB();

    // ?SetCurrentReplay@Dialog_Replay@@QAEXH@Z
    ARTS_IMPORT void SetCurrentReplay(i32 arg1);

    // ?SetDescription@Dialog_Replay@@QAEXPAD@Z
    ARTS_IMPORT void SetDescription(char* arg1);

    // ?SetDriverStats@Dialog_Replay@@QAEXPAD00@Z
    ARTS_IMPORT void SetDriverStats(char* arg1, char* arg2, char* arg3);

public:
    // Members from 0x0BC; everything below that belongs to PUMenuBase.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i8* field_BC; // 0x0BC
    u8 field_C0[124]; // 0x0C0
    string field_80; // 0x13C
    void* field_144; // 0x144
    u8 field_148[4]; // 0x148
    datCallback field_14C; // 0x14C
    mmTextNode* field_158; // 0x158
    mmTextNode* field_15C; // 0x15C
    uiWidget* field_160; // 0x160
    u8 field_164[4]; // 0x164
};

// ??_7Dialog_Replay@@6B@
// vtable at 0x005B3EDC
check_size(Dialog_Replay, 0x168);
