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

// phBound is the base collision volume - the sphere, box, polygon and cached-polygon bounds all
// derive from it. These nine are its defaults.
//
// SetFriction and SetElasticity are each a PAIR overloaded on parameters, not a setter and a
// misnamed getter: `f32 SetFriction()` reads the value and `void SetFriction(f32)` writes it. The
// base stores neither - the setter is ignored and the reader answers a constant 0.5 for both, so
// an unspecialised bound is half-friction and half-elastic. A bound with real material properties
// overrides both halves.
//
// The rest say "no contact" and "change nothing", which is the safe answer for a volume with no
// geometry: TestSphere reports a miss, ModifyInvMassMatrix leaves the mass matrix alone, and
// EffectiveMass hands back the mass it was given.
//
// GetVertex is NOT ported, even though the zero vector it returns now has a name. Its extent is
// recorded as 48 bytes and data still points at 0x00487740, sixteen bytes in, so stripping it
// would delete something else that is referenced - build gate 6 catches it. The blocker is the
// recorded extent, not the global.

#include "phbound.h"

#include "vector7/matrix34.h"
#include "vector7/vector3.h"

// ?SetFriction@phBound@@UAEMXZ - 0x0045CF60
//
// The reader half of the pair. 0.5 is the engine-wide default coefficient.
f32 phBound::SetFriction()
{
    return 0.5f;
}

// ?SetFriction@phBound@@UAEXM@Z - 0x0045CF40
//
// The writer half. The base keeps no friction field, so the value is dropped.
void phBound::SetFriction(f32 arg1)
{
    (void) arg1;
}

// ?SetElasticity@phBound@@UAEMXZ - 0x0045CF70
f32 phBound::SetElasticity()
{
    return 0.5f;
}

// ?SetElasticity@phBound@@UAEXM@Z - 0x0045CF50
void phBound::SetElasticity(f32 arg1)
{
    (void) arg1;
}

// ?TestSphere@phBound@@UBE_NABVVector3@@MAAVphImpactBase@@@Z - 0x004876C0
//
// The impact-reporting overload. A bound with no geometry is never hit, so the phImpactBase is
// left untouched.
bool phBound::TestSphere(const Vector3& arg1, f32 arg2, phImpactBase& arg3) const
{
    (void) arg1;
    (void) arg2;
    (void) arg3;

    return false;
}

// ?TestSphere@phBound@@UBE_NABVVector3@@MAAV2@1AAM@Z - 0x004876B0
//
// The overload that returns the contact point, normal and depth through out-parameters. All three
// are left alone on a miss.
bool phBound::TestSphere(const Vector3& arg1, f32 arg2, Vector3& arg3, Vector3& arg4, f32& arg5) const
{
    (void) arg1;
    (void) arg2;
    (void) arg3;
    (void) arg4;
    (void) arg5;

    return false;
}

// ?ModifyInvMassMatrix@phBound@@UBEXPAVMatrix34@@HABVVector3@@1@Z - 0x0045CF90
//
// Where a bound with distributed mass would adjust the inverse mass matrix at a contact. The base
// has no mass distribution, so the matrix is left as the caller built it.
void phBound::ModifyInvMassMatrix(Matrix34* arg1, i32 arg2, const Vector3& arg3, const Vector3& arg4) const
{
    (void) arg1;
    (void) arg2;
    (void) arg3;
    (void) arg4;
}

// ?ModifyInvMassMatrix@phBound@@UBEXPAVMatrix34@@HABVVector3@@@Z - 0x0045CF80
//
// The single-point overload, same default.
void phBound::ModifyInvMassMatrix(Matrix34* arg1, i32 arg2, const Vector3& arg3) const
{
    (void) arg1;
    (void) arg2;
    (void) arg3;
}

// ?EffectiveMass@phBound@@UBEMABVVector3@@0HM@Z - 0x0045CFA0
//
// Returns the mass it was handed, unchanged. A bound that redistributes mass across a contact
// overrides this; the base behaves as a point mass.
f32 phBound::EffectiveMass(const Vector3& arg1, const Vector3& arg2, i32 arg3, f32 arg4) const
{
    (void) arg1;
    (void) arg2;
    (void) arg3;

    return arg4;
}

define_dummy_symbol(ph_phbound);
