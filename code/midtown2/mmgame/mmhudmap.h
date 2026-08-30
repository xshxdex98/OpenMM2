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

class Matrix34;
struct OppIconInfo;
class Vector3;
class datParser;
class mmGame;

class mmHudMap : public asNode
{
public:
    enum IconType : i32;
    enum IndicatorType : i32;

    // ??0mmHudMap@@QAE@XZ
    ARTS_IMPORT mmHudMap();

    // ??1mmHudMap@@UAE@XZ
    ARTS_IMPORT virtual ~mmHudMap();

    // ?Cull@mmHudMap@@UAEXXZ
    ARTS_IMPORT virtual void Cull();

    // ?Update@mmHudMap@@UAEXXZ
    ARTS_IMPORT virtual void Update();

    // ?Reset@mmHudMap@@UAEXXZ
    ARTS_IMPORT virtual void Reset();

    // ?FileIO@mmHudMap@@UAEXAAVdatParser@@@Z
    ARTS_IMPORT virtual void FileIO(datParser& arg1);

    // ?GetClassName@mmHudMap@@UAEPADXZ
    ARTS_IMPORT virtual char* GetClassName();

    // ?Activate@mmHudMap@@QAEXXZ
    ARTS_IMPORT void Activate();

    // ?Deactivate@mmHudMap@@QAEXXZ
    ARTS_IMPORT void Deactivate();

    // ?GetOrient@mmHudMap@@QAE_NXZ
    ARTS_IMPORT bool GetOrient();

    // ?GetZoomIn@mmHudMap@@QAE_NXZ
    ARTS_IMPORT bool GetZoomIn();

    // ?Init@mmHudMap@@QAEXPAVMatrix34@@PAVmmGame@@PAD@Z
    ARTS_IMPORT void Init(Matrix34* arg1, mmGame* arg2, char* arg3);

    // ?RegisterCopsnRobbers@mmHudMap@@QAEXPAPAVVector3@@@Z
    ARTS_IMPORT void RegisterCopsnRobbers(Vector3** arg1);

    // ?RegisterOpponents@mmHudMap@@QAEXPAUOppIconInfo@@H@Z
    ARTS_IMPORT void RegisterOpponents(OppIconInfo* arg1, i32 arg2);

    // ?SetOrient@mmHudMap@@QAEX_N@Z
    ARTS_IMPORT void SetOrient(bool arg1);

    // ?SetZoomIn@mmHudMap@@QAEX_N@Z
    ARTS_IMPORT void SetZoomIn(bool arg1);

    // ?ToggleMapOrient@mmHudMap@@QAEXXZ
    ARTS_IMPORT void ToggleMapOrient();

    // ?ToggleMapRes@mmHudMap@@QAEXXZ
    ARTS_IMPORT void ToggleMapRes();

protected:
    // ?GetCurrentMapMode@mmHudMap@@IAEHXZ
    ARTS_IMPORT i32 GetCurrentMapMode();

    // ?GetNextMapMode@mmHudMap@@IAEHXZ
    ARTS_IMPORT i32 GetNextMapMode();

    // ?SetMapMode@mmHudMap@@IAEXH@Z
    ARTS_IMPORT void SetMapMode(i32 arg1);

private:
    // ?DrawCops@mmHudMap@@AAEXXZ
    ARTS_IMPORT void DrawCops();

    // ?DrawCopsnRobbers@mmHudMap@@AAEXXZ
    ARTS_IMPORT void DrawCopsnRobbers();

    // ?DrawIcon@mmHudMap@@AAEXW4IconType@1@ABVMatrix34@@@Z
    ARTS_IMPORT void DrawIcon(mmHudMap::IconType arg1, const Matrix34& arg2);

    // ?DrawIndicator@mmHudMap@@AAEXW4IndicatorType@1@ABVVector3@@@Z
    ARTS_IMPORT void DrawIndicator(mmHudMap::IndicatorType arg1, const Vector3& arg2);

    // ?DrawOpponents@mmHudMap@@AAEXXZ
    ARTS_IMPORT void DrawOpponents();

    // ?DrawPlayer@mmHudMap@@AAEXXZ
    ARTS_IMPORT void DrawPlayer();

    // ?DrawWaypoints@mmHudMap@@AAEXXZ
    ARTS_IMPORT void DrawWaypoints();

public:
    // Members from 0x018; everything below that belongs to asNode.
    // Offset order is mandatory - the original code reads these at fixed offsets.
    u8 field_18[4]; // 0x018
    i32 field_1C; // 0x01C
    i32 field_20; // 0x020
    i32 field_24; // 0x024
    i32 field_28; // 0x028
    u8 field_2C[12]; // 0x02C
    i32 field_38; // 0x038
    u8 field_3C; // 0x03C
    u8 field_3D; // 0x03D
    u8 field_3E[2]; // 0x03E
    i32 field_40; // 0x040
    i32 field_44; // 0x044
    u8 field_48[4]; // 0x048
    f32 Approach Rate; // 0x04C
    f32 field_50; // 0x050
    f32 ZoomInDist; // 0x054
    f32 ZoomOutDist; // 0x058
    f32 ZoomInDistFS; // 0x05C
    f32 ZoomOutDistFS; // 0x060
    f32 field_64; // 0x064
    f32 IconScaleMin; // 0x068
    f32 IconScaleMax; // 0x06C
    f32 IconScaleMinFS; // 0x070
    f32 IconScaleMaxFS; // 0x074
    f32 Ocean Color; // 0x078
    f32 field_7C; // 0x07C
    f32 field_80; // 0x080
    f32 Size; // 0x084
    f32 field_88; // 0x088
    f32 Pos; // 0x08C
    f32 field_90; // 0x090
    u8 field_94[4]; // 0x094
    i32 field_98; // 0x098
    u8 field_9C[32]; // 0x09C
    u16 field_BC; // 0x0BC
    u8 field_BE[2]; // 0x0BE
    i32 field_C0; // 0x0C0
    i32 field_C4; // 0x0C4
};

// ??_7mmHudMap@@6B@
// vtable at 0x005B0D00
check_size(mmHudMap, 0xC8);
