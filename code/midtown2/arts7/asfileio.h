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

#include "misc/base.h"

class datParser;

class asFileIO : public Base
{
public:
    // ??0asFileIO@@QAE@XZ
    ARTS_IMPORT asFileIO();

    // ??1asFileIO@@UAE@XZ
    ARTS_IMPORT virtual ~asFileIO();

    // ?SetName@asFileIO@@QAEXPBD@Z
    ARTS_IMPORT void SetName(const char* arg1);

protected:
    // ?FileIO@asFileIO@@MAEXAAVdatParser@@@Z
    ARTS_EXPORT virtual void FileIO(datParser& arg1);

    // ?AfterLoad@asFileIO@@MAEXXZ
    ARTS_EXPORT virtual void AfterLoad();

    // ?BeforeSave@asFileIO@@MAEXXZ
    ARTS_EXPORT virtual void BeforeSave();

    // ?Save@asFileIO@@MAE_NXZ
    ARTS_IMPORT virtual bool Save();

    // ?Load@asFileIO@@MAE_NXZ
    ARTS_IMPORT virtual bool Load();

    // ?GetDirName@asFileIO@@MAEPBDXZ
    ARTS_IMPORT virtual const char* GetDirName();

    // ?GetClassName@asFileIO@@MAEPBDXZ
    ARTS_IMPORT virtual const char* GetClassName();

public:
    // Members from 0x004; everything below that belongs to Base.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    i8* Name; // 0x004
};

// ??_7asFileIO@@6B@
// vtable at 0x005B871C
check_size(asFileIO, 0x8);
