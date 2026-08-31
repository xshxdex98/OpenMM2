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

// The subway train as an aiVehicle.
//
// It reports lane 0 rather than -1: a train IS on a fixed path, just a rail rather than a road
// lane, so lane 0 is the only lane it can be in. Its distance and speed accessors read the class's
// own fields and wait on aiSubway's layout being named at 0x5C through 0x74.

#include "aisubway.h"

// ?Type@aiSubway@@UAEHXZ
//
// 6 in the shared aiVehicle type enum.
i32 aiSubway::Type()
{
    return 6;
}

// ?CurrentLane@aiSubway@@UAEHXZ
//
// Always the single rail.
i32 aiSubway::CurrentLane()
{
    return 0;
}

// ?DrawId@aiSubway@@UAEXXZ
void aiSubway::DrawId()
{}

define_dummy_symbol(mmai_aisubway);
