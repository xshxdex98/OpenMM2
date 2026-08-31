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

// vehCarModel is the car's drawable half - geometry, shaders and the reflection pass.
//
// The three ported here are the ones that answer with a constant. Its accessors are not: GetEntity,
// GetVelocity, GetMatrix and GetVisible all read through the class's own fields, and GetVelocity
// and GetMatrix chase a pointer at 0x1C into the sim, so they wait on vehCarModel's own layout
// being named rather than on anything here.

#include "vehcarmodel.h"

// ?AttachEntity@vehCarModel@@UAEPAVdgPhysEntity@@XZ - 0x004CEFD0
//
// The model never creates an entity: the car's physics entity belongs to vehCar and the model is
// handed it, so there is nothing to attach.
dgPhysEntity* vehCarModel::AttachEntity()
{
    return nullptr;
}

// ?DrawReflected@vehCarModel@@UAEXM@Z - 0x004CF080
//
// Cars are not drawn into the reflection pass - only the city is - so the hook is empty even
// though the class is the one that would draw it.
void vehCarModel::DrawReflected(f32 arg1)
{
    (void) arg1;
}

// ?SizeOf@vehCarModel@@UAEIXZ - 0x004CDFE0
//
// 0xCC, emitted as `mov eax, 0xcc; ret`. That is the 1999 compiler's own sizeof, so it is
// independent evidence for the class's size rather than a number anyone chose - and it agrees
// with what the constructor allocates.
u32 vehCarModel::SizeOf()
{
    return 0xCC;
}

define_dummy_symbol(veh_vehcarmodel);
