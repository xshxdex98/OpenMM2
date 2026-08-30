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

class Quaternion
{
public:
    // ?FromMatrix@Quaternion@@QAEXABVMatrix34@@@Z
    ARTS_IMPORT void FromMatrix(const Matrix34& arg1);

    // ?Multiply@Quaternion@@QAEXABV1@0@Z
    ARTS_IMPORT void Multiply(const Quaternion& arg1, const Quaternion& arg2);

    // ?Multiply@Quaternion@@QAEXABV1@@Z
    ARTS_IMPORT void Multiply(const Quaternion& arg1);

    // ?MultiplyInverse@Quaternion@@QAEXABV1@@Z
    ARTS_IMPORT void MultiplyInverse(const Quaternion& arg1);

    // ?Print@Quaternion@@QBEXPBD@Z
    ARTS_IMPORT void Print(const char* arg1) const;

    // ?Print@Quaternion@@QBEXXZ
    ARTS_IMPORT void Print() const;

    // ?Slerp@Quaternion@@QAEXMABV1@0@Z
    ARTS_IMPORT void Slerp(f32 arg1, const Quaternion& arg2, const Quaternion& arg3);
};

// check_size(Quaternion, 0x0); // TODO: no layout in the IDB type library
