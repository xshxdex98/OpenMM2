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

// The player as the traffic AI sees them.
//
// CurrentLane returning -1 is the important one. Every other aiVehicle reports the lane it is
// driving in, but the player is not on the rail network, so the honest answer is "no lane" and the
// traffic code has to treat it as a special case rather than plan around a lane the player is not
// in.

#include "aivehicleplayer.h"

// ?Type@aiVehiclePlayer@@UAEHXZ
//
// 4 in the shared aiVehicle type enum.
i32 aiVehiclePlayer::Type()
{
    return 4;
}

// ?CurrentLane@aiVehiclePlayer@@UAEHXZ
//
// -1, meaning no lane: the player drives wherever they like.
i32 aiVehiclePlayer::CurrentLane()
{
    return -1;
}

// ?DrawId@aiVehiclePlayer@@UAEXXZ
void aiVehiclePlayer::DrawId()
{}

define_dummy_symbol(mmai_aivehicleplayer);
