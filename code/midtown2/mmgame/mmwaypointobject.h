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
#include "vector7/matrix34.h"
#include "vector7/vector2.h"
#include "vector7/vector3.h"

class Vector4;

class mmWaypointObject : public asNode
{
public:
    // ??0mmWaypointObject@@QAE@AAVVector4@@PADHMHM@Z
    ARTS_IMPORT mmWaypointObject(Vector4& arg1, char* arg2, i32 arg3, f32 arg4, i32 arg5, f32 arg6);

    // ??1mmWaypointObject@@UAE@XZ
    ARTS_IMPORT virtual ~mmWaypointObject();

    // ?Activate@mmWaypointObject@@QAEXXZ
    ARTS_IMPORT void Activate();

    // ?CalculateGatePoints@mmWaypointObject@@QAEXXZ
    ARTS_IMPORT void CalculateGatePoints();

    // ?Deactivate@mmWaypointObject@@QAEXXZ
    ARTS_IMPORT void Deactivate();

    // ?GetDrawFlag@mmWaypointObject@@QAEHXZ
    ARTS_IMPORT i32 GetDrawFlag();

    // ?GetHitFlag@mmWaypointObject@@QAEHXZ
    ARTS_IMPORT i32 GetHitFlag();

    // ?LineIntersect@mmWaypointObject@@QAEHVVector2@@000M@Z
    ARTS_IMPORT i32 LineIntersect(Vector2 arg1, Vector2 arg2, Vector2 arg3, Vector2 arg4, f32 arg5);

    // ?Move@mmWaypointObject@@QAEXXZ
    ARTS_IMPORT void Move();

    // ?PlaneHit@mmWaypointObject@@QAEHVMatrix34@@VVector2@@1VVector3@@@Z
    ARTS_IMPORT i32 PlaneHit(Matrix34 arg1, Vector2 arg2, Vector2 arg3, Vector3 arg4);

    // ?RadiusHit@mmWaypointObject@@QAEHVVector3@@@Z
    ARTS_IMPORT i32 RadiusHit(Vector3 arg1);

    // ?Reset@mmWaypointObject@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?SetHeading@mmWaypointObject@@QAEXM@Z
    ARTS_IMPORT void SetHeading(f32 arg1);

    // ?SetHeadingType@mmWaypointObject@@QAEXH@Z
    ARTS_IMPORT void SetHeadingType(i32 arg1);

    // ?SetHitFlag@mmWaypointObject@@QAEXH@Z
    ARTS_IMPORT void SetHitFlag(i32 arg1);

    // ?SetPos@mmWaypointObject@@QAEXVVector3@@@Z
    ARTS_IMPORT void SetPos(Vector3 arg1);

    // ?SetRadius@mmWaypointObject@@QAEXM@Z
    ARTS_IMPORT void SetRadius(f32 arg1);

    // ?Update@mmWaypointObject@@UAEXXZ
    ARTS_IMPORT virtual void Update();

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 field_18[4]; // 0x018
    i32 field_1C; // 0x01C
    i32 field_20; // 0x020
    i32 field_24; // 0x024
    i32 field_28; // 0x028
    i32 field_2C; // 0x02C
    i32 field_30; // 0x030
    i32 field_34; // 0x034
    i32 field_38; // 0x038
    i32 field_3C; // 0x03C
    f32 field_40; // 0x040
    f32 field_44; // 0x044
    f32 field_48; // 0x048
    i32 field_4C; // 0x04C
    u8 field_50[4]; // 0x050
    i32 field_54; // 0x054
    f32 field_58; // 0x058
};

// ??_7mmWaypointObject@@6B@
// vtable at 0x005B1180
check_size(mmWaypointObject, 0x5C);
