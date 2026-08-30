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

#include "dg/dgunhitybangerinstance.h"

class Vector3;

class aiTrafficLightInstance : public dgUnhitYBangerInstance
{
public:
    // ?Draw@aiTrafficLightInstance@@UAEXH@Z
    ARTS_IMPORT virtual void Draw(i32 arg1);

    // ?DrawGlow@aiTrafficLightInstance@@UAEXXZ
    ARTS_IMPORT virtual void DrawGlow();

    // ?SizeOf@aiTrafficLightInstance@@UAEIXZ
    ARTS_IMPORT virtual u32 SizeOf();

    // ?Init@aiTrafficLightInstance@@UAEHPADAAVVector3@@1H0@Z
    ARTS_IMPORT virtual i32 Init(char* arg1, Vector3& arg2, Vector3& arg3, i32 arg4, char* arg5);
};

// ??_7aiTrafficLightInstance@@6B@
// vtable at 0x005B54AC
// check_size(aiTrafficLightInstance, 0x0); // TODO: no layout in the IDB type library
