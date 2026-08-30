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

class datParser;
class vehCarSim;

class vehStuck : public asNode
{
public:
    // ??0vehStuck@@QAE@XZ
    ARTS_IMPORT vehStuck();

    // ??1vehStuck@@UAE@XZ
    ARTS_IMPORT virtual ~vehStuck();

    // ?FileIO@vehStuck@@UAEXAAVdatParser@@@Z
    ARTS_IMPORT virtual void FileIO(datParser& arg1);

    // ?GetClassName@vehStuck@@UAEPADXZ
    ARTS_IMPORT virtual char* GetClassName();

    // ?GetDirName@vehStuck@@UAEPBDXZ
    ARTS_IMPORT virtual const char* GetDirName();

    // ?Impact@vehStuck@@QAEXXZ
    ARTS_IMPORT void Impact();

    // ?Init@vehStuck@@QAEXPAVvehCarSim@@PBD@Z
    ARTS_IMPORT void Init(vehCarSim* arg1, const char* arg2);

    // ?Pegged@vehStuck@@QAEHXZ
    ARTS_IMPORT i32 Pegged();

    // ?Reset@vehStuck@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Update@vehStuck@@UAEXXZ
    ARTS_IMPORT virtual void Update();
};

// ??_7vehStuck@@6B@
// vtable at 0x005B2EB8
// check_size(vehStuck, 0x54); // size known, members are not - cannot verify
