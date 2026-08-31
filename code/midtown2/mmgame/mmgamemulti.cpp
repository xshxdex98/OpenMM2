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

// mmGameMulti is the multiplayer game mode. These four are the parts of it that were left empty.
//
// NextRace and UpdateNetWorkAudio are the seams of a session that would have advanced through a
// series and mixed positional voice; MM2 ships one race per session and no voice. SendCarTuning
// would have replicated a peer's handling tune, which retail does not synchronise at all -
// everyone runs the tune from their own local files, and that is why a modified TUNE desyncs
// handling between players rather than being rejected.

#include "mmgamemulti.h"

// ?NextRace@mmGameMulti@@UAEXXZ - 0x00421B70
void mmGameMulti::NextRace()
{}

// ?UpdateDebugKeyInput@mmGameMulti@@UAEXH@Z - 0x0043ADC0
//
// The development build's debug keys, compiled out of retail.
void mmGameMulti::UpdateDebugKeyInput(i32 arg1)
{
    (void) arg1;
}

// ?SendCarTuning@mmGameMulti@@QAEXPBDK@Z - 0x0043B630
//
// Never sent. Handling comes from each client's own TUNE files, which is why mismatched car mods
// desync handling between players instead of being caught.
void mmGameMulti::SendCarTuning(const char* arg1, ulong arg2)
{
    (void) arg1;
    (void) arg2;
}

// ?UpdateNetWorkAudio@mmGameMulti@@QAEXXZ - 0x0043B7C0
void mmGameMulti::UpdateNetWorkAudio()
{}

define_dummy_symbol(mmgame_mmgamemulti);
