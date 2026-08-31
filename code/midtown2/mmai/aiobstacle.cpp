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

// aiObstacle is what the traffic AI sees in front of it - a parked car, a banger, a pedestrian.
// These three are the base's answers, and each is the conservative one.
//
// BreakThreshold returning 1e8 is the interesting one: it is the force needed to break through the
// obstacle, and the base returns a number nothing in the game can reach. So an obstacle is
// immovable unless it says otherwise, and the AI plans around it rather than through it.

#include "aiobstacle.h"

// ?InAccident@aiObstacle@@UAEHXZ - 0x0053F5D0
//
// i32 rather than bool - the mangled name says H, and the AI stores it as an int.
i32 aiObstacle::InAccident()
{
    return 0;
}

// ?BreakThreshold@aiObstacle@@UAEMXZ - 0x0053F5E0
//
// Effectively infinite: nothing in the game applies 1e8 of force, so the base obstacle never
// breaks. The bangers override this with a real threshold.
f32 aiObstacle::BreakThreshold()
{
    return 1e8f;
}

// ?Drivable@aiObstacle@@UAEHXZ - 0x0053F5F0
//
// Not drivable through. Only the obstacles the AI is allowed to ignore answer otherwise.
i32 aiObstacle::Drivable()
{
    return 0;
}

define_dummy_symbol(mmai_aiobstacle);
