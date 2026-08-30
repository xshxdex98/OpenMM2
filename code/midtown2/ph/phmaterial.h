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

class Stream;
class datAsciiTokenizer;

class phMaterial
{
public:
    // ??0phMaterial@@QAE@XZ
    ARTS_IMPORT phMaterial();

    // ?Copy@phMaterial@@UAEXABV1@@Z
    ARTS_IMPORT virtual void Copy(const phMaterial& arg1);

    // ?Load@phMaterial@@QAEXAAVdatAsciiTokenizer@@@Z
    ARTS_IMPORT void Load(datAsciiTokenizer& arg1);

    // ?LoadBinary@phMaterial@@QAEXPAVStream@@@Z
    ARTS_IMPORT void LoadBinary(Stream* arg1);

    // ?Save@phMaterial@@UAEXAAVdatAsciiTokenizer@@@Z
    ARTS_IMPORT virtual void Save(datAsciiTokenizer& arg1);

    // ?SaveBinary@phMaterial@@UAEXPAVStream@@@Z
    ARTS_IMPORT virtual void SaveBinary(Stream* arg1);

    // ?SetName@phMaterial@@QAEXPBD@Z
    ARTS_IMPORT void SetName(const char* arg1);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i8 Name[32]; // 0x004
    i16 field_24; // 0x024
    i16 field_26; // 0x026
    f32 field_28; // 0x028
    f32 field_2C; // 0x02C
};

// ??_7phMaterial@@6B@
// vtable at 0x005B2404
check_size(phMaterial, 0x30);
