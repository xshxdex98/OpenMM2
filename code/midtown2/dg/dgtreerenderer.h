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

class dgBangerInstance;

class dgTreeRenderer
{
public:
    // ??0dgTreeRenderer@@QAE@XZ
    ARTS_IMPORT dgTreeRenderer();

    // ?AddTree@dgTreeRenderer@@QAEXPAVdgBangerInstance@@H@Z
    ARTS_IMPORT void AddTree(dgBangerInstance* arg1, i32 arg2);

    // ?RenderTrees@dgTreeRenderer@@QAEXXZ
    ARTS_IMPORT void RenderTrees();

public:
    static dgTreeRenderer*& Access_sm_Instance() { return sm_Instance; }

private:
    // ?sm_Instance@dgTreeRenderer@@0PAV1@A
    ARTS_IMPORT static dgTreeRenderer* sm_Instance;
};

// check_size(dgTreeRenderer, 0x1); // size known, members are not - cannot verify
