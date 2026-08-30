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

#include "cambasecs.h"

class datParser;
class vehCar;

class camPreCS : public camBaseCS
{
public:
    // ??0camPreCS@@QAE@XZ
    ARTS_IMPORT camPreCS();

    // ??1camPreCS@@UAE@XZ
    ARTS_IMPORT virtual ~camPreCS();

    // ?Update@camPreCS@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@camPreCS@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?FileIO@camPreCS@@UAEXAAVdatParser@@@Z
    ARTS_IMPORT virtual void FileIO(datParser& arg1);

    // ?GetClassName@camPreCS@@UAEPADXZ
    ARTS_IMPORT virtual char* GetClassName();

    // ?MakeActive@camPreCS@@UAEXXZ
    ARTS_IMPORT virtual void MakeActive();

    // ?Init@camPreCS@@QAEXPAVvehCar@@@Z
    ARTS_IMPORT void Init(vehCar* arg1);
};

// ??_7camPreCS@@6B@
// vtable at 0x005B4F94
// check_size(camPreCS, 0x90); // size known, members are not - cannot verify
