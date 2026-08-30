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
#include "vector7/vector3.h"

class datParser;
class phInertialCS;
class vehCarSim;

class vehWheel : public asNode
{
public:
    // ??0vehWheel@@QAE@XZ
    ARTS_IMPORT vehWheel();

    // ??1vehWheel@@UAE@XZ
    ARTS_IMPORT virtual ~vehWheel();

    // ?AddNormalLoad@vehWheel@@QAEXM@Z
    ARTS_IMPORT void AddNormalLoad(f32 arg1);

    // ?ComputeConstants@vehWheel@@QAEXXZ
    ARTS_IMPORT void ComputeConstants();

    // ?ComputeDwtdw@vehWheel@@QAEXMPAM00@Z
    ARTS_IMPORT void ComputeDwtdw(f32 arg1, f32* arg2, f32* arg3, f32* arg4);

    // ?CopyVars@vehWheel@@QAEXPAV1@@Z
    ARTS_IMPORT void CopyVars(vehWheel* arg1);

    // ?FileIO@vehWheel@@UAEXAAVdatParser@@@Z
    ARTS_IMPORT virtual void FileIO(datParser& arg1);

    // ?GetClassName@vehWheel@@UAEPADXZ
    ARTS_IMPORT virtual char* GetClassName();

    // ?GetSurfaceSound@vehWheel@@QAEHXZ
    ARTS_IMPORT i32 GetSurfaceSound();

    // ?GetVisualDispLat@vehWheel@@QAEMXZ
    ARTS_IMPORT f32 GetVisualDispLat();

    // ?GetVisualDispLong@vehWheel@@QAEMXZ
    ARTS_IMPORT f32 GetVisualDispLong();

    // ?GetVisualDispVert@vehWheel@@QAEMXZ
    ARTS_IMPORT f32 GetVisualDispVert();

    // ?Init@vehWheel@@QAEXPAVvehCarSim@@PBDPADVVector3@@PAVphInertialCS@@HH@Z
    ARTS_IMPORT void Init(vehCarSim* arg1, const char* arg2, char* arg3, Vector3 arg4, phInertialCS* arg5, i32 arg6, i32 arg7);

    // ?Reset@vehWheel@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?SetInputs@vehWheel@@QAEXMMM@Z
    ARTS_IMPORT void SetInputs(f32 arg1, f32 arg2, f32 arg3);

    // ?SetNormalLoad@vehWheel@@QAEXM@Z
    ARTS_IMPORT void SetNormalLoad(f32 arg1);

    // ?Update@vehWheel@@UAEXXZ
    ARTS_IMPORT virtual void Update();

private:
    // ?CalcSuspensionForce@vehWheel@@AAEXM_NM@Z
    ARTS_IMPORT void CalcSuspensionForce(f32 arg1, bool arg2, f32 arg3);

    // ?ComputeFriction@vehWheel@@AAEMMPAM@Z
    ARTS_IMPORT f32 ComputeFriction(f32 arg1, f32* arg2);

