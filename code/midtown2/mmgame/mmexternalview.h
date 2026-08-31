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

#include "mmgearindicator.h"
#include "mmlineargauge.h"
#include "mmslidinggauge.h"
#include "mmspeedindicator.h"

class Base_vtbl;
class gfxBitmap;
class mmPlayer;
class vehCarSim;

class mmExternalView : public asNode
{
public:
    // ??0mmExternalView@@QAE@XZ
    ARTS_IMPORT mmExternalView();

    // ??1mmExternalView@@UAE@XZ
    ARTS_IMPORT virtual ~mmExternalView();

    // ?Cull@mmExternalView@@UAEXXZ
    ARTS_IMPORT virtual void Cull();

    // ?Update@mmExternalView@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@mmExternalView@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?ResChange@mmExternalView@@UAEXHH@Z
    ARTS_IMPORT virtual void ResChange(i32 arg1, i32 arg2);

    // ?GetClassNameA@mmExternalView@@UAEPADXZ
    ARTS_IMPORT virtual char* GetClassNameA();

    // ?Init@mmExternalView@@QAEXPAVmmPlayer@@@Z
    ARTS_IMPORT void Init(mmPlayer* arg1);

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    vehCarSim* CarSim; // 0x018
    mmPlayer* Player; // 0x01C
    i32 DestX; // 0x020
    i32 DestY; // 0x024
    gfxBitmap* MouseBar; // 0x028
    gfxBitmap* MouseAr; // 0x02C
    i32 field_30; // 0x030
    i32 field_34; // 0x034
    i32 field_38; // 0x038
    i32 field_3c; // 0x03C
    i32 field_40; // 0x040
    i32 field_44; // 0x044
    mmLinearGauge LinearGauge; // 0x048
    mmSlidingGauge SlidingGauge; // 0x068
    mmGearIndicator GearIndicator; // 0x08C
    mmSpeedIndicator SpeedIndicator; // 0x0CC
};

// ??_7mmExternalView@@6B@
// vtable at 0x005B0DB0
check_size(mmExternalView, 0x104);
