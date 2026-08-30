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

#include "ph/phmaterialmgr.h"

class datAsciiTokenizer;
class lvlMaterial;
class phMaterial;

class lvlMaterialMgr : public phMaterialMgr
{
public:
    // ?Load@lvlMaterialMgr@@UAEPAVphMaterial@@PAVdatAsciiTokenizer@@@Z
    ARTS_IMPORT virtual phMaterial* Load(datAsciiTokenizer* arg1);

    // ?CreateInstance@lvlMaterialMgr@@SAAAV1@XZ
    ARTS_IMPORT static lvlMaterialMgr& CreateInstance();

    // ?DeleteInstance@lvlMaterialMgr@@SAXXZ
    ARTS_IMPORT static void DeleteInstance();

    // ?Find@lvlMaterialMgr@@QAEPAVlvlMaterial@@PBD@Z
    ARTS_IMPORT lvlMaterial* Find(const char* arg1);

    // ?GetInstance@lvlMaterialMgr@@SAAAV1@XZ
    ARTS_IMPORT static lvlMaterialMgr& GetInstance();

    // ?Lookup@lvlMaterialMgr@@QAEPAVlvlMaterial@@H@Z
    ARTS_IMPORT lvlMaterial* Lookup(i32 arg1);

    // ?Instance@lvlMaterialMgr@@3PAV1@A
    ARTS_IMPORT static lvlMaterialMgr* Instance;

protected:
    // ??0lvlMaterialMgr@@IAE@XZ
    ARTS_IMPORT lvlMaterialMgr();
};

// ??_7lvlMaterialMgr@@6B@
// vtable at 0x005B1B04
// check_size(lvlMaterialMgr, 0x1C); // size known, members are not - cannot verify