    // ?GetBumpDisplacement@vehWheel@@AAEMM@Z
    ARTS_IMPORT f32 GetBumpDisplacement(f32 arg1);

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    vehCarSim* pCarSim; // 0x018
    phInertialCS* pInertialCS; // 0x01C
    u32 field_20; // 0x020
    Matrix34 field_24; // 0x024
    f32 TireDispLimitLat; // 0x054
    f32 TireDispLimitLong; // 0x058
    f32 TireDampCoefLat; // 0x05C
    f32 TireDampCoefLong; // 0x060
    f32 TireDragCoefLat; // 0x064
    f32 TireDragCoefLong; // 0x068
    f32 SteeringLimit; // 0x06C
    f32 CamberLimit; // 0x070
    u32 WobbleLimit; // 0x074
    f32 BrakeCoef; // 0x078
    f32 HandbrakeCoef; // 0x07C
    u32 SteeringOffset; // 0x080
    f32 SuspensionLimit; // 0x084
    f32 SuspensionExtent; // 0x088
    f32 SuspensionFactor; // 0x08C
    f32 SuspensionDampCoef; // 0x090
    u32 field_94; // 0x094
    u32 field_98; // 0x098
    u32 field_9C; // 0x09C
    u32 field_A0; // 0x0A0
    u32 field_A4; // 0x0A4
    u32 field_A8; // 0x0A8
    u32 field_AC; // 0x0AC
    u32 field_B0; // 0x0B0
    u8 field_B4; // 0x0B4
    u32 field_B8; // 0x0B8
    u32 field_BC; // 0x0BC
    Vector3 field_C0; // 0x0C0
    u32 field_CC; // 0x0CC
    u32 field_D0; // 0x0D0
    u32 field_D4; // 0x0D4
    f32 field_D8; // 0x0D8
    u32 field_DC; // 0x0DC
    u32 field_E0; // 0x0E0
    u32 field_E4; // 0x0E4
    u32 field_E8; // 0x0E8
    u32 field_EC; // 0x0EC
    u32 field_F0; // 0x0F0
    u32 field_F4; // 0x0F4
    u32 field_F8; // 0x0F8
    u32 field_FC; // 0x0FC
    u32 field_100; // 0x100
    u32 field_104; // 0x104
    u32 field_108; // 0x108
    u32 field_10C; // 0x10C
    u32 field_110; // 0x110
    u32 field_114; // 0x114
    u32 field_118; // 0x118
    u32 field_11C; // 0x11C
    u32 field_120; // 0x120
    u32 field_124; // 0x124
    u32 field_128; // 0x128
    u32 field_12C; // 0x12C
    u32 field_130; // 0x130
    u32 field_134; // 0x134
    u32 field_138; // 0x138
    u32 field_13C; // 0x13C
    u32 field_140; // 0x140
    u32 field_144; // 0x144
    u32 field_148; // 0x148
    u32 field_14C; // 0x14C
    u32 field_150; // 0x150
    u32 field_154; // 0x154
    u32 field_158; // 0x158
    u32 field_15C; // 0x15C
    u32 field_160; // 0x160
    u32 field_164; // 0x164
    u32 field_168; // 0x168
    u32 field_16C; // 0x16C
    f32 field_170; // 0x170
    f32 field_174; // 0x174
    f32 field_178; // 0x178
    f32 field_17C; // 0x17C
    f32 field_180; // 0x180
    f32 field_184; // 0x184
    f32 field_188; // 0x188
    f32 field_18C; // 0x18C
    f32 field_190; // 0x190
    f32 field_194; // 0x194
    f32 field_198; // 0x198
    f32 field_19C; // 0x19C
    f32 field_1A0; // 0x1A0
    u32 field_1A4; // 0x1A4
    u32 field_1A8; // 0x1A8
    u32 field_1AC; // 0x1AC
    Vector3 field_1B0; // 0x1B0
    f32 field_1BC; // 0x1BC
    f32 field_1C0; // 0x1C0
    f32 NormalLoad; // 0x1C4
    f32 field_1C8; // 0x1C8
    f32 field_1CC; // 0x1CC
    f32 field_1D0; // 0x1D0
    f32 field_1D4; // 0x1D4
    u32 field_1D8; // 0x1D8
    u32 field_1DC; // 0x1DC
    f32 field_1E0; // 0x1E0
    f32 field_1E4; // 0x1E4
    u32 field_1E8; // 0x1E8
    f32 field_1EC; // 0x1EC
    f32 field_1F0; // 0x1F0
    u32 field_1F4; // 0x1F4
    f32 field_1F8; // 0x1F8
    f32 field_1FC; // 0x1FC
    f32 field_200; // 0x200
    f32 field_204; // 0x204
    f32 field_208; // 0x208
    f32 field_20C; // 0x20C
    f32 field_210; // 0x210
    f32 field_214; // 0x214
    f32 field_218; // 0x218
    f32 field_21C; // 0x21C
    f32 field_220; // 0x220
    u32 field_224; // 0x224
    f32 field_228; // 0x228
    f32 field_22C; // 0x22C
    f32 field_230; // 0x230
    f32 field_234; // 0x234
    u32 field_238; // 0x238
    f32 field_23C; // 0x23C
    f32 field_240; // 0x240
    f32 field_244; // 0x244
    f32 field_248; // 0x248
    f32 field_24C; // 0x24C
    f32 OptimumSlipPercent; // 0x250
    f32 StaticFric; // 0x254
    f32 SlidingFric; // 0x258
    f32 field_25C; // 0x25C
    f32 field_260; // 0x260
    f32 field_264; // 0x264
    u32 field_268; // 0x268
};

// ??_7vehWheel@@6B@
// vtable at 0x005B2DCC
check_size(vehWheel, 0x26C);
