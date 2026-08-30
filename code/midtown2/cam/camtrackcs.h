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

#include "misc/spline.h"
#include "vector7/matrix34.h"
#include "vector7/vector3.h"

#include "camcarcs.h"

class datParser;

class camTrackCS : public camCarCS
{
public:
    // ??0camTrackCS@@QAE@XZ
    ARTS_IMPORT camTrackCS();

    // ??1camTrackCS@@UAE@XZ
    ARTS_IMPORT virtual ~camTrackCS();

    // ?AfterLoad@camTrackCS@@UAEXXZ
    ARTS_IMPORT virtual void AfterLoad();

    // ?FileIO@camTrackCS@@UAEXAAVdatParser@@@Z
    ARTS_IMPORT virtual void FileIO(datParser& arg1);

    // ?GetClassName@camTrackCS@@UAEPADXZ
    ARTS_IMPORT virtual char* GetClassName();

    // ?MakeActive@camTrackCS@@UAEXXZ
    ARTS_IMPORT virtual void MakeActive();

    // ?Reset@camTrackCS@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?SwingToRear@camTrackCS@@QAEXXZ
    ARTS_IMPORT void SwingToRear();

    // ?Update@camTrackCS@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?UpdateInput@camTrackCS@@UAEXXZ
    ARTS_IMPORT virtual void UpdateInput();

private:
    // ?Collide@camTrackCS@@AAEXVVector3@@@Z
    ARTS_IMPORT void Collide(Vector3 arg1);

    // ?Front@camTrackCS@@AAEXM@Z
    ARTS_IMPORT void Front(f32 arg1);

    // ?MinMax@camTrackCS@@AAEXVMatrix34@@@Z
    ARTS_IMPORT void MinMax(Matrix34 arg1);

    // ?PreApproach@camTrackCS@@AAEXXZ
    ARTS_IMPORT void PreApproach();

    // ?Rear@camTrackCS@@AAEXM@Z
    ARTS_IMPORT void Rear(f32 arg1);

    // ?UpdateCar@camTrackCS@@AAEXXZ
    ARTS_IMPORT void UpdateCar();

    // ?UpdateHill@camTrackCS@@AAEXXZ
    ARTS_IMPORT void UpdateHill();

    // ?UpdateSwing@camTrackCS@@AAEXXZ
    ARTS_IMPORT void UpdateSwing();

    // ?UpdateTrack@camTrackCS@@AAEXXZ
    ARTS_IMPORT void UpdateTrack();

public:
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 field_4[24]; // 0x004
    Matrix34 field_1C; // 0x01C
    u8 field_4C[52]; // 0x04C
    f32 field_80; // 0x080
    i32 field_84; // 0x084
    f32 field_88; // 0x088
    i32 field_8C; // 0x08C
    u8 field_90[16]; // 0x090
    i32 field_A0; // 0x0A0
    i32 field_A4; // 0x0A4
    f32 field_A8; // 0x0A8
    i32 field_AC; // 0x0AC
    i32 field_B0; // 0x0B0
    u8 field_B4[4]; // 0x0B4
    f32 field_B8; // 0x0B8
    f32 field_BC; // 0x0BC
    f32 field_C0; // 0x0C0
    u8 field_C4[4]; // 0x0C4
    i32 field_C8; // 0x0C8
    f32 field_CC; // 0x0CC
    f32 field_D0; // 0x0D0
    i32 field_D4; // 0x0D4
    u8 field_D8[48]; // 0x0D8
    i32 field_108; // 0x108
    i32 ReverseOn; // 0x10C
    i32 field_110; // 0x110
    i32 Offset; // 0x114
    f32 field_118; // 0x118
    f32 field_11C; // 0x11C
    i32 CollideType; // 0x120
    i32 MinMaxOn; // 0x124
    i32 TrackBreak; // 0x128
    f32 MinAppXZPos; // 0x12C
    f32 MaxAppXZPos; // 0x130
    i32 MinSpeed; // 0x134
    f32 MaxSpeed; // 0x138
    f32 AppInc; // 0x13C
    i32 AppDec; // 0x140
    f32 MinHardSteer; // 0x144
    f32 DriftDelay; // 0x148
    f32 VertOffset; // 0x14C
    f32 FrontRate; // 0x150
    i32 RearRate; // 0x154
    i32 FlipDelay; // 0x158
    i32 SteerOn; // 0x15C
    i32 SteerMin; // 0x160
    f32 SteerAmt; // 0x164
    f32 HillMin; // 0x168
    f32 HillMax; // 0x16C
    f32 HillLerp; // 0x170
    i32 RevDelay; // 0x174
    i32 RevOnApp; // 0x178
    f32 RevOffApp; // 0x17C
    f32 field_180; // 0x180
    i32 field_184; // 0x184
    i32 field_188; // 0x188
    i32 field_18C; // 0x18C
    i32 field_190; // 0x190
    i32 field_194; // 0x194
    i32 field_198; // 0x198
    i32 field_19C; // 0x19C
    i32 field_1A0; // 0x1A0
    i32 field_1A4; // 0x1A4
    i32 field_1A8; // 0x1A8
    i32 field_1AC; // 0x1AC
    i32 field_1B0; // 0x1B0
    i32 field_1B4; // 0x1B4
    i32 field_1B8; // 0x1B8
    Spline field_1BC; // 0x1BC
    i32 field_228; // 0x228
    i32 field_22C; // 0x22C
    i32 field_230; // 0x230
    i32 field_234; // 0x234
    i32 field_238; // 0x238
    i32 field_23C; // 0x23C
    i32 field_240; // 0x240
    i32 field_244; // 0x244
    u8 field_248; // 0x248
    u8 field_249[7]; // 0x249
    i32 field_250; // 0x250
    u8 field_254[4]; // 0x254
    i32 field_258; // 0x258
    i32 field_25C; // 0x25C
    u8 field_260[8]; // 0x260
    i32 field_268; // 0x268
    i32 field_26C; // 0x26C
    i32 field_270; // 0x270
    i32 field_274; // 0x274
    i32 field_278; // 0x278
    i32 field_27C; // 0x27C
    i32 field_280; // 0x280
    i32 field_284; // 0x284
    i32 field_288; // 0x288
    i32 field_28C; // 0x28C
    i32 field_290; // 0x290
    i32 field_294; // 0x294
};

// ??_7camTrackCS@@6B@
// vtable at 0x005B4EC4
check_size(camTrackCS, 0x298);
