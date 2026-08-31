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

// aiMap owns the traffic network - the roads, intersections and the ambient vehicles driving them.
// These five are the parts of it that do nothing in the retail build.
//
// Cull and UpdatePaused are asNode overrides: the map is not drawn as a node and does not tick
// while paused, so both are empty. The three Draw* entries are debug visualisers - the road
// network overlay, the next ambient vehicle, and the checkpoint marker - compiled out of the
// shipped build but still occupying their addresses.

#include "aimap.h"

#include "vector7/vector3.h"

// ?UpdatePaused@aiMap@@UAEXXZ - 0x005374E0
//
// Traffic does not advance while the game is paused, and nothing else needs updating, so the
// asNode hook is empty rather than absent.
void aiMap::UpdatePaused()
{}

// ?Cull@aiMap@@UAEXXZ - 0x005374F0
//
// The map itself draws nothing: the ambient vehicles are drawn as instances through the city's own
// traversal, not through this node.
void aiMap::Cull()
{}

// ?DrawNextVisibleAmbient@aiMap@@QAEXXZ - 0x00537500
void aiMap::DrawNextVisibleAmbient()
{}

// ?DrawX@aiMap@@QAEXAAVVector3@@@Z - 0x00537510
//
// Drew a marker cross at the position in the development build.
void aiMap::DrawX(Vector3& arg1)
{
    (void) arg1;
}

// ?DrawFinalCheckPoint@aiMap@@QAEXXZ - 0x0053B860
void aiMap::DrawFinalCheckPoint()
{}

define_dummy_symbol(mmai_aimap);
