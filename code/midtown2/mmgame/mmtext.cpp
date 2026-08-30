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

#include "mmtext.h"

#include "ui/uiscale.h"

define_dummy_symbol(mmgame_mmtext);

// `_strtok` and `_atoi` from the game's own 1999 CRT, and `?StringDuplicate@@YAPADPBD@Z` at
// 0x004C8180 - the engine's strdup, which allocates through operator new. Declared here rather
// than included; see misc/miscfuncs.cpp for why <cstdlib> and <cstring> must stay out.
extern "C" i32 __cdecl atoi(const char* str);
extern "C" char* __cdecl strtok(char* str, const char* delimiters);

// ?StringDuplicate@@YAPADPBD@Z
ARTS_IMPORT char* StringDuplicate(const char* arg1);

// CreateFontA, reached through its IAT slot rather than through a declaration.
//
// tools/implibs.py builds the import libraries from midtown2.exe's own import table, which names
// every import UNDECORATED - build/implibs/GDI32.lib carries `__imp__CreateFontA` and
// `_CreateFontA`, not `__imp__CreateFontA@56`. So neither obvious declaration works: a
// __declspec(dllimport) __stdcall one asks for the decorated name and does not find it, and a
// __cdecl one finds the thunk but then both sides pop the arguments and the stack unwinds wrong.
//
// Declaring the slot itself as what it is - a pointer to a __stdcall entry point - gets both
// halves right, and is exactly how the original reaches it (`call dword ptr [0x5B0040]` at
// 0x0053225B). The C identifier is spelled with ONE leading underscore because MSVC prepends
// another to every extern "C" name on x86; `_imp__CreateFontA` here is `__imp__CreateFontA` in
// the object file.
extern "C" {
extern void*(__stdcall* _imp__CreateFontA)(i32 height, i32 width, i32 escapement, i32 orientation,
    i32 weight, u32 italic, u32 underline, u32 strike_out, u32 char_set, u32 out_precision,
    u32 clip_precision, u32 quality, u32 pitch_and_family, const char* face_name);
}

// PROOF_QUALITY and the (DEFAULT_PITCH | FF_DONTCARE) that follows it. Both are literal 2s in the
// original and are spelled out here so the argument list can be read against the API.
#define MM_PROOF_QUALITY 2
#define MM_DEFAULT_PITCH 2

// Read off the disassembly of ?CreateLocFont@mmText@@SAPAXPBULocString@@H@Z at 0x005321C0. The
// function is absent from MM2_RE_KIT/MM2_PSEUDOCODE/mmText/ - the kit filed it under an ImPlot
// name in _MISLABELED_FUNCTIONS.c, and that transcription lost the branch this function exists
// for, showing both size fields assigned to the same variable in sequence.
//
// A LocString is a flat char[512] holding one comma-separated row:
//
//     <face name>,<height at 320>,<height at 640>,<charset>,<weight>
//
// and arg2 is the screen WIDTH, which every caller passes as gfxPipeline::Access_m_iWidth().
//
// THE CHANGE. The original picks one of the two authored heights and uses it as an absolute pixel
// count: `cmp ecx, 0x280; jl ...` - below 640 wide take the first, otherwise take the second, and
// nothing scales after that. So at 2560x1440 the menu text is rendered at the size it was drawn
// for a 640x480 screen and comes out a third of the height it should be.
//
// The fix is the game's own: mmCRHUD already scales its number fonts linearly off the vertical
// resolution with a floor at 12 pixels, `20 * m_iHeight / 480`, at 0x004389xx and five other
// sites. uiScale::FontHeight is that expression. The 640 entry becomes the DESIGN height rather
// than a breakpoint, and the 320 entry is kept for the one case scaling cannot serve - a screen
// narrower than the design space, where the artists' small-screen face is a better answer than a
// shrunken large-screen one.
//
// At exactly 640x480 this returns the same font the original returns.

// ?CreateLocFont@mmText@@SAPAXPBULocString@@H@Z
void* mmText::CreateLocFont(const LocString* arg1, i32 arg2)
{
    // StringDuplicate because strtok writes into its argument and the LocString is the loaded
    // localisation resource, which must not be modified.
    char* row = StringDuplicate(reinterpret_cast<const char*>(arg1));

    const char* face_name = strtok(row, ",");
    i32 small_height = atoi(strtok(nullptr, ","));
    i32 design_height = atoi(strtok(nullptr, ","));
    i32 char_set = atoi(strtok(nullptr, ","));
    i32 weight = atoi(strtok(nullptr, "\r\n"));

    i32 height = (arg2 < static_cast<i32>(UI_DESIGN_WIDTH)) ? small_height
                                                            : uiScale::FontHeight(design_height);

    void* font = _imp__CreateFontA(height, 0, 0, 0, weight, 0, 0, 0, static_cast<u32>(char_set), 0,
        0, MM_PROOF_QUALITY, MM_DEFAULT_PITCH, face_name);

    operator delete(row);

    return font;
}
