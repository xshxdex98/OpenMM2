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

#include "vector7/matrix34.h"
#include "vector7/vector3.h"

#include "camcarcs.h"

class Base_vtbl;
class asNode;
class camViewCS;
class datParser;
class vehCar;

class camPovCS : public camCarCS
{
public:
    // ??0camPovCS@@QAE@XZ
    ARTS_IMPORT camPovCS();

    // ??1camPovCS@@UAE@XZ
    ARTS_IMPORT virtual ~camPovCS();

    // ?Update@camPovCS@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@camPovCS@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?FileIO@camPovCS@@UAEXAAVdatParser@@@Z
    ARTS_IMPORT virtual void FileIO(datParser& arg1);

    // ?AfterLoad@camPovCS@@UAEXXZ
    ARTS_EXPORT virtual void AfterLoad();

    // ?GetClassName@camPovCS@@UAEPADXZ
    ARTS_IMPORT virtual char* GetClassName();

    // ?MakeActive@camPovCS@@UAEXXZ
    ARTS_EXPORT virtual void MakeActive();

    // ?UpdateInput@camPovCS@@UAEXXZ
    ARTS_EXPORT virtual void UpdateInput();

private:
    // ?UpdatePOV@camPovCS@@AAEXXZ
    ARTS_IMPORT void UpdatePOV();

public:
    // Members from 0x110; everything below that belongs to camCarCS.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i32 unk_110; // 0x110
    Vector3 Offset; // 0x114
    Vector3 ReverseOffset; // 0x120
    f32 POVJitterAmp; // 0x12C
    Vector3 unk_130; // 0x130
    f32 Angle; // 0x13C
    f32 Pitch; // 0x140
    f32 Yaw; // 0x144
};

// ??_7camPovCS@@6B@
// vtable at 0x005B4E80
check_size(camPovCS, 0x148);
