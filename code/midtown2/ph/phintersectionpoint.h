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

class Matrix34;
class Vector3;

class phIntersectionPoint
{
public:
    // ??0phIntersectionPoint@@QAE@XZ
    ARTS_IMPORT phIntersectionPoint();

    // ?Set@phIntersectionPoint@@QAEXABVVector3@@0MME@Z
    ARTS_IMPORT void Set(const Vector3& arg1, const Vector3& arg2, f32 arg3, f32 arg4, u8 arg5);

    // ?Transform@phIntersectionPoint@@QAEXPBVMatrix34@@@Z
    ARTS_IMPORT void Transform(const Matrix34* arg1);
};

// check_size(phIntersectionPoint, 0x24); // size known, members are not - cannot verify
