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

// vehCarSim's damage reset.
//
// ClearDamage is empty on the SIM, which is the interesting part: damage lives on vehCarDamage,
// and vehCarDamage::Reset calls its own ClearDamage and then zeroes a byte at 0x394. The sim's
// copy exists so the car can clear damage through whichever half it holds, and only one of the two
// does any work.

#include "vehcarsim.h"

// ?ClearDamage@vehCarSim@@QAEXXZ - 0x004CC880
void vehCarSim::ClearDamage()
{}

define_dummy_symbol(veh_vehcarsim);
