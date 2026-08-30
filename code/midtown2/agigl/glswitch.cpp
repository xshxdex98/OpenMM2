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

#include "glswitch.h"

#include "data7/datargparser.h"
#include "data7/printer.h"

define_dummy_symbol(agigl_glswitch);

// THIS TRANSLATION UNIT DEFINES NO GAME SYMBOL, ON PURPOSE.
//
// tools/build.py quarantines any object that defines a mangled game symbol which data/ported.json
// does not list, and moves it to build/pending/. That is the right behaviour for the port itself,
// but the -gl predicate has to be linked in even while every port that uses it is still held back:
// otherwise turning on the first registration would also be the first time this code was ever
// compiled into the image. Keeping it in its own object with no game symbol means it links from
// the day it is written.

// -1 not yet decided, 0 off, 1 on.
static i32 agiGLSwitchState = -1;

bool agiGLEnabled()
{
#ifndef ARTS_AGIGL
    // No device layer in this build. Answering anything but false would send a caller into code
    // that was compiled out, and every one of those callers is on the boot path.
    return false;
#else
    if (agiGLSwitchState >= 0)
        return agiGLSwitchState != 0;

    // THE ORDERING HAZARD, AND IT IS NOT HYPOTHETICAL.
    //
    // datArgParser::Get self-initialises: `if (!datArgParser::Argc) datArgParser::Init();` - and
    // the no-argument Init() at 0x004C5BB0 is a single `ret`. It does not read a command line. So
    // before Main() has called datArgParser::Init(argc, argv) at 0x004C5BC0, the hash table is
    // empty and EVERY Get returns false, silently and without an error.
    //
    // A latched false from that window would be permanent and would look exactly like "the user
    // did not pass -gl". Refusing to latch until Argc is non-zero costs one integer compare per
    // call in the window and removes the failure mode entirely.
    if (datArgParser::Argc == 0)
        return false;

    // ?Get@datArgParser@@SA_NPBD@Z - the presence-only overload. The kit calls this one Exists
    // (0x004C6190) and its whole body is `HashTable::Access(&ArgHash, key) != 0`, so a bare -gl
    // with no value after it is exactly what it tests for.
    //
    // datArgParser::Init strips the leading '-' before inserting, which is why the key here is
    // "gl" and not "-gl"; every other flag in the game is spelled the same way (see the string
    // table at ClassName+444: "ref", "blade", "sw", "window", "width" ...).
    agiGLSwitchState = datArgParser::Get("gl") ? 1 : 0;

    if (agiGLSwitchState)
        Displayf("GL: -gl given, the OpenGL device layer is active");

    return agiGLSwitchState != 0;
#endif
}

void agiGLSetEnabled(bool enabled)
{
#ifndef ARTS_AGIGL
    if (enabled)
        Errorf("GL: -gl requested but this build has no agigl - staying on DirectDraw");

    agiGLSwitchState = 0;
#else
    agiGLSwitchState = enabled ? 1 : 0;
#endif
}
