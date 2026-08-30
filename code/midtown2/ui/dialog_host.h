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

#include "pumenubase.h"

class UITextField;

class Dialog_Host : public PUMenuBase
{
public:
    // ??0Dialog_Host@@QAE@HMMMMPAD@Z
    ARTS_IMPORT Dialog_Host(i32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, char* arg6);

    // ??1Dialog_Host@@UAE@XZ
    ARTS_IMPORT virtual ~Dialog_Host();

    // ?Clear@Dialog_Host@@QAEXXZ
    ARTS_IMPORT void Clear();

    // ?PasswordCallback@Dialog_Host@@QAEXXZ
    ARTS_IMPORT void PasswordCallback();

    // ?PreSetup@Dialog_Host@@UAEXXZ
    ARTS_IMPORT virtual void PreSetup();

public:
    // Members from 0x0BC; everything below that belongs to PUMenuBase.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i8 SessionPassword[40]; // 0x0BC
    i8 PasswordFieldSessionPassword[40]; // 0x0E4
    UITextField* PasswordTextField; // 0x10C
    u8 IsPasswordSet[4]; // 0x110
    i32 MaxNumPlayers; // 0x114
    i32 TextRollerMaxNumPlayers; // 0x118
};

// ??_7Dialog_Host@@6B@
// vtable at 0x005B3F24
check_size(Dialog_Host, 0x11C);
