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

#include "camcarcs.h"

class Matrix34;
class Vector3;
class vehCar;

class camAICS : public camCarCS
{
public:
    // ??0camAICS@@QAE@XZ
    ARTS_IMPORT camAICS();

    // ??1camAICS@@UAE@XZ
    ARTS_IMPORT virtual ~camAICS();

    // ?Init@camAICS@@QAEXPAVvehCar@@@Z
    ARTS_IMPORT void Init(vehCar* arg1);

    // ?MakeActive@camAICS@@UAEXXZ
    ARTS_IMPORT virtual void MakeActive();

    // ?Reset@camAICS@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?SetMatrix@camAICS@@QAEXPAVMatrix34@@@Z
    ARTS_IMPORT void SetMatrix(Matrix34* arg1);

    // ?SetPosition@camAICS@@QAEXAAVVector3@@@Z
    ARTS_IMPORT void SetPosition(Vector3& arg1);

    // ?Update@camAICS@@UAEXXZ
    ARTS_IMPORT virtual void Update();
};

// ??_7camAICS@@6B@
// vtable at 0x005B501C
// check_size(camAICS, 0x110); // size known, members are not - cannot verify
