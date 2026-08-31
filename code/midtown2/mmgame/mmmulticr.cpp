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

// mmMultiCR is Cops and Robbers. Four of its entry points do nothing.
//
// SelectTeams returning 0 is the notable one: team assignment is decided by the host and sent to
// each client, so the client-side selection this would have run never happens locally.
//
// SwitchState is NOT ported. It writes the game state to offset 0x270, which falls inside a byte
// blob in mmMultiCR's recovered layout rather than a named field, so it waits on that layout.
// SendGoldDeliver is not a stub - it calls asNetwork::Send - and is left in assembly.

#include "mmmulticr.h"

// ?UpdateGameInput@mmMultiCR@@UAEXH@Z
//
// Cops and Robbers takes no mode-specific input beyond the normal driving controls.
void mmMultiCR::UpdateGameInput(i32 arg1)
{
    (void) arg1;
}

// ?GetWaypoints@mmMultiCR@@UAEPAVmmWaypoints@@XZ
//
// No waypoint set: the mode has no fixed route, unlike a circuit or checkpoint race.
mmWaypoints* mmMultiCR::GetWaypoints()
{
    return nullptr;
}

// ?SelectTeams@mmMultiCR@@AAEHXZ
//
// Private (`AAE`). Teams come from the host, so there is nothing to select locally.
i32 mmMultiCR::SelectTeams()
{
    return 0;
}

// ?SendSetup@mmMultiCR@@AAEXK@Z
void mmMultiCR::SendSetup(ulong arg1)
{
    (void) arg1;
}

define_dummy_symbol(mmgame_mmmulticr);
