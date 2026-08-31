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

// vehTrailer's post-update hook.
//
// The trailer is integrated as part of the car it is hitched to rather than on its own, so there
// is nothing to do after the physics step. Its other one-line members are not stubs - GetInst and
// GetICS return the addresses of subobjects embedded at 0x1E8 and 0x210 - and they wait on
// vehTrailer's layout being named at those offsets.

#include "vehtrailer.h"

// ?PostUpdate@vehTrailer@@UAEXXZ - 0x004D7C40
void vehTrailer::PostUpdate()
{}

define_dummy_symbol(veh_vehtrailer);
