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
class UIMenu;
class Vector4;
class WArrayEntry;

class WArray
{
public:
    // ??0WArray@@QAE@XZ
    ARTS_IMPORT WArray();

    // ??1WArray@@QAE@XZ
    ARTS_IMPORT ~WArray();

    // ?AddWidgetData@WArray@@QAEXHHAAVVector4@@PAD@Z
    ARTS_IMPORT void AddWidgetData(i32 menuID, i32 widgetID, Vector4& position, char* arg4);

    // ?DumpMenu@WArray@@QAEXPAVStream@@PAVUIMenu@@@Z
    ARTS_IMPORT void DumpMenu(Stream* arg1, UIMenu* arg2);

    // ?Flush@WArray@@QAEXXZ
    ARTS_IMPORT void Flush();

    // ?Init@WArray@@QAEXH@Z
    ARTS_IMPORT void Init(i32 arg1);

    // ?Read@WArray@@QAEXPAD@Z
    ARTS_EXPORT void Read(char* arg1);

    // ?RetrieveWidgetData@WArray@@QAEHHHAAM000@Z
    ARTS_IMPORT i32 RetrieveWidgetData(i32 arg1, i32 arg2, f32& arg3, f32& arg4, f32& arg5, f32& arg6);

    // ?Write@WArray@@QAEXPAD@Z
    ARTS_IMPORT void Write(char* arg1);

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    WArrayEntry* pValues; // 0x000
    u32 MaxSize; // 0x004
    u32 Size; // 0x008
};

check_size(WArray, 0xC);
