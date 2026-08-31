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

// camTrackCS is the cinematic camera that follows a spline through the city. Three of its hooks
// are empty and AfterLoad sets one field.
//
// AfterLoad writes CameraNear = 0.5, against camPovCS's 0.1 at the same offset - a camera watching
// from a distance can afford a near plane five times further out than the cockpit view, and the
// extra depth range is worth having.

#include "camtrackcs.h"

// ?MakeActive@camTrackCS@@UAEXXZ - 0x0051DAE0
void camTrackCS::MakeActive()
{}

// ?AfterLoad@camTrackCS@@UAEXXZ - 0x0051DAF0
//
// 0x3F000000, which is 0.5f exactly.
void camTrackCS::AfterLoad()
{
    CameraNear = 0.5f;
}

// ?UpdateInput@camTrackCS@@UAEXXZ - 0x0051DC60
//
// The track camera is driven by its spline parameter, not by the player.
void camTrackCS::UpdateInput()
{}

// ?UpdateSwing@camTrackCS@@AAEXXZ - 0x0051EB30
//
// Private (`AAE`). Would have added a lateral swing as the camera passes the car.
void camTrackCS::UpdateSwing()
{}

define_dummy_symbol(cam_camtrackcs);
