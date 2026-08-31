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

// gfxPacket is a batched draw primitive. Three of its statics are inert in the retail build - the
// coloured-per-vertex override and the two reference/property capture hooks - and MakeList is the
// identity.
//
// MakeList casting a gfxPacket* straight to a gfxPacketList* is not a mistake: the list is the
// packet's own header reinterpreted, so the first packet of a chain IS the list. The cast is what
// the binary does, so it is written as a reinterpret rather than hidden behind a helper.

#include "gfxpacket.h"

// ?ForceCPV@gfxPacket@@SAXI@Z - 0x004B3310
//
// Would have forced colour-per-vertex on for a packet regardless of its material.
void gfxPacket::ForceCPV(u32 arg1)
{
    (void) arg1;
}

// ?BeginRef@gfxPacket@@SAXXZ - 0x004B3320
void gfxPacket::BeginRef()
{}

// ?BeginProp@gfxPacket@@SAXXZ - 0x004B3330
void gfxPacket::BeginProp()
{}

// ?MakeList@gfxPacket@@SAPAVgfxPacketList@@PAV1@@Z - 0x004B4860
//
// Returns its argument unchanged. `PAV1@` is a pointer to this same class, so the packet and the
// list share an address and the conversion is purely a change of view.
gfxPacketList* gfxPacket::MakeList(gfxPacket* arg1)
{
    return reinterpret_cast<gfxPacketList*>(arg1);
}

define_dummy_symbol(gfx_gfxpacket);
