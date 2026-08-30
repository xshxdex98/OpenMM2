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

    // ?FileIO@camPreCS@@UAEXAAVdatParser@@@Z
    ARTS_IMPORT virtual void FileIO(datParser& arg1);

    // ?GetClassName@camPreCS@@UAEPADXZ
    ARTS_IMPORT virtual char* GetClassName();

    // ?Init@camPreCS@@QAEXPAVvehCar@@@Z
    ARTS_IMPORT void Init(vehCar* arg1);

    // ?MakeActive@camPreCS@@UAEXXZ
    ARTS_IMPORT virtual void MakeActive();

    // ?Reset@camPreCS@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Update@camPreCS@@UAEXXZ
    ARTS_IMPORT virtual void Update();
};

// ??_7camPreCS@@6B@
// vtable at 0x005B4F94
// check_size(camPreCS, 0x0); // TODO: no layout in the IDB type library
