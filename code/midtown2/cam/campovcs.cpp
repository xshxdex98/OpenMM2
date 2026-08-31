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

// camPovCS is the in-cockpit camera. Two of its hooks are empty and the third sets one field.
//
// AfterLoad is not a stub: it writes CameraNear = 0.1, against camTrackCS's 0.5 at the same
// offset. The cockpit view has the dashboard and the car's own interior right against the eye, so
// it needs a near plane five times closer than a camera watching from outside. That single float
// is the difference between seeing the dash and clipping through it.

#include "campovcs.h"

// ?MakeActive@camPovCS@@UAEXXZ - 0x0051D530
void camPovCS::MakeActive()
{}

// ?UpdateInput@camPovCS@@UAEXXZ - 0x0051D590
//
// The POV camera does not track the stick - it is rigidly attached to the car's head position.
void camPovCS::UpdateInput()
{}

// ?AfterLoad@camPovCS@@UAEXXZ - 0x0051D710
//
// 0x3DCCCCCD, which is 0.1f. Written after the DAT file is read, so it overrides whatever the
// tune file supplied.
void camPovCS::AfterLoad()
{
    CameraNear = 0.1f;
}

define_dummy_symbol(cam_campovcs);
