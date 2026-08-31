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

// camBaseCS is the base of the camera control states - chase, POV, track, cinematic. These three
// are the hooks a state overrides only if it needs them.
//
// GetDirName is not ported: it returns the string literal at 0x005D4423 offset by one byte, which
// is the "tune_camera" directory name sharing storage with a longer string. That belongs with the
// other string ports rather than being spelled as pointer arithmetic here.

#include "cambasecs.h"

// ?MakeActive@camBaseCS@@UAEXXZ - 0x00521520
//
// Called when this state becomes the live camera. The base has no state to prime.
void camBaseCS::MakeActive()
{}

// ?UpdateInput@camBaseCS@@UAEXXZ - 0x00520410
//
// Only the states the player can steer - the chase and POV cameras - read the stick here.
void camBaseCS::UpdateInput()
{}

// ?SetST@camBaseCS@@UAEXPAM@Z - 0x0051D750
//
// The spline parameter pair, used by the track camera. Ignored by every other state.
void camBaseCS::SetST(f32* arg1)
{
    (void) arg1;
}

define_dummy_symbol(cam_cambasecs);
