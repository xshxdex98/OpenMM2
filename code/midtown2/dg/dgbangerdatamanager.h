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

#include "dgbangerdata.h"

class gfxTexture;

class dgBangerDataManager : public asNode
{
public:
    // ??0dgBangerDataManager@@QAE@XZ
    ARTS_IMPORT dgBangerDataManager();

    // ??1dgBangerDataManager@@UAE@XZ
    ARTS_IMPORT virtual ~dgBangerDataManager();

    // ?GetClassName@dgBangerDataManager@@UAEPADXZ
    ARTS_IMPORT virtual char* GetClassName();

    // ?AddBangerDataEntry@dgBangerDataManager@@QAEHPBD0@Z
    ARTS_IMPORT i32 AddBangerDataEntry(const char* name, const char* arg2);

    // ?ChangeData@dgBangerDataManager@@QAEXXZ
    ARTS_IMPORT void ChangeData();

public:
    static dgBangerDataManager*& Access_Instance() { return Instance; }

protected:
    // ?Instance@dgBangerDataManager@@1PAV1@A
    ARTS_IMPORT static dgBangerDataManager* Instance;

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i32 BangerCount; // 0x018
    dgBangerData Bangers[512]; // 0x01C
    gfxTexture* ParticleTextures[20]; // 0x2A81C
    asNode GlassBirthRules; // 0x2A86C
    f32 field_2A884; // 0x2A884
    f32 field_2A888; // 0x2A888
    f32 field_2A88C; // 0x2A88C
    f32 field_2A890; // 0x2A890
    f32 field_2A894; // 0x2A894
    f32 field_2A898; // 0x2A898
    f32 field_2A89C; // 0x2A89C
    f32 field_2A8A0; // 0x2A8A0
    f32 field_2A8A4; // 0x2A8A4
    u8 field_2A8A8; // 0x2A8A8
    u32 field_2A8AC; // 0x2A8AC
    f32 field_2A8B0; // 0x2A8B0
    f32 field_2A8B4; // 0x2A8B4
    f32 field_2A8B8; // 0x2A8B8
    f32 field_2A8BC; // 0x2A8BC
    f32 field_2A8C0; // 0x2A8C0
};

// ??_7dgBangerDataManager@@6B@
// vtable at 0x005B13F8
check_size(dgBangerDataManager, 0x2A8C4);
