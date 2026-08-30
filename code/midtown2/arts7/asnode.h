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

#include "ascullable.h"

class datParser;

class asNode : public asCullable
{
public:
    // ??0asNode@@QAE@XZ
    ARTS_IMPORT asNode();

    // ??1asNode@@UAE@XZ
    ARTS_IMPORT virtual ~asNode();

    // ?Update@asNode@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@asNode@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?ResChange@asNode@@UAEXHH@Z
    ARTS_IMPORT virtual void ResChange(i32 arg1, i32 arg2);

    // ?UpdatePaused@asNode@@UAEXXZ
    ARTS_IMPORT virtual void UpdatePaused();

    // ?FileIO@asNode@@UAEXAAVdatParser@@@Z
    ARTS_IMPORT virtual void FileIO(datParser& arg1);

    // ?AfterLoad@asNode@@UAEXXZ
    ARTS_IMPORT virtual void AfterLoad();

    // ?BeforeSave@asNode@@UAEXXZ
    ARTS_IMPORT virtual void BeforeSave();

    // ?Save@asNode@@UAE_NXZ
    ARTS_IMPORT virtual bool Save();

    // ?Load@asNode@@UAE_NXZ
    ARTS_IMPORT virtual bool Load();

    // ?GetClassName@asNode@@UAEPADXZ
    ARTS_IMPORT virtual char* GetClassName();

    // ?GetClassNameA@asNode@@UAEPADXZ
    ARTS_IMPORT virtual char* GetClassNameA();

    // ?GetDirName@asNode@@UAEPBDXZ
    ARTS_IMPORT virtual const char* GetDirName();

    // ?AddChild@asNode@@QAEHPAV1@@Z
    ARTS_IMPORT i32 AddChild(asNode* arg1);

    // ?GetChild@asNode@@QAEPAV1@H@Z
    ARTS_IMPORT asNode* GetChild(i32 arg1);

    // ?GetLastChild@asNode@@QAEPAV1@XZ
    ARTS_IMPORT asNode* GetLastChild();

    // ?GetNext@asNode@@QAEPAV1@XZ
    ARTS_IMPORT asNode* GetNext();

    // ?InsertChild@asNode@@QAEHHPAV1@@Z
    ARTS_IMPORT i32 InsertChild(i32 arg1, asNode* arg2);

    // ?NumChildren@asNode@@QAEHXZ
    ARTS_IMPORT i32 NumChildren();

    // ?RemoveAllChildren@asNode@@QAEXXZ
    ARTS_IMPORT void RemoveAllChildren();

    // ?RemoveChild@asNode@@QAEHH@Z
    ARTS_IMPORT i32 RemoveChild(i32 arg1);

    // ?RemoveChild@asNode@@QAEHPAV1@@Z
    ARTS_IMPORT i32 RemoveChild(asNode* arg1);

    // ?SetName@asNode@@QAEXPBD@Z
    ARTS_IMPORT void SetName(const char* arg1);

    // ?SwitchTo@asNode@@QAEXH@Z
    ARTS_IMPORT void SwitchTo(i32 arg1);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    asNode* Next; // 0x004
    asNode* Children; // 0x008
    asNode* Parent; // 0x00C
    u32 Flags; // 0x010
    i8* pName; // 0x014
};

// ??_7asNode@@6B@
// vtable at 0x005B28E8
check_size(asNode, 0x18);
