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

#include "arts7/asnode.h"

class gizTrainMgr : public asNode
{
public:
    // ??0gizTrainMgr@@QAE@XZ
    ARTS_IMPORT gizTrainMgr();

    // ??1gizTrainMgr@@UAE@XZ
    ARTS_IMPORT virtual ~gizTrainMgr();

    // ?Update@gizTrainMgr@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@gizTrainMgr@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?Init@gizTrainMgr@@QAE_NPAD00@Z
    ARTS_IMPORT bool Init(char* arg1, char* arg2, char* arg3);

private:
    // ?ApplyTuning@gizTrainMgr@@AAEXXZ
    ARTS_IMPORT void ApplyTuning();
};

// ??_7gizTrainMgr@@6B@
// vtable at 0x005B614C
// check_size(gizTrainMgr, 0x0); // TODO: no layout in the IDB type library
