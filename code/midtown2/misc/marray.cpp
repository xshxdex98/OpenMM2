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

#include "marray.h"

#include "data7/datassetmanager.h"
#include "data7/printer.h"
#include "stream/stream.h"
#include "ui/uiscale.h"
#include "vector7/vector4.h"

define_dummy_symbol(misc_marray);

// Declared by hand rather than included; see the same block in misc/warray.cpp for why.
extern "C" i32 __cdecl atoi(const char* str);
extern "C" char* __cdecl strtok(char* str, const char* delimiters);

// ?fgets@@YAHPADHPAVStream@@@Z - the engine's own line reader at 0x004C9F80, which takes a
// Stream* and not a FILE*. Declared here rather than in the generated stream/stream.h, which a
// run of tools/genheaders.py would overwrite. NOT taken from <cstdio> either: that declares the
// CRT one with _ACRTIMP and the call becomes __imp__fgets, which -NODEFAULTLIB cannot resolve.
ARTS_IMPORT i32 fgets(char* arg1, i32 arg2, Stream* arg3);

// Transcribed from the disassembly of ?Read@MArray@@QAEXPAD@Z at 0x004F0F40. The same function as
// WArray::Read one column narrower - menu.csv carries a BACKGROUND field and no per-widget id -
// and it has the same cancelling-divisor bug, fixed the same way. misc/warray.cpp carries the
// full explanation of why the original's division by the live screen size is a no-op.
//
// This is the more visible half of the two. MArray holds the menu FRAMES, so before the fix every
// menu box is 640x480 pixels in the top-left corner at any resolution, with its widgets pinned
// inside it.

// ?Read@MArray@@QAEXPAD@Z
void MArray::Read(char* file_name)
{
    Stream* stream = datAssetManager::Open("tune", file_name, "csv", false, true);

    if (!stream)
    {
        Errorf("Can't open menu data file '%s'", file_name);

        return;
    }

    char buffer[128];

    fgets(buffer, 128, stream);
    strtok(buffer, ",\r\n");

    // MENU NAME,BACKGROUND,ID,X,Y,W,H
    while (fgets(buffer, 128, stream))
    {
        strtok(buffer, ",");
        strtok(nullptr, ",");
        i32 menu_id = atoi(strtok(nullptr, ","));

        // Rotated exactly as WArray::Read stores it - the X column into `w`, the rest down one.
        // RetrieveMenuData returns them in the same rotated order.
        Vector4 position;

        position.w = uiScale::NormX(static_cast<f32>(atoi(strtok(nullptr, ","))));
        position.x = uiScale::NormY(static_cast<f32>(atoi(strtok(nullptr, ","))));
        position.y = uiScale::NormW(static_cast<f32>(atoi(strtok(nullptr, ","))));
        position.z = uiScale::NormH(static_cast<f32>(atoi(strtok(nullptr, "\r\n"))));

        // The original passes a null name here, not the menu name it just tokenised.
        // AddMenuData takes the argument and never stores it.
        AddMenuData(menu_id, position, nullptr);
    }

    stream->Close();
}
