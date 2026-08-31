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

// DMusicObject wraps the DirectMusic port enumeration. Three of its entry points never got a real
// implementation: MM2 uses whatever port DirectMusic hands it rather than rating the available
// ones and choosing.
//
// That is why GetNumPorts reports one and FindBestPort reports none - the selection logic the
// three were written for was never wired up.

#include "dmusicobject.h"

// ?FindBestPort@DMusicObject@@QAEHAAU_DMUS_PORTCAPS@@@Z - 0x00516510
//
// -1 is "no port chosen", which sends the caller to the DirectMusic default.
i32 DMusicObject::FindBestPort(_DMUS_PORTCAPS& arg1)
{
    (void) arg1;

    return -1;
}

// ?GetNumPorts@DMusicObject@@QAEHXZ - 0x00516530
//
// One, always - the default port. Nothing enumerates the real list.
i32 DMusicObject::GetNumPorts()
{
    return 1;
}

// ?RatePort@DMusicObject@@AAEHAAU_DMUS_PORTCAPS@@@Z - 0x00516520
//
// Private (`AAE`), and the scoring function FindBestPort would have called. Every port rates zero.
i32 DMusicObject::RatePort(_DMUS_PORTCAPS& arg1)
{
    (void) arg1;

    return 0;
}

define_dummy_symbol(audio_dmusicobject);
