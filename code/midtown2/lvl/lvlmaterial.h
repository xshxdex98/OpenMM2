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

#include "ph/phmaterial.h"

class datAsciiTokenizer;

class lvlMaterial : public phMaterial
{
public:
    // ??0lvlMaterial@@QAE@XZ
    ARTS_IMPORT lvlMaterial();

    // ?Copy@lvlMaterial@@QAEXABV1@@Z
    ARTS_IMPORT void Copy(const lvlMaterial& arg1);

    // ?Load@lvlMaterial@@QAEXAAVdatAsciiTokenizer@@@Z
    ARTS_IMPORT void Load(datAsciiTokenizer& arg1);

    // ?Save@lvlMaterial@@UAEXAAVdatAsciiTokenizer@@@Z
    ARTS_IMPORT virtual void Save(datAsciiTokenizer& arg1);

public:
    // Members from 0x030; everything below that belongs to phMaterial.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i32 field_30; // 0x030
    i32 field_34; // 0x034
    i32 field_38; // 0x038
    i32 field_3C; // 0x03C
    u16 field_40; // 0x040
    u16 field_42; // 0x042
    f32 field_44; // 0x044
    i32 field_48; // 0x048
};

// ??_7lvlMaterial@@6B@
// vtable at 0x005B1AF8
check_size(lvlMaterial, 0x4C);
