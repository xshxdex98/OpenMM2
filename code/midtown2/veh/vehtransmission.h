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

class datParser;
class vehCarSim;

class vehTransmission : public asNode
{
public:
    // ??0vehTransmission@@QAE@XZ
    ARTS_IMPORT vehTransmission();

    // ??1vehTransmission@@UAE@XZ
    ARTS_IMPORT virtual ~vehTransmission();

    // ?Update@vehTransmission@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@vehTransmission@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?FileIO@vehTransmission@@UAEXAAVdatParser@@@Z
    ARTS_IMPORT virtual void FileIO(datParser& arg1);

    // ?GetClassName@vehTransmission@@UAEPADXZ
    ARTS_IMPORT virtual char* GetClassName();

    // ?Automatic@vehTransmission@@QAEXH@Z
    ARTS_IMPORT void Automatic(i32 arg1);

    // ?ComputeConstants@vehTransmission@@QAEXXZ
    ARTS_IMPORT void ComputeConstants();

    // ?Downshift@vehTransmission@@QAEHXZ
    ARTS_IMPORT i32 Downshift();

    // ?Init@vehTransmission@@QAEXPAVvehCarSim@@@Z
    ARTS_IMPORT void Init(vehCarSim* arg1);

    // ?SetCurrentGear@vehTransmission@@QAEHH@Z
    ARTS_IMPORT i32 SetCurrentGear(i32 arg1);

    // ?SetForward@vehTransmission@@QAEXXZ
    ARTS_IMPORT void SetForward();

    // ?SetNeutral@vehTransmission@@QAEXXZ
    ARTS_IMPORT void SetNeutral();

    // ?SetReverse@vehTransmission@@QAEXXZ
    ARTS_IMPORT void SetReverse();

    // ?Upshift@vehTransmission@@QAEHXZ
    ARTS_IMPORT i32 Upshift();

private:
    // ?GearRatioFromMPH@vehTransmission@@AAEMM@Z
    ARTS_IMPORT f32 GearRatioFromMPH(f32 arg1);

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u32 field_18; // 0x018
    u32 field_1C; // 0x01C
    u32 field_20; // 0x020
    u32 CurrentGear; // 0x024
    u32 field_28; // 0x028
    f32 GearChangeTime; // 0x02C
    f32 field_30; // 0x030
    u32 field_34; // 0x034
    u8 field_38[24]; // 0x038
    u32 ManualNumGears; // 0x050
    i32 AutoNumGears; // 0x054
    f32 field_58; // 0x058
    u32 field_5C; // 0x05C
    u8 field_60[24]; // 0x060
    u32 field_78; // 0x078
    u32 field_7C; // 0x07C
    u8 field_80[24]; // 0x080
    u32 field_98; // 0x098
    u32 field_9C; // 0x09C
    u8 field_A0[24]; // 0x0A0
    u32 field_B8; // 0x0B8
    u32 field_BC; // 0x0BC
    u8 field_C0[24]; // 0x0C0
    f32 Reverse; // 0x0D8
    f32 Low; // 0x0DC
    f32 High; // 0x0E0
    f32 UpshiftBias; // 0x0E4
    f32 DownshiftBiasMin; // 0x0E8
    f32 DownshiftBiasMax; // 0x0EC
    f32 GearBias; // 0x0F0
};

// ??_7vehTransmission@@6B@
// vtable at 0x005B2D2C
check_size(vehTransmission, 0xF4);
