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
#include "vector7/vector2.h"
#include "vector7/vector3.h"

class Base_vtbl;
class Matrix34;
struct OppIconInfo;
class asMeshSetForm;
class datParser;
class mmGame;
class mmPlayer;
class mmWaypoints;

class mmHudMap : public asNode
{
public:
    enum IconType : i32;
    enum IndicatorType : i32;

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
    ARTS_IMPORT void Init(Matrix34* mouse_platform_pos, mmGame* viewport, char* arg3);

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
    ARTS_IMPORT void SetMapMode(i32 ctx);

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
    mmWaypoints* Waypoints; // 0x018
    i32 ShowWaypoints; // 0x01C
    Vector3* GoldLocation; // 0x020
    Vector3* BankLocation; // 0x024
    Vector3* HideoutLocation; // 0x028
    i32 field_2C; // 0x02C
    mmPlayer* Player; // 0x030
    OppIconInfo* IconInfo; // 0x034
    i32 ShowAllCops; // 0x038
    bool MapOnLeft; // 0x03C
    bool MapMissing; // 0x03D
    u8 pad_3E[2]; // 0x03E
    i32 LastNonFSMapMode; // 0x040
    i32 MapMode; // 0x044
    Matrix34* PlayerMatrix; // 0x048
    f32 ApproachRate; // 0x04C
    f32 ZoomLevel; // 0x050
    f32 ZoomInDist; // 0x054
    f32 ZoomOutDist; // 0x058
    f32 ZoonInDistFS; // 0x05C
    f32 ZoomOutDistFS; // 0x060
    f32 IconScale; // 0x064
    f32 IconScaleMin; // 0x068
    f32 IconScaleMax; // 0x06C
    f32 IconScaleMinFS; // 0x070
    f32 IconScaleMaxFS; // 0x074
    Vector3 BackgroundColor; // 0x078
    Vector2 Size; // 0x084
    Vector2 Position; // 0x08C
    i32 Viewport; // 0x094
    asMeshSetForm* HudmapModel; // 0x098
    u8 gap[32]; // 0x09C
    i16 NumOpponents; // 0x0BC
    u8 pad_BE[2]; // 0x0BE
    asMeshSetForm* HudmapSquareModel; // 0x0C0
    asMeshSetForm* HudmapTriModel; // 0x0C4
};

// ??_7mmHudMap@@6B@
// vtable at 0x005B0D00
check_size(mmHudMap, 0xC8);
