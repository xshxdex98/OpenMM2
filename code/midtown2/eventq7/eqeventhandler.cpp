/*
    OpenMM2 - An Open Source Re-Implementation of Midtown Madness 2

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

// eqEventHandler is the Windows-side event pump. Six of its entry points do nothing in the retail
// build: the graphics begin/end pair, the app activation callbacks, and the minimise hook.
//
// MM2 drives device loss and window activation from pcwindis rather than from the event queue, so
// these were reduced to a `ret` rather than removed - the addresses are still referenced.

#include "eqeventhandler.h"

// ?BeginGfx@eqEventHandler@@QAEHHHH@Z - 0x004A1730
//
// Zero is success here, not failure: the caller treats a non-zero return as a device error.
i32 eqEventHandler::BeginGfx(i32 arg1, i32 arg2, i32 arg3)
{
    (void) arg1;
    (void) arg2;
    (void) arg3;

    return 0;
}

// ?EndGfx@eqEventHandler@@QAEXXZ - 0x004A1740
void eqEventHandler::EndGfx()
{}

// ?RestoreApp@eqEventHandler@@QAEXXZ - 0x004A1750
//
// Restoring from minimised. pcwindis re-creates the device, so the queue has nothing to do.
void eqEventHandler::RestoreApp()
{}

// ?MinimizeApp@eqEventHandler@@QAEXH@Z - 0x004A1760
void eqEventHandler::MinimizeApp(i32 arg1)
{
    (void) arg1;
}

// ?SetActive@eqEventHandler@@QAEXH@Z - 0x004A1770
void eqEventHandler::SetActive(i32 arg1)
{
    (void) arg1;
}

// ?ClearActive@eqEventHandler@@QAEXH@Z - 0x004A1780
void eqEventHandler::ClearActive(i32 arg1)
{
    (void) arg1;
}

define_dummy_symbol(eventq7_eqeventhandler);
