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
#include "vector7/vector3.h"

class Matrix34;
class modShader;
class modStatic;

class RadialGauge : public asNode
{
public:
    // ??0RadialGauge@@QAE@XZ
    ARTS_IMPORT RadialGauge();

    // ??1RadialGauge@@UAE@XZ
    ARTS_IMPORT virtual ~RadialGauge();

    // ?Cull@RadialGauge@@UAEXXZ
    ARTS_IMPORT virtual void Cull();

    // ?Update@RadialGauge@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Cull@RadialGauge@@QAEXPBVMatrix34@@@Z
    ARTS_IMPORT void Cull(const Matrix34* arg1);

    // ?Init@RadialGauge@@QAEXPAVmodStatic@@PAPAVmodShader@@VVector3@@PBM3M@Z
    ARTS_IMPORT void Init(modStatic* arg1, modShader** arg2, Vector3 arg3, const f32* arg4, const f32* arg5, f32 arg6);

public:
    static bool& Access_bDebugPivot() { return bDebugPivot; }

private:
    // ?GetArrowAngle@RadialGauge@@AAEMXZ
    ARTS_IMPORT f32 GetArrowAngle();

    // ?bDebugPivot@RadialGauge@@0_NA
    ARTS_IMPORT static bool bDebugPivot;
};

// ??_7RadialGauge@@6B@
// vtable at 0x005B11B4
// check_size(RadialGauge, 0x114); // size known, members are not - cannot verify
