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

#include "warray.h"

#include "data7/datassetmanager.h"
#include "data7/printer.h"
#include "stream/stream.h"
#include "ui/uiscale.h"
#include "vector7/vector4.h"

define_dummy_symbol(misc_warray);

// The two CRT entry points this file needs, declared by hand rather than included.
//
// <cstdlib> and <cstring> declare them with _ACRTIMP, which becomes __declspec(dllimport) and
// turns every call into a call to __imp__atoi / __imp__strtok. The link runs -NODEFAULTLIB
// against the game's own statically linked 1999 CRT (tools/link.py), where the only names that
// exist are the undecorated `_atoi` at 0x581743 and `_strtok` at 0x5825F1 already in game.asm.
// Same reasoning as the `toupper` declaration in misc/miscfuncs.cpp.
extern "C" i32 __cdecl atoi(const char* str);
extern "C" char* __cdecl strtok(char* str, const char* delimiters);

// ?fgets@@YAHPADHPAVStream@@@Z - the engine's own line reader at 0x004C9F80, which takes a
// Stream* and not a FILE*. Declared here rather than in the generated stream/stream.h, which a
// run of tools/genheaders.py would overwrite. NOT taken from <cstdio> either: that declares the
// CRT one with _ACRTIMP and the call becomes __imp__fgets, which -NODEFAULTLIB cannot resolve.
ARTS_IMPORT i32 fgets(char* arg1, i32 arg2, Stream* arg3);

// Transcribed from the disassembly of ?Read@WArray@@QAEXPAD@Z at 0x004E7840, with every helper
// resolved by address against data/symbols.json rather than taken from the dump's own labels.
// The Hex-Rays output in MM2_RE_KIT has the shape of this function right and four of the names in
// it wrong: datAssetManager::Open reads as ImPlot::PlotBarsH<int>, and every gfxPipeline::Access_m_iHeight()
// reads as an std::function constructor. See docs/ui_scaling.md.
//
// THE ONE CHANGE FROM THE ORIGINAL IS THE DIVISOR, and it is the single biggest visual break in
// MM2's UI. The original divides the authored pixel by gfxPipeline::Access_m_iWidth() / m_iHeight - the
// LIVE screen size - and the widget's Init multiplies the result back by that same live size a
// moment later. The two cancel exactly:
//
//     authored_px / m_iWidth * m_iWidth == authored_px
//
// so every widget in tune/widget.csv lands at its authored 640x480 pixel offset no matter what
// resolution the game is running at. At 2560x1440 the menus collapse into the top-left 640x480
// corner of the screen. Dividing by the size the coordinates were AUTHORED against instead is the
// whole fix, and that is all uiScale::NormX and friends do.

// ?Read@WArray@@QAEXPAD@Z
void WArray::Read(char* arg1)
{
    Stream* stream = datAssetManager::Open("tune", arg1, "csv", false, true);

    if (!stream)
    {
        Errorf("Can't open widget data file '%s'", arg1);

        return;
    }

    char buffer[128];

    // The header row, read and discarded. The original tokenises it too, which matters only
    // because strtok is stateful - the loop below starts a fresh string on every pass anyway.
    fgets(buffer, 128, stream);
    strtok(buffer, ",\r\n");

    // MENU NAME,ID,WIDGET NAME,ID,X,Y,W,H,DESC
    while (fgets(buffer, 128, stream))
    {
        strtok(buffer, ",");
        i32 menu_id = atoi(strtok(nullptr, ","));

        strtok(nullptr, ",");
        i32 widget_id = atoi(strtok(nullptr, ","));

        // The four columns go into the Vector4 ROTATED - the X column lands in `w` and each of
        // the others moves down one slot. That is not a transcription slip: the stores are to
        // [ebp-0Ch], [ebp-18h], [ebp-14h], [ebp-10h] against a Vector4 based at [ebp-18h], and
        // RetrieveWidgetData reads them back in the same order, so neither half may be
        // "corrected". Each statement is separately sequenced, so the strtok calls below stay in
        // column order.
        Vector4 pos;

        pos.w = uiScale::NormX(static_cast<f32>(atoi(strtok(nullptr, ","))));
        pos.x = uiScale::NormY(static_cast<f32>(atoi(strtok(nullptr, ","))));
        pos.y = uiScale::NormW(static_cast<f32>(atoi(strtok(nullptr, ","))));
        pos.z = uiScale::NormH(static_cast<f32>(atoi(strtok(nullptr, ","))));

        // Null on a row with no DESC column. AddWidgetData takes the pointer and stores only the
        // three fields above, but the argument is still passed.
        char* description = strtok(nullptr, "\r\n");

        AddWidgetData(menu_id, widget_id, pos, description);
    }

    stream->Close();
}
