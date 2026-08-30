/*
    OpenMM2 - An Open Source Re-Implementation of Midtown Madness 2
    Copyright (C) 2026 The OpenMM2 contributors

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

#pragma once

#include "core/arts.h"

#include "ph/phpolygon.h"

class Vector3;

class sdlPoly : public phPolygon
{
public:
    // ?SetFlatQuad@sdlPoly@@QAE_NHHHHHM@Z
    ARTS_IMPORT bool SetFlatQuad(i32 arg1, i32 arg2, i32 arg3, i32 arg4, i32 arg5, f32 arg14);

    // ?SetFlatTri@sdlPoly@@QAE_NHHHHM@Z
    ARTS_IMPORT bool SetFlatTri(i32 arg1, i32 arg2, i32 arg3, i32 arg4, f32 arg10);

    // ?SetQuad@sdlPoly@@QAE_NHABVVector3@@000@Z
    ARTS_IMPORT bool SetQuad(i32 arg1, const Vector3& arg4, const Vector3& arg3, const Vector3& arg4, const Vector3& arg5);

    // ?SetQuad@sdlPoly@@QAE_NHHMHMHMHM@Z
    ARTS_IMPORT bool SetQuad(i32 arg1, i32 arg2, f32 arg3, i32 arg4, f32 arg5, i32 arg6, f32 arg7, i32 arg8, f32 arg9);

    // ?SetTri@sdlPoly@@QAE_NHABVVector3@@00@Z
    ARTS_IMPORT bool SetTri(i32 arg1, const Vector3& arg4, const Vector3& arg3, const Vector3& arg4);

    // ?SetWall@sdlPoly@@QAE_NHABVVector3@@0MM@Z
    ARTS_IMPORT bool SetWall(i32 arg1, const Vector3& arg4, const Vector3& arg3, f32 arg4, f32 arg5);

    // ?SetWall@sdlPoly@@QAE_NHHHMM@Z
    ARTS_IMPORT bool SetWall(i32 arg1, i32 arg2, i32 arg3, f32 argC, f32 arg5);

public:
    static i32& Access_sm_Count() { return sm_Count; }

protected:
    // ?InitNoArea@sdlPoly@@IAE_NHHHHH@Z
    ARTS_IMPORT bool InitNoArea(i32 arg1, i32 arg2, i32 arg3, i32 arg4, i32 arg5);

    // ?sm_Count@sdlPoly@@1HA
    ARTS_IMPORT static i32 sm_Count;
};

// check_size(sdlPoly, 0x60); // size known, members are not - cannot verify
