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

// The traffic AI's vehicle kinds, and the one number that identifies each.
//
// Type() is a small enum shared across the aiVehicle hierarchy - 0 for an ambient car, 4 for the
// player, 6 for a subway train - and the traffic code branches on it rather than on the vtable
// where the distinction is about behaviour instead of implementation.
//
// CurrentLane is the other one worth reading together: an ambient car and a subway both report a
// real lane, but the PLAYER reports -1. The player is not on the rail network at all, so every
// lane query about them has to fail rather than return a plausible lane.
//
// aiVehicleAmbient::Type is NOT ported, though it is one instruction. data/symbols.json records
// its extent as 208 bytes - distance to the next symbol, not the function - and data still points
// at 0x00551CD0, 0x00551CF0 and 0x00551D40 inside that range. Stripping it would delete code that
// is still referenced, which build gate 6 catches. It becomes portable once the extent is
// narrowed, not by overriding the gate.

#include "aivehicleambient.h"

// ?DrawId@aiVehicleAmbient@@UAEXXZ
//
// Drew the vehicle's network id above it in the development build.
void aiVehicleAmbient::DrawId()
{}

// ?DrawBBox@aiVehicleAmbient@@QAEXF@Z
//
// The debug bounding box. i16 rather than i32 - `F` in the mangled name.
void aiVehicleAmbient::DrawBBox(i16 arg1)
{
    (void) arg1;
}

define_dummy_symbol(mmai_aivehicleambient);
