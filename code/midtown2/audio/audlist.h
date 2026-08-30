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

class audList
{
public:
    // ?Count@audList@@QAEHXZ
    ARTS_IMPORT i32 Count();

    // ?FindObject@audList@@QAEPAV1@PAX@Z
    ARTS_IMPORT audList* FindObject(void* arg1);

    // ?FindObjectAt@audList@@QAEPAXH@Z
    ARTS_IMPORT void* FindObjectAt(i32 arg1);

    // ?FindObjectPos@audList@@QAEHPAX@Z
    ARTS_IMPORT i32 FindObjectPos(void* arg1);

    // ?GetNext@audList@@QAEPAV1@XZ
    ARTS_IMPORT audList* GetNext();

    // ?GetPrev@audList@@QAEPAV1@XZ
    ARTS_IMPORT audList* GetPrev();

    // ?Init@audList@@QAEXXZ
    ARTS_IMPORT void Init();

    // ?LinkNext@audList@@QAEPAV1@PAV1@@Z
    ARTS_IMPORT audList* LinkNext(audList* arg1);

    // ?LinkPrev@audList@@QAEPAV1@PAV1@@Z
    ARTS_IMPORT audList* LinkPrev(audList* arg1);

    // ?SetNext@audList@@QAEPAV1@PAV1@@Z
    ARTS_IMPORT audList* SetNext(audList* arg1);

    // ?SetPrev@audList@@QAEPAV1@PAV1@@Z
    ARTS_IMPORT audList* SetPrev(audList* arg1);

    // ?Unlink@audList@@QAE_NXZ
    ARTS_IMPORT bool Unlink();
};

// check_size(audList, 0x0); // TODO: no layout in the IDB type library
